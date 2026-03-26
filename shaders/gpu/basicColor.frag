#version 450

layout(set = 3, binding = 0) uniform FragmentUniforms {
    float alpha;
};

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, alpha);
}
