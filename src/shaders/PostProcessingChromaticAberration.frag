#version 330 core

#define N_POINTS 4

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0/200;

const vec2 redOffset = vec2(-offset, -offset);
const vec2 greenOffset = vec2(0, -offset);
const vec2 blueOffset = vec2(offset, offset);

void main(){
    vec4 red   = texture(screenTexture, fragTexCoords-redOffset);
    vec4 green = texture(screenTexture, fragTexCoords-greenOffset);
    vec4 blue  = texture(screenTexture, fragTexCoords-blueOffset);
    color = vec4(red.r, green.g, blue.b, 1.0);
}

