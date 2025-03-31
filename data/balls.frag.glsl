#version 330 core
out vec4 FragColor;

in vec4 color;

void main()
{
    FragColor = vec4(color.r + 0.5,
            color.g + 0.5,
            color.b + 0.5,
            1.0);
}
