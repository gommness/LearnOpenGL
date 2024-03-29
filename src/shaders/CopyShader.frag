#version 330 core

#define N_POINTS 4

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D screenTexture;
uniform vec3 colorFactor = vec3(1.0, 1.0, 1.0);
uniform vec2 imageSize;
uniform vec2 bufferSize;

vec2 scale;

void main(){
    //scale = bufferSize/imageSize;
    scale = bufferSize;
    //scale = imageSize;
    //scale = vec2(1.0,1.0);
    color = texture(screenTexture, (gl_FragCoord.xy) / scale) * vec4(colorFactor, 1.0);
}


