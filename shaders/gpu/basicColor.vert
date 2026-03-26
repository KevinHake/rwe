#version 450

layout(set = 1, binding = 0) uniform VertexUniforms {
    mat4 mvpMatrix;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
    fragColor = color;
}
