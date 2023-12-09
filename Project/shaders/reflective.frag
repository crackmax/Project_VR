#version 400 core
out vec4 FragColor;
precision mediump float;

in vec3 v_frag_coord;
in vec3 v_normal;

uniform vec3 u_view_pos;

uniform samplerCube cubemapSampler;

void main() {
    vec3 N = normalize(v_normal);
    vec3 V = normalize(u_view_pos - v_frag_coord);
    vec3 R = reflect(-V, N);
    
    // Sample from the cube map
    vec3 cubeMapColor = texture(cubemapSampler, R).rgb;

    // Combine the reflections (you might want to adjust the weights)
    vec3 finalColor = 0.99 * cubeMapColor;

    FragColor = vec4(finalColor, 1.0);
}
