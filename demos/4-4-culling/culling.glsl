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
    }
    FragColor = texColour;
}
@end

@program simple vs fs
