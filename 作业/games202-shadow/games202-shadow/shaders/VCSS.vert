#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightView;
uniform mat4 lightOrtho;

out VS_OUT{

    vec3 lightSpacePos;
    vec3 normal;
    vec2 texCoord;

}vs_out;

void main(){

    vec4 worldPos = model * vec4(pos, 1.0f);
    gl_Position = projection * view * worldPos;
    vs_out.normal = vec3(transpose(inverse(lightView * model)) * vec4(normal, 1.0f));
    vs_out.texCoord = texCoord;
    vs_out.lightSpacePos = vec3(lightView * worldPos);

}