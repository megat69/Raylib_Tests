// Standard library
#include <memory>

// External libs
#include <raylib.h>
#include <toml.hpp>

// Local files
#include "game.h"


int main() {
    // Loads the config file
    std::unique_ptr<toml::value> config = std::make_unique<toml::value>(toml::parse("config.toml"));

    // Initializes Raylib
    InitWindow(toml::find<int>(*config, "base_width"), toml::find<int>(*config, "base_height"), "Raylib tests !");
    if (!IsWindowReady()) {
        TraceLog(LOG_FATAL, "Couldn't initialize Raylib window !");
        return -1;
    }

    // Removes quitting from the Escape key
    SetExitKey(KEY_NULL);

    // Creates an instance of the game
    std::shared_ptr<Game> game = std::make_shared<Game>(std::move(config));

    // Initializes the program
    game->init();

    // Raylib main loop
    while (!WindowShouldClose()) {
        // Main update for the program
        game->update();

        // Individual draw calls
        game->draw();
    }

    // Closes the Raylib window and exits the program
    game->deinit();
    CloseWindow();
    return 0;
}