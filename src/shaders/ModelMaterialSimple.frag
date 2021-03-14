#version 330 core

#define N_POINTS 4


struct Material{
    sampler2D texture_diffuse1;
};

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;

uniform Material material;
uniform vec3 viewPos;

out vec4 color;

void main(){
    vec4 surfaceColor = texture(material.texture_diffuse1, fragTexCoords);
    color = surfaceColor;
}

