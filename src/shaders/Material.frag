#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragNormal;
in vec3 fragPos;

out vec4 color;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){
    // ambient light
    vec3 ambient = light.ambient * material.ambient;

    // diffuse light
    vec3 norm = normalize(fragNormal);
    vec3 lightDirection = normalize(light.position - fragPos);
    float diff = max(0.0, dot(norm, lightDirection));
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular light
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(0, dot(reflectionDirection, viewDirection)), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}
