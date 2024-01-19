#include "game.h"
#include <iostream>

Game::Game() {
    // Loads the TOML config
    reloadConfig();
}

Game::Game(std::unique_ptr<toml::value> _config) : m_config{ std::move(_config) } {}

Game::~Game() {}


void Game::init() {
    /**
     * \brief Initializes the program.
     * Will execute an 'onConfigChange' action to load the config options.
    */
    // Loads the config
    onConfigChange();

    // Loads the texture
    texture = LoadTexture("assets/mountain-view-1.png");
}

void Game::deinit() {
    UnloadTexture(texture);
}

void Game::update() {
    /**
     * \brief Updates the whole game each frame.
    */
    if (IsKeyPressed(KEY_K)) {
        reloadConfig();
    }

    // Updates the water shader
    m_waterShader.update();
}

void Game::draw() const {
    /**
     * \brief Is responsible for most draw calls each frame.
    */
    BeginDrawing();
        ClearBackground(BLACK);

        // Draws the water shader
        m_waterShader.draw();

        // Displays the framerate counter if wanted
        if (toml::find<bool>(*m_config, "show_fps_counter")) {
            Color color = LIME;                         // Good FPS
            int fps = GetFPS();

            if ((fps < 30) && (fps >= 15)) color = ORANGE;  // Warning FPS
            else if (fps < 15) color = RED;             // Low FPS

            const int fontSize = 20;
            const char* fpsText = TextFormat("%2i FPS", fps);
            const int textWidth = MeasureText(fpsText, fontSize);

            DrawRectangle(0, 0, textWidth + 13, fontSize + 9, BLACK);
            DrawText(fpsText, 6, 6, fontSize, color);
        }
    EndDrawing();
}

/////////////////////// GAME FUNCTIONS ///////////////////////
void Game::onConfigChange() {
    /**
     * \brief Gets called every time a change to the config is made.
     * Reloads the config information.
    */
    // Caps the framerate to the monitor refresh rate if wanted
    if (toml::find<bool>(*m_config, "sync_framerate_to_monitor_refresh_rate"))
        SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    // Sets the framerate target if wanted
    else if (toml::find<bool>(*m_config, "lock_framerate"))
        SetTargetFPS(toml::find<int>(*m_config, "target_framerate"));
    
    else
        SetTargetFPS(0);
}

void Game::reloadConfig() {
    /**
     * \brief Reloads the config from the config file.
    */
    m_config = std::make_unique<toml::value>(toml::parse("config.toml"));
    onConfigChange();
}