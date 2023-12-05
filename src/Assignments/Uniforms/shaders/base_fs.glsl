#version 450

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3 color;
};

layout(location = 0) out vec4 vFragColor;

void main() {
    // Modify the pixel color using the values from the interface block
    vFragColor = vec4(strength * color, 1.0);
}