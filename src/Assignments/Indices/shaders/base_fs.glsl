#version 410

in vec3 vertexColor;  // Input color from the vertex shader

out vec4 fragColor;  // Output color

void main() {
    fragColor = vec4(vertexColor, 1.0);
}