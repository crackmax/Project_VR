#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;
 
out vec4 fragColor;


void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec3 color1 = vec3(0.6, 0.2, 0.8);
	vec3 color2 = vec3(0.12, 0.659, 0.741);
	
	vec3 pixel;
	if( fragCoord.x < 175.0) {
		pixel = color2;
	} else {
		pixel =  color1;
	}

	fragColor = vec4(pixel, 1.0);
}