#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoords;

out vec2 fragTexCoords;

void main(){
    gl_Position = vec4(aPos.x, 1-aPos.y, 0.0f, 1.0f);
    fragTexCoords = texCoords;
}
