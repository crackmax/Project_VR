#version 330 core

in vec2 f_tex_coord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform vec3 u_light_pos;
uniform float u_light_intensity;
uniform vec3 u_light_color;

void main() {
    // Calculate lighting in the fragment shader
    vec3 lightDir = normalize(u_light_pos - FragPos);
    float diffusion = max(0.0, dot(Normal, lightDir)) * u_light_intensity;
    vec3 diffuse = u_light_color * diffusion * 0.9;

    // Sample the texture
    vec4 textureColor = texture(textureSampler, f_tex_coord);
    
    // Apply diffuse lighting to the texture color
    vec3 finalColor = textureColor.rgb * diffuse;

    FragColor = vec4(finalColor, 1.0);
}
