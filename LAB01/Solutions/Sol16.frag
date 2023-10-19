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
	vec3 black = vec3(0.0); vec3 white = vec3(1.0); vec3 gray = vec3(0.3);

	vec3 col1 = vec3(0.216, 0.471, 0.698); //blue
	vec3 col2 = vec3(1.00, 0.329, 0.298); //red
	vec3 col3 = vec3(0.867, 0.910, 0.247); //yellow
	vec3 ret; float d;

	if(p.x < 1./3.) {
		ret = gray;
		d = disk(r, vec2(-1.1,0.3), 0.3); if (d==1.0) ret = col1;
		d = disk(r, vec2(-1.3,0.0), 0.3); if (d==1.0) ret = col2;
		d = disk(r, vec2(-1.05,-0.3), 0.3); if (d==1.0) ret = col3;
	}
	else if(p.x < 2./3.) {
		ret = black;
		ret += disk(r, vec2(0.10,0.3), 0.3)*col1;
		ret += disk(r, vec2(-0.1,0.0), 0.3)*col2;
		ret += disk(r, vec2(.15,-0.3), 0.3)*col3;
	}
	else {
		ret = white;
		ret -= disk(r, vec2(1.10,0.3), 0.3)*col1;
		ret -= disk(r, vec2(1.05,0.0), 0.3)*col2;
		ret -= disk(r, vec2(1.35,-0.3), 0.3)*col3;
	}

	fragColor = vec4(ret, 1.0);
}