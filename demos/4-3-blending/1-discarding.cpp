#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sjd/camera.h>
#include <sjd/sok_texture.h>
#include <vector>

#define SOKOL_DEBUG
#define SOKOL_IMPL
#ifndef __EMSCRIPTEN__
#define SOKOL_GLCORE
#else
#define SOKOL_GLES3
#endif
#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_log.h>
#include <sokol/sokol_fetch.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_time.h>

// add the shader after glm
#include "1-discarding.glsl.h"

#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

namespace state {
    sg_pipeline pip_cubes;
    sg_pipeline pip_plane;
    sg_pipeline pip_vegetation;
    sg_bindings bind_cubes;
    sg_bindings bind_plane;
    sg_bindings bind_vegetation;
    sg_pass_action pass_action;
    sjd::Camera camera(glm::vec3(3.0f, 0.8f, 4.0f),
                       glm::vec3(1.0f, 0.4f, 0.0f));
    uint64_t last_time;
    float deltaTime;
    std::vector<glm::vec3> vegetation {
        glm::vec3(-1.5f,  0.0f, -0.48f),
        glm::vec3( 1.5f,  0.0f,  0.51f),
        glm::vec3( 0.0f,  0.0f,  0.7f),
        glm::vec3(-0.3f,  0.0f, -2.3f),
        glm::vec3( 0.5f,  0.0f, -0.6f)
    };
}

static sg_sampler_desc grass_sampler_desc = {
    .min_filter = SG_FILTER_LINEAR,
    .mag_filter = SG_FILTER_LINEAR,
    .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
    .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
    .compare = SG_COMPAREFUNC_NEVER,
};

static void fail_callback() {
    state::pass_action = sg_pass_action {
        .colors = {{ .load_action=SG_LOADACTION_CLEAR,
            .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f }
        }}
    };
}

static void init(void) {
    sg_setup(sg_desc {
        .logger {
            .func = slog_func
        },
        .environment = sglue_environment(),
    });

    sfetch_setup(sfetch_desc_t {
        .max_requests = 8,
        .num_channels = 1,
        .num_lanes = 1,
        .logger {
            .func = slog_func
        },
    });

    stm_setup();

    std::vector<float> vertices {
        // positions          // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f
    };

    sg_buffer cube_buffer = sg_make_buffer(sg_buffer_desc {
        .size = vertices.size() * sizeof(float),
        .data = sg_range {
            .ptr = vertices.data(),
            .size = vertices.size() * sizeof(float),
        },
        .label = "cube-vertices"
    });

    state::bind_cubes.vertex_buffers[0] = cube_buffer;

    std::vector<float> planeVertices = {
        // positions        // texture Coords
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    sg_buffer plane_buffer = sg_make_buffer(sg_buffer_desc {
        .size = planeVertices.size() * sizeof(float),
        .data = sg_range {
            .ptr = planeVertices.data(),
            .size = planeVertices.size() * sizeof(float),
        },
        .label = "plane-vertices"
    });

    state::bind_plane.vertex_buffers[0] = plane_buffer;

    std::vector<float> vegetationVertices {
        // positions          // texture coords
        -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.0f,  0.0f,  0.0f,
    };

    sg_buffer vegetation_buffer = sg_make_buffer(sg_buffer_desc {
        .size = vegetationVertices.size() * sizeof(float),
        .data = sg_range {
            .ptr = vegetationVertices.data(),
            .size = vegetationVertices.size() * sizeof(float),
        },
        .label = "vegetation-vertices"
    });

    state::bind_vegetation.vertex_buffers[0] = vegetation_buffer;

    // create shader from code-generated sg_shader_desc
    sg_shader simple_shd = sg_make_shader(simple_shader_desc(sg_query_backend()));

    // we need to initialise layout seperately to the pipeline
    // because we cant do array initilisation of structs in C++
    sg_vertex_layout_state layout {};
    layout.attrs[ATTR_simple_aPos].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_simple_aTexCoords].format = SG_VERTEXFORMAT_FLOAT2;

    // create a pipeline object
    state::pip_cubes = sg_make_pipeline(sg_pipeline_desc {
        .shader = simple_shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "object-pipeline"
    });

    // create a pipeline object (default render states are fine for triangle)
    state::pip_plane = sg_make_pipeline(sg_pipeline_desc {
        .shader = simple_shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "plane-pipeline"
    });

    // create a pipeline object (default render states are fine for triangle)
    state::pip_vegetation = sg_make_pipeline(sg_pipeline_desc {
        .shader = simple_shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "vegetation-pipeline"
    });

    //
    // a pass action to clear framebuffer
    state::pass_action = sg_pass_action {
        .colors = {{
	    .load_action=SG_LOADACTION_CLEAR,
	    .clear_value={0.1f, 0.1f, 0.1f, 1.0f} 
	}}
    };
    
    SokTexture marble("../data/marble.jpg",
                          state::bind_cubes,
                          IMG__texture1,
                          SMP_texture1_smp,
                          true,
                          fail_callback);

    SokTexture metal("../data/metal.png",
                                   state::bind_plane,
                                   IMG__texture1,
                                   SMP_texture1_smp,
                                   true,
                                   fail_callback);

    SokTexture vegetation("../data/grass.png",
                          state::bind_vegetation,
                          IMG__texture1,
                          SMP_texture1_smp,
                          true,
                          fail_callback,
                          &grass_sampler_desc);

}

