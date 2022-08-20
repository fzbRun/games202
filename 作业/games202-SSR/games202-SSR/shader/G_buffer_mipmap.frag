#version 330 core

out float mipmap;

//in float depth;
in vec2 texCoord;

uniform sampler2D depth_texture;
uniform int level;

void main(){

    //mipmap = depth;
    //gl_FragDepth = depth;

    vec2 texelSize = textureSize(depth_texture, 0);

    if(mod(texCoord.x, 2) == 0.0f && mod(texCoord.y, 2) == 0.0f){

        float depth1 = texture(depth_texture, texCoord, level).w; //左下
        float depth2 = texture(depth_texture, texCoord + texelSize * vec2(1.0f, 0.0f), level).w;  //右下
        float depth3 = texture(depth_texture, texCoord + texelSize * vec2(1.0f, 1.0f), level).w;  //右上
        float depth4 = texture(depth_texture, texCoord + texelSize * vec2(0.0f, 1.0f), level).w;  //左上
        mipmap = min(min(depth1, depth2), min(depth3, depth4));
        
    }else if(mod(texCoord.x, 2) == 1.0f && mod(texCoord.y, 2) == 0.0f){

        float depth1 = texture(depth_texture, texCoord + texelSize * vec2(-1.0f, 0.0f), level).w; //左下
        float depth2 = texture(depth_texture, texCoord), level.w;  //右下
        float depth3 = texture(depth_texture, texCoord + texelSize * vec2(0.0f, 1.0f), level).w;  //右上
        float depth4 = texture(depth_texture, texCoord + texelSize * vec2(-1.0f, 1.0f), level).w;  //左上
        mipmap = min(min(depth1, depth2), min(depth3, depth4));

    }else if(mod(texCoord.x, 2) == 1.0f && mod(texCoord.y, 2) == 1.0f){

        float depth1 = texture(depth_texture, texCoord + texelSize * vec2(-1.0f, -1.0f), level).w; //左下
        float depth2 = texture(depth_texture, texCoord + texelSize * vec2(0.0f, -1.0f), level).w;  //右下
        float depth3 = texture(depth_texture, texCoord, level).w;  //右上
        float depth4 = texture(depth_texture, texCoord + texelSize * vec2(-1.0f, 0.0f), level).w;  //左上
        mipmap = min(min(depth1, depth2), min(depth3, depth4));

    }else{

        float depth1 = texture(depth_texture, texCoord + texelSize * vec2(0.0f, -1.0f), level).w; //左下
        float depth2 = texture(depth_texture, texCoord + texelSize * vec2(1.0f, -1.0f), level).w;  //右下
        float depth3 = texture(depth_texture, texCoord + texelSize * vec2(1.0f, 0.0f), level).w;  //右上
        float depth4 = texture(depth_texture, texCoord, level).w;  //左上
        mipmap = min(min(depth1, depth2), min(depth3, depth4));

    }

}