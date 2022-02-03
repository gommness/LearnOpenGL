#version 330 core

#define N_POINTS 4


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

float cellShade(vec3 lightDir, vec3 normal, float umbral) {
    return step(umbral, dot(lightDir, normal));
}

vec3 computeDirectionalLight(DirectionalLight light, Material material, vec2 texCoords, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diffuseAlike = clamp(step(0.2,dot(lightDir, normal)), 0.2, 1.0);
    float specularAlike = clamp(step(0.2,dot(reflectDir, viewDir)), 0.2, 1.0);
    //return vec3(max(diffuseAlike, specularAlike));
    return vec3(specularAlike)*vec3(texture(material.texture_diffuse1, texCoords));
}

vec3 computePointLight(PointLight light, Material material, vec2 texCoords, vec3 normal, vec3 viewDir, vec3 fragPos){
    vec3 lightDir = normalize(fragPos - light.position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float diffuseAlike = dot(lightDir, normal);
    float specularAlike = 0.0;//step(0.99,dot(normal, viewDir));
    //return vec3(max(diffuseAlike, specularAlike));
    return vec3(specularAlike);
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
    vec3 ambientLight= vec3(0.11f, 0.11f, 0.11f);
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = computeDirectionalLight(dirLight, material, fragTexCoords, fragNormal, viewDir);
    //for(int i=0; i<N_POINTS; ++i){
    //    result += computePointLight(pointLight[i], material, fragTexCoords, fragNormal, viewDir, fragPos);
    //}
    result += ambientLight*vec3(texture(material.texture_diffuse1, fragTexCoords));
    
    color = fragColor * vec4(result, texture(material.texture_diffuse1, fragTexCoords).a);
}

