#version 330 core

#define RED_DECAY 0.002
#define GREEN_DECAY 0.0075
#define BLUE_DECAY 0.05

in vec2 fragTexCoords;

layout(location = 0) out vec4 color;

uniform sampler2D screenTexture;
uniform vec2 imageSize;
uniform vec2 bufferSize;

vec2 scale;

vec4 getCol(int x, int y) {
    vec4 col = texture(screenTexture, (gl_FragCoord.xy+vec2(x, y)) / scale);
    return col;
}

int getState(int x, int y) {
    int r = int(getCol(x,y).r);
    if (r == 1)
        return 1;
    else
        return 0;
}

void main(){
    scale = imageSize;
    int sum = getState(-1, -1) +
              getState(-1,  0) +
              getState(-1,  1) +
              getState( 0, -1) +
              getState( 0,  1) +
              getState( 1, -1) +
              getState( 1,  0) +
              getState( 1,  1);
    if (sum == 3) {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    } else if (sum == 2) {
        float current = float(getState(0, 0));
        if (current == 1) {
            color = vec4(current, current, current, 1.0);
        } else {
            color = getCol(0, 0) - vec4(RED_DECAY,GREEN_DECAY,BLUE_DECAY,0.0);
        }
    } else {
        color = getCol(0, 0) - vec4(RED_DECAY,GREEN_DECAY,BLUE_DECAY,0.0);
    } 
    /*
    int cell = get(0,0);
    if (cell == 0) {
        color = vec4(1,1,1,1);
    } else {
        color = vec4(0,0,0,0);
    }
    */
    //color = texture(screenTexture, (gl_FragCoord.xy) / scale) + vec4(0.4, 0.0, 0.0, 1);
}

