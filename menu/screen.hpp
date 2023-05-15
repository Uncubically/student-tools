#ifndef MENU_SCREEN
#define MENU_SCREEN



#include "../cpp_utils/_init.hpp"


namespace ConsMenu {
    /*
    Represents a screen. This is something that is displayed to the console.
    */
    class Screen {

        // Shows the screen.
        virtual void show() {
            Console::clear_console();
        }
    };
}



#endif