#version 330 core

#define N_POINTS 1

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

vec3 computeDirectionalLight(DirectionalLight light, float shininess, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(lightDir, normal), 0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), shininess);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, float shininess, vec3 normal, vec3 viewDir, vec3 vertPos){
    vec3 lightDir = normalize(light.position - vertPos );

    // Diffuse shading
    float diff = max(dot(lightDir, normal), 0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0), shininess);
    // Attenuation
    float dist = length(light.position - vertPos);
    float attenuation = 1.0f/(light.constant + light.linear*dist + light.quadratic*dist*dist);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    return ambient + diffuse + specular;
}

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoords;
out vec4 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform DirectionalLight dirLight;
uniform PointLight pointLight[N_POINTS];
uniform vec3 viewPos;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 color;

void main(){
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    fragPos = vec3(model * vec4(aPos, 1.0f));
    fragNormal = mat3(transpose(inverse(model)))*normal;
    fragTexCoords = vec2(texCoords.x, texCoords.y);

    float shininess = 0.7f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0.0f);
    result = computeDirectionalLight(dirLight, shininess, fragNormal, viewDir);
    for(int i=0; i<N_POINTS; ++i){
        result += computePointLight(pointLight[i], shininess, fragNormal, viewDir, fragPos);
    }
    vec3 ambientLight= vec3(0.01f, 0.01f, 0.01f);
    result += ambientLight;
    fragColor = vec4(result, 1.0);
}
