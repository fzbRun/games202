#version 330 core
layout(triangles_adjacency) in;
layout(points, max_vertices = 1) out;

out float depth;

float getScreenDepth(vec4 clipPos){

    vec4 ndcPos = clipPos / clipPos.w;
    return ndcPos.z * 0.5f + 0.5f;

}

void main(){

    depth = min(min(getScreenDepth(gl_in[0].gl_Position), getScreenDepth(gl_in[1].gl_Position)), min(getScreenDepth(gl_in[2].gl_Position), getScreenDepth(gl_in[5].gl_Position)));
    vec4 pos;
    for(int i = 0; i < 4; i++){
        pos += gl_in[i].gl_Position;
    }
    gl_Position = pos / 4;
	EmitVertex();
    EndPrimitive();
}