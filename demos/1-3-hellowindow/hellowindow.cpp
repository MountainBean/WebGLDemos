#define SOKOL_IMPL
#define SOKOL_GLCORE
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_app.h>
#include <sokol/sokol_glue.h>
#include <sokol/sokol_log.h>

static struct {
    sg_pass_action pass_action;
} state;

void init() {
    sg_desc sgd {};
    sgd.environment = sglue_environment();
    sgd.logger.func = slog_func;
    sg_setup(&sgd);

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

sapp_desc sokol_main([[maybe_unused]] int argc,[[maybe_unused]] char* argv[]) {
    sapp_desc sad {};
    sad.init_cb = init;
    sad.frame_cb = frame;
    sad.cleanup_cb = cleanup;
    sad.event_cb = event;
    sad.width = 800;
    sad.height = 600;
    sad.high_dpi = true;
    sad.window_title = "Rendering - LearnOpenGL";
    return sad;
}

