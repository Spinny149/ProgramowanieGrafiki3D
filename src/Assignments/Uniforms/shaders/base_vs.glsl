#version 420

layout(location = 0) in vec3 inPosition;

out vec3 fragPosition;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragPosition = inPosition;
}