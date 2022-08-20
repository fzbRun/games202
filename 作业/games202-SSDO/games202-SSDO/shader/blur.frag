#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D colorAndOcclusion_texture;
uniform bool horizontal;

uniform float weight[5] = float[] (0.054054, 0.1216216, 0.227027, 0.1945946, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(colorAndOcclusion_texture, 0); // gets size of single texel
    vec4 result;
    if(horizontal)
    {
        for(int i = -2; i < 3; ++i)
        {
            result += texture(colorAndOcclusion_texture, TexCoords + vec2(tex_offset.x * i, 0.0)) * weight[i+2];
        }
    }
    else
    {
        for(int i = -2; i < 3; ++i)
        {
            result += texture(colorAndOcclusion_texture, TexCoords + vec2(0.0, tex_offset.y * i)) * weight[i+2];
        }
    }
    FragColor = result;
}