#version 330 core
layout(location = 0) out vec4 colorAndDepth;
layout(location = 1) out vec3 normal;

in vec3 Normal;
in vec2 texCoord;

uniform sampler2D wood_texture;

void main(){
    vec3 color = texture(wood_texture, texCoord).rgb;
    colorAndDepth = vec4(color, gl_FragCoord.z);
    normal = Normal;
}