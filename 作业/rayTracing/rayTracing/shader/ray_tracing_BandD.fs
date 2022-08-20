#version 330 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D BandD;

void main(){
	vec3 color;

	for(int x = -2; x < 3; x++){
		for(int y = -2; y < 3; y++){
			color += texture(BandD,texCoords + vec2(x,y)).rgb;
		}
	}
	color /= 25.0f;	

	color = color / (vec3(1.0f) + color);
	FragColor = vec4(color,1.0f);
}