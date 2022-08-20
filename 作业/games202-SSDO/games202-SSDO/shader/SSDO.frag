#version 330 core

in vec2 texCoord;

out vec4 colorAndOcclusion;

uniform sampler2D normalAndDepth_texture;
uniform sampler2D color_texture;
uniform sampler2D noise_texture;
uniform mat4 projection;
uniform vec3 samplers[64];

float nearPlane = 0.1f;
float farPlane = 100.0f;

//深度重构
vec3 getViewPos(float depth){

    vec4 ndcPos;
    ndcPos.xy = texCoord * 2.0f - 1.0f;
    ndcPos.z = depth * 2.0f - 1.0f;
    ndcPos.w = 1.0f;
    vec4 viewPos = inverse(projection) * ndcPos;
    return viewPos.xyz / viewPos.w;

}

//得到屏幕空间位置
vec2 getScreenPos(vec3 point){

    vec4 clipPos = projection * vec4(point, 1.0f);
    vec2 ndcPos = clipPos.xy / clipPos.w;
    vec2 screenPos = ndcPos * 0.5f + 0.5f;
    return screenPos;

}

void main(){

    vec4 normalAndDepth = texture(normalAndDepth_texture, texCoord);
    vec3 normal = normalAndDepth.xyz;   //已经归一化过了。
    vec3 viewPos = getViewPos(normalAndDepth.w);

    //vec2 noise_texelSize = vec2(1024.0f / 4.0f);
    //vec3 noise = normalize(texture(noise_texture, texCoord * noise_texelSize).xyz);
    vec2 noise_texelSize = vec2(1024.0f) / textureSize(noise_texture, 0);
    vec3 noise = normalize(texture(noise_texture, texCoord * noise_texelSize)).rgb;
    vec3 tangent = normalize(noise - normal * dot(normal, noise));
    vec3 bitangent = normalize(cross(normal, tangent));
    mat3 TBN = mat3(tangent, bitangent, normal);

    vec4 result;
    for(int i = 0; i < 64; i++){

        vec3 viewPos_sampler = viewPos + TBN * samplers[i];
        vec2 uv = getScreenPos(viewPos_sampler);
        vec4 normalAndDepth_sampler = texture(normalAndDepth_texture, uv);
        vec3 viewPos_sampler_real = getViewPos(normalAndDepth_sampler.w);
        //判断采样点与实际点的深度，如果实际更浅，那么说明着色点到采样点的方向会被挡住。
        //如果被挡住，那么判断是否可以传递间接光（法线与法线点乘）
        //如果没挡住说明可以传递环境光
        vec3 dir = viewPos_sampler_real - viewPos;
        float distance = pow(length(dir), 2);
        float check = step(viewPos_sampler_real.z, viewPos_sampler.z) * step(distance, 1.0f);
        vec3 color = texture(color_texture, uv).rgb;

        result.xyz += check * color * (1.0f - distance) * max(dot(normal, normalize(dir)), 0.0f);
        result.w += 1.0f - check;

    }

    colorAndOcclusion = result / 64;

}