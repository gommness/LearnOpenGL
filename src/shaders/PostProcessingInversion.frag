#version 330 core

#define N_POINTS 4

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D screenTexture;

void main(){
    vec3 col = 1-texture(screenTexture, fragTexCoords).rgb;
    color = vec4(col, 1.0);
}

