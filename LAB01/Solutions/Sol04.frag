#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

vec2 fragCoord = gl_FragCoord.xy;
vec2 r = vec2(fragCoord.x / iResolution.x,
			  fragCoord.y / iResolution.y);


void main() {
	
	vec3 color1 = vec3(0.6, 0.2, 0.8);
	vec3 color2 = vec3(0.12, 0.659, 0.741);
	vec3 color3 = vec3(0.8, 0.4, 0.467);
	vec3 pixel;

	if(r.x < 1.0/3.0) {
		pixel = color1;
	} else if (r.x < 2.0/3.0){
		pixel = color2;
	} else {
		pixel = color3;
	}

	fragColor = vec4(pixel, 1.0);
}