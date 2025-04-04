#define SOKOL_IMPL
#ifndef __EMSCRIPTEN__
#define SOKOL_GLCORE
#else
#define SOKOL_GLES3
#endif
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "1-in-out.glsl.h"
#include <cstdio>

/* application state */
static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

static void init(void) {
    sg_desc sgd {};
    sgd.environment = sglue_environment();
    sg_setup(&sgd);

    sg_shader shd = sg_make_shader(simple_shader_desc(sg_query_backend()));

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    sg_buffer_desc sgb_vbo {};
    sgb_vbo.size = sizeof(vertices);
    sgb_vbo.data = SG_RANGE(vertices);
    sgb_vbo.label = "triangle-vertices";
    state.bind.vertex_buffers[0] = sg_make_buffer(&sgb_vbo);

    sg_pipeline_desc sgpld {};
    sgpld.shader = shd;
    sgpld.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
    sgpld.label = "triangle-pipeline";
    state.pip = sg_make_pipeline(&sgpld);

    sg_pass_action sgpa {};
    sgpa.colors[0].load_action = SG_LOADACTION_CLEAR;
    sgpa.colors[0].clear_value={0.2f, 0.3f, 0.3f, 1.0f};
    state.pass_action = sgpa;
}

void frame() {
    sg_pass sgp {};
    sgp.action = state.pass_action;
    sgp.swapchain = sglue_swapchain();
    sg_begin_pass(&sgp);
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&state.bind);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup() {
    sg_shutdown();
}

void event(const sapp_event* e) {
    if (e -> type == SAPP_EVENTTYPE_KEY_DOWN) {
        if (e -> key_code == SAPP_KEYCODE_ESCAPE) {
            sapp_request_quit();
        }
    }
}

sapp_desc sokol_main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    sapp_desc sad {};
    sad.init_cb = init;
    sad.frame_cb = frame;
    sad.cleanup_cb = cleanup;
    sad.event_cb = event;
    sad.width = 800;
    sad.height = 600;
    sad.high_dpi = true;
    sad.window_title = "In Out - LearnOpenGL";
    return sad;
}

