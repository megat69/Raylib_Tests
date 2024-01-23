#pragma once
#include <raylib.h>


/**
 * \brief Handles the options menu.
*/
class OptionsMenu {
protected:
    bool _isOpen;

public:
    OptionsMenu();

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
};