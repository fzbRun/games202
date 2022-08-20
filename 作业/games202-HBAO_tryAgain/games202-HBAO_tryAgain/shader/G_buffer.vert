#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){

    gl_Position = projection * view * model * vec4(vertex, 1.0f);
    Normal = vec3(transpose(inverse(view * model)) * vec4(normal, 1.0f));

}