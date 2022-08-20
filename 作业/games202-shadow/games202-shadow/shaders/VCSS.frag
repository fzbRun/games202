#version 330 core

uniform sampler2D texture_diffuse1;
uniform sampler2D depthMap;
uniform mat4 lightView;
uniform mat4 lightOrtho;
uniform vec3 viewPos;

out vec4 FragColor;

in VS_OUT{

    vec3 lightSpacePos;
    vec3 normal;
    vec2 texCoord;

}fs_in;

vec3 getColor(vec3 objectColor){

    vec3 normal = normalize(fs_in.normal);
    vec3 lightDir = normalize(-fs_in.lightSpacePos);
    vec3 viewDir = normalize(vec3(lightView * vec4(viewPos, 1.0f)) - fs_in.lightSpacePos);
    vec3 halfDir = normalize(viewDir + lightDir);

    float diffuse = dot(normal, lightDir) * 0.5f + 0.5f;
    float specular = pow(max(dot(normal, halfDir), 0.0f), 32);
    
    return (diffuse + specular) * objectColor;

}

float getAvgDepth(vec3 screenPos){

    /*
    vec2 mapSize = textureSize(depthMap, 0);
    vec2 texelSize = 1.0f / mapSize;
    mapSize = vec2(10);    //全屏太大了，3070都hold不住啊！
    int blockers = 0;   //遮挡像素的数量
    float avgDepth;
    vec2 halfSize = mapSize / 2;
    for(int x = int(-halfSize.x); x < int(halfSize.x); x++){
        for(int y = int(-halfSize.y); y < int(halfSize.y); y++){

            float depth = texture(depthMap, screenPos.xy + vec2(x,y) * texelSize).r;
            float add = step(depth, screenPos.z);
            blockers += int(add);
            avgDepth += add * depth;

        }
    }
    if(blockers == 0){
        return screenPos.z; //如果着色点是最前面的，那么就返回着色点深度
    }
    return avgDepth / blockers;
    */

    //VSSM版本
    
    float depth = screenPos.z;
    float avgAllDepth = texture2DLod(depthMap, vec2(0.5f, 0.5f), 10).r; //纹理大小为1024，所以第十级就是1，那么就是所有深度的均值
    float avgAllDepthPow2 = texture2DLod(depthMap, vec2(0.5f, 0.5f), 10).g;   //深度平方的均值
    float avgAllDepthDX = avgAllDepthPow2 - pow(avgAllDepth, 2);    //DX = E(X^2) - E(X)^2， 得到方差
    //切比雪夫不等式， 得到深度小于着色点深度的像素占纹理的百分比。
    float closerPercentage = pow(avgAllDepthDX, 2) / (pow(avgAllDepthDX, 2) + pow(depth - avgAllDepth, 2));
    //假设深度大于着色点深度的像素深度值都为作色点深度。（但其实本例中大部分深度都为1，所以我们应该把大于的像素深度值都设为1更精确）
    float avgCloserDepth = (avgAllDepth - (1.0f - closerPercentage) * depth) / closerPercentage;
    return avgCloserDepth;
    
}

float nearPlane = 1.0f;
float farPlane = 100.0f;

float getDepthLine(float depth){

    float z = depth * 2.0f - 1.0f; // 回到NDC空间，0-1
    return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane)); //变为线性深度，也就是光照view空间下的深度

}

float getSize(float avgDepth, float depth){
    
    float blockerDepth = getDepthLine(avgDepth);    //变回线性空间
    float reviceDepth = getDepthLine(depth);
    float size = (reviceDepth - blockerDepth) / blockerDepth;
    //size = step(0.5f, size - int(size)) + int(size);  //在textureLod函数中自动四舍五入
    //size = step(0.0f, size) * size;   //小于0，就改为0。不能乘size，不知道为什么，画面会变得全黑。
    if(size < 0.0f){
        size = 0.0f;
    }
    return step(size, 10.0f) * size + step(10.0f, size) * 10.0f;

}

float getShadow(){

    vec4 lightClipPos = lightOrtho * vec4(fs_in.lightSpacePos, 1.0f);
    vec3 screenPos = (lightClipPos.xyz / lightClipPos.w) * 0.5f + 0.5f;
    float FragDepth = screenPos.z;
    if(FragDepth > 1.0f){
        return 1.0f;
    }
    float avgDepth = getAvgDepth(screenPos);
    float size = getSize(avgDepth, FragDepth);
    
    //size *= 10.0f;    //PCSS需要加大，VCSS不需要
    vec2 texelSize = 1.0f / textureSize(depthMap, 0);
    float shadow;
    int halfSize = int(size / 2);
    for(int i = -halfSize; i <= halfSize; i++){
        for(int j = -halfSize; j <= halfSize; j++){

            float depth = texture(depthMap, screenPos.xy + vec2(i,j) * texelSize).r;
            shadow += step(FragDepth, depth);   //如果采样到的深度大于着色点深度，那么说明着色点没有被挡到。

        }
    }
    //size = step(size, 1.0f) + size;
    if(size < 1.0f){
        size = 1.0f;
    }
    size = pow(int(size), 2);
    return shadow / size;
    

    //VSSM版本
    //这里我不知道如何通过size判断使用mipmap的哪个级别，所以我将size作为级别。
    /*
    avgDepth = textureLod(depthMap, screenPos.xy, size).r;
    float avgDepthPow2 = textureLod(depthMap, screenPos.xy, size).g;
    float avgDepthDX = avgDepthPow2 - pow(avgDepth, 2);
    float closerPercentage = pow(avgDepthDX, 2) / (pow(avgDepthDX, 2) + pow(screenPos.z - avgDepth, 2));
    return 1.0f - closerPercentage;
    */
}

void main(){

    vec3 objectColor = texture(texture_diffuse1, fs_in.texCoord).rgb;
    vec3 ambient = 0.1f * objectColor;
    vec3 DaScolor = getColor(objectColor);
    float shadow = getShadow();
    FragColor = vec4(ambient + DaScolor * shadow, 1.0f);

}
