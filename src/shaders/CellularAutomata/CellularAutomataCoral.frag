#version 330 core

#define RED_DECAY 0.01
#define GREEN_DECAY 0.01
#define BLUE_DECAY 0.005

#define RED_MIN 0
#define GREEN_MIN 0
#define BLUE_MIN 0

in vec2 fragTexCoords;

layout(location = 0) out vec4 color;

uniform sampler2D screenTexture;
uniform vec2 imageSize;
uniform vec2 bufferSize;
vec2 scale;

vec4 maxCol = vec4(1.0, 1.0, 1.0, 1.0);
vec4 zeroCol = vec4(0.0, 0.0, 0.0, 1.0);

vec4 getCol(int x, int y) {
    vec4 col = texture(screenTexture, (gl_FragCoord.xy+vec2(x, y)) / scale);
    return col;
}

vec4 decayCol(int x, int y) {
    //return zeroCol;
    vec4 col = getCol(x,y);
    if (col == vec4(0,0,0,1))
        return col;
    col -= vec4(RED_DECAY, GREEN_DECAY, BLUE_DECAY, 0.0);
    return vec4(max(col.r, RED_MIN), max(col.g, GREEN_MIN), max(col.b, BLUE_MIN), 1.0);
}

int getState(int x, int y) {
    int g = int(getCol(x,y).g);
    if (g == 1)
        return 1;
    else
        return 0;
}

vec4 anneal() {
    // B3/S45678
    int sum = getState(-1, -1) +
              getState(-1,  0) +
              getState(-1,  1) +
              getState( 0, -1) +
              getState( 0,  1) +
              getState( 1, -1) +
              getState( 1,  0) +
              getState( 1,  1);
    int current = getState(0,0);
    if (current == 0 && sum == 3) {
        return maxCol;
    } else if (sum >= 4) {
        return getCol(0,0);
    } else {
        return decayCol(0,0);
    }
}

vec4 bufferCopy() {
    return getCol(0, -1);
}

void main(){
    scale = imageSize;
    vec2 pixel = gl_FragCoord.xy/scale;
    float golVerticalLimit = 0.25;

    color = anneal();
}

