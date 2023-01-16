#version 330 core

#define RED_DECAY 0.002
#define GREEN_DECAY 0.0075
#define BLUE_DECAY 0.05

#define RED_MIN 0.2
#define GREEN_MIN 0.05
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
    vec4 col = getCol(x,y);
    if (col == vec4(0,0,0,1))
        return col;
    col -= vec4(RED_DECAY, GREEN_DECAY, BLUE_DECAY, 0.0);
    return vec4(max(col.r, RED_MIN), max(col.g, GREEN_MIN), max(col.b, BLUE_MIN), 1.0);
}

int getState(int x, int y) {
    int r = int(getCol(x,y).r);
    if (r == 1)
        return 1;
    else
        return 0;
}

vec4 gameOfLife() {
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
    } else if (sum == 2) {
        float current = float(getState(0, 0));
        if (current == 1) {
            return maxCol;
        } else {
            return decayCol(0,0);
        }
    } else {
        return decayCol(0,0);
    } 
}

vec4 rule30() {
    int stateLeft = getState(-1, 1);
    int stateCenter = getState(0, 1);
    int stateRight = getState(1, 1);
    
    if(stateLeft == 1) {
        if(stateCenter == 1) {
            if(stateRight == 1) {
                // 111
                return zeroCol; 
            } else {
                // 110
                return zeroCol; 
            }
        } else {
            if(stateRight == 1) {
                // 101
                return zeroCol; 
            } else {
                // 100
                return maxCol;
            }
        }
    } else {
        if(stateCenter == 1) {
            if(stateRight == 1) {
                // 011
                return maxCol;
            } else {
                // 010
                return maxCol;
            }
        } else {
            if(stateRight == 1) {
                // 001
                return maxCol;
            } else {
                // 000
                return zeroCol;
            }
        }
    }
}

vec4 bufferCopy() {
    return getCol(0, -1);
}

void main(){
    vec2 pixel = gl_FragCoord.xy*scale;
    float golVerticalLimit = 0.25;

    if (pixel.y >= golVerticalLimit) {
        // Game Of Life Area
        color = gameOfLife();
    } else if (gl_FragCoord.y <= 2) {
        // Rule 30 Area
        if(gl_FragCoord.y <= 1) {
            color = zeroCol;
        } else {
            color = rule30();
        }
    } else {
        // Rule 30 Copy Area
        color = bufferCopy();
    }

    /*
    int cell = get(0,0);
    if (cell == 0) {
        color = vec4(1,1,1,1);
    } else {
        color = vec4(0,0,0,0);
    }
    */
    scale = imageSize;
    //color = texture(screenTexture, (gl_FragCoord.xy) / scale) + vec4(0.4, 0.0, 0.0, 1);
}


