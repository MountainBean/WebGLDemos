#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sjd/camera.h>
#include <sjd/sok_texture.h>

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
#include "1-multiple-lights.glsl.h"

#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

namespace state {
    sg_pipeline pip_object;
    sg_pipeline pip_light;
    sg_bindings bind_object;
    sg_bindings bind_light;
    sg_pass_action pass_action;
    std::vector<glm::vec3> cube_positions;
    glm::vec3 dirLight_colour;
    glm::vec3 spotLight_colour;
    std::vector<glm::vec3> light_colours;
    std::vector<glm::vec3> light_positions;
    sjd::Camera camera(glm::vec3(0.2f, 0.8f, 4.0f));
    bool spotlight {true};
    uint64_t last_time;
    float deltaTime;
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

    state::light_colours = {
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
    };

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
        glm::vec3(-1.3f,  1.0f, -1.5f),
    };

    std::vector<float> vertices {
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
    state::light_positions = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    state::light_colours = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.1f, 0.1f),
        glm::vec3(0.8f, 0.5f, 0.1f),
        glm::vec3(0.9f, 0.9f, 0.1f),
    };

    sg_buffer cube_buffer = sg_make_buffer(sg_buffer_desc {
        .size = vertices.size() * sizeof(float),
        .data = sg_range {
            .ptr = vertices.data(),
            .size = vertices.size() * sizeof(float),
        },
        .label = "cube-vertices"
    });

    state::bind_object.vertex_buffers[0] = cube_buffer;
    state::bind_light.vertex_buffers[0] = cube_buffer;

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
    // create shader from code-generated sg_shader_desc
    sg_shader light_cube_shd = sg_make_shader(light_cube_shader_desc(sg_query_backend()));

    // create a pipeline object (default render state:: are fine for triangle)
    state::pip_light = sg_make_pipeline(sg_pipeline_desc {
        .shader = light_cube_shd,
        .layout = layout,
        .depth {    // Our first 3D elements so we need to enable depth testing
            .compare = SG_COMPAREFUNC_LESS_EQUAL,   // discard fragments that are further away
            .write_enabled = true,
        },
        .label = "light-pipeline"
    });

    // a pass action to clear framebuffer
    state::pass_action = sg_pass_action {
        .colors = {{
	    .load_action=SG_LOADACTION_CLEAR,
	    .clear_value={0.1f, 0.1f, 0.1f, 1.0f} 
	}}
    };
    
    SokTexture container2("../data/container2.png",
                          state::bind_object,
                          IMG__diffuse_texture,
                          SMP_diffuse_texture_smp,
                          true,
                          fail_callback);

    SokTexture container2_specular("../data/container2_specular.png",
                                   state::bind_object,
                                   IMG__specular_texture,
                                   SMP_specular_texture_smp,
                                   true,
                                   fail_callback);

}

void frame(void) {
    sfetch_dowork();

    state::deltaTime = static_cast<float>(stm_sec(stm_laptime(&state::last_time)));

    if (state::spotlight)
        state::spotLight_colour = glm::vec3(1.0f);
    else
        state::spotLight_colour = glm::vec3(0.0f);

    state::dirLight_colour = glm::vec3(0.2f);

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


    // Prepare and draw object
    sg_apply_pipeline(state::pip_object);
    sg_apply_bindings(state::bind_object);


    fs_params_t fs_params = {
        .viewPos {state::camera.pos}
    };
    sg_apply_uniforms(UB_fs_params, SG_RANGE(fs_params));

    fs_material_t fs_material = {
        .shininess = 32.0f,
    };
    sg_apply_uniforms(UB_fs_material, SG_RANGE(fs_material));

    fs_dir_light_t fs_dir_light = {
        .direction = glm::vec3(-0.2f, -1.0f, -0.3f),
        .ambient = state::dirLight_colour * 0.2f,
        .diffuse = state::dirLight_colour * 0.5f,
        .specular = state::dirLight_colour * 1.0f,
    };
    sg_apply_uniforms(UB_fs_dir_light, SG_RANGE(fs_dir_light));

    fs_point_lights_t fs_point_lights {};
    for (size_t i {0}; i < state::light_positions.size(); i++) {
        fs_point_lights.position[i] = glm::vec4(state::light_positions[i], 1.0f);
        fs_point_lights.ambient[i] = glm::vec4(state::light_colours[i] * 0.2f, 1.0f);
        fs_point_lights.diffuse[i] = glm::vec4(state::light_colours[i] * 0.5f, 1.0f);
        fs_point_lights.specular[i] = glm::vec4(state::light_colours[i] * 1.0f, 1.0f);
        fs_point_lights.attenuation[i] = glm::vec4(1.0f, 0.09f, 0.032f, 1.0f);
    };
    sg_apply_uniforms(UB_fs_point_lights, SG_RANGE(fs_point_lights));

    fs_spot_light_t fs_spot_light = {
        .position = state::camera.pos,
        .direction = state::camera.front,
        .cutOff = glm::cos(glm::radians(12.5f)),
        .outerCutOff = glm::cos(glm::radians(17.5f)),
        .ambient = state::spotLight_colour * 0.2f,
        .diffuse = state::spotLight_colour * 0.5f,
        .specular = state::spotLight_colour * 1.0f,
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032f,

    };
    sg_apply_uniforms(UB_fs_spot_light, SG_RANGE(fs_spot_light));

    int i {0};
    for (glm::vec3 position : state::cube_positions) {

        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        float angle = 20.0f * i;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
        vs_params.model = model;

        sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

        sg_draw(0, 36, 1);
        ++i;
    }

    // Prepare and draw object
    sg_apply_pipeline(state::pip_light);
    sg_apply_bindings(state::bind_light);
    
    i = 0;
    for (glm::vec3 light_pos : state::light_positions) {
        light_cube_fs_params_t light_cube_fs_params = {
            .lightColour {state::light_colours[i]}
        };
        sg_apply_uniforms(UB_light_cube_fs_params, SG_RANGE(light_cube_fs_params));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), light_pos);
        model = glm::scale(model, glm::vec3(0.2f));
        vs_params.model = model;
        sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

        sg_draw(0, 36, 1);
        ++i;
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
        if (e->key_code == SAPP_KEYCODE_F) {
            state::spotlight = !state::spotlight;
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
        .window_title = "Specular Map - LearnOpenGL",
        .logger {
            .func = slog_func
        },
#ifdef _WIN32
        .win32_console_utf8 = true,
        .win32_console_attach = true,
#endif
    };
}

