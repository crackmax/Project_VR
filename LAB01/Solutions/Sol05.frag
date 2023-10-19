#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 r = vec2(fragCoord.x / iResolution.x,
			  fragCoord.y / iResolution.y);

	vec3 backgroundColor = vec3(1.0);
	vec3 color1 = vec3(0.6, 0.2, 0.8);
	vec3 color2 = vec3(0.12, 0.659, 0.741);
	vec3 color3 = vec3(0.8, 0.4, 0.467);
	vec3 color4 = vec3(0.867, 0.8, 0.467);
	vec3 pixel = backgroundColor;
	float eps = 0.01;

	if(abs(r.y - 0.3)<0.01) pixel = color4;

	float leftCoord = 0.15;
	float rightCoord = leftCoord + eps;
	if(r.x < rightCoord && r.x > leftCoord) pixel = color2;

	float lineCoordinate1 = 0.4;
	float lineThickness1 = 0.03;
	if(abs(r.x - lineCoordinate1) < lineThickness1) pixel = color1;

	if(abs(r.y - 0.6)<0.01) pixel = color3; 

	float lineCoordinate2 = 0.8;
	float lineThickness2 = 0.01;
	if(abs(r.x - lineCoordinate2) < lineThickness2) pixel = color2; 

	fragColor = vec4(pixel, 1.0);
}