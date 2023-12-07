#version 410

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;  // Output color to the fragment shader

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vertexColor = aColor;
}