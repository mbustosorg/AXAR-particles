#version 150 core

uniform   vec3 startPositions[1000 * 20]; // Number of entities * TRAIL_LENGTH
uniform   float transitionFactor;

in vec4   iPosition;
in vec4   iSphericalPosition;
in vec4   iColor;
in float  iIndex;
in float  iDelay;
in mat4   iTranslation;
in mat4   iRotation;

out vec4  position;
out vec4  sphericalPosition;
out vec4  color;
out float index;
out float delay;
out mat4  translation;
out mat4  rotation;

void main()
{
    delay = iDelay - 1.0f;
    if (delay < 0.0f) {
        sphericalPosition = iRotation * iSphericalPosition;
        position = iRotation * iTranslation * iSphericalPosition;
    } else {
        sphericalPosition = iSphericalPosition;
        position = iPosition;
    }
    color = iColor;
    index = iIndex;
    translation = iTranslation;
    rotation = iRotation;
}
