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
    vec3 surfaceColor = vec3(texture(material.texture_diffuse1, fragTexCoords));
    color = vec4(surfaceColor, 1.0);
}

