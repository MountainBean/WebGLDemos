@ctype mat4 glm::mat4
@ctype vec3 glm::vec3

@vs vs
in vec3 aPos;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * model *vec4(aPos, 1.0);
}
@end

@fs fs
out vec4 FragColor;

layout(binding = 1) uniform fs_params {
    vec3 objectColour;
    vec3 lightColour;
    float ambientStrength;
};

void main() {
    vec3 ambient = ambientStrength * lightColour;

    vec3 result = ambient * objectColour;
    FragColor = vec4(result, 1.0);
}
@end

@fs light_cube_fs
out vec4 FragColor;

layout(binding = 1) uniform light_cube_fs_params {
    vec3 lightColour;
};

void main() {
    FragColor = vec4(lightColour, 1.0);
}
@end

@program ambient vs fs
@program light_cube vs light_cube_fs
