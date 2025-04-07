#define SOKOL_IMPL
#define STB_IMAGE_IMPLEMENTATION
#ifndef __EMSCRIPTEN__
#define SOKOL_GLCORE
#else
#define SOKOL_GLES3
#endif
#include <array>
#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_fetch.h>
#include <stb/stb_image.h>
#include "1-texture.glsl.h"
#include <cstdint>

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

static void init() {
    sg_setup(sg_desc {
        .environment = sglue_environment()
    });

    sfetch_setup(sfetch_desc_t {
        .max_requests = 1,
        .num_channels = 1,
        .num_lanes = 1
    });
    sg_alloc_image_smp(state.bind, IMG__ourTexture, SMP_ourTexture_smp);

    std::array vertices {
        // positions         // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    state.bind.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(vertices),
        .data = SG_RANGE(vertices),
        .label = "quad-vertices"
    });

    std::array<uint16_t, 6> indices {
        0, 1, 3,
        1, 2, 3
    };
    state.bind.index_buffer = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(indices),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "quad-indices"
    });

    sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));

    // we have to initialise attrs because we cant do array initilisation of structs in C++
    sg_vertex_layout_state layout {};
    layout.attrs[ATTR_simple_position].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_simple_aColor].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_simple_aTexCoord].format = SG_VERTEXFORMAT_FLOAT2;

    state.pip = sg_make_pipeline(sg_pipeline_desc {
        .shader = shd,
        .layout = layout,
        .index_type = SG_INDEXTYPE_UINT16,
        .label = "triangle-pipeline"
    });


    state.pass_action = sg_pass_action {
        .colors = {{
            .load_action=SG_LOADACTION_CLEAR,
            .clear_value={0.2f, 0.3f, 0.3f, 1.0f}
        }}
    };

    sfetch_send(sfetch_request_t {
        .path = "../data/container.jpg",
        .callback = fetch_callback,
        .buffer = SFETCH_RANGE(state.file_buffer),
    });
}

static void fetch_callback(const sfetch_response_t* response) {
    if (response -> fetched) {
        int width;
        int height;
        int nrChannels;
        int desired_channels = 4;
        stbi_uc* pixels = stbi_load_from_memory(
            (stbi_uc*)response -> data.ptr,
            static_cast<int>(response -> data.size),
            &width,
            &height,
            &nrChannels,
            desired_channels);
        if (pixels) {
            sg_init_image(state.bind.images[IMG__ourTexture], sg_image_desc {
                .width = width,
                .height = height,
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .data {
                    .subimage = {{{
                        .ptr = pixels,
                        .size = static_cast<size_t>(width * height * 4),
                    }}}
                }
            });
            stbi_image_free(pixels);
        }
    }
    else if (response -> failed) {
        state.pass_action = sg_pass_action {
            .colors = {{
                .load_action = SG_LOADACTION_CLEAR,
                .clear_value = {1.0f, 0.0f, 0.0f, 0.0f}
            }}
        };
    }
}

void frame() {
    sfetch_dowork();
    sg_begin_pass(sg_pass {
        .action = state.pass_action,
        .swapchain = sglue_swapchain(),
    });
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(state.bind);
    sg_draw(0,6,1);
    sg_end_pass();
    sg_commit();
}

void cleanup() {
    sg_shutdown();
    sfetch_shutdown();
}

void event(const sapp_event* e) {
    if (e -> type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (e -> key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
    }
}

sapp_desc sokol_main([[maybe_unused]] int argc,[[maybe_unused]] char* argv[]) {
    return sapp_desc {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .high_dpi = true,
        .window_title = "Texture - LearnOpenGL",
    };
}
