@ctype mat4 glm::mat4

@vs vs
in vec3 aPos;

out vec4 color;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = vec4(aPos, 1.0);
}
@end

@fs fs
out vec4 FragColor;

in vec4 color;

void main()
{
    FragColor = vec4(color.r + 0.5,
            color.g + 0.5,
            color.b + 0.5,
            1.0);
}
@end

@program balls vs fs
