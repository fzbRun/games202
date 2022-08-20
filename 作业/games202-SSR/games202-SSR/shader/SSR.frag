#version 330 core

out vec3 color;

in vec2 texCoord;

uniform sampler2D normalAndDepth_texture;
uniform sampler2D color_texture;
//uniform sampler2D depth_texture1;
//uniform sampler2D depth_texture2;
//uniform sampler2D depth_texture3;
//uniform sampler2D depth_texture4;
uniform mat4 projection;

vec3 getViewPos(float depth){

    vec4 ndcPos;
    ndcPos.xy = texCoord * 2.0f - 1.0f;
    ndcPos.z = depth * 2.0f - 1.0f;
    ndcPos.w = 1.0f;
    vec4 clipPos = inverse(projection) * ndcPos;
    return clipPos.xyz / clipPos.w;

}

vec2 getScreenPos(vec3 point){

    vec4 clipPos = projection * vec4(point, 1.0f);
    vec2 screenPos = clipPos.xy / clipPos.w;
    return screenPos * 0.5f + 0.5f;

}

void main(){

    vec4 normalAndDepth = texture(normalAndDepth_texture, texCoord);
    vec3 viewPos = getViewPos(normalAndDepth.w);

    vec3 normal = normalAndDepth.xyz;

    vec3 viewDir = normalize(-viewPos) / 2;
    vec3 refDir = normalize(reflect(-viewDir, normal));

    vec2 screenPos = getScreenPos(viewPos);
    vec2 screenPos_next = getScreenPos(viewPos + refDir);
    vec2 stepDir = screenPos_next - screenPos;

    vec3 result = vec3(0.0f);

    /*
    //可以将x轴或y轴缩放到1.0f
    if(stepDir.x > stepDir.y){
        stepDir = vec2(1.0f, stepDir.y * 1.0f / stepDir.x);
    }else{
        stepDir = vec2(stepDir.x * 1.0f / stepDir.y, 1.0f);
    }
    */
    
    for(float i = 0.0f; i < 100.0f; i++){

        screenPos_next = screenPos + i * stepDir;
        if(screenPos_next.x < 0.0f || screenPos_next.x > 1.0f || screenPos_next.y < 0.0f || screenPos_next.y > 1.0f){
            break;
        }
        vec3 viewPos_next = viewPos + i * refDir;

        float sample_depth = texture(normalAndDepth_texture, screenPos_next).w;
        vec3 sample_viewPos = getViewPos(sample_depth);  //采样点的观察空间深度

        if(sample_viewPos.z > viewPos_next.z){

            float distance = length(sample_viewPos - viewPos);
            result = texture(color_texture, screenPos_next).rgb * (1.0f / (1.0f + pow(distance, 2)));
            break;

        }
        
    }
    

    /*
    //采用Hi-Z Buffer的方法加速
    int index = 0;
    int isSpot = 0;
    screenPos_next = screenPos;
    for(int i = 0; i < 10; i++){

        screenPos_next += i * stepDir;
        if(screenPos_next.x < 0.0f || screenPos_next.x > 1.0f || screenPos_next.y < 0.0f || screenPos_next.y > 1.0f){
            break;
        }
        vec3 viewPos_next = viewPos + i * refDir * pow(index, 2);

        float sample_depth;
        if(index == 0){
            sample_depth = texture(normalAndDepth_texture, screenPos_next).w;
        }else if(index == 1){
            screenPos_next += stepDir;
            sample_depth = texture(depth_texture1, screenPos_next, 1).r;   //得到的是裁剪空间的深度，不是屏幕空间的。
        }else if(index == 2){
            screenPos_next += stepDir * 3;
            sample_depth = texture(depth_texture2, screenPos_next, 1).r;
        }else if(index == 3){
            screenPos_next += stepDir * 7;
            sample_depth = texture(depth_texture3, screenPos_next, 1).r;
        }else if(index == 4){
            screenPos_next += stepDir * 15;
            sample_depth = texture(depth_texture4, screenPos_next, 1).r;
        }
        vec3 sample_viewPos = getViewPos(sample_depth);  //采样点的观察空间深度

        if(sample_viewPos.z > viewPos_next.z){

            if(index == 0){
                float distance = length(sample_viewPos - viewPos);
                result = texture(color_texture, screenPos_next).rgb * (1.0f / (1.0f + pow(distance, 2)));
                break;
            }else{
                isSpot = 1;
                screenPos_next -= stepDir * pow(index, 2);
                index -= 1;
            }

        }else{

            index -= int(step(0.0f, isSpot));
            index += int(step(index, 4) * step(isSpot, 1.0f));

        }

    }
    */
    color = result * texture(color_texture, texCoord).w;

}