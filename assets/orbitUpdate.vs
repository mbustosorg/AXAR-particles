#version 150 core

in vec4   iPosition;
in vec4   iColor;
in float  iIndex;
in float  iDelay;
in mat4   iTranslation;
in mat4   iRotation;

out vec4  position;
out vec4  color;
out float index;
out float delay;
out mat4  translation;
out mat4  rotation;

void main()
{
    delay = iDelay - 1.0f;
    if (delay < 0.0f) {
        delay = iDelay;
        position = iTranslation * iRotation * iPosition;
    } else {
        position = iPosition;
    }
    color = iColor;
    index = iIndex;
    translation = iTranslation;
    rotation = iRotation;
}
