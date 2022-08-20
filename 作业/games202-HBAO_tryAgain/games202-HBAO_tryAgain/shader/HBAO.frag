#version 330 core

in vec2 texCoord;

out float occlusion;

uniform sampler2D normalAndDepth_texture;
uniform sampler2D noise_texture;
uniform mat4 projection;

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

//获得采样点的sin值
float getSin(vec3 point){

    float Tan = point.z / sqrt(pow(point.x, 2) + pow(point.y, 2));
    return Tan / sqrt(1.0f + pow(Tan, 2));

}

//随机采样，将采样方向随机化
mat2 getNoiseMatrix(){

    float noise = radians(texture(noise_texture, texCoord).r * 90); //旋转范围为PI/2
    return mat2(cos(noise), -sin(noise), sin(noise), cos(noise));   //绕z轴旋转

}

void main(){

    vec4 normalAndDepth = texture(normalAndDepth_texture, texCoord);
    vec3 normal = normalAndDepth.xyz;   //已经归一化过了。
    float sinT = -sqrt(1.0f - pow(dot(normal, vec3(0.0f, 0.0f, 1.0f)), 2)); //切线与xy平面的夹角等于法线与z轴的夹角。
    vec3 viewPos = getViewPos(normalAndDepth.w);

    mat2 noiseMatrix = getNoiseMatrix();
    vec2 upDir = noiseMatrix * vec2(0.0f, 1.0f);
    vec2 underDir = noiseMatrix * vec2(0.0f, -1.0f);
    vec2 rightDir = noiseMatrix * vec2(1.0f, 0.0f);
    vec2 leftDir = noiseMatrix * vec2(-1.0f, 0.0f);
    vec2 dir[4] = vec2[](upDir, underDir, rightDir, leftDir);

    vec2 texelSize = 1.0f / textureSize(normalAndDepth_texture, 0);
    float ao = 0.0f;

    for(int i = 0; i < 4; i++){

        float maxSinH = sinT;

        //每个方向采样10次
        for(int j = 2; j < 12; j++){

            vec2 uv = texCoord + dir[i] * j * texelSize;
            vec4 normalAndDepth_sampler = texture(normalAndDepth_texture, uv);
            vec3 viewPos_sampler = getViewPos(normalAndDepth_sampler.w);
            //float sinH = getSin(viewPos_sampler - viewPos);
            float sinH = normalize(viewPos_sampler - viewPos).z; //getSin求的，不就是归一化后的z值吗？不知道normalize和getSin哪个慢。
            float distance = pow(length(viewPos_sampler - viewPos), 2);
            float check = step(maxSinH, sinH) * step(distance, 1.0f);   //比较sin值和判断采样点是否离着色点过远。
            ao += (1.0f - distance) * ((sinH - sinT) - (maxSinH - sinT)) * check;
            maxSinH = check * sinH + (1.0f - check) * maxSinH;
            
        }

    }

    occlusion = 1.0f - ao / 4;

}