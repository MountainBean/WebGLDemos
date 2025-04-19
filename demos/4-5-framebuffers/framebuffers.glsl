@ctype mat4 glm::mat4
@ctype vec3 glm::vec3

@vs vs
in vec3 aPos;
in vec2 aTexCoords;

out vec2 TexCoords;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}
@end

@fs fs
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform texture2D _texture1;
layout(binding = 0) uniform sampler texture1_smp;
#define texture1 sampler2D(_texture1, texture1_smp)

void main() {
    vec4 texColour = texture(texture1, TexCoords);
    if (texColour.a < 0.1) {
        discard;
    };
    FragColor = texColour;
}
@end

@vs vs_framebuffer
in vec3 aPos;
in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}

@end

@fs fs_default_framebuffer
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform texture2D _screenTexture;
layout(binding = 0) uniform sampler screenTexture_smp;
#define screenTexture sampler2D(_screenTexture, screenTexture_smp)

void main() {
    FragColor = texture(screenTexture, TexCoords);
}
@end
@fs fs_inverted_framebuffer
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform texture2D _screenTexture;
layout(binding = 0) uniform sampler screenTexture_smp;
#define screenTexture sampler2D(_screenTexture, screenTexture_smp)

void main() {
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}
@end
@fs fs_greyscale_framebuffer
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform texture2D _screenTexture;
layout(binding = 0) uniform sampler screenTexture_smp;
#define screenTexture sampler2D(_screenTexture, screenTexture_smp)

void main() {
    FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
}
@end
@fs fs_sharpen_framebuffer
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform texture2D _screenTexture;
layout(binding = 0) uniform sampler screenTexture_smp;
#define screenTexture sampler2D(_screenTexture, screenTexture_smp)

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}
@end
@fs fs_blur_framebuffer
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform texture2D _screenTexture;
layout(binding = 0) uniform sampler screenTexture_smp;
#define screenTexture sampler2D(_screenTexture, screenTexture_smp)

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}
@end
@fs fs_edge_framebuffer
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 0) uniform texture2D _screenTexture;
layout(binding = 0) uniform sampler screenTexture_smp;
#define screenTexture sampler2D(_screenTexture, screenTexture_smp)

const float offset = 1.0 / 300.0;

void main() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float kernel[9] = float[](
        1.0, 1.0, 1.0,
        1.0,-8.0, 1.0,
        1.0, 1.0, 1.0
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}
@end

@program simple vs fs
@program framebuffer vs_framebuffer fs_default_framebuffer
@program inverted_framebuffer vs_framebuffer fs_inverted_framebuffer
@program greyscale_framebuffer vs_framebuffer fs_greyscale_framebuffer
@program sharpen_framebuffer vs_framebuffer fs_sharpen_framebuffer
@program blur_framebuffer vs_framebuffer fs_blur_framebuffer
@program edge_framebuffer vs_framebuffer fs_edge_framebuffer
