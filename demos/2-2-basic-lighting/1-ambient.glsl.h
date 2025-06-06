#pragma once
/*
    #version:1# (machine generated, don't edit!)

    Generated by sokol-shdc (https://github.com/floooh/sokol-tools)

    Cmdline:
        sokol-shdc -i .\1-ambient.glsl -o .\1-ambient.glsl.h -l glsl430:glsl300es

    Overview:
    =========
    Shader program: 'ambient':
        Get shader desc: ambient_shader_desc(sg_query_backend());
        Vertex Shader: vs
        Fragment Shader: fs
        Attributes:
            ATTR_ambient_aPos => 0
    Shader program: 'light_cube':
        Get shader desc: light_cube_shader_desc(sg_query_backend());
        Vertex Shader: vs
        Fragment Shader: light_cube_fs
        Attributes:
            ATTR_light_cube_aPos => 0
    Bindings:
        Uniform block 'vs_params':
            C struct: vs_params_t
            Bind slot: UB_vs_params => 0
        Uniform block 'fs_params':
            C struct: fs_params_t
            Bind slot: UB_fs_params => 1
        Uniform block 'light_cube_fs_params':
            C struct: light_cube_fs_params_t
            Bind slot: UB_light_cube_fs_params => 1
*/
#if !defined(SOKOL_GFX_INCLUDED)
#error "Please include sokol_gfx.h before 1-ambient.glsl.h"
#endif
#if !defined(SOKOL_SHDC_ALIGN)
#if defined(_MSC_VER)
#define SOKOL_SHDC_ALIGN(a) __declspec(align(a))
#else
#define SOKOL_SHDC_ALIGN(a) __attribute__((aligned(a)))
#endif
#endif
#define ATTR_ambient_aPos (0)
#define ATTR_light_cube_aPos (0)
#define UB_vs_params (0)
#define UB_fs_params (1)
#define UB_light_cube_fs_params (1)
#pragma pack(push,1)
SOKOL_SHDC_ALIGN(16) typedef struct vs_params_t {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
} vs_params_t;
#pragma pack(pop)
#pragma pack(push,1)
SOKOL_SHDC_ALIGN(16) typedef struct fs_params_t {
    glm::vec3 objectColour;
    uint8_t _pad_12[4];
    glm::vec3 lightColour;
    float ambientStrength;
} fs_params_t;
#pragma pack(pop)
#pragma pack(push,1)
SOKOL_SHDC_ALIGN(16) typedef struct light_cube_fs_params_t {
    glm::vec3 lightColour;
    uint8_t _pad_12[4];
} light_cube_fs_params_t;
#pragma pack(pop)
/*
    #version 430

    uniform vec4 vs_params[12];
    layout(location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = ((mat4(vs_params[8], vs_params[9], vs_params[10], vs_params[11]) * mat4(vs_params[4], vs_params[5], vs_params[6], vs_params[7])) * mat4(vs_params[0], vs_params[1], vs_params[2], vs_params[3])) * vec4(aPos, 1.0);
    }

*/
static const uint8_t vs_source_glsl430[326] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x34,0x33,0x30,0x0a,0x0a,0x75,0x6e,
    0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x73,0x5f,0x70,0x61,
    0x72,0x61,0x6d,0x73,0x5b,0x31,0x32,0x5d,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,
    0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x69,
    0x6e,0x20,0x76,0x65,0x63,0x33,0x20,0x61,0x50,0x6f,0x73,0x3b,0x0a,0x0a,0x76,0x6f,
    0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,
    0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x28,0x28,
    0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x38,
    0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x39,0x5d,0x2c,
    0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x30,0x5d,0x2c,0x20,
    0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x31,0x5d,0x29,0x20,0x2a,
    0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,
    0x34,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x35,0x5d,
    0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x36,0x5d,0x2c,0x20,
    0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x37,0x5d,0x29,0x29,0x20,0x2a,
    0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,
    0x30,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x5d,
    0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x32,0x5d,0x2c,0x20,
    0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x33,0x5d,0x29,0x29,0x20,0x2a,
    0x20,0x76,0x65,0x63,0x34,0x28,0x61,0x50,0x6f,0x73,0x2c,0x20,0x31,0x2e,0x30,0x29,
    0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #version 430

    uniform vec4 fs_params[2];
    layout(location = 0) out vec4 FragColor;

    void main()
    {
        FragColor = vec4((fs_params[1].xyz * fs_params[1].w) * fs_params[0].xyz, 1.0);
    }

