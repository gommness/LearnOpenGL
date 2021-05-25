#version 330 core

#define RED_DECAY 0.1
#define GREEN_DECAY 0.1
#define BLUE_DECAY 0.05

#define RED_MIN 0
#define GREEN_MIN 0
#define BLUE_MIN 0

in vec2 fragTexCoords;

layout(location = 0) out vec4 color;

uniform sampler2D screenTexture;
uniform vec2 scale;

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
    vec4 col = getCol(x,y);
    bool red = (int(col.r) == 1);
    bool green = (int(col.g) == 1);
    bool blue = (int(col.b) == 1);
    if (red && green && blue)
        return 1;
    else
        return 0;
}

vec4 lifeWithoutDeath() {
    // B3/S012345678
    int sum = getState(-1, -1) +
              getState(-1,  0) +
              getState(-1,  1) +
              getState( 0, -1) +
              getState( 0,  1) +
              getState( 1, -1) +
              getState( 1,  0) +
              getState( 1,  1);
    if (sum == 3) {
        return maxCol;
    } else {
        return getCol(0,0);
    }
}

vec4 bufferCopy() {
    return getCol(0, -1);
}

void main(){
    vec2 pixel = gl_FragCoord.xy/scale;
    float golVerticalLimit = 0.25;

    color = lifeWithoutDeath();
}




