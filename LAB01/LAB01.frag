#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec3 grey = vec3(1.0,1.0,1.0);
	vec3 green = vec3(0.0, 1.0, 0.0);
	vec3 red = vec3(1.0, 0.0, 0.0);
	vec3 pixel;
	pixel = (fragCoord.x > iResolution.x / 2.0) ? color1 : color2;

	fragColor = vec4(pixel, 1.0);
}









