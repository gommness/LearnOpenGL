#version 330 core

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

int getState(int x, int y) {
    int r = int(getCol(x,y).r);
    if (r == 1)
        return 1;
    else
        return 0;
}

int rule() {
    int stateLeft = getState(-1, 1);
    int stateCenter = getState(0, 1);
    int stateRight = getState(1, 1);
    
    if(stateLeft == 1) {
        if(stateCenter == 1) {
            if(stateRight == 1) {
                #insert rule111
            } else {
                #insert rule110
            }
        } else {
            if(stateRight == 1) {
                #insert rule101
            } else {
                #insert rule100
            }
        }
    } else {
        if(stateCenter == 1) {
            if(stateRight == 1) {
                #insert rule011
            } else {
                #insert rule010
            }
        } else {
            if(stateRight == 1) {
                #insert rule001
            } else {
                #insert rule000
            }
        }
    }
}

vec4 bufferCopy() {
    return getCol(0, -1);
}

void main(){
    if (gl_FragCoord.y <= 1) {
        color = rule()*maxCol;
    } else {
        color = bufferCopy();
    }
}
