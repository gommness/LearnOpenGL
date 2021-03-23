#version 330 core

#define N_POINTS 4

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D screenTexture;

void main(){
    vec3 col = texture(screenTexture, fragTexCoords).rgb;
    float average = (col.r+col.g+col.b)/3.0;
    color = vec4(average, average, average, 1.0);
}

