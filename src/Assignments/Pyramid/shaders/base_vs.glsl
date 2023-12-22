#version 460

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec3 vertex_color;

flat out vec3 in_color;

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

void main() {
    
    gl_Position = PVM * a_vertex_position;
    in_color = vertex_color;
}