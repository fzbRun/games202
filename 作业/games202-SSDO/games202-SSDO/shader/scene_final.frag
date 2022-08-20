#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D colorAndOcclusion_texture;
uniform sampler2D G_buffer_normalAndDepth_texture;
uniform sampler2D G_buffer_color_texture;
uniform vec3 lightPos;
uniform mat4 projection;

//深度重构
vec3 getViewPos(float depth){

    vec4 ndcPos;
    ndcPos.xy = texCoord * 2.0f - 1.0f;
    ndcPos.z = depth * 2.0f - 1.0f;
    ndcPos.w = 1.0f;
    vec4 viewPos = inverse(projection) * ndcPos;
    return viewPos.xyz / viewPos.w;

}

void main(){

    vec4 normalAndDepth = texture(G_buffer_normalAndDepth_texture, texCoord);
    vec3 normal =normalAndDepth.rgb;
    vec3 viewPos = getViewPos(normalAndDepth.w);

    vec4 colorAndOcclusion = texture(colorAndOcclusion_texture, texCoord);
    float occlusion = colorAndOcclusion.w;
    vec3 lightColor = colorAndOcclusion.rgb;

    vec3 objectColor = texture(G_buffer_color_texture, texCoord).rgb;
    vec3 lightDir = normalize(lightPos - viewPos);
    vec3 viewDir = normalize(-viewPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    float ambient = 0.3f * occlusion;
    float diffuse = max(dot(lightDir, normal), 0.0f);  //已经算过cos了
    float specular = pow(max(dot(normal, halfDir), 0.0f), 32);

    vec3 color = (ambient + diffuse + specular) * objectColor  + lightColor * objectColor * 3;
    FragColor = vec4(color, 1.0f);

}