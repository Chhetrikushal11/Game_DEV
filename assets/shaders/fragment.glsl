#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;
uniform sampler2D baseColorTexture;

void main()
{
    vec4 texColor = texture(baseColorTexture, vUV);
    FragColor = texColor;
}