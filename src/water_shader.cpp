#include "water_shader.h"


WaterShader::WaterShader() {
    // Creates the perlin noise
    Image perlin1 = GenImagePerlinNoise(PERLIN_NOISE_SIZE, PERLIN_NOISE_SIZE, 0, 0, 1.f);
    Image perlin2 = GenImagePerlinNoise(PERLIN_NOISE_SIZE, PERLIN_NOISE_SIZE, 0, 0, 1.f);
    m_Perlin1 = LoadTextureFromImage(perlin1);
    m_Perlin2 = LoadTextureFromImage(perlin2);
    UnloadImage(perlin1);
    UnloadImage(perlin2);

    // Loads the water shader
    m_WaterShader = LoadShader(0, "src/shader/water_shader.fs");

    // Loads the texture for the water shader
    Image blankImage = GenImageColor(GetScreenWidth(), GetScreenHeight(), BLANK);
    m_WaterTexture = LoadTextureFromImage(blankImage);
    UnloadImage(blankImage);

    // Starts the time uniform for the shader
    m_time = 0.f;
    m_uniformTimeLocation = GetShaderLocation(m_WaterShader, "time");
    SetShaderValue(m_WaterShader, m_uniformTimeLocation, &m_time, SHADER_UNIFORM_FLOAT);

    // Starts the wave coefficients uniforms of the shader
    const int precision = 100;
    for (int i = 0; i < NB_WAVE_COEFFICIENTS; i++)
        m_waveCoefficients[i] = GetRandomValue(1 * precision, 3 * precision) / (float)(NB_WAVE_COEFFICIENTS * 10.f * precision);
    m_uniformWaveCoefficientsLocation = GetShaderLocation(m_WaterShader, "waveCoefficients");
    int uniformNbWaveCoefficientsLocation = GetShaderLocation(m_WaterShader, "nbWaveCoefficients");
    SetShaderValue(m_WaterShader, uniformNbWaveCoefficientsLocation, &NB_WAVE_COEFFICIENTS, SHADER_UNIFORM_INT);
    for (int i = 0; i < NB_WAVE_COEFFICIENTS; i++)
        SetShaderValue(m_WaterShader, m_uniformWaveCoefficientsLocation + i, &m_waveCoefficients[i], SHADER_UNIFORM_FLOAT);
}

WaterShader::~WaterShader() {
    UnloadTexture(m_Perlin1);
    UnloadTexture(m_Perlin2);
    UnloadShader(m_WaterShader);
    UnloadTexture(m_WaterTexture);
}

void WaterShader::update() {
    m_time = (float)GetTime();
    SetShaderValue(m_WaterShader, m_uniformTimeLocation, &m_time, SHADER_UNIFORM_FLOAT);
}

void WaterShader::draw() const {
    BeginShaderMode(m_WaterShader);
        DrawTexture(m_WaterTexture, 0, 0, WHITE);
    EndShaderMode();
}

const Texture2D& WaterShader::getWaterTexture() const {
    return m_WaterTexture;
}