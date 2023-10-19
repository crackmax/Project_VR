#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec3 color = vec3(0.6, 0.2, 0.8);
	float alpha = 1.0;

	vec4 pixel = vec4(color, alpha);
	fragColor = pixel;
}


