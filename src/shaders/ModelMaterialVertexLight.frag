#version 330 core

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};


in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;
in vec4 fragColor;

out vec4 color;

uniform Material material;

void main(){
    color = fragColor * texture(material.texture_diffuse1, fragTexCoords);
}

