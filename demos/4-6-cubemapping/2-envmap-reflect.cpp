#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sjd/camera.h>
#include <vector>

// incl before defining SOKOL_IMPL
#include <sjd/sok_texture.h>
#include <sjd/sok_texture_cube.h>

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
#include <sokol/sokol_glue.h>
#include <sokol/sokol_fetch.h>
#include <sokol/sokol_time.h>

// add the shader after glm
#include "2-envmap-reflect.glsl.h"

#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

namespace state {
    sg_pipeline pip_cube;
    sg_pipeline pip_skybox;
    sg_bindings bind_cube;
    sg_bindings bind_skybox;
    sg_pass_action pass_action;
    sjd::Camera camera(glm::vec3(3.0f, 0.8f, 4.0f));
    uint64_t last_time;
    float deltaTime;
#ifdef __EMSCRIPTEN__
    std::array<std::string, 6> cubemapPaths = {
        "../data/skybox/right.jpg",
        "../data/skybox/left.jpg",
        "../data/skybox/top.jpg",
        "../data/skybox/bottom.jpg",
        "../data/skybox/front.jpg",
        "../data/skybox/back.jpg",
    };
#else
    std::array<std::string, 6> cubemapPaths = {
        "../embuild/data/skybox/right.jpg",
        "../embuild/data/skybox/left.jpg",
        "../embuild/data/skybox/top.jpg",
        "../embuild/data/skybox/bottom.jpg",
        "../embuild/data/skybox/front.jpg",
        "../embuild/data/skybox/back.jpg",
    };
#endif
}

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
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    state::bind_cube.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = vertices.size() * sizeof(float),
        .data = sg_range {
            .ptr = vertices.data(),
            .size = vertices.size() * sizeof(float),
        },
        .label = "cube-vertices"
    });

    std::vector<float> skybox_vertices = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    state::bind_skybox.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = skybox_vertices.size() * sizeof(float),
        .data = sg_range {
            .ptr = skybox_vertices.data(),
            .size = skybox_vertices.size() * sizeof(float),
        },
        .label = "skybox-vertices"
    });

    // create shader from code-generated sg_shader_desc
    sg_shader shd = sg_make_shader(reflect_shader_desc(sg_query_backend()));
    sg_shader skybox_shd = sg_make_shader(skybox_shader_desc(sg_query_backend()));

    // we need to initialise layout seperately to the pipeline
    // because we cant do array initilisation of structs in C++
    sg_vertex_layout_state layout {};
    layout.attrs[ATTR_reflect_aPos].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_reflect_aNormal].format = SG_VERTEXFORMAT_FLOAT3;

    // create a pipeline object (default render state:: are fine for triangle)
    state::pip_cube = sg_make_pipeline(sg_pipeline_desc {
        .shader = shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "cube-pipeline"
    });

    sg_vertex_layout_state layout_skybox {};
    layout_skybox.attrs[ATTR_skybox_aPos].format = SG_VERTEXFORMAT_FLOAT3;

    // create a pipeline object (default render state:: are fine for triangle)
    state::pip_skybox = sg_make_pipeline(sg_pipeline_desc {
        .shader = skybox_shd,
        .layout = layout_skybox,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS_EQUAL,   // discard fragments that are further away
        },
        .label = "skybox-pipeline"
    });

    // a pass action to clear framebuffer
    state::pass_action = sg_pass_action {
        .colors = {{
	    .load_action=SG_LOADACTION_CLEAR,
	    .clear_value={0.1f, 0.1f, 0.1f, 1.0f} 
	}}
    };

    // Load textures. (don't forget to init sfetch first and shutdown in cleanup)
    static TextureCube skybox(state::cubemapPaths,
                              state::bind_skybox,
                              IMG__skybox,
                              SMP_skybox_smp,
                              false,
                              fail_callback);

    sg_image skybox_img_id = state::bind_skybox.images[IMG__skybox];
    state::bind_cube.images[IMG__skybox] = skybox_img_id;
    state::bind_cube.samplers[SMP_skybox_smp] = state::bind_skybox.samplers[SMP_skybox_smp];

}

void frame(void) {
    state::deltaTime = static_cast<float>(stm_sec(stm_laptime(&state::last_time)));
    sfetch_dowork();

    // Movements

    if (!sapp_mouse_locked()) {
        sapp_lock_mouse(true);
    }
    state::camera.moveCamera(state::deltaTime);

    glm::mat4 view = state::camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(state::camera.zoom), static_cast<float>(sapp_width()) / sapp_height(), 0.1f, 100.0f);


    sg_begin_pass(sg_pass { 
	.action = state::pass_action,
	.swapchain = sglue_swapchain()
    });
    sg_apply_pipeline(state::pip_cube);
    sg_apply_bindings(state::bind_cube);

    vs_params_t vs_params = {
        .view = view,
        .projection = projection
    };

    glm::mat4 model(1.0f);
    vs_params.model = model;

    fs_params_t fs_params {
        .cameraPos = state::camera.pos
    };

    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));
    sg_apply_uniforms(UB_fs_params, SG_RANGE(fs_params));

    sg_draw(0, 36, 1);

    sg_apply_pipeline(state::pip_skybox);
    sg_apply_bindings(state::bind_skybox);

    vs_params.view = glm::mat4(glm::mat3(view));
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

    sg_draw(0, 36, 1);


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
        .window_title = "Skybox - LearnOpenGL",
        .logger {
            .func = slog_func
        },
#ifdef _WIN32
        .win32_console_utf8 = true,
        .win32_console_attach = true,
#endif

    };
}

