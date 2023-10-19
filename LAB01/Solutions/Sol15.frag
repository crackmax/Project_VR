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
	vec2 r = 2.0*vec2(fragCoord.xy - 0.5*iResolution.xy)/iResolution.y;

	float ret = disk(r, vec2(0.0,0.0), 0.5);

	fragColor = vec4(vec3(ret), 1.0);
}