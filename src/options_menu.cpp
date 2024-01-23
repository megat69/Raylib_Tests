#include "options_menu.h"

float OptionsMenu::menuOpeningDuration = .5;


OptionsMenu::OptionsMenu() : 
    _isOpen{ false },
    animationInProgress{ false }, 
    overlayPosition{ -GetScreenHeight() },
    animationProgress{ 0.0 }
{
    
}

void OptionsMenu::init() {}
void OptionsMenu::update() {
    if (IsKeyPressed(KEY_O)) open();
    if (IsKeyPressed(KEY_C)) close();

    // If the animation is in progress, adds the delta time to it
    if (animationInProgress) {
        animationProgress += GetFrameTime() * (isOpen() ? 1 : -1);
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
void OptionsMenu::draw() const {
    // Skips the drawing of the menu if it cannot be seen
    if (overlayPosition == -GetScreenHeight()) return;
    DrawRectangle(0, overlayPosition, GetScreenWidth(), GetScreenHeight(), (Color){
        240, 240, 240, 204
    });
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