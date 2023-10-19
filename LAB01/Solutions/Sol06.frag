#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;


void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 r = vec2(fragCoord.xy / iResolution.xy);

	vec3 backgroundColor = vec3(1.0);
	vec3 axesColorX = vec3(1.0, 0.0, 0.0);
	vec3 axesColorY = vec3(0.0, 1.0, 0.0);
	vec3 gridColor = vec3(0.5);
	
	vec3 pixel = backgroundColor;
	
	const float tickWidth = 0.1;

	for(int k=1; k<10; k++){
		float i = float(k)/10.0;
		if(abs(r.x - i)<0.002) pixel = gridColor;
		if(abs(r.y - i)<0.002) pixel = gridColor;
	}

	if(abs(r.x)<0.005) pixel = axesColorX;
	if(abs(r.y)<0.006) pixel = axesColorY;

	fragColor = vec4(pixel, 1.0);
}