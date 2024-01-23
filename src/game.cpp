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

    // Loads the render targets, one with a pixel art size, and the UI one with the window size
    m_renderTarget = LoadRenderTexture(RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);
    m_UiTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    // Initializes the water shader
    m_waterShader.init();
}

void Game::deinit() {
    // Deinits the water shader
    m_waterShader.deinit();

    // Unloads the render targets
    UnloadRenderTexture(m_renderTarget);
    UnloadRenderTexture(m_UiTarget);
}

void Game::update() {
    /**
     * \brief Updates the whole game each frame.
    */
    if (IsKeyPressed(KEY_K)) {
        reloadConfig();
    }

    // Updates the options menu
    m_optionsMenu.update();

    // Updates the water shader if the game is not paused
    if (!m_optionsMenu.isOpen())
        m_waterShader.update();
}

void Game::draw() const {
    /**
     * \brief Is responsible for most draw calls each frame.
    */
    // Draws to the render target
    BeginTextureMode(m_renderTarget);
        ClearBackground(BLANK);

        // Draws the water shader
        m_waterShader.draw();
    EndTextureMode();

    // Draws to the UI render target
    BeginTextureMode(m_UiTarget);
        ClearBackground(BLANK);

        // Draws the UI menu
        m_optionsMenu.draw();
    EndTextureMode();

    // Draws the render target then the render UI
    BeginDrawing();
        Color drySand{(unsigned char)(0.964 * 255), (unsigned char)(0.843 * 255), (unsigned char)(0.69 * 255), 255};
        ClearBackground(drySand);

        // Draws the render target
        DrawTexturePro(
            m_renderTarget.texture, 
            (Rectangle) { 
                0, 0, 
                (float)m_renderTarget.texture.width, (float)-m_renderTarget.texture.height 
            }, 
            (Rectangle) { 
                0, 0, 
                (float)GetScreenWidth(), (float)GetScreenHeight()
            }, 
            (Vector2) { 0, 0 }, 0.f,
            WHITE
        );

        // Draws the UI render target
        DrawTextureRec(m_UiTarget.texture, (Rectangle) { 0, 0, (float)m_UiTarget.texture.width, (float)-m_UiTarget.texture.height }, (Vector2) { 0, 0 }, WHITE);

        // Displays the framerate counter if wanted
        if (toml::find<bool>(*m_config, "show_fps_counter")) {
            Color color = LIME;                             // Good FPS
            int fps = GetFPS();

            if ((fps < 30) && (fps >= 15)) color = ORANGE;  // Warning FPS
            else if (fps < 15) color = RED;                 // Low FPS

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
    
    // Changes to window resolution
    SetWindowSize(toml::find<int>(*m_config, "base_width"), toml::find<int>(*m_config, "base_height"));

    // Changes to screen mode
    char screenMode = toml::find<char>(*m_config, "screen_mode");  // 0 for WINDOWED, 1 for EXCLUSIVE FULLSCREEN, 2 for BORDERLESS WINDOWED
    switch (screenMode) {
        case 0:  // WINDOWED
            if (IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE))
                ToggleBorderlessWindowed();
            if (IsWindowFullscreen())
                ToggleFullscreen();
            break;
        case 1:  // FULLSCREEN EXCLUSIVE
            if (!IsWindowFullscreen())
                ToggleFullscreen();
            break;
        case 2:  // BORDERLESS WINDOWED
            if (!IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE))
                ToggleBorderlessWindowed();
            break;
    }
}

void Game::reloadConfig() {
    /**
     * \brief Reloads the config from the config file.
    */
    m_config = std::make_unique<toml::value>(toml::parse("config.toml"));
    onConfigChange();
}