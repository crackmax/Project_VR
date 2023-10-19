#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;
 
out vec4 fragColor;

void plot(vec2 r, float y, float lineThickness, vec3 color, inout vec3 pixel) {
	if(abs(y-r.y)<lineThickness) pixel = color;
}

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 r = 2.0*vec2(fragCoord.xy -0.5* iResolution.xy)/iResolution.y;

	vec3 bgCol = vec3(0.0);
	vec3 pixel = bgCol;
	vec3 axesColorX = vec3(1.0, 0.0, 0.0);
	vec3 axesColorY = vec3(0.0, 1.0, 0.0);
	vec3 gridColor = vec3(0.2, 0.3, 0.2);
	vec3 color1 = vec3(0.6, 0.2, 0.8);
	vec3 color2 = vec3(0.12, 0.659, 0.741);
	vec3 color3 = vec3(0.8, 0.4, 0.467);

	const float tickWidth = 0.1;
	for(float i=-2.0; i<2.0; i+=tickWidth) {
		if(abs(r.x - i)<0.004) pixel = gridColor;
		if(abs(r.y - i)<0.004) pixel = gridColor;
	}
	if( abs(r.x)<0.006) pixel = axesColorX;
	if( abs(r.y)<0.006) pixel = axesColorY;

	if(abs(r.x*r.x-0.2 - r.y) < 0.01) pixel = color1;
	if(abs(sin(3.14*r.x) - r.y) < 0.02) pixel = color1;

	plot(r, 0.6*exp(-10.0*(r.x+0.8)*(r.x+0.8)) - 0.1, 0.015, color2, pixel);
	plot(r, tanh(r.x), 0.015, color3, pixel);

	fragColor = vec4(pixel, 1.0);
}