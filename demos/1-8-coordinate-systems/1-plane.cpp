#include "glm/ext/matrix_transform.hpp"
#define SOKOL_IMPL
#define STB_IMAGE_IMPLEMENTATION
#ifndef __EMSCRIPTEN__
#define SOKOL_GLCORE
#else
#define SOKOL_GLES3
#endif
#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_fetch.h>
#include <sokol/sokol_time.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sjd/camera.h>

#include <array>

// add the shader after glm
#include "shaders.glsl.h"

#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

static const sg_sampler_desc global_sampler_desc = {
    .min_filter = SG_FILTER_LINEAR,
    .mag_filter = SG_FILTER_LINEAR,
    .wrap_u = SG_WRAP_REPEAT,
    .wrap_v = SG_WRAP_REPEAT,
    .compare = SG_COMPAREFUNC_NEVER,
};

static void sg_alloc_image_smp(sg_bindings& bindings, uint16_t image_index, uint16_t smp_index) {
    bindings.images[image_index] = sg_alloc_image();
    bindings.samplers[smp_index] = sg_alloc_sampler();
    sg_init_sampler(bindings.samplers[smp_index], global_sampler_desc);
}

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
    uint8_t file_buffer[512 * 1024];
} state;

static void fetch_callback(const sfetch_response_t*);

static void init(void) {
    sg_setup(sg_desc {
        .environment = sglue_environment()
    });

    sfetch_setup(sfetch_desc_t {
        .max_requests = 2,
        .num_channels = 1,
        .num_lanes = 1
    });


    // Use helper function that uses sokol_fetch to get files asynchronously
    // draw cllas containing an "inclomplete" image handle will silently
    // be dropped.
    sg_alloc_image_smp(state.bind, IMG__texture1, SMP_texture1_smp);
    sg_alloc_image_smp(state.bind, IMG__texture2, SMP_texture2_smp);

    // flip images vertically after loading
    stbi_set_flip_vertically_on_load(true);

    std::array<float, 20> vertices {
        // positions         // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left 
    };
    state.bind.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(vertices),
        .data = SG_RANGE(vertices),
        .label = "quad-vertices"
    });

    std::array<uint16_t, 6> indices = {
            0, 1, 3,
            1, 2, 3
    };

    state.bind.index_buffer = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(indices),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "quad-indices"
    });

    // create shader from code-generated sg_shader_desc
    sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));

    // we need to initialise layout seperately to the pipeline
    // because we cant do array initilisation of structs in C++
    sg_vertex_layout_state layout {};
    layout.attrs[ATTR_simple_aPos].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_simple_aTexCoord].format = SG_VERTEXFORMAT_FLOAT2;

    // create a pipeline object (default render states are fine for triangle)
    state.pip = sg_make_pipeline(sg_pipeline_desc {
        .shader = shd,
        /* if the vertex layout doesn't have gaps, don't need to provide strides and offsets */
        .layout = layout,
        .index_type = SG_INDEXTYPE_UINT16,
        .label = "triangle-pipeline"
    });
    
    // a pass action to clear framebuffer
    state.pass_action = sg_pass_action {
        .colors = {{
	    .load_action=SG_LOADACTION_CLEAR,
	    .clear_value={0.2f, 0.3f, 0.3f, 1.0f} 
	}}
    };

    sg_image image1 = state.bind.images[IMG__texture1];
    sg_image image2 = state.bind.images[IMG__texture2];

    /* start loading the JPG file */
    sfetch_send(sfetch_request_t {
        .path = "../data/container.jpg",
        .callback = fetch_callback,
        .buffer = SFETCH_RANGE(state.file_buffer),
        .user_data = SFETCH_RANGE(image1),
    });

    /* start loading the PNG file
       we can use the same buffer because we are serializing the request (see sfetch_setup) */
    sfetch_send(sfetch_request_t {
        .path = "../data/ahhprofile.png",
        .callback = fetch_callback,
        .buffer = SFETCH_RANGE(state.file_buffer),
        .user_data = SFETCH_RANGE(image2),
    });
}

/* The fetch-callback is called by sokol_fetch.h when the data is loaded,
   or when an error has occurred.
*/
static void fetch_callback(const sfetch_response_t* response) {
    if (response->fetched) {
        /* the file data has been fetched, since we provided a big-enough
           buffer we can be sure that all data has been loaded here
        */
        int img_width, img_height, num_channels;
        const int desired_channels = 4;
        stbi_uc* pixels = stbi_load_from_memory(
            static_cast<const stbi_uc*>(response -> data.ptr),
            static_cast<int>(response -> data.size),
            &img_width, &img_height,
            &num_channels, desired_channels);
        if (pixels) {
            sg_image image = *static_cast<sg_image*>(response->user_data);
            sg_init_image(image, sg_image_desc {
                .width = img_width,
                .height = img_height,
                /* set pixel_format to RGBA8 for WebGL */
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .data = {
		    .subimage = {{{
			.ptr = pixels,
			.size = static_cast<size_t>(img_width * img_height * 4),
		    }}}
		}
            });
            stbi_image_free(pixels);
        }
    }
    else if (response->failed) {
        // if loading the file failed, set clear color to red
        state.pass_action = sg_pass_action {
            .colors = {{
                .load_action = SG_LOADACTION_CLEAR,
                .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f }
            }}
        };
    }
}

void frame(void) {
    sfetch_dowork();

    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // note that we're translating the scene in the reverse direction of where we want to move
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(sapp_width()) / sapp_height(), 0.1f, 100.0f);

    sg_begin_pass(sg_pass { 
	.action = state.pass_action,
	.swapchain = sglue_swapchain()
    });
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(state.bind);

    vs_params_t vs_params = {
        .model = model,
        .view = view,
        .projection = projection
    };
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

    sg_draw(0, 6, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sg_shutdown();
    sfetch_shutdown();
}

void event(const sapp_event* e) {
    if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (e->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
    }
}

sapp_desc sokol_main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    return sapp_desc {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .high_dpi = true,
        .window_title = "Plane - LearnOpenGL",
    };
}

