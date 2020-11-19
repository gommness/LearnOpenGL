#version 330 core

in vec3 fragNormal;
in vec3 fragPos;

out vec4 color;

uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.3f;
    float specularStrength = 0.5f;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff*lightColor;

    vec3 viewDir = normalize(viewPos-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 ambient = ambientStrength * lightColor;
    color = vec4((ambient + diffuse + specular) * objColor, 1.0f);
    //color = vec4(lightColor, 1.0f)*texture(tex0, texCoord);
}
