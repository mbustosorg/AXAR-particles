#version 150 core

in vec4   iPosition;
in vec4   iColor;
in vec4   iPPosition;
in float  iIndex;
in mat4   iTranslation;
in mat4   iRotation;
in mat4   iTransition;

out vec4  position;
out vec4  color;
out vec4  pposition;
out float index;
out mat4  translation;
out mat4  rotation;
out mat4  transition;

void main()
{
    position = iTranslation * iRotation * iPosition;
    color = iColor;
    index = iIndex;
    pposition = iPPosition;
    translation = iTranslation;
    rotation = iRotation;
    transition = iTransition;
}
