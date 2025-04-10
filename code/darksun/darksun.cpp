#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sjd/icosahedron.h>
#include <sjd/camera.h>

#define SOKOL_IMPL
#ifndef __EMSCRIPTEN__
#define SOKOL_GLCORE
#else
// we're compileing for webgl
#define SOKOL_GLES3
#endif

#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_log.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_time.h>

#include "darksun.glsl.h"

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
    glm::vec3 dark_pos;
    glm::vec3 light_colour;
    glm::vec3 dark_colour;
    sjd::Camera camera {};
    uint64_t last_time;
    float deltaTime;
}

void init(void) {
    sg_setup(sg_desc {
        .logger {
            .func = slog_func
        },
        .environment = sglue_environment()
    });

    // initialise sokol time
    stm_setup();

    state::light_colour = glm::vec3(1.0f);
    state::dark_colour = glm::vec3(-1.0f);
    state::light_pos = glm::vec3(1.0f, 1.2f, 2.0f);

    // get vertices
    sjd::Icosahedron myShape {};
    std::array<float, 360> vertices = myShape.getPrimVerticesNorms();

    state::bind.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(vertices),
        .data = SG_RANGE(vertices),
        .label = "icosahedron-vertices",
    });

    // create shader
    sg_shader shd = sg_make_shader(darksun_shader_desc(sg_query_backend()));

    // we need to initialise layout seperately to the pipeline
    // because we cant do array initilisation of structs in C++
    sg_vertex_layout_state layout {};
    layout.attrs[ATTR_darksun_aPos].format = SG_VERTEXFORMAT_FLOAT3;
    layout.attrs[ATTR_darksun_aNormal].format = SG_VERTEXFORMAT_FLOAT3;

    // create a pipeline object (default render state:: are fine for triangle)
    state::pip_object = sg_make_pipeline(sg_pipeline_desc {
        .shader = shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS_EQUAL,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "object-pipeline"
    });

    // Now we make the seperate shader for the light cube
    sg_shader light_sphere_shd = sg_make_shader(light_sphere_shader_desc(sg_query_backend()));

    // create a pipeline object (default render states are fine for triangle)
    state::pip_light = sg_make_pipeline(sg_pipeline_desc {
        .shader = light_sphere_shd,
        .layout = layout,
        .depth {   
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true,
        },
        .label = "light-sphere-pipeline"
    });

    state::pass_action = sg_pass_action {
        .colors = {{
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = {0.2f, 0.2f, 0.2f, 1.0f},
        }}
    };
}

void frame(void) {
    
    state::deltaTime = static_cast<float>(stm_sec(stm_laptime(&state::last_time)));
    state::camera.moveCamera(state::deltaTime);

    // rotate light source
    state::light_pos = glm::vec3(1.3f * sinf(static_cast<float>(-stm_sec(stm_now()))),
                                 0.2f,
                                 1.3f * cosf(static_cast<float>(-stm_sec(stm_now()))));

    // rotate dark source
    state::dark_pos = glm::vec3(1.3f * sinf(stm_sec(stm_now()) * 2.5f),
                                 1.3f * sinf(stm_sec(stm_now()) * 2.5f + 3.0f),
                                 1.3f * cosf(stm_sec(stm_now()) * -2.5f));

    // change light colour
    /*state::light_colour = 0.5f * glm::vec3(sinf((stm_sec(stm_now()) * 2.0f))+1,*/
    /*                                sinf((stm_sec(stm_now()) * 0.7f))+1,*/
    /*                                sinf((stm_sec(stm_now()) * 1.3f))+1);*/

    // Movements

    if (!sapp_mouse_locked()) {
        sapp_lock_mouse(true);
    }

    sg_begin_pass(sg_pass {
        .action = state::pass_action,
        .swapchain = sglue_swapchain(),
    });

    glm::mat4 view = state::camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(sapp_width())/sapp_height(), 0.1f, 100.0f);
    vs_params_t vs_params {
        .view = view,
        .projection = projection,
    };


    sg_apply_pipeline(state::pip_object);
    sg_apply_bindings(state::bind);

    glm::mat4 model = glm::rotate(glm::mat4(1.0f), 
                                  static_cast<float>(stm_sec(stm_now())),
                                  glm::vec3(0.5f, 1.0f, 0.0f));
    vs_params.model = model;
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

    fs_params_t fs_params = {
        .viewPos {state::camera.pos}
    };
    sg_apply_uniforms(UB_fs_params, SG_RANGE(fs_params));

    fs_material_t fs_material = {
        .ambient = glm::vec3(0.8f, 0.8f, 0.8f),
        .diffuse = glm::vec3(0.8f, 0.8f, 0.8f),
        .specular = glm::vec3(0.8f, 0.8f, 0.8f),
        .shininess = 32.0f,
    };
    sg_apply_uniforms(UB_fs_material, SG_RANGE(fs_material));

    fs_light_t fs_light = {
        .position = state::light_pos,
        .ambient = state::light_colour * 0.4f,
        .diffuse = state::light_colour * 0.5f,
        .specular = state::light_colour * 1.0f,
    };
    sg_apply_uniforms(UB_fs_light, SG_RANGE(fs_light));

    fs_dark_t fs_dark = {
        .position = state::dark_pos,
        .diffuse = state::dark_colour * 0.5f,
    };
    sg_apply_uniforms(UB_fs_dark, SG_RANGE(fs_dark));

    sg_draw(0, 60, 1);

    // Prepare and draw Light Sphere
    sg_apply_pipeline(state::pip_light);
    sg_apply_bindings(state::bind);

    model = glm::translate(glm::mat4(1.0f), state::light_pos);
    model = glm::scale(model, glm::vec3(0.2f));
    vs_params.model = glm::rotate(model, 
                                  static_cast<float>(8*stm_sec(stm_now())),
                                  glm::vec3(0.1f, 1.0f, 0.5f));
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

    light_sphere_fs_params_t light_sphere_fs_params = {
        .lightColour {state::light_colour}
    };
    sg_apply_uniforms(UB_light_sphere_fs_params, SG_RANGE(light_sphere_fs_params));

    sg_draw(0, 60, 1);

    model = glm::translate(glm::mat4(1.0f), state::dark_pos);
    model = glm::scale(model, glm::vec3(0.2f));
    vs_params.model = glm::rotate(model, 
                                  static_cast<float>(8*stm_sec(stm_now())),
                                  glm::vec3(0.1f, 1.0f, 0.5f));
    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

    light_sphere_fs_params = {
        .lightColour {state::dark_colour}
    };
    sg_apply_uniforms(UB_light_sphere_fs_params, SG_RANGE(light_sphere_fs_params));

    sg_draw(0, 60, 1);

    sg_end_pass();
    sg_commit();
}

void cleanup() {
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

sapp_desc sokol_main([[maybe_unused]] int argc,[[maybe_unused]] char* argv[]) {
    return sapp_desc {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .high_dpi = true,
        .window_title = "Darksun (sokol)",
        .logger {
            .func = slog_func
        },
    };
}
