#version 330 core

in vec2 fragTexCoords;
in vec3 fragNormal;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
    vec3 norm = normalize(fragNormal);
    vec3 lDir = vec3(1,-1,-1);
    vec3 lightDirection = normalize(-lDir);
    float diff = max(0.0, dot(norm, lightDirection));
    color = vec4(0.2+diff, 0.2, 0.2, 1);
    //vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragTexCoords));

    //color = vec4(1,1,1,1);
    //color = vec4(texture(texture_diffuse1, fragTexCoords));
}
