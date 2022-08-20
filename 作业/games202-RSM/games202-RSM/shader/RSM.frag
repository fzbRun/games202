#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D wood_texture;
uniform sampler2D colorAndDepth_shadow_texture;
uniform sampler2D normal_shadow_texture;
uniform sampler2D colorAndDepth_shadingPoint_texture;
uniform sampler2D normal_shadingPoint_texture;
uniform mat4 lightProjection;
uniform mat4 projection;
uniform float cutOff;
uniform vec3 lightNormal;

vec3 getViewPos(float depth, mat4 matrix){

    float FragDepth = (depth * 2.0f) - 1.0f;
    vec4 ndcPos = vec4(1.0f);
    ndcPos.xy = (texCoord * 2.0f) - 1.0f;
    ndcPos.z = FragDepth;
    vec4 viewPos = inverse(matrix) * ndcPos;
    viewPos = viewPos / viewPos.w; 
    return viewPos.xyz;

}

float getAtten(vec3 shadingPoint){

    float distance = length(shadingPoint);
    return 1.0f / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

}

void main(){

    //先判断一下当前顶点是不是次级发光物,如果顶点在聚光范围内，那么就是次级发光物(当然这里是近似的做法)
    vec4 colorAndDepth = texture(colorAndDepth_shadingPoint_texture, texCoord);
    vec3 viewPos = getViewPos(colorAndDepth.a, projection);
    vec3 lightDir = normalize(-viewPos);
    vec3 lightN = normalize(lightNormal);
    float theta = dot(-lightDir, lightN);

    vec3 normal = normalize(texture(normal_shadingPoint_texture, texCoord).rgb);    
    float inRange = clamp((theta - 0.93f) / 0.0448f, 0.0f, 1.0f) * max(dot(normal, lightDir), 0.0f) * getAtten(viewPos);
    //float inRange = step(cutOff, theta) * max(dot(normal, lightDir), 0.0f) * getAtten(viewPos);

    vec2 shadowMapSize = textureSize(colorAndDepth_shadow_texture, 0);
    vec2 texelSize = 1.0f / shadowMapSize;
    vec3 irradiance;
    //采样4个角。
    for(int x = -4; x < 4; x++){
        for(int y = -4; y < 4; y++){

            vec4 colorAndDepth_lightPoint = texture(colorAndDepth_shadow_texture, texelSize * vec2(x, y) + texCoord);
            vec3 lightPoint_normal = normalize(texture(normal_shadow_texture, texelSize * vec2(x, y) + texCoord).rgb);  
            vec3 lightPointViewPos = getViewPos(colorAndDepth_lightPoint.a, lightProjection);
            vec3 pointDir = normalize(viewPos - lightPointViewPos); //次级发光物射向着色点

            float r = length(lightPointViewPos - viewPos);
            //这里(r + pow(r, 2))的原因是r^2如果太近，数值过大，那么离次级光照物近的地方过亮，所以加个r。
            //irradiance += 0.1f * max(dot(pointDir, lightPoint_normal), 0.0f) * max(dot(-pointDir, normal), 0.0f) / (1 + pow(r, 2)) * colorAndDepth_lightPoint.rgb;
            irradiance += 0.1f * max(dot(pointDir, lightPoint_normal), 0.0f) * max(dot(-pointDir, normal), 0.0f) / (1 + pow(r, 2)) * vec3(0.5f);

        }
    }

    //irradiance = irradiance * step(inRange, 0.001f) * colorAndDepth.rgb;
    irradiance = irradiance * colorAndDepth.rgb;
    vec3 color = inRange * colorAndDepth.rgb;
    FragColor = vec4(irradiance / 3.1415 + color, 1.0f);

}