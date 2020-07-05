#version 330 core

out vec4 fragColor;
in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D tex;

void main(){
    fragColor = texture(tex, texCoord);
}



