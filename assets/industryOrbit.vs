#version 150 core

uniform   float transitionFactor;

in vec4   iPosition;
in vec4   iStartPosition;
in vec4   iSphericalPosition;
in vec4   iColor;
in float  iIndex;
in float  iDelay;
in mat4   iTranslation;
in mat4   iRotation;

out vec4  position;
out vec4  startPosition;
out vec4  sphericalPosition;
out vec4  color;
out float index;
out float delay;
out mat4  translation;
out mat4  rotation;

float quarticInOut(in float t) {
    if (t < 0.5) return (8.0f * t * t * t * t);
    else return(-0.5 * pow(2.0 - 2.0 * t, 4.0) + 1.0);
}

void main()
{
    delay = iDelay;
    position = iPosition;
    if (transitionFactor - delay > 0.0f) {
        sphericalPosition = iRotation * iSphericalPosition;
        if ((transitionFactor - delay) < 180.0f) {
            position = iStartPosition - (iStartPosition - iRotation * iTranslation * iSphericalPosition) *
                        quarticInOut((transitionFactor - delay) / 180.0f);
        } else {
            position = iRotation * iTranslation * iSphericalPosition;
        }
    } else {
        sphericalPosition = iSphericalPosition;
        position = iStartPosition; //startPositions[int(iIndex)];
    }
    startPosition = iStartPosition;
    color = iColor;
    index = iIndex;
    translation = iTranslation;
    rotation = iRotation;
}