void frame(void) {
    sfetch_dowork();

    state::deltaTime = static_cast<float>(stm_sec(stm_laptime(&state::last_time)));

    if (!sapp_mouse_locked()) {
        sapp_lock_mouse(true);
    }
    state::camera.moveCamera(state::deltaTime);

    sg_begin_pass(sg_pass { 
	.action = state::pass_action,
	.swapchain = sglue_swapchain()
    });

    glm::mat4 view = state::camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(state::camera.zoom), static_cast<float>(sapp_width()) / sapp_height(), 0.1f, 100.0f);
    vs_params_t vs_params = {
        .view = view,
        .projection = projection
    };

    // Plane
    sg_apply_pipeline(state::pip_plane);
    sg_apply_bindings(state::bind_plane);

    glm::mat4 model = glm::mat4(1.0f);
    vs_params.model = model;
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));
    sg_draw(0, 6, 1);

    // Cubes
    sg_apply_pipeline(state::pip_cubes);
    sg_apply_bindings(state::bind_cubes);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
    vs_params.model = model;
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));
    sg_draw(0, 36, 1);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    vs_params.model = model;
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));
    sg_draw(0, 36, 1);

    // Vegetation
    sg_apply_pipeline(state::pip_vegetation);
    sg_apply_bindings(state::bind_vegetation);

    for (glm::vec3 position : state::vegetation) {
        vs_params.model = glm::translate(glm::mat4(1.0f), position);
        sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));
        sg_draw(0, 6, 1);
    }

    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sfetch_shutdown();
    sg_shutdown();
}

void event(const sapp_event* e) {
    if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (e->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
        if (e->key_code == SAPP_KEYCODE_SPACE)
            state::camera.processKeyboard(sjd::Camera::UP, sjd::Camera::PRESS);
        if (e->key_code == SAPP_KEYCODE_C)
            state::camera.processKeyboard(sjd::Camera::DOWN, sjd::Camera::PRESS);
        if (e->key_code == SAPP_KEYCODE_W)
            state::camera.processKeyboard(sjd::Camera::FORWARD, sjd::Camera::PRESS);
        if (e->key_code == SAPP_KEYCODE_S) 
            state::camera.processKeyboard(sjd::Camera::BACKWARD, sjd::Camera::PRESS);
        if (e->key_code == SAPP_KEYCODE_A)
            state::camera.processKeyboard(sjd::Camera::LEFT, sjd::Camera::PRESS);
        if (e->key_code == SAPP_KEYCODE_D)
            state::camera.processKeyboard(sjd::Camera::RIGHT, sjd::Camera::PRESS);
    }

    if (e->type == SAPP_EVENTTYPE_KEY_UP) {
        if (e->key_code == SAPP_KEYCODE_SPACE)
            state::camera.processKeyboard(sjd::Camera::UP, sjd::Camera::RELEASE);
        if (e->key_code == SAPP_KEYCODE_C)
            state::camera.processKeyboard(sjd::Camera::DOWN, sjd::Camera::RELEASE);
        if (e->key_code == SAPP_KEYCODE_W)
            state::camera.processKeyboard(sjd::Camera::FORWARD, sjd::Camera::RELEASE);
        if (e->key_code == SAPP_KEYCODE_S) 
            state::camera.processKeyboard(sjd::Camera::BACKWARD, sjd::Camera::RELEASE);
        if (e->key_code == SAPP_KEYCODE_A)
            state::camera.processKeyboard(sjd::Camera::LEFT, sjd::Camera::RELEASE);
        if (e->key_code == SAPP_KEYCODE_D)
            state::camera.processKeyboard(sjd::Camera::RIGHT, sjd::Camera::RELEASE);
    }
    if (e->type == SAPP_EVENTTYPE_TOUCHES_BEGAN) {
        state::camera.lastX = e->touches[0].pos_x;
        state::camera.lastY = e->touches[0].pos_y;
    }
    if (e->type == SAPP_EVENTTYPE_TOUCHES_MOVED) {
        float offsetX = e -> touches[0].pos_x - state::camera.lastX;
        float offsetY = state::camera.lastY - e -> touches[0].pos_y;
        state::camera.lastX = e->touches[0].pos_x;
        state::camera.lastY = e->touches[0].pos_y;
        state::camera.processMouseMovement(offsetX, offsetY);
    }
    if (e->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
        state::camera.processMouseMovement(e->mouse_dx, -e->mouse_dy);
    }
    if (e->type == SAPP_EVENTTYPE_MOUSE_SCROLL) {
        state::camera.processMouseScroll(e->scroll_y);
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
        .window_title = "Depth Testing (ALWAYS) - LearnOpenGL",
        .logger {
            .func = slog_func
        },
#ifdef _WIN32
        .win32_console_utf8 = true,
        .win32_console_attach = true,
#endif
    };
}

