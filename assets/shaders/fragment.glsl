#version 330 core

struct Light
{
    vec3 color;
    vec3 position;
};

uniform Light uLight;
uniform vec3 uCameraPos;

in vec3 vertexColor;
in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform float uTime;
uniform sampler2D baseColorTexture; 
void main()
{
    vec3 norm = normalize(vNormal);
    
    // Diffuse
    vec3 lightDir = normalize(uLight.position - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;
    
    // Specular
    vec3 viewDir = normalize(uCameraPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * uLight.color;
    
    vec3 result = diffuse + specular;
    vec4 texColor = texture(baseColorTexture, vUV);
    FragColor = texColor * vec4(result, 1.0);
}