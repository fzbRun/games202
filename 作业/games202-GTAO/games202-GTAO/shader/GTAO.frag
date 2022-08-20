#version 330 core

in vec2 texCoord;

out vec3 visibility;

uniform sampler2D normalAndDepth_texture;
uniform sampler2D color_texture;
uniform sampler2D noise_texture;
uniform mat4 projection;
uniform vec2 dirs[21];

vec3 getViewPos(float depth){

    vec4 ndcPos;
    ndcPos.xy = texCoord * 2.0f - 1.0f;
    ndcPos.z = depth * 2.0f - 1.0f;
    ndcPos.w = 1.0f;
    vec4 clipPos = inverse(projection) * ndcPos;
    return ndcPos.xyz / ndcPos.w;

}

mat2 getNoiseDir(vec2 uv){

    float noise = radians(texture(noise_texture, texCoord).r);
    return mat2(cos(noise), -sin(noise), sin(noise), cos(noise));

}

float fastSqrt(float x){
    return float(0x1FBD1DF5 + (int(x) >> 1));
}

float fastAcos(float x){

    float res = -0.156583 * abs(x) + 3.1415926f / 2.0f;
    res *= fastSqrt(1.0f - abs(x));
    return x >= 0 ? res : 3.1415926f - res;

}

float getVd(float h1, float h2, float n){
    return (-cos(2 * h1 - n) + cos(n) + 2 * h1 * sin(n)) / 4.0f + (-cos(2 * h2 - n) + cos(n) + 2 * h2 * sin(n)) / 4.0f;
}

vec3 getMultiBounce(float vd, vec3 albedo){

    vec3 a = 2.0404 * albedo - 0.3324;
    vec3 b = -4.7951 * albedo + 0.6417;
    vec3 c = 2.7552 * albedo + 0.6903;

    return max(vec3(vd), ((vd * a + b) * vd + c) * vd);

}

void main(){

    vec4 normalAndDepth = texture(normalAndDepth_texture, texCoord);
    vec3 normal = normalize(normalAndDepth.xyz);
    float depth = normalAndDepth.w;
    vec3 shadingPoint = getViewPos(depth);
    mat2 rotate = getNoiseDir(texCoord);
    //vec2 dirs[8] = {vec2(-1.0f, 0.0f), normalize(vec2(-0.8f, 0.5f)) ,normalize(vec2(-1.0f, 1.0f)), normalize(vec2(-0.5f, 0.8f)),
    //vec2(0.0f, 1.0f), normalize(vec2(0.5f, 0.8f)), normalize(vec2(1.0f, 1.0f)), normalize(vec2(0.8f, 0.5f))};
    vec3 viewDir = normalize(-shadingPoint);

    vec3 result;
    vec2 texelSize = textureSize(normalAndDepth_texture, 0);
    for(int i = 0; i < 21; i++){

        vec2 dir = rotate * dirs[i];
        vec2 dir_opposite = -dir;
        float maxH1 = -1.57f;
        float maxH2 = 1.57f;

        for(int j = 0; j < 10; j++){

            vec4 normalAndDepth_sampleH1 = texture(normalAndDepth_texture, texCoord + dir * i * texelSize);
            float depth_sampleH1 = normalAndDepth_sampleH1.w;
            vec3 samplePointH1 = getViewPos(depth_sampleH1);
            vec3 ds = samplePointH1 - shadingPoint;
            float check = step(length(ds), 1.0f);   //判断是否超出半球半径
            float h1 = -fastAcos(dot(normalize(ds), viewDir)) * check;   //认为h1是负的,与积分域有关
            maxH1 = max(h1, maxH1);

            vec4 normalAndDepth_sampleH2 = texture(normalAndDepth_texture, texCoord + dir_opposite * i * texelSize);
            float depth_sampleH2 = normalAndDepth_sampleH2.w;
            vec3 samplePointH2 = getViewPos(depth_sampleH2);
            vec3 dt = samplePointH2 - shadingPoint;
            check = step(length(dt), 1.0f);   //判断是否超出半球半径
            float h2 = fastAcos(dot(normalize(dt), viewDir)) * check;
            maxH2 = min(h2, maxH2);

        }

        vec3 sn = normalize(cross(viewDir, vec3(dir, 0.0f)));
        vec3 np = normalize(normal - sn * dot(normal, sn));
        float n = fastAcos(dot(np, viewDir));
        maxH1 = n + max(maxH1 - n, -1.57f);
        maxH2 = n + min(maxH2 - n, 1.57f);
        float vd = getVd(maxH1, maxH2, n);
        
        vec3 albedo = texture(color_texture, texCoord).rgb;  //这里是基于假设被着色点反射率与周围环境大致相同。
        result += getMultiBounce(vd, albedo);

    }

    visibility = result / 5.0f;

}