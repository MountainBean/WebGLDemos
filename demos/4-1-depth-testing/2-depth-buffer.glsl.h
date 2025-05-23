#pragma once
/*
    #version:1# (machine generated, don't edit!)

    Generated by sokol-shdc (https://github.com/floooh/sokol-tools)

    Cmdline:
        sokol-shdc -i .\2-depth-buffer.glsl -o .\2-depth-buffer.glsl.h -l glsl430:glsl300es

    Overview:
    =========
    Shader program: 'simple':
        Get shader desc: simple_shader_desc(sg_query_backend());
        Vertex Shader: vs
        Fragment Shader: fs
        Attributes:
            ATTR_simple_aPos => 0
            ATTR_simple_aTexCoords => 1
    Bindings:
        Uniform block 'vs_params':
            C struct: vs_params_t
            Bind slot: UB_vs_params => 0
*/
#if !defined(SOKOL_GFX_INCLUDED)
#error "Please include sokol_gfx.h before 2-depth-buffer.glsl.h"
#endif
#if !defined(SOKOL_SHDC_ALIGN)
#if defined(_MSC_VER)
#define SOKOL_SHDC_ALIGN(a) __declspec(align(a))
#else
#define SOKOL_SHDC_ALIGN(a) __attribute__((aligned(a)))
#endif
#endif
#define ATTR_simple_aPos (0)
#define ATTR_simple_aTexCoords (1)
#define UB_vs_params (0)
#pragma pack(push,1)
SOKOL_SHDC_ALIGN(16) typedef struct vs_params_t {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
} vs_params_t;
#pragma pack(pop)
/*
    #version 430

    uniform vec4 vs_params[12];
    layout(location = 0) in vec3 aPos;
    layout(location = 0) out vec2 TexCoords;
    layout(location = 1) in vec2 aTexCoords;

    void main()
    {
        gl_Position = ((mat4(vs_params[8], vs_params[9], vs_params[10], vs_params[11]) * mat4(vs_params[4], vs_params[5], vs_params[6], vs_params[7])) * mat4(vs_params[0], vs_params[1], vs_params[2], vs_params[3])) * vec4(aPos, 1.0);
        TexCoords = aTexCoords;
    }

*/
static const uint8_t vs_source_glsl430[436] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x34,0x33,0x30,0x0a,0x0a,0x75,0x6e,
    0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x73,0x5f,0x70,0x61,
    0x72,0x61,0x6d,0x73,0x5b,0x31,0x32,0x5d,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,
    0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x69,
    0x6e,0x20,0x76,0x65,0x63,0x33,0x20,0x61,0x50,0x6f,0x73,0x3b,0x0a,0x6c,0x61,0x79,
    0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,
    0x29,0x20,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x32,0x20,0x54,0x65,0x78,0x43,0x6f,
    0x6f,0x72,0x64,0x73,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,
    0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x31,0x29,0x20,0x69,0x6e,0x20,0x76,0x65,
    0x63,0x32,0x20,0x61,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x73,0x3b,0x0a,0x0a,
    0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,
    0x20,0x20,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,
    0x28,0x28,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,
    0x5b,0x38,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x39,
    0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x30,0x5d,
    0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x31,0x5d,0x29,
    0x20,0x2a,0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,
    0x73,0x5b,0x34,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,
    0x35,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x36,0x5d,
    0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x37,0x5d,0x29,0x29,
    0x20,0x2a,0x20,0x6d,0x61,0x74,0x34,0x28,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,
    0x73,0x5b,0x30,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,
    0x31,0x5d,0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x32,0x5d,
    0x2c,0x20,0x76,0x73,0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x33,0x5d,0x29,0x29,
    0x20,0x2a,0x20,0x76,0x65,0x63,0x34,0x28,0x61,0x50,0x6f,0x73,0x2c,0x20,0x31,0x2e,
    0x30,0x29,0x3b,0x0a,0x20,0x20,0x20,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,
    0x73,0x20,0x3d,0x20,0x61,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x73,0x3b,0x0a,
    0x7d,0x0a,0x0a,0x00,
};
/*
    #version 430

    layout(location = 0) out vec4 FragColor;
    layout(location = 0) in vec2 TexCoords;

    void main()
    {
        FragColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
    }

*/
static const uint8_t fs_source_glsl430[189] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x34,0x33,0x30,0x0a,0x0a,0x6c,0x61,
    0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,
    0x30,0x29,0x20,0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x34,0x20,0x46,0x72,0x61,0x67,
    0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,
    0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,0x29,0x20,0x69,0x6e,0x20,0x76,
    0x65,0x63,0x32,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x73,0x3b,0x0a,0x0a,
    0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,
    0x20,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,
    0x63,0x34,0x28,0x67,0x6c,0x5f,0x46,0x72,0x61,0x67,0x43,0x6f,0x6f,0x72,0x64,0x2e,
    0x7a,0x2c,0x20,0x67,0x6c,0x5f,0x46,0x72,0x61,0x67,0x43,0x6f,0x6f,0x72,0x64,0x2e,
    0x7a,0x2c,0x20,0x67,0x6c,0x5f,0x46,0x72,0x61,0x67,0x43,0x6f,0x6f,0x72,0x64,0x2e,
    0x7a,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
