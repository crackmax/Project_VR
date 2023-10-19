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
	vec3 pixel = bgCol;
	vec3 color1 = vec3(0.6, 0.2, 0.8);
	vec3 color2 = vec3(0.12, 0.659, 0.741);
	

	float ret = smoothstep(0.30, 0.55, p.y);

	pixel = mix(color2, color1, ret);
	fragColor = vec4(pixel, 1.0);
}