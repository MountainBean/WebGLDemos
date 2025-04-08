#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sjd/camera.h>
#include <sjd/sok_texture.h>
#include <array>

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
#include "1-diffuse-map.glsl.h"

#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

namespace state {
    sg_pipeline pip_object;
    sg_pipeline pip_light;
    sg_bindings bind;
    sg_pass_action pass_action;
    glm::vec3 light_pos;
    glm::vec3 light_colour;
    sjd::Camera camera(glm::vec3(0.2f, 0.8f, 4.0f));
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

    state::light_colour = glm::vec3(1.0f);
    state::light_pos = glm::vec3(1.0f, 1.2f, 2.0f);

    std::array<float, 288> vertices {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    state::bind.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(vertices),
        .data = SG_RANGE(vertices),
        .label = "cube-vertices"
    });

    // create shader from code-generated sg_shader_desc
    sg_shader phong_shd = sg_make_shader(phong_shader_desc(sg_query_backend()));

    // we need to initialise layout seperately to the pipeline
    // because we cant do array initilisation of structs in C++
    sg_vertex_layout_state layout {};
    layout.attrs[ATTR_phong_aPos].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_phong_aNormal].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_phong_aTexCoords].format = SG_VERTEXFORMAT_FLOAT2;

    // create a pipeline object (default render state:: are fine for triangle)
    state::pip_object = sg_make_pipeline(sg_pipeline_desc {
        .shader = phong_shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS_EQUAL,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "object-pipeline"
    });

    // Now we make the seperate shader for the light cube
    sg_shader light_cube_shd = sg_make_shader(light_cube_shader_desc(sg_query_backend()));

    // create a pipeline object (default render states are fine for triangle)
    state::pip_light = sg_make_pipeline(sg_pipeline_desc {
        .shader = light_cube_shd,
        .layout = layout,
        .depth {   
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true,
        },
        .label = "light-cube-pipeline"
    });

    // a pass action to clear framebuffer
    state::pass_action = sg_pass_action {
        .colors = {{
	    .load_action=SG_LOADACTION_CLEAR,
	    .clear_value={0.1f, 0.1f, 0.1f, 1.0f} 
	}}
    };
    
    SokTexture container2("data/container2.png",
                          state::bind,
                          IMG__diffuse_texture,
                          SMP_diffuse_texture_smp,
                          true);

}

void frame(void) {
    sfetch_dowork();

    state::deltaTime = static_cast<float>(stm_sec(stm_laptime(&state::last_time)));

    // rotate light source
    state::light_pos = glm::vec3(1.3f * sinf(static_cast<float>(stm_sec(stm_now()))),
                                 0.8f,
                                 1.3f * cosf(static_cast<float>(stm_sec(stm_now()))));

    // change light colour
    /*state::light_colour = 0.5f * glm::vec3(sinf((stm_sec(stm_now()) * 2.0f))+1,*/
    /*                                sinf((stm_sec(stm_now()) * 0.7f))+1,*/
    /*                                sinf((stm_sec(stm_now()) * 1.3f))+1);*/

    // Movements

    if (!sapp_mouse_locked()) {
        sapp_lock_mouse(true);
    }

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


    // Prepare and draw object
    sg_apply_pipeline(state::pip_object);
    sg_apply_bindings(state::bind);

    vs_params.model = glm::mat4(1.0f);
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

    fs_params_t fs_params = {
        .viewPos {state::camera.pos}
    };
    sg_apply_uniforms(UB_fs_params, SG_RANGE(fs_params));

    fs_material_t fs_material = {
        .specular = glm::vec3(0.5f, 0.5f, 0.5f),
        .shininess = 32.0f,
    };
    sg_apply_uniforms(UB_fs_material, SG_RANGE(fs_material));

    fs_light_t fs_light = {
        .position = state::light_pos,
        .ambient = state::light_colour * 0.2f,
        .diffuse = state::light_colour * 0.5f,
        .specular = state::light_colour * 1.0f,
    };
    sg_apply_uniforms(UB_fs_light, SG_RANGE(fs_light));

    sg_draw(0, 36, 1);

    // Prepare and draw Light Cube
    sg_apply_pipeline(state::pip_light);
    sg_apply_bindings(state::bind);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), state::light_pos);
    vs_params.model = glm::scale(model, glm::vec3(0.2f));
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));
    light_cube_fs_params_t light_cube_fs_params = {
        .lightColour {state::light_colour}
    };
    sg_apply_uniforms(UB_light_cube_fs_params, SG_RANGE(light_cube_fs_params));

    sg_draw(0, 36, 1);

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
        .window_title = "Material - LearnOpenGL",
        .logger {
            .func = slog_func
        },

    };
}

