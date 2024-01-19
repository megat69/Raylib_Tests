#pragma once
#include <raylib.h>

const int PERLIN_NOISE_SIZE = 512;
const int NB_WAVE_COEFFICIENTS = 3;

class WaterShader {
protected:
    Texture2D m_WaterTexture;
    Texture2D m_Perlin1;
    Texture2D m_Perlin2;
    Shader m_WaterShader;
    float m_time;
    int m_uniformTimeLocation;
    float m_waveCoefficients[NB_WAVE_COEFFICIENTS];
    int m_uniformWaveCoefficientsLocation;

public:
    /**
     * \brief Creates the water texture and perlin.
    */
    WaterShader();
    ~WaterShader();

    void update();
    void draw() const;

    const Texture2D& getWaterTexture() const;
};