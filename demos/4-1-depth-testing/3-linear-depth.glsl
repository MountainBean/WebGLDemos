@ctype mat4 glm::mat4
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

@vs vs
in vec3 aPos;
in vec3 aTexCoords;

out vec3 TexCoords;

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
in vec3 TexCoords;

out vec4 FragColor;

float near = 0.1;
float far = 100;

float lineariseDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float depth = lineariseDepth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth), 1.0);
}
@end

@program simple vs fs
