#version 330 core
layout(location = 0) out vec4 normalAndDepth;
layout(location = 1) out vec4 color;

in vec3 Normal;
in vec2 texCoord;

void main(){

    normalAndDepth = vec4(normalize(Normal), gl_FragCoord.z);
    color = vec4(1.0f);

}