*/
static const uint8_t fs_source_glsl430[184] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x34,0x33,0x30,0x0a,0x0a,0x75,0x6e,
    0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x66,0x73,0x5f,0x70,0x61,
    0x72,0x61,0x6d,0x73,0x5b,0x32,0x5d,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,
    0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x6f,0x75,
    0x74,0x20,0x76,0x65,0x63,0x34,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,
    0x3b,0x0a,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,
    0x0a,0x20,0x20,0x20,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,
    0x20,0x76,0x65,0x63,0x34,0x28,0x28,0x66,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,
    0x5b,0x31,0x5d,0x2e,0x78,0x79,0x7a,0x20,0x2a,0x20,0x66,0x73,0x5f,0x70,0x61,0x72,
    0x61,0x6d,0x73,0x5b,0x31,0x5d,0x2e,0x77,0x29,0x20,0x2a,0x20,0x66,0x73,0x5f,0x70,
    0x61,0x72,0x61,0x6d,0x73,0x5b,0x30,0x5d,0x2e,0x78,0x79,0x7a,0x2c,0x20,0x31,0x2e,
    0x30,0x29,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #version 430

    uniform vec4 light_cube_fs_params[1];
    layout(location = 0) out vec4 FragColor;

    void main()
    {
        FragColor = vec4(light_cube_fs_params[0].xyz, 1.0);
    }

*/
static const uint8_t light_cube_fs_source_glsl430[168] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x34,0x33,0x30,0x0a,0x0a,0x75,0x6e,
    0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x6c,0x69,0x67,0x68,0x74,
    0x5f,0x63,0x75,0x62,0x65,0x5f,0x66,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,
    0x31,0x5d,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,
    0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,
    0x34,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x0a,0x76,0x6f,
    0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,
    0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,
    0x28,0x6c,0x69,0x67,0x68,0x74,0x5f,0x63,0x75,0x62,0x65,0x5f,0x66,0x73,0x5f,0x70,
    0x61,0x72,0x61,0x6d,0x73,0x5b,0x30,0x5d,0x2e,0x78,0x79,0x7a,0x2c,0x20,0x31,0x2e,
    0x30,0x29,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #version 300 es

    uniform vec4 vs_params[12];
    layout(location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = ((mat4(vs_params[8], vs_params[9], vs_params[10], vs_params[11]) * mat4(vs_params[4], vs_params[5], vs_params[6], vs_params[7])) * mat4(vs_params[0], vs_params[1], vs_params[2], vs_params[3])) * vec4(aPos, 1.0);
    }

*/
static const uint8_t vs_source_glsl300es[329] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x30,0x30,0x20,0x65,0x73,0x0a,
    0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x73,
    0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x32,0x5d,0x3b,0x0a,0x6c,0x61,0x79,
    0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,
    0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x33,0x20,0x61,0x50,0x6f,0x73,0x3b,0x0a,
    0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,
    0x20,0x20,0x20,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x20,0x3d,
    0x20,0x28,0x28,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,
    0x73,0x5b,0x38,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,
    0x39,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x30,
    0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x31,0x5d,
    0x29,0x20,0x2a,0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,
    0x6d,0x73,0x5b,0x34,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,
    0x5b,0x35,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x36,
    0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x37,0x5d,0x29,
    0x29,0x20,0x2a,0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,
    0x6d,0x73,0x5b,0x30,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,
    0x5b,0x31,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x32,
    0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x33,0x5d,0x29,
    0x29,0x20,0x2a,0x20,0x76,0x65,0x63,0x34,0x28,0x61,0x50,0x6f,0x73,0x2c,0x20,0x31,
    0x2e,0x30,0x29,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #version 300 es
    precision mediump float;
    precision highp int;

    uniform highp vec4 fs_params[2];
    layout(location = 0) out highp vec4 FragColor;

    void main()
    {
        FragColor = vec4((fs_params[1].xyz * fs_params[1].w) * fs_params[0].xyz, 1.0);
    }

