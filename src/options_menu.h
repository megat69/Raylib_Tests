#pragma once
#include <raylib.h>
#include <cmath>

#include "constants.h"


struct OptionsMenuRenderProperties {
    RenderTexture2D renderTarget;
    int positionY;
};


/**
 * \brief Handles the options menu.
*/
class OptionsMenu {
protected:
    bool _isOpen, animationInProgress;
    int overlayPosition;
    double animationProgress;
    RenderTexture2D renderTarget;

    // Static constants
    static float menuOpeningDuration;
    static Color overlayColor;

public:
    OptionsMenu();
    ~OptionsMenu();
    void init();
    void update();
    const OptionsMenuRenderProperties draw() const;
    void onConfigChange();

    /**
     * \brief Whether the menu currently is open.
     * \return Whether the options menu is currently open.
    */
    bool isOpen() const;

    /**
     * \brief Opens the options menu if not already open.
    */
    void open();

    /**
     * \brief Closes the options menu.
    */
    void close();

    /**
     * \brief Gives how many percent the animation of opening/closing the menu was completed through an animation curve.
     * \param animationProgress How much time out of the total animation time has already been spent (percentage).
     * \return The y position (in percentage) of the menu.
    */
    double menuToggleAnimationCurve(double animationProgress);
};