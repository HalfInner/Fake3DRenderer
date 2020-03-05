#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vertexColor;
out vec3 lightColor;
out vec3 lightPos;
out vec3 viewPos;

out vec3 FragPos;
out vec3 Normal;

uniform vec3 colorObject;
uniform vec3 lightColorObject;
uniform vec3 lightPosObject;
uniform vec3 viewPosObject;

uniform vec3 pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

    vertexColor = colorObject;
    lightColor = lightColorObject;
    lightPos = lightPosObject;
    viewPos = viewPosObject;

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}