*/
static const uint8_t fs_source_glsl300es[245] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x30,0x30,0x20,0x65,0x73,0x0a,
    0x70,0x72,0x65,0x63,0x69,0x73,0x69,0x6f,0x6e,0x20,0x6d,0x65,0x64,0x69,0x75,0x6d,
    0x70,0x20,0x66,0x6c,0x6f,0x61,0x74,0x3b,0x0a,0x70,0x72,0x65,0x63,0x69,0x73,0x69,
    0x6f,0x6e,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x69,0x6e,0x74,0x3b,0x0a,0x0a,0x75,
    0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,
    0x34,0x20,0x66,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x32,0x5d,0x3b,0x0a,
    0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,
    0x3d,0x20,0x30,0x29,0x20,0x6f,0x75,0x74,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,
    0x65,0x63,0x34,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x0a,
    0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,
    0x20,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,
    0x63,0x34,0x28,0x28,0x66,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x5d,
    0x2e,0x78,0x79,0x7a,0x20,0x2a,0x20,0x66,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,
    0x5b,0x31,0x5d,0x2e,0x77,0x29,0x20,0x2a,0x20,0x66,0x73,0x5f,0x70,0x61,0x72,0x61,
    0x6d,0x73,0x5b,0x30,0x5d,0x2e,0x78,0x79,0x7a,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,
    0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #version 300 es
    precision mediump float;
    precision highp int;

    uniform highp vec4 light_cube_fs_params[1];
    layout(location = 0) out highp vec4 FragColor;

    void main()
    {
        FragColor = vec4(light_cube_fs_params[0].xyz, 1.0);
    }

