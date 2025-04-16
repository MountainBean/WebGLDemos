@ctype mat4 glm::mat4
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

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


void main() {
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
@end

@program simple vs fs
