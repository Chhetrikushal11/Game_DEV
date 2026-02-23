#version 330 core

struct Light
{
    vec3 color;
    vec3 position;
};

uniform Light uLight;

in vec3 vertexColor;
in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform float uTime;
uniform sampler2D brickTexture;

void main()
{
    vec3 norm = normalize(vNormal);

    vec3 lightDir = normalize(uLight.position - vFragPos);

    float diff = max(dot(norm, lightDir),0.0);

    vec3 diffuse = diff * uLight.color;
    vec4 texColor = texture(brickTexture, vUV);
    FragColor = texColor * vec4(diffuse, 1.0);
}