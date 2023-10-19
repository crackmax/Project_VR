#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;
 
out vec4 fragColor;

float disk(vec2 r, vec2 center, float radius){
	float d = length(r-center);
	return (length(d) <= radius) ? 1.0: 0.0;
}

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 p = vec2(fragCoord.xy/iResolution.xy);
	vec2 r = 2.0*vec2(fragCoord.xy - 0.5*iResolution.xy)/iResolution.y;
	vec3 col1=vec3(0.216, 0.471, 0.698);
	vec3 col2=vec3(1.00, 0.329, 0.298);
	vec3 ret = vec3(0.3);

	if(p.x < 1./2.){
		vec2 q = r - vec2(-0.5, 0.);
		float amplitude = 0.8;
		float y = 0.8*sin(0.5*iTime);
		float radius = 0.15 + 0.05*sin(iTime*8.0);
		if(disk(q, vec2(0.0, y), radius)==1.0) ret = col2;
	}
	else if(p.x > 1./2.) {
		vec2 q = r - vec2(0.5,0.0);
		for(int k = -10; k<10; k+=2){
			float y = float(k)/10.0;
			float x = 0.2*cos(iTime*5.0 + y*3.14);
			if(disk(q, vec2(x,y), 0.06)==1.0) ret = col1;
		}
	}
	fragColor = vec4(ret, 1.0);
}