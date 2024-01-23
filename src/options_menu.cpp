#include "options_menu.h"

float OptionsMenu::menuOpeningDuration = .5;
Color OptionsMenu::overlayColor = {255, 255, 255, 204};


OptionsMenu::OptionsMenu() : 
    _isOpen{ false },
    animationInProgress{ false }, 
    overlayPosition{ -GetScreenHeight() },
    animationProgress{ 0.0 }
{
    renderTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}
OptionsMenu::~OptionsMenu() {
    UnloadRenderTexture(renderTarget);
}

void OptionsMenu::init() {}
void OptionsMenu::update() {
    if (IsKeyPressed(KEY_O)) {
        if (isOpen())
            close();
        else
            open();
    }

    // If the animation is in progress, adds the delta time to it
    if (animationInProgress) {
        animationProgress += GetFrameTime() * (isOpen() ? 1 : -1.6);  // -n is negative to make the overlay raise and n to make it raise faster than it lowers
        // Clamps the animation progress
        if (animationProgress >= menuOpeningDuration) {
            animationInProgress = false;
            animationProgress = menuOpeningDuration;
        }
        else if (animationProgress <= 0.0) {
            animationInProgress = false;
            animationProgress = 0.0;
        }
    }

    double positionPercentage = menuToggleAnimationCurve(animationProgress / menuOpeningDuration);
    overlayPosition = -GetScreenHeight();

    // Calculates the position of the overlay
    overlayPosition += (int)(GetScreenHeight() * positionPercentage);
}

const OptionsMenuRenderProperties OptionsMenu::draw() const {
    BeginTextureMode(renderTarget);
        // Skips the drawing of the menu if it cannot be seen
        if (overlayPosition != -GetScreenHeight()) {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), overlayColor);
        }
    EndTextureMode();
    
    // Returns the render texture
    return (OptionsMenuRenderProperties){renderTarget, overlayPosition};
}


bool OptionsMenu::isOpen() const {
    return _isOpen;
}

void OptionsMenu::open() {
    if (!isOpen()) {
        animationInProgress = true;
        _isOpen = true;
    }
}
void OptionsMenu::close() {
    if (isOpen()) {
        animationInProgress = true;
        _isOpen = false;
    }
}


double OptionsMenu::menuToggleAnimationCurve(double animationProgress) {
    return sqrt(animationProgress);
}

void OptionsMenu::onConfigChange() {
    UnloadRenderTexture(renderTarget);
    renderTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
}