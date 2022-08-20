#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D color_texture;
uniform sampler2D SSR_texture;

void main(){

    float specular = 0.5f;
    vec3 color = texture(color_texture, texCoord).rgb * specular;
    vec3 refColor = texture(SSR_texture, texCoord).rgb * (1.0f - specular);
    FragColor = vec4(refColor + color, 1.0f);

}