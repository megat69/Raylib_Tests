#include "game.h"

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
}

void Game::draw() const {
    /**
     * \brief Is responsible for most draw calls each frame.
    */
    // Displays the framerate counter if wanted
    if (toml::find<bool>(*m_config, "show_fps_counter"))
        DrawFPS(3, 3);

    BeginDrawing();
        ClearBackground(BLACK);
        float heightScale = (float)GetScreenHeight() / (float)texture.height;
        float widthScale = (float)GetScreenWidth() / (float)texture.width;
        DrawTextureEx(texture, (Vector2){0, 0}, 0.f, std::max(heightScale, widthScale), WHITE);
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