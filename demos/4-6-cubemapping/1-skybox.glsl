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
    gl_Position = projection * view * model *vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}
@end

@fs fs
out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform texture2D _texture1;
layout(binding = 0) uniform sampler texture1_smp;
#define texture1 sampler2D(_texture1, texture1_smp)

void main() {
    FragColor = texture(texture1, TexCoords);
}
@end

@vs vs_skybox
in vec3 aPos;

out vec3 TexCoords;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
@end

@fs fs_skybox
out vec4 FragColor;

in vec3 TexCoords;

layout(binding = 0) uniform textureCube _skybox;
layout(binding = 0) uniform sampler skybox_smp;
#define skybox samplerCube(_skybox, skybox_smp)

void main() {
    FragColor = texture(skybox, TexCoords);
}
@end

@program simple vs fs
@program skybox vs_skybox fs_skybox
