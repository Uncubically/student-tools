#ifndef FRONTEND_ASSIGNMAN_MAINMENU
#define FRONTEND_ASSIGNMAN_MAINMENU


#include <iostream>

#include "../../menu/_init.hpp"
#include "../../cpp_utils/_init.hpp"
#include "../../backend/_init.hpp"



namespace FAssignMan {
    class AddSubjectScreen : public ConsMenu::Screen {
        public:
            void show() override {
                std::string subject_name, subject_abbr, subject_code;
                std::optional<std::string> teacher_name;

                std::cout << "Create a subject: " << std::endl;
                subject_name = Console::prompt_user<std::string>("Enter name of subject: ");
                subject_abbr = Console::prompt_user<std::string>("Enter abbreviation of subject: ");
                subject_code = Console::prompt_user<std::string>("Enter subject code of subject: ");
                teacher_name = Console::prompt_user<std::string>("Enter name of teacher: ", true);

                std::cout << "\n\n";
            }
    };
    class AddSubjectChoice : public ConsMenu::Choice {
        public:
            AddSubjectChoice() : ConsMenu::Choice("Add Subject", AddSubjectScreen()) {}
    };


    class MainMenu : public ConsMenu::SelectMenu {
        public:
            MainMenu() {
                this->choices = {
                    std::make_unique<AddSubjectChoice>(AddSubjectChoice())
                };
            }
    }
}



#endif