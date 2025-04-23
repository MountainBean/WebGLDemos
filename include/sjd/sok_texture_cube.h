#ifndef SOK_TEXTURE_CUBE_H
#define SOK_TEXTURE_CUBE_H
#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif
#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
#include <string>
#define SOKOL_DEBUG
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_fetch.h>
#include <sokol/sokol_log.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#ifdef __clang__
// clangd doesn't like leaving struct elements value-initialised
#pragma clang diagnostic ignored "-Wmissing-designated-field-initializers"
#endif

static std::vector<uint8_t> TextureCubeFileBuffer {std::vector<uint8_t>(6*1024*1024)};
static uint32_t TextureCubeBufferOffset {1024 * 1024};

static const sg_sampler_desc TextureCubeSamplerDesc = {
    .min_filter = SG_FILTER_LINEAR,
    .mag_filter = SG_FILTER_LINEAR,
    .wrap_u = SG_WRAP_CLAMP_TO_EDGE,
    .wrap_v = SG_WRAP_CLAMP_TO_EDGE,
    .wrap_w = SG_WRAP_CLAMP_TO_EDGE,
    .compare = SG_COMPAREFUNC_NEVER,
};

class TextureCube {
public:
    struct imgRequest {
        sg_image imgId {};
        int cubeFace {};
        TextureCube* instance;
    };

    TextureCube(const std::array<std::string, 6>& paths, sg_bindings& bindings, uint16_t image_index, uint16_t smp_index, bool flip_vert=false, void(*fail_callback)() = nullptr, sg_sampler_desc* custom_sampler_desc=nullptr) {

        sg_alloc_image_smp(bindings, image_index, smp_index, custom_sampler_desc);
        stbi_set_flip_vertically_on_load(flip_vert);

        image = bindings.images[image_index];

        failCallback = fail_callback;


        imgRequest req_data {
            .imgId = image,
            .instance = this
        };

        for (int i {0}; i < 6; i++) {
            req_data.cubeFace = i;
            sfetch_send(sfetch_request_t {
                .path = paths[i].c_str(),
                .callback = fetch_callback,
                .buffer = sfetch_range_t {
                    .ptr = TextureCubeFileBuffer.data() + (i * TextureCubeBufferOffset),
                    .size = TextureCubeBufferOffset},
                .user_data = SFETCH_RANGE(req_data),    // user_data gets memcpy'd
            });
        }
    }
    static void fetch_callback(const sfetch_response_t* response);

    sg_image image {};
    int fetchedSizes[6] {};
    int finishedRequests {};
    bool failed {};
    void(*failCallback)();


    void sg_alloc_image_smp(sg_bindings& bindings, uint16_t image_index, uint16_t smp_index, sg_sampler_desc* custom_sampler_desc=nullptr) {
        bindings.images[image_index] = sg_alloc_image();
        bindings.samplers[smp_index] = sg_alloc_sampler();
        if (custom_sampler_desc)
            sg_init_sampler(bindings.samplers[smp_index], custom_sampler_desc);
        else
            sg_init_sampler(bindings.samplers[smp_index], TextureCubeSamplerDesc);
    }

private:
    static bool loadCubemap(TextureCube* texcube) {
        const int desired_channels = 4;
        std::array<int, 6> img_widths;
        std::array<int, 6> img_heights;
        std::array<stbi_uc*, 6> pixels_ptrs;
        sg_image_data img_content;

        for (int i = 0; i < 6; ++i) {
            int num_channel;
            pixels_ptrs[i] = stbi_load_from_memory(
                static_cast<const stbi_uc*>(TextureCubeFileBuffer.data() + (i * TextureCubeBufferOffset)),
                texcube->fetchedSizes[i],
                &img_widths[i], &img_heights[i],
                &num_channel, desired_channels);

            img_content.subimage[i][0].ptr = pixels_ptrs[i];
            img_content.subimage[i][0].size = img_widths[i] * img_heights[i] * desired_channels;
        }

        bool valid = img_widths[0] > 0 && img_heights[0] > 0;

        for (int i = 1; i < 6; ++i) {
            if (img_widths[i] != img_widths[0] || img_heights[i] != img_heights[0]) {
                valid = false;
                break;
            }
        }

        if (valid) {
            /* initialize the sokol-gfx texture */
            sg_init_image(texcube->image, sg_image_desc {
                .type = SG_IMAGETYPE_CUBE,
                .width = img_widths[0],
                .height = img_heights[0],
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .data = img_content
            });
        }

        for (int i = 0; i < 6; ++i) {
            stbi_image_free(pixels_ptrs[i]);
        }

        return valid;

    }

};

inline void TextureCube::fetch_callback(const sfetch_response_t* response) {
    imgRequest request = *(imgRequest*)response->user_data;
    TextureCube* texcube = request.instance;

    if (response->fetched) {
        texcube->fetchedSizes[request.cubeFace] = response->data.size;
    }
    else if (response->failed) {
        texcube->failed = true;
    }
    ++texcube->finishedRequests;

    if (texcube->finishedRequests == 6) {
        if (!texcube->failed) {
            texcube->failed = !loadCubemap(texcube);
        }
        
        if (texcube->failed) {
            texcube->failCallback();
        }
    }
}


#endif
