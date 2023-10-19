#version 330 core
precision mediump float;
uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

in vec2 uv; 
out vec4 fragColor;


//  For this exo the coordinate uv are put that way
//   1^
//	  |
//    |
//	  |_______
//    |       |
//    |   c   |
//    |_______|_______________________  
//   v|       |       |       |      |
//    |   a   |   d   |   e   |   f  |
//    |_______|_______|_______|______|
//    |       |
//    |   b   |
//   0|_______|_______________________>
//					u                1
//
//In 3D space the cube is positionned like that
//
//                
//               y   
//               |      z  
//               |     / 
//               |    /
//            _______
//			 /  c   /| 
//			/______/ | ---------> x
//			|      |d|
//			|   a  | /
//			|______|/
// 
// 

void disk(vec2 r, vec2 center, float radius, vec3 col, inout vec3 pixel){
	if(length(r-center) < radius){
		pixel = col;
	}
}

void main() {
	vec4 fragCoord = gl_FragCoord;
	vec3 pixel = vec3(uv.x,uv.y,0.3);
	vec2 p = fragCoord.xy / iResolution.xy;
	pixel = vec3(p.xy, 0.3);

	//TODO 
	//Try to put a grid on the cube using the uv variable to do something like a rubik's cube
	//See the difference when you use the screen coordinates
	//See the other comment above to better understand how the uv variable is set across the faces

	fragColor = vec4(pixel,1.0);
}