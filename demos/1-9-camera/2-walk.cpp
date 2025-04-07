#include "glm/ext/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sjd/camera.h>
#include <array>

// incl before defining SOKOL_IMPL
#include <sjd/sok_texture.h>

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
#include "shaders.glsl.h"

#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

namespace state {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
    std::array<glm::vec3, 10> cube_positions;
    sjd::Camera camera;
    uint64_t last_time;
    float deltaTime;
    bool moveUp;
    bool moveDown;
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
}

static void init(void) {
    sg_setup(sg_desc {
        .logger {
            .func = slog_func
        },
        .environment = sglue_environment(),
    });

    sfetch_setup(sfetch_desc_t {
        .max_requests = 2,
        .num_channels = 1,
        .num_lanes = 1,
        .logger {
            .func = slog_func
        },
    });

    stm_setup();

    state::cube_positions = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    std::array<float, 180> vertices {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    state::bind.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(vertices),
        .data = SG_RANGE(vertices),
        .label = "cube-vertices"
    });

    // create shader from code-generated sg_shader_desc
    sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));

    // we need to initialise layout seperately to the pipeline
    // because we cant do array initilisation of structs in C++
    sg_vertex_layout_state layout {};
    layout.attrs[ATTR_simple_aPos].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_simple_aTexCoord].format = SG_VERTEXFORMAT_FLOAT2;

    // create a pipeline object (default render state:: are fine for triangle)
    state::pip = sg_make_pipeline(sg_pipeline_desc {
        .shader = shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS_EQUAL,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "cube-pipeline"
    });

    // a pass action to clear framebuffer
    state::pass_action = sg_pass_action {
        .colors = {{
	    .load_action=SG_LOADACTION_CLEAR,
	    .clear_value={0.2f, 0.3f, 0.3f, 1.0f} 
	}}
    };

    // Load textures. (don't forget to init sfetch first and shutdown in cleanup
    static SokTexture texture1("../data/container.jpg", state::bind, IMG__texture1, SMP_texture1_smp, true);
    static SokTexture texture2("../data/ahhprofile.png", state::bind, IMG__texture2, SMP_texture2_smp, true);
}

void frame(void) {
    state::deltaTime = static_cast<float>(stm_sec(stm_laptime(&state::last_time)));
    sfetch_dowork();

    // Movements
    if (state::moveUp) {
        state::camera.processKeyboard(state::camera.UP, state::deltaTime);
    }
    if (state::moveDown) {
        state::camera.processKeyboard(state::camera.DOWN, state::deltaTime);
    }
    if (state::moveForward) {
        state::camera.processKeyboard(state::camera.FORWARD, state::deltaTime);
    }
    if (state::moveBackward) {
        state::camera.processKeyboard(state::camera.BACKWARD, state::deltaTime);
    }
    if (state::moveLeft) {
        state::camera.processKeyboard(state::camera.LEFT, state::deltaTime);
    }
    if (state::moveRight) {
        state::camera.processKeyboard(state::camera.RIGHT, state::deltaTime);
    }

    glm::mat4 view = glm::lookAt(state::camera.pos,
                                 state::camera.pos + state::camera.front,
                                 state::camera.up);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(sapp_width()) / sapp_height(), 0.1f, 100.0f);

    sg_begin_pass(sg_pass { 
	.action = state::pass_action,
	.swapchain = sglue_swapchain()
    });
    sg_apply_pipeline(state::pip);
    sg_apply_bindings(state::bind);

    vs_params_t vs_params = {
        .view = view,
        .projection = projection
    };

    int i {0};
    for (glm::vec3 cube_position : state::cube_positions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), cube_position);
        float angle = 20.f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        vs_params.model = model;

        sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

        sg_draw(0, 36, 1);
        ++i;
    }


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
        if (e->key_code == SAPP_KEYCODE_SPACE) {
            state::moveUp = true;
        }
        if (e->key_code == SAPP_KEYCODE_C) {
            state::moveDown = true;
        }
        if (e->key_code == SAPP_KEYCODE_W) {
            state::moveForward = true;
        }
        if (e->key_code == SAPP_KEYCODE_S) {
            state::moveBackward = true;
        }
        if (e->key_code == SAPP_KEYCODE_A) {
            state::moveLeft = true;
        }
        if (e->key_code == SAPP_KEYCODE_D) {
            state::moveRight = true;
        }
    }

    if (e->type == SAPP_EVENTTYPE_KEY_UP) {
        if (e->key_code == SAPP_KEYCODE_SPACE) {
            state::moveUp = false;
        }
        if (e->key_code == SAPP_KEYCODE_C) {
            state::moveDown = false;
        }
        if (e->key_code == SAPP_KEYCODE_W) {
            state::moveForward = false;
        }
        if (e->key_code == SAPP_KEYCODE_S) {
            state::moveBackward = false;
        }
        if (e->key_code == SAPP_KEYCODE_A) {
            state::moveLeft = false;
        }
        if (e->key_code == SAPP_KEYCODE_D) {
            state::moveRight = false;
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
        .window_title = "Cube - LearnOpenGL",
        .logger {
            .func = slog_func
        },

    };
}

