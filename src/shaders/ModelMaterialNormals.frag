#version 330 core

#define N_POINTS 1


struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;
in vec4 fragColor;

uniform DirectionalLight dirLight;
uniform PointLight pointLight[N_POINTS];
uniform Material material;
uniform vec3 viewPos;

out vec4 color;

void main(){
    //vec3 norm = normalize(fragNormal);
    color = vec4(fragNormal, 1.0);
}
