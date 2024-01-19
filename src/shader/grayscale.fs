#version 330

// Input vertex attributes
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main() {
    vec4 currentColor = texture(texture0, fragTexCoord);
    // float luminance = (currentColor.r + currentColor.g + currentColor.b) / 3.0;
    // finalColor = vec4(luminance, luminance, luminance, currentColor.a); // Grayscale
    finalColor = vec4(sin(currentColor.r), sin(currentColor.g), sin(currentColor.b), currentColor.a);
}