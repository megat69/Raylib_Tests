#include "options_menu.h"


OptionsMenu::OptionsMenu() : _isOpen{ false } {

}


bool OptionsMenu::isOpen() const {
    return _isOpen;
}

void OptionsMenu::open() {
    if (!isOpen()) {
        // TODO: Open the menu

        _isOpen = true;
    }
}
void OptionsMenu::close() {
    if (isOpen()) {
        // TODO: Close the menu

        _isOpen = false;
    }
}