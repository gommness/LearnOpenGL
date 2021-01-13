#version 330 core

struct Material {
    sampler2D diffuse; // opaque type => cannot be instantiated
    sampler2D specular;
    float shininess;
};
// now that Material contains an opaque type (cannot be instantiated unless it is a uniform)
// the Material struct itself has become opaque, thus we can only instantiate it via uniform

struct Light{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;

out vec4 color;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){
    // ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragTexCoords));

    // diffuse light
    vec3 norm = normalize(fragNormal);
    vec3 lightDirection = normalize(light.position - fragPos);
    float diff = max(0.0, dot(norm, lightDirection));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragTexCoords));

    // specular light
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(0, dot(reflectionDirection, viewDirection)), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragTexCoords));

    float theta = dot(light.direction, normalize(-lightDirection));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.1, 1.0);

    vec3 result = intensity*(ambient + diffuse + specular);
    color = vec4(result, 1.0f);
}

