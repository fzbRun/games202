#version 330 core
layout(location = 0) out vec4 colorAndDepth;
layout(location = 1) out vec3 normal;

in vec3 Normal;

void main(){
    colorAndDepth = vec4(1.0f, 0.0f, 0.0f, gl_FragCoord.z);
    normal = Normal;
}