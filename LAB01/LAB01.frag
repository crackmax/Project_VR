#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 uv = fragCoord/iResolution.xy;
	vec3 col = 0.5 + 0.5*cos(iTime + uv.xyx+vec3(0,2,4));
	fragColor = vec4(col,1.0);
}









