#version 330 core

#define N_POINTS 4


struct Material{
    sampler2D texture_diffuse;
    sampler2D texture_specular;
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

vec3 computeDirectionalLight(DirectionalLight light, Material material, vec2 texCoords, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(lightDir, normal), 0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

    vec3 surfaceColor = vec3(texture(material.texture_diffuse, texCoords));
    vec3 ambient = light.ambient * surfaceColor;
    vec3 diffuse = light.diffuse * diff * surfaceColor;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, texCoords));

    return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, Material material, vec2 texCoords, vec3 normal, vec3 viewDir, vec3 fragPos){
    vec3 lightDir = normalize(fragPos - light.position);

    // Diffuse shading
    float diff = max(dot(lightDir, normal), 0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    // Attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0f/(light.constant + light.linear*dist + light.quadratic*dist*dist);

    vec3 surfaceColor = vec3(texture(material.texture_diffuse, texCoords));
    vec3 ambient = light.ambient * surfaceColor;
    vec3 diffuse = light.diffuse * diff * surfaceColor;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, texCoords));

    return ambient + diffuse + specular;
}

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
    vec3 ambientLight= vec3(0.05f, 0.05f, 0.05f);
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = computeDirectionalLight(dirLight, material, fragTexCoords, fragNormal, viewDir);
    for(int i=0; i<N_POINTS; ++i){
        result += computePointLight(pointLight[i], material, fragTexCoords, fragNormal, viewDir, fragPos);
    }
    result += ambientLight;
    
    color = fragColor * vec4(result, 1.0);
}
