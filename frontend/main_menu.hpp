#ifndef FRONTEND_MAINMENU
#define FRONTEND_MAINMENU


#include <memory>
#include "../menu/_init.hpp"

#include "./assignman/_init.hpp"



namespace Frontend {
    class ChoiceAssignMan : public ConsMenu::Choice {
        public:
            ChoiceAssignMan() : ConsMenu::Choice("AssignMan", FAssignMan::MainMenu()) {}
    };
    // Represents the main menu.
    class MainMenu : public ConsMenu::SelectMenu {
        public:
            MainMenu() {
                this->is_main_menu = true;
                this->title = "Welcome to the main menu!";
                this->desc = "Wawa.";
                this->choices = {
                    std::make_unique<ChoiceAssignMan>(ChoiceAssignMan())
                };
            }
    };
}



#endif