#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

float disk(vec2 r, vec2 center, float radius){
	float d = length(r-center);
	return (length(d) <= radius) ? 1.0: 0.0;
}

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 p = vec2(fragCoord.xy/iResolution.xy);
	vec2 r = 2.0*vec2(fragCoord.xy -0.5*iResolution.xy)/iResolution.y;
	vec2 center = 2.0*vec2(iMouse.xy -0.5*iResolution.xy)/iResolution.y;
	center.y = -center.y;

	vec3 bgColor = vec3(0.1);
	vec3 circleColor = 0.5 + 1.5*cos(iTime + 3.5*p.xyx+vec3(0,2,4));
	
	vec3 pixel = bgColor;

	if(disk(r,center, 0.2)==1.0) pixel = circleColor;
	
	fragColor = vec4(pixel,1.0);
}