#define SOKOL_IMPL
#ifndef __EMSCRIPTEN__
#define SOKOL_GLCORE
#else
// we're compileing for webgl
#define SOKOL_GLES3
#endif
#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_log.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sjd/icosahedron.h>

#include "balls.glsl.h"

namespace state {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
}

namespace alternate {
sg_pipeline pip_triangles;
sg_pipeline pip_lines;
sg_buffer bind_indices_triangles;
sg_buffer bind_indices_lines;
}

namespace keys {
bool F_KEY {false};
}

void init(void) {
    sg_setup(sg_desc {
        .environment = sglue_environment()
    });

    // initialise sokol time
    stm_setup();

    // get vertices
    sjd::Icosahedron myShape {};

    state::bind.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(myShape.getVertices()),
        .data = SG_RANGE(myShape.getVertices()),
        .label = "icosahedron-vertices",
    });

    alternate::bind_indices_triangles = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(myShape.getIndices()),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(myShape.getIndices()),
        .label = "icosahedron-indices",
    });
    alternate::bind_indices_lines = sg_make_buffer(sg_buffer_desc {
        .size = sizeof(myShape.getLineIndices()),
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(myShape.getLineIndices()),
        .label = "icosahedron-indices",
    });
    // set starting index_buffer
    state::bind.index_buffer = alternate::bind_indices_lines;

    // create shader
    sg_shader shd = sg_make_shader(balls_shader_desc(sg_query_backend()));

    // default pipeline renders triangle primatives
    alternate::pip_triangles = sg_make_pipeline(sg_pipeline_desc {
        .shader = shd,
        .layout = {
            .attrs = {{.format = SG_VERTEXFORMAT_FLOAT3}},
        },
        .depth = {
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true,
        },
        .index_type = SG_INDEXTYPE_UINT16,
        .label = "triangles-pipeline",
    });

    // alt pipeline renders line primatives
    alternate::pip_lines = sg_make_pipeline(sg_pipeline_desc {
        .shader = shd,
        .layout = {
            .attrs = {{.format = SG_VERTEXFORMAT_FLOAT3}},
        },
        .depth = {
            .compare = SG_COMPAREFUNC_LESS_EQUAL,
            .write_enabled = true,
        },
        .primitive_type = SG_PRIMITIVETYPE_LINES,
        .index_type = SG_INDEXTYPE_UINT16,
        .label = "lines-pipeline",
    });

    state::pip = alternate::pip_lines;


    state::pass_action = sg_pass_action {
        .colors = {{
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = {0.2f, 0.2f, 0.2f, 1.0f},
        }}
    };
}

void frame(void) {
    
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), 
                                  static_cast<float>(stm_sec(stm_now())),
                                  glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(sapp_width())/sapp_height(), 0.1f, 100.0f);


    sg_begin_pass(sg_pass {
        .action = state::pass_action,
        .swapchain = sglue_swapchain(),
    });
    sg_apply_pipeline(state::pip);
    sg_apply_bindings(state::bind);

    vs_params_t vs_params {
        .model = model,
        .view = view,
        .projection = projection,
    };

    sg_apply_uniforms(UB_vs_params, SG_RANGE(vs_params));

    sg_draw(0, 60, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup() {
    sg_shutdown();
}

void swap_prims() {
    if (state::pip.id == alternate::pip_triangles.id) {
        state::pip = alternate::pip_lines;
        state::bind.index_buffer = alternate::bind_indices_lines;
    } else {
        state::pip = alternate::pip_triangles;
        state::bind.index_buffer = alternate::bind_indices_triangles;
    }
}

void event(const sapp_event* e) {
    if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (e->key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
        else if (e->key_code == SAPP_KEYCODE_F) {
            keys::F_KEY = true;
        }
    }
    else if (e -> type == SAPP_EVENTTYPE_KEY_UP) {
        if (e -> key_code == SAPP_KEYCODE_F && keys::F_KEY) {
        // F_KEY was previously pressed (debouncing)
            keys::F_KEY = false;
            swap_prims();
        }
    }
    else if (e -> type == SAPP_EVENTTYPE_TOUCHES_BEGAN) {
            keys::F_KEY = true;
    }
    else if (e -> type == SAPP_EVENTTYPE_TOUCHES_ENDED) {
        if (keys::F_KEY) {
        // Screen was previously pressed (debouncing)
            keys::F_KEY = false;
            swap_prims();
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
        .window_title = "Icosohedron (sokol)",
    };
}
