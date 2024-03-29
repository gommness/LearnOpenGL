#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec4 color;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoords;
out vec4 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
    fragPos = vec3(model * vec4(aPos, 1.0f));
    fragNormal = mat3(transpose(inverse(model)))*normal;
    fragTexCoords = vec2(texCoords.x, texCoords.y);
    fragColor = color;
}
