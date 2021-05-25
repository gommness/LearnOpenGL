#version 330 core

#define N_POINTS 4

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D screenTexture;
uniform vec2 scale;
uniform vec3 colorFactor = vec3(1.0, 1.0, 1.0);

void main(){
    color = texture(screenTexture, (gl_FragCoord.xy) / scale) * vec4(colorFactor, 1.0);
}


