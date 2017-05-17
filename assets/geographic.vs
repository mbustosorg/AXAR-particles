#version 150 core

in vec4   iPosition;
in vec4   iPPosition;
in vec4   iHome;
in float  iDamping;
in vec4   iColor;
in float  iIndex;
in mat4   iTranslation;
in mat4   iRotation;

out vec4  position;
out vec4  pposition;
out vec4  home;
out float damping;
out vec4  color;
out float index;
out mat4  translation;
out mat4  rotation;

void main()
{
    position = iTranslation * iRotation * iPosition;
    color =     iColor;
    index =     iIndex;
    pposition = iPPosition;
    damping = iDamping;
    home = iHome;
    translation = iTranslation;
    rotation = iRotation;
}
