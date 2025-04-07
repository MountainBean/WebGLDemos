#ifndef SOK_TEXTURE_H
#define SOK_TEXTURE_H
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_fetch.h>
#include <array>
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

static void fetch_callback(const sfetch_response_t* response);

static const sg_sampler_desc global_sampler_desc = {
    .min_filter = SG_FILTER_LINEAR,
    .mag_filter = SG_FILTER_LINEAR,
    .wrap_u = SG_WRAP_REPEAT,
    .wrap_v = SG_WRAP_REPEAT,
    .compare = SG_COMPAREFUNC_NEVER,
};

class SokTexture {
public:

    SokTexture(const std::string& path, sg_bindings& bindings, uint16_t image_index, uint16_t smp_index, bool flip_vert=false) {

        sg_alloc_image_smp(bindings, image_index, smp_index);
        stbi_set_flip_vertically_on_load(flip_vert);

        image = bindings.images[image_index];

        // start loading the given file 
        sfetch_send(sfetch_request_t {
            .path = path.c_str(),
            .callback = fetch_callback,
            .buffer = SFETCH_RANGE(file_buffer),
            .user_data = SFETCH_RANGE(image),
        });
    }

    sg_image image {};
    std::array<uint8_t, 512 * 1028> file_buffer;

private:
    void sg_alloc_image_smp(sg_bindings& bindings, uint16_t image_index, uint16_t smp_index) {
        bindings.images[image_index] = sg_alloc_image();
        bindings.samplers[smp_index] = sg_alloc_sampler();
        sg_init_sampler(bindings.samplers[smp_index], global_sampler_desc);
    }
};

static void fetch_callback(const sfetch_response_t* response) {
    if (response->fetched) {
        int img_width;
        int img_height;
        int nrChannels;
        const int desired_channels = 4;
        stbi_uc* pixels = stbi_load_from_memory(
            static_cast<const stbi_uc*>(response -> data.ptr),
            static_cast<int>(response -> data.size),
            &img_width, &img_height,
            &nrChannels, desired_channels);
        if (pixels) {
            sg_image image = *static_cast<sg_image*>(response->user_data);
            sg_init_image(image, sg_image_desc {
                .width = img_width,
                .height = img_height,
                // set pixel_format to RGBA8 for WebGL
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .data = {
                    .subimage = {{{
                        .ptr = pixels,
                        .size = static_cast<size_t>(img_width * img_height * 4),
                    }}}
                }
            });
            stbi_image_free(pixels);
        }
    }
    else if (response->failed) {
        std::cout << "ERROR::TEXTURE::Failed to load texture" << std::endl;
    }
}


#endif