/*
    #version 300 es

    uniform vec4 vs_params[12];
    layout(location = 0) in vec3 aPos;
    out vec2 TexCoords;
    layout(location = 1) in vec2 aTexCoords;

    void main()
    {
        gl_Position = ((mat4(vs_params[8], vs_params[9], vs_params[10], vs_params[11]) * mat4(vs_params[4], vs_params[5], vs_params[6], vs_params[7])) * mat4(vs_params[0], vs_params[1], vs_params[2], vs_params[3])) * vec4(aPos, 1.0);
        TexCoords = aTexCoords;
    }

*/
static const uint8_t vs_source_glsl300es[418] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x30,0x30,0x20,0x65,0x73,0x0a,
    0x0a,0x75,0x6e,0x69,0x66,0x6f,0x72,0x6d,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x73,
    0x5f,0x70,0x61,0x72,0x61,0x6d,0x73,0x5b,0x31,0x32,0x5d,0x3b,0x0a,0x6c,0x61,0x79,
    0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x30,
    0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x33,0x20,0x61,0x50,0x6f,0x73,0x3b,0x0a,
    0x6f,0x75,0x74,0x20,0x76,0x65,0x63,0x32,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,
    0x64,0x73,0x3b,0x0a,0x6c,0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,
    0x69,0x6f,0x6e,0x20,0x3d,0x20,0x31,0x29,0x20,0x69,0x6e,0x20,0x76,0x65,0x63,0x32,
    0x20,0x61,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x73,0x3b,0x0a,0x0a,0x76,0x6f,
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
    0x3b,0x0a,0x20,0x20,0x20,0x20,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x73,0x20,
    0x3d,0x20,0x61,0x54,0x65,0x78,0x43,0x6f,0x6f,0x72,0x64,0x73,0x3b,0x0a,0x7d,0x0a,
    0x0a,0x00,
};
/*
    #version 300 es
    precision mediump float;
    precision highp int;

    layout(location = 0) out highp vec4 FragColor;
    in highp vec2 TexCoords;

    void main()
    {
        FragColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
    }

*/
static const uint8_t fs_source_glsl300es[229] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x33,0x30,0x30,0x20,0x65,0x73,0x0a,
    0x70,0x72,0x65,0x63,0x69,0x73,0x69,0x6f,0x6e,0x20,0x6d,0x65,0x64,0x69,0x75,0x6d,
    0x70,0x20,0x66,0x6c,0x6f,0x61,0x74,0x3b,0x0a,0x70,0x72,0x65,0x63,0x69,0x73,0x69,
    0x6f,0x6e,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x69,0x6e,0x74,0x3b,0x0a,0x0a,0x6c,
    0x61,0x79,0x6f,0x75,0x74,0x28,0x6c,0x6f,0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x3d,
    0x20,0x30,0x29,0x20,0x6f,0x75,0x74,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,
    0x63,0x34,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x69,0x6e,
    0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,0x32,0x20,0x54,0x65,0x78,0x43,
    0x6f,0x6f,0x72,0x64,0x73,0x3b,0x0a,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,
    0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x46,0x72,0x61,0x67,0x43,0x6f,
    0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,0x28,0x67,0x6c,0x5f,0x46,0x72,
    0x61,0x67,0x43,0x6f,0x6f,0x72,0x64,0x2e,0x7a,0x2c,0x20,0x67,0x6c,0x5f,0x46,0x72,
    0x61,0x67,0x43,0x6f,0x6f,0x72,0x64,0x2e,0x7a,0x2c,0x20,0x67,0x6c,0x5f,0x46,0x72,
    0x61,0x67,0x43,0x6f,0x6f,0x72,0x64,0x2e,0x7a,0x2c,0x20,0x31,0x2e,0x30,0x29,0x3b,
    0x0a,0x7d,0x0a,0x0a,0x00,
};
static inline const sg_shader_desc* simple_shader_desc(sg_backend backend) {
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
            desc.attrs[1].base_type = SG_SHADERATTRBASETYPE_FLOAT;
            desc.attrs[1].glsl_name = "aTexCoords";
            desc.uniform_blocks[0].stage = SG_SHADERSTAGE_VERTEX;
            desc.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[0].size = 192;
            desc.uniform_blocks[0].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[0].glsl_uniforms[0].array_count = 12;
            desc.uniform_blocks[0].glsl_uniforms[0].glsl_name = "vs_params";
            desc.label = "simple_shader";
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
            desc.attrs[1].base_type = SG_SHADERATTRBASETYPE_FLOAT;
            desc.attrs[1].glsl_name = "aTexCoords";
            desc.uniform_blocks[0].stage = SG_SHADERSTAGE_VERTEX;
            desc.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
            desc.uniform_blocks[0].size = 192;
            desc.uniform_blocks[0].glsl_uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
            desc.uniform_blocks[0].glsl_uniforms[0].array_count = 12;
            desc.uniform_blocks[0].glsl_uniforms[0].glsl_name = "vs_params";
            desc.label = "simple_shader";
        }
        return &desc;
    }
    return 0;
}
