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

// Random function to generate a point, see https://thebookofshaders.com/12/
vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

// Wave function
float getWaveHeight(vec2 uv) {
    /*
     * Returns the height of the wave for the given uv coordinate.
     */
    float currentCoefficient = uv.x * nbWaveCoefficients;
    float result = (sin(time * waveSpeed) + abs(sin(currentCoefficient * PI)) * waveCoefficients[int(currentCoefficient)]) * waveHeightMultiplier;
    return result;
}

float getVoronoiValue(vec2 uv) {
    /*
     * Returns the value [0...1] of the voronoi computation at the given time.
     */
    vec2 st = uv / resolution;

    // Scale
    st *= 7.464;  // Fiddle with the value until something fits well

    // Tiles the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    // Minimum distance between cells
    float minimumDistance = 0.944;  // Fiddle with the value until something fits well

    // Main loop
    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x),float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);

			// Animate the point
            point = 0.5 + 0.5*sin(time + 3.667*point);

			// Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;

            // Distance to the point
            float dist = length(diff);

            // Keep the closer distance
            minimumDistance = min(minimumDistance, dist);
        }
    }

    return minimumDistance;
}

void main() {
    vec4 currentColor = texture(texture0, fragTexCoord);

    // v = getVoronoiValue(fragTexCoord);
    // finalColor = vec4(v, v, v, 1.);
    // return;

    // Gets the height of the wave at the current time
    float waveHeight = getWaveHeight(fragTexCoord);
    
    // If the pixel is not covered by the wave, discards it and moves on
    if (fragTexCoord.y > waveHeight) {
        finalColor = vec4(0., 0., 0., 0.);
        return;
    } 
    // If the pixel is at the top of the wave we add foam
    else if (fragTexCoord.y > waveHeight - 0.03) {
        finalColor = vec4(1., 1., 1., 1.);
        return;
    }
    // If the pixel is covered by Voronoi we add foam
    // else if (getVoronoiValue(fragTexCoord) > 0.9) {
    //     finalColor = vec4(1., 1., 1., 1.);
    //     return;
    // }

    // If the pixel is covered by the wave, it will be a linear interpolation between the color
    // at the top of the water and the one at the bottom
    finalColor = vec4(mix(waterColorTop, waterColorBottom, fragTexCoord.y * (waveHeight / waveHeightMultiplier)), 1.);
}
