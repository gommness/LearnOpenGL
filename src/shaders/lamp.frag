#version 330 core

out vec4 color;

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    color = vec4(lightColor, 1.0f);
}

