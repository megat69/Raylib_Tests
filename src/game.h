#pragma once
#include <memory>

#include <raylib.h>
#include <toml.hpp>


class Game {
public:
    // Member variables
    std::unique_ptr<toml::value> m_config;
    Texture2D texture;
    Shader grayscaleShader;

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
};