*/
static const uint8_t light_cube_fs_source_glsl300es[229] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x30,0x30,0x20,0x65,0x73,0x0a,
    0x70,0x72,0x65,0x63,0x69,0x73,0x69,0x6f,0x6e,0x20,0x6d,0x65,0x64,0x69,0x75,0x6d,
    0x70,0x20,0x66,0x6c,0x6f,0x61,0x74,0x3b,0x0a,0x70,0x72,0x65,0x63,0x69,0x73,0x69,
    0x6f,0x6e,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x69,0x6e,0x74,0x3b,0x0a,0x0a,0x75,
    0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,
    0x34,0x20,0x6c,0x69,0x67,0x68,0x74,0x5f,0x63,0x75,0x62,0x65,0x5f,0x66,0x73,0x5f,
    0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x5d,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,
    0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,
    0x6f,0x75,0x74,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,0x34,0x20,0x46,
    0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x0a,0x76,0x6f,0x69,0x64,0x20,
    0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x46,0x72,0x61,
    0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,0x28,0x6c,0x69,
    0x67,0x68,0x74,0x5f,0x63,0x75,0x62,0x65,0x5f,0x66,0x73,0x5f,0x70,0x61,0x72,0x61,
    0x6d,0x73,0x5b,0x30,0x5d,0x2e,0x78,0x79,0x7a,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,
    0x0a,0x7d,0x0a,0x0a,0x00,
};
static inline const sg_shader_desc* ambient_shader_desc(sg_backend backend) {
    if (backend == SG_BACKEND_GLCORE) {
        static sg_shader_desc desc;
        static bool valid;
        if (!valid) {
            valid = true;
            desc.vertex_func.source = (const char*)vs_source_glsl430;
            desc.vertex_func.entry = "main";
            desc.fragment_func.source = (const char*)fs_source_glsl430;
            desc.fragment_func.entry = "main";
            desc.attrs[0].base_type = SG_SHADERATTRBASETYPE_FLOAT;
            desc.attrs[0].glsl_name = "aPos";
            desc.uniform_blocks[0].stage = SG_SHADERSTAGE_VERTEX;
            desc.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[0].size = 192;
            desc.uniform_blocks[0].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[0].glsl_uniforms[0].array_count = 12;
            desc.uniform_blocks[0].glsl_uniforms[0].glsl_name = "vs_params";
            desc.uniform_blocks[1].stage = SG_SHADERSTAGE_FRAGMENT;
            desc.uniform_blocks[1].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[1].size = 32;
            desc.uniform_blocks[1].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[1].glsl_uniforms[0].array_count = 2;
            desc.uniform_blocks[1].glsl_uniforms[0].glsl_name = "fs_params";
            desc.label = "ambient_shader";
        }
        return &desc;
    }
    if (backend == SG_BACKEND_GLES3) {
        static sg_shader_desc desc;
        static bool valid;
        if (!valid) {
            valid = true;
            desc.vertex_func.source = (const char*)vs_source_glsl300es;
            desc.vertex_func.entry = "main";
            desc.fragment_func.source = (const char*)fs_source_glsl300es;
            desc.fragment_func.entry = "main";
            desc.attrs[0].base_type = SG_SHADERATTRBASETYPE_FLOAT;
            desc.attrs[0].glsl_name = "aPos";
            desc.uniform_blocks[0].stage = SG_SHADERSTAGE_VERTEX;
            desc.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[0].size = 192;
            desc.uniform_blocks[0].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[0].glsl_uniforms[0].array_count = 12;
            desc.uniform_blocks[0].glsl_uniforms[0].glsl_name = "vs_params";
            desc.uniform_blocks[1].stage = SG_SHADERSTAGE_FRAGMENT;
            desc.uniform_blocks[1].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[1].size = 32;
            desc.uniform_blocks[1].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[1].glsl_uniforms[0].array_count = 2;
            desc.uniform_blocks[1].glsl_uniforms[0].glsl_name = "fs_params";
            desc.label = "ambient_shader";
        }
        return &desc;
    }
    return 0;
}
static inline const sg_shader_desc* light_cube_shader_desc(sg_backend backend) {
    if (backend == SG_BACKEND_GLCORE) {
        static sg_shader_desc desc;
        static bool valid;
        if (!valid) {
            valid = true;
            desc.vertex_func.source = (const char*)vs_source_glsl430;
            desc.vertex_func.entry = "main";
            desc.fragment_func.source = (const char*)light_cube_fs_source_glsl430;
            desc.fragment_func.entry = "main";
            desc.attrs[0].base_type = SG_SHADERATTRBASETYPE_FLOAT;
            desc.attrs[0].glsl_name = "aPos";
            desc.uniform_blocks[0].stage = SG_SHADERSTAGE_VERTEX;
            desc.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[0].size = 192;
            desc.uniform_blocks[0].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[0].glsl_uniforms[0].array_count = 12;
            desc.uniform_blocks[0].glsl_uniforms[0].glsl_name = "vs_params";
            desc.uniform_blocks[1].stage = SG_SHADERSTAGE_FRAGMENT;
            desc.uniform_blocks[1].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[1].size = 16;
            desc.uniform_blocks[1].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[1].glsl_uniforms[0].array_count = 1;
            desc.uniform_blocks[1].glsl_uniforms[0].glsl_name = "light_cube_fs_params";
            desc.label = "light_cube_shader";
        }
        return &desc;
    }
    if (backend == SG_BACKEND_GLES3) {
        static sg_shader_desc desc;
        static bool valid;
        if (!valid) {
            valid = true;
            desc.vertex_func.source = (const char*)vs_source_glsl300es;
            desc.vertex_func.entry = "main";
            desc.fragment_func.source = (const char*)light_cube_fs_source_glsl300es;
            desc.fragment_func.entry = "main";
            desc.attrs[0].base_type = SG_SHADERATTRBASETYPE_FLOAT;
            desc.attrs[0].glsl_name = "aPos";
            desc.uniform_blocks[0].stage = SG_SHADERSTAGE_VERTEX;
            desc.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[0].size = 192;
            desc.uniform_blocks[0].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[0].glsl_uniforms[0].array_count = 12;
            desc.uniform_blocks[0].glsl_uniforms[0].glsl_name = "vs_params";
            desc.uniform_blocks[1].stage = SG_SHADERSTAGE_FRAGMENT;
            desc.uniform_blocks[1].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[1].size = 16;
            desc.uniform_blocks[1].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[1].glsl_uniforms[0].array_count = 1;
            desc.uniform_blocks[1].glsl_uniforms[0].glsl_name = "light_cube_fs_params";
            desc.label = "light_cube_shader";
        }
        return &desc;
    }
    return 0;
}
