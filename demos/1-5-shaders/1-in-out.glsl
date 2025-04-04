@vs vs
in vec4 position;

out vec4 vertexColor; // specify a color output to the fragment shader

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    vertexColor = vec4(position.x + 0.5, position.y + 0.5, position.z + 0.5, 1.0); 
}
@end

@fs fs
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}
@end

@program simple vs fs
