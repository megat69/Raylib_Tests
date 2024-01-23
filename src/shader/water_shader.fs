#version 330
#define MAX_WAVE_COEFFICIENTS 5

// Input vertex attributes
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;
uniform float waveCoefficients[MAX_WAVE_COEFFICIENTS];
uniform int nbWaveCoefficients;
uniform vec2 resolution;

// Output fragment color
out vec4 finalColor;

// Constants
const float PI = 3.14159;
const vec3 waterColorTop = vec3(0.064, 0.404, 0.670);
const vec3 waterColorBottom = vec3(0.454902, 0.8, 0.956863);
const vec3 waterColorNuke = vec3(0.254902, 1.0, 0.556863);
const float waveHeightMultiplier = 1.5;
const float waveSpeed = 0.6;
const float foamHeight = 0.03;

// Wave function
float getWaveHeight(vec2 uv) {
    /*
     * Returns the height of the wave for the given uv coordinate.
     */
    float currentCoefficient = uv.x * nbWaveCoefficients;
    float result = (sin(time * waveSpeed) + abs(sin(currentCoefficient * PI)) * waveCoefficients[int(currentCoefficient)]) * waveHeightMultiplier;
    return result;
}

void main() {
    vec4 currentColor = texture(texture0, fragTexCoord);

    // Gets the height of the wave at the current time
    float waveHeight = getWaveHeight(fragTexCoord);
    
    // If the pixel is not covered by the wave, discards it and moves on
    if (fragTexCoord.y > waveHeight) {
        finalColor = vec4(0., 0., 0., 0.);
        return;
    } 
    // If the pixel is at the top of the wave we add foam
    else if (fragTexCoord.y > waveHeight - foamHeight) {
        float distanceToWaveHeightNormalized = (waveHeight - fragTexCoord.y) * (1 / foamHeight);
        float lightVal = 1. - distanceToWaveHeightNormalized * 0.2;
        finalColor = vec4(lightVal, lightVal, lightVal, 1.);
        return;
    }
    // If the pixel is foamy (from perlin) we add foam
    // else if (perlin1_val > 0.9 || perlin2_val > 0.95) {
    //     finalColor = vec4(1., 1., 1., 1.);
    //     return;
    // }

    // If the pixel is covered by the wave, it will be a linear interpolation between the color
    // at the top of the water and the one at the bottom
    finalColor = vec4(mix(waterColorTop, waterColorBottom, fragTexCoord.y * (waveHeight / waveHeightMultiplier)), 1.);
}
