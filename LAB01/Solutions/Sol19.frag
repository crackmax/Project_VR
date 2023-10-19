#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

in vec2 uv; 
out vec4 fragColor;

void disk(vec2 r, vec2 center, float radius, vec3 col, inout vec3 pixel){
	if(length(r-center) < radius){
		pixel = col;
	}
}

void main() {
	vec4 fragCoord = gl_FragCoord;
	vec3 pixel;

	vec3 gridColor=vec3 (0.0);
	vec3 color1 = vec3(0.0,1.0,0.0); //green
	vec3 color2 = vec3(1.0,0.0,0.0); //red
	vec3 color3 = vec3(0.0,0.0,1.0); //blue
	vec3 color4 = vec3(0.95,0.56,0.05); //orange
	vec3 color5 = vec3(1.0,1.0,0.0); //yellow
	vec3 color6 = vec3(0.95,0.95,0.95); //white

	if(uv.y < 0.25) pixel = color5;
	else if(uv.y > 0.5) pixel = color6;
	else if(uv.x < 0.25) pixel = color1;
	else if(uv.x < 0.5) pixel = color2;
	else if(uv.x < 0.75) pixel = color3;
	else pixel = color4;


	const float tickWidth = 0.0833;
	for(float i = -1; i < 1; i += tickWidth){
		if (abs(uv.x - i) < 0.004) pixel= gridColor;
		if (abs(uv.y - i) < 0.004) pixel= gridColor;
	}

	//not usefull but if you waant the trick with the mouse
	vec2 r = 2.0*vec2(fragCoord.xy -0.5*iResolution.xy)/iResolution.y;
	vec2 center = 2.0*vec2(iMouse.xy -0.5*iResolution.xy)/iResolution.y;
	center.y = -center.y;
	vec3 col_circle = 0.5 + 0.5*cos(center.xyx+vec3(0,2,4));
	disk(r, center, 0.2, col_circle, pixel);
	fragColor = vec4(pixel,1.0);
}