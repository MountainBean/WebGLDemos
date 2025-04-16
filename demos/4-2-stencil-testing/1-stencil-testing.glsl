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
    FragColor = texture(texture1, TexCoords);
}
@end

@fs fs_stencil_outline
in vec2 TexCoords;
out vec4 FragColor;

void main() {
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}

@end

@program simple vs fs
@program stencilOutline vs fs_stencil_outline
