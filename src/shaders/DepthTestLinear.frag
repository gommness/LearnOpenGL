#version 330 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;

out vec4 color;

float linearizeDepth(float depth){
    float near = 0.1;
    float far = 50.0;
    float z = depth*2.0-1.0;
    return (2.0*near)/(far+near-z*(far-near));
}

void main(){
    float depth = linearizeDepth(gl_FragCoord.z);
    color = vec4(vec3(depth), 1.0f);
}
