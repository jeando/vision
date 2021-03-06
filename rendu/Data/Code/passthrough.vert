#version 330 core

uniform mat4 modelViewProjectionMatrix;

in vec3 in_Position;
in vec3 in_Color;

out vec3 pass_Color;

void main(void)
{
     gl_Position = modelViewProjectionMatrix * vec4(in_Position, 1.);
     pass_Color = in_Color;
}
