@ctype mat4 glm::mat4
@ctype vec3 glm::vec3
//--------------------------------------------
@vs vs
in vec3 aPos;
in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
}
@end
//--------------------------------------------
@fs fs
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

layout(binding = 1) uniform fs_params {
    vec3 cameraPos;
};

layout(binding = 0) uniform textureCube _skybox;
layout(binding = 0) uniform sampler skybox_smp;
#define skybox samplerCube(_skybox, skybox_smp)


void main() {
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
@end
//--------------------------------------------
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
//--------------------------------------------
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
//--------------------------------------------
@program reflect vs fs
@program skybox vs_skybox fs_skybox
