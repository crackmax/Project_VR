#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 r = vec2(fragCoord.xy - 0.5*iResolution.xy);
	r = 2.0 * r.xy / iResolution.xy;
	vec3 backgroundColor = vec3(1.0);
	vec3 axesColorX = vec3(1.0, 0.0, 0.0);
	vec3 axesColorY = vec3(0.0, 1.0, 0.0);
	vec3 gridColor = vec3(0.5);
	
	vec3 pixel = backgroundColor;
	const float tickWidth = 0.1;

	
	if( mod(r.x, tickWidth)<0.008) pixel = gridColor;
	if( mod(r.y, tickWidth)<0.008) pixel = gridColor;
	

	if(abs(r.x)<0.005) pixel = axesColorX;
	if(abs(r.y)<0.006) pixel = axesColorY;

	fragColor = vec4(pixel, 1.0);
}