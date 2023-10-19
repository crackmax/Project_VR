#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

void disk(vec2 r, vec2 center, float radius, vec3 color, inout vec3 pixel){
	if( length(r-center) < radius) pixel = color;
}

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 r = 2.0*vec2(fragCoord.xy - 0.5*iResolution.xy)/iResolution.y;

	vec3 bgCol = vec3(0.3);
	vec3 color1 = vec3(0.6, 0.2, 0.8);
	vec3 color2 = vec3(0.12, 0.659, 0.741);
	vec3 color3 = vec3(0.8, 0.4, 0.467);
	vec3 pixel=bgCol;

	float radius = 0.8;
	
	disk(r, vec2(0,0), radius, color1, pixel);
	disk(r, vec2(0,0), 0.3, color2, pixel);

	vec2 center = vec2(0.9, -0.4);
	disk(r, center, 0.6, color3, pixel);

	fragColor = vec4(pixel, 1.0);
}