#version 330 core

out vec4 fragColor;
in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main(){
    vec2 targetTexCoord = vec2(texCoord.x, 1-texCoord.y);
    fragColor = mix(texture(tex0, texCoord), texture(tex1, targetTexCoord), 0.5);
}



