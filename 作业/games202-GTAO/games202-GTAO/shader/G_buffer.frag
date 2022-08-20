#version 330 core
layout(location = 0) out vec4 normalAndDepth;
layout(location = 1) out vec4 color;

in vec3 Normal;
in vec2 texCoord;

uniform sampler2D texture_diffuse1;

float getLineDpeth(float depth){

    float z = depth * 2.0f - 1.0f;
    return (2 * 0.1f * 100.0f) / (0.1f + 100.0f - z * (100.0f - 0.1f));

}

void main(){

    normalAndDepth = vec4(normalize(Normal), gl_FragCoord.z);
    vec4 objectColor = texture(texture_diffuse1, texCoord);
    color = vec4(vec3(0.95f), 0.0f) * objectColor;

}