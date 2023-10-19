#version 330 core
#define PI 3.14
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

out vec4 fragColor;

float plot(vec2 st, float pct){
  return  smoothstep( pct-0.02, pct, st.y) -
          smoothstep( pct, pct+0.02, st.y);
}

void main() {
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 uv = fragCoord/iResolution.xy;
	vec2 center = vec2(0.5,0.5);

	vec3 colorA = vec3(0.695,0.079,0.025);
	vec3 colorB = vec3(0.436,0.603,0.725);
	vec3 color = vec3(0.0);
	
	float m = 0.05;
	float pct = mod(length(uv-center),m)/m;

	color = mix(colorA, colorB, pct);

	fragColor = vec4(color,1.0);
}









