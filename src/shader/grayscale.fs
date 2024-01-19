#version 330

// Input vertex attributes
in vec2 fragTexCoord;

// Input uniform values
uniform sampler2D texture0;

// Output fragment color
out vec4 finalColor;

void main() {
    vec4 currentColor = texture(texture0, fragTexCoord);
    float luminance = (currentColor.r + currentColor.g + currentColor.b) / 3.0;
    finalColor = vec4(luminance, currentColor.a);
}