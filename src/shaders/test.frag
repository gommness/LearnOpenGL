#version 330 core

#define N_POINTS 4

in vec2 fragTexCoords;

out vec4 color;

uniform sampler2D screenTexture;
uniform vec3 colorFactor = vec3(1.0, 0.9, 0.9);
uniform vec2 imageSize;
uniform vec2 bufferSize;

vec2 scale;

void main(){
    //scale = bufferSize/imageSize;
    //scale = bufferSize;
    scale = imageSize;
    //scale = vec2(1.0,1.0);
    vec2 fCoord = gl_FragCoord.xy/scale;
    color = texture(screenTexture, fCoord) * vec4(colorFactor, 1.0);
}



