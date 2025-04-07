#include <cstdint>
#define SOKOL_IMPL
#ifndef __EMSCRIPTEN__
#define SOKOL_GLCORE
#else
#define SOKOL_GLES3
#endif
#include <sokol/sokol_app.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_glue.h>
#include "1-triangle.glsl.h"

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

    /* a vertex buffer with 3 vertices */
    float vertices[] = {
        // positions
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,      // bottom right
        -0.5f, -0.5f, 0.0f,     // bottom left
        -0.5f,  0.5f, 0.0f       // top
    };
    sg_buffer_desc sgbd_vbo {};
    sgbd_vbo.size = sizeof(vertices);
    sgbd_vbo.data = SG_RANGE(vertices);
    sgbd_vbo.label = "quad-vertices";
    state.bind.vertex_buffers[0] = sg_make_buffer(&sgbd_vbo);

    uint16_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    sg_buffer_desc sgbd_vao {};
    sgbd_vao.type = SG_BUFFERTYPE_INDEXBUFFER;
    sgbd_vao.size = sizeof(indices);
    sgbd_vao.data = SG_RANGE(indices);
    sgbd_vao.label = "quad-indices";
    state.bind.index_buffer = sg_make_buffer(&sgbd_vao);
    
    sg_pipeline_desc sgpld {};
    sgpld.shader = shd;
    sgpld.index_type = SG_INDEXTYPE_UINT16;
    sgpld.layout.attrs[ATTR_simple_position].format = SG_VERTEXFORMAT_FLOAT3;
    sgpld.label = "quad-pipeline";
    state.pip = sg_make_pipeline(&sgpld);

    sg_pass_action sgpa {};
    sgpa.colors[0].load_action=SG_LOADACTION_CLEAR;
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
    sg_draw(0, 6, 1);
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
    sad.window_title = "Quad - LearnOpenGL";
    return sad;
}
