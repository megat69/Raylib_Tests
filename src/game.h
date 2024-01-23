#pragma once
#include <memory>

#include <raylib.h>
#include <toml.hpp>

#include "constants.h"
#include "water_shader.h"
#include "options_menu.h"


class Game {
public:
    // Member variables
    std::unique_ptr<toml::value> m_config;  // Unique pointer to the config data
    RenderTexture2D m_renderTarget;         // Render texture of pre-defined size for every in-game element
    RenderTexture2D m_UiTarget;             // Render texture for the game's UI, same size as the window
    WaterShader m_waterShader;
    OptionsMenu m_optionsMenu;

    // Constructor
    Game();
    Game(std::unique_ptr<toml::value> _config);
    // Destructor
    ~Game();


    /**
     * \brief Initializes the program.
    */
    void init();


    /**
     * \brief De-initializes the program, freeing all memory from the game.
    */
    void deinit();

    /**
     * \brief Is responsible for most draw calls each frame.
    */
    void draw() const;

    /**
     * \brief Updates the whole game each frame.
    */
    void update();

    ////////// GAME FUNCTIONS //////////
    /**
     * \brief Gets called every time a change to the config is made.
    */
    void onConfigChange();

    
    /**
     * \brief Reloads the config from the config file.
    */
    void reloadConfig();

    /**
     * \brief Loads a new water shader.
    */
    void loadWaterShader();
};