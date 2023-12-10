#version 330 core

in vec2 f_tex_coord;
in vec3 v_frag_coord;
in mat3 TBN;

out vec4 FragColor;

uniform vec3 u_view_pos;
uniform sampler2D textureSampler;
uniform sampler2D normalMap;
uniform vec3 u_light_pos;
uniform vec3 u_light_color;
uniform float u_light_intensity;

float specularCalculation(vec3 N, vec3 L, vec3 V, float shininess) {
    vec3 R = reflect(-L, N);
    float cosTheta = dot(R, V);
    float spec = pow(max(cosTheta, 0.0), shininess);
    return 0.8 * spec;
}

void main() {

    vec3 L = normalize(u_light_pos - v_frag_coord);
    vec3 V = normalize(u_view_pos - v_frag_coord);
    // normals are [-1,1] and color is [0,1]
    vec3 normalMap = normalize(texture(normalMap, f_tex_coord).rgb * 2.0 - 1.0);
    vec3 N = normalize(TBN * normalMap);
    float diff = max(dot(N, L), 0.0);
    
    float shininess = 32.0;
    float specular = specularCalculation(N, L, V, shininess);
    

    float distance = length(u_light_pos - v_frag_coord);
    float attenuation = 1.0 / (0.03 * distance + 0.008 * distance * distance);
    
    float light = 0.06 + attenuation * (diff + specular);

    vec4 textureColor = texture(textureSampler, f_tex_coord);
    vec3 finalColor = vec3(textureColor.rgb * vec3(light) * u_light_color);

    FragColor = vec4(finalColor, 1.0);
}
