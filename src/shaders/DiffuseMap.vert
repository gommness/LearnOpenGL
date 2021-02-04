#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    fragPos = vec3(model * vec4(aPos, 1.0f));
    fragNormal = mat3(transpose(inverse(model)))*normal;
    fragTexCoords = texCoords;
}



