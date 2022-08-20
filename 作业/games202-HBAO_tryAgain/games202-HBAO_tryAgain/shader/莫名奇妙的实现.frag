#version 330 core

in vec2 texCoord;

out float occlusion;

uniform sampler2D normalAndDepth_texture;
uniform sampler2D noise_texture;
uniform mat4 projection;

float nearPlane = 0.1f;
float farPlane = 100.0f;

//将屏幕空间的深度转为视角空间的深度
float getLineDepth(float depth){

    float z = depth * 2.0f - 1.0f;
    return (2 * nearPlane * farPlane) / (nearPlane + farPlane - z * (farPlane - nearPlane));

}

//深度重构
vec3 getViewPos(float depth){

    vec4 ndcPos;
    ndcPos.xy = texCoord * 2.0f - 1.0f;
    ndcPos.z = depth * 2.0f - 1.0f;
    ndcPos.w = 1.0f;
    vec4 viewPos = inverse(projection) * ndcPos;
    return viewPos.xyz / viewPos.w;

}

//获得采样点的sin值
float getSin(vec3 point){

    float Tan = point.z / sqrt(pow(point.x, 2) + pow(point.y, 2));
    return Tan / sqrt(1.0f + pow(Tan, 2));

}

//随机采样，将采样方向随机化
mat2 getNoiseMatrix(){

    float noise = radians(texture(noise_texture, texCoord).r * 5);
    return mat2(cos(noise), -sin(noise), sin(noise), cos(noise));   //绕z轴旋转

}

void main(){

    vec4 normalAndDepth = texture(normalAndDepth_texture, texCoord);
    float lineDepth = getLineDepth(normalAndDepth.w);
    vec3 normal = normalAndDepth.xyz;   //已经归一化过了。
    //vec3 tangent = normalize(cross(normal, vec3(1.0f, 0.0f, 0.0f)));
    float sinT = -(1.0f - sqrt(dot(normal, vec3(0.0f, 0.0f, 1.0f)))); //切线与xy平面的夹角等于法线与z轴的夹角。
    vec3 viewPos = getViewPos(normalAndDepth.w);

    mat2 noiseMatrix = getNoiseMatrix();
    vec2 upDir = noiseMatrix * vec2(0.0f, 1.0f);
    vec2 underDir = noiseMatrix * vec2(0.0f, -1.0f);
    vec2 rightDir = noiseMatrix * vec2(1.0f, 0.0f);
    vec2 leftDir = noiseMatrix * vec2(-1.0f, 0.0f);
    vec2 dir[4] = vec2[](upDir, underDir, rightDir, leftDir);

    vec2 texelSize = 1.0f / textureSize(normalAndDepth_texture, 0);
    float ao;

    for(int i = 0; i < 4; i++){

        float maxSinH = 0.0f;
        float distance_final = 0.0f;

        //每个方向采样10次
        for(int j = 2; j < 12; j++){

            vec2 uv = texCoord + dir[i] * j * texelSize;
            vec4 normalAndDepth_sampler = texture(normalAndDepth_texture, uv);
            vec3 viewPos_sampler = getViewPos(normalAndDepth_sampler.w);
            float sinH = getSin(normalize(viewPos_sampler - viewPos)) + sin(radians(30.0f));
            float distance = length(viewPos_sampler - viewPos);
            float check = step(maxSinH, sinH) * step(pow(distance, 2), 1.0f);   //比较sin值和判断采样点是否离着色点过远。
            maxSinH = step(0.0f, check) * sinH + step(check, 1.0f) * maxSinH;
            distance_final = step(0.0f, check) * distance + step(check, 1.0f) * distance_final;
            
        }

        ao += (maxSinH - sinT) * (1.0f - pow(distance_final, 2));

    }

    //occlusion = 1.0f - occlusion / 4;
    occlusion = step(1.0f, ao / 16);

}