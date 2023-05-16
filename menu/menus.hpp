#ifndef MENU_MENUS
#define MENU_MENUS



#include <iostream>
#include <string>
#include <vector>

#include "../cpp_utils/_init.hpp"
#include "screen.hpp"


namespace ConsMenu {
    class Menu : public Screen {
        /*
        Represents a screen for user input to move to other screens.
        */
        public:
            bool is_main_menu = false;

            // The title of the menu.
            std::string title;
            Console::Color::SpecStyle title_specstyle = Console::Color::SpecStyle(false, Console::Color::white, Console::Color::black, true);

            // The description of the menu.
            std::string desc;
            Console::Color::SpecStyle desc_specstyle = Console::Color::SpecStyle();

            // The style of the border.
            std::string border = "+---==---";
            Console::Color::SpecStyle border_specstyle = Console::Color::SpecStyle(false, Console::Color::blue, Console::Color::black);

            // The base color used in the menu.
            Console::Color::GenStyle base_color;



            /*
            Draws the borders found above and below the title and description.
            */
            void show_border() {
                Console::Color::SpecStyle base = Console::Color::SpecStyle::from_genstyle(this->base_color);
                std::string new_border = Console::Size::get_fill_hor(this->border, Console::Size::get_size());
                std::cout << this->border_specstyle.get_str() << new_border << base.get_str() << std::endl;
            };


            /*
            Shows the header, which includes the title, description, and borders.
            */
            void show_header() {
                Console::Color::SpecStyle base = Console::Color::SpecStyle::from_genstyle(this->base_color);

                this->show_border();
                std::cout
                    << this->title_specstyle.get_str() << this->title << std::endl
                    << this->desc_specstyle.get_str() << this->desc << std::endl;
                this->show_border();
                std::cout << base.get_str() << std::endl;
            };

            /*
            Draws the rest of the menu. Should be overridden.
            */
            virtual void show() override {
                this->base_color.set_console_color();
                this->show_header();
            };
    };

};



#endif
