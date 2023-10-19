#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;
 
out vec4 fragColor;

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 p = vec2(fragCoord.xy / iResolution.xy);

	vec3 bgCol = vec3(0.0);
	vec3 pixel=bgCol;

	float ret = smoothstep(0.30, 0.55, p.y);

	pixel = vec3(ret);
	fragColor = vec4(pixel, 1.0);
}