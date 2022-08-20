#version 330 core
layout(location = 0) out vec4 normalAndDepth;
layout(location = 1) out vec3 color;

in vec3 Normal;
in vec2 texCoord;

void main(){

    normalAndDepth = vec4(normalize(Normal), gl_FragCoord.z);
    color = vec3(0.95f);

}