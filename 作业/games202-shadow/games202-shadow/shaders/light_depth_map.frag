#version 330 core

out vec4 FragDepth;

void main(){
    
    float depth = gl_FragCoord.z;
    float depthPow2 = pow(depth, 2);
    FragDepth = vec4(depth, depthPow2, 0.0f, 1.0f);

}