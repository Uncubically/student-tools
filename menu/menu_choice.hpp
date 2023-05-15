#ifndef MENU_CHOICE
#define MENU_CHOICE



#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <memory>


#include "../cpp_utils/_init.hpp"
#include "../exc.hpp"
#include "menus.hpp"
#include "excs.hpp"



namespace ConsMenu {
    /*
    Represents a choice presented in a ChoiceMenu.
    */
    class Choice {
        public:
            std::string description = "This is the default description.";
            std::unique_ptr<Menu> menu = std::make_unique<Menu>(Menu());

            Choice() {};
            Choice(std::string _description, Menu &_menu) {
                this->description = _description;
                this->menu = std::make_unique<Menu>(_menu);
            }


            void show_menu() {
                this->menu->show();
            }
    };



    /*
    Contains the information needed to get the output of the presented choice.
    */
    class SelectResult {
        public:
            // The row number of the choice in the screen.
            short int row = 1;

            // The width of the choice.
            int width = 1;

            // The index of the chosen choice.
            int result = 1;

            SelectResult(short int _row = 1, int _width = 1, int _result = 1) {
                this->row = _row;
                this->width = _width;
                this->result = _result;
            };
    };



    class SelectMenu : public Menu {
        public:
            // Represents the choice used to go back to the previous menu, namely, "x".
            static std::string back_tag;

            // Represents the choice used to exit the program, namely, "xx".
            static std::string exit_tag;


            // The opening tag for choices, namely, "[".
            static std::string tag_open_br;

            // The closing tag, namely, "]".
            static std::string tag_close_br;


            // If true, this would hide the exit tag.
            bool is_main_menu = false;


            // The text to display before the choices.
            std::string choice_text = "Please pick an appropriate choice:";

            // The choices itself.
            std::vector<std::unique_ptr<Choice>> choices;

            // The color used when typing the choice.
            Console::Color::SpecStyle typing_color = Console::Color::SpecStyle(false, Console::Color::black, Console::Color::bold_white, true);



            SelectMenu() : Menu() {};



            /*
            Adds the opening and closing brackets to a tag.
            Ex. "x" -> "[x]"
            */
            std::string add_style_to_tag(std::string &tag) {
                return this->tag_open_br + tag + this->tag_close_br;
            };


            /*
            Gets the string to display the default tags, namely the "back" and "exit" tags.
            */
            std::string get_default_choices_display() {
                std::string back_tag_display = !this->is_main_menu ?
                    (add_style_to_tag(this->back_tag) + " Go back to previous menu.\n")
                    : "";

                return back_tag_display + (add_style_to_tag(this->exit_tag) + " Exit the program.");
            };


            /*
            Gets the string to display the choices.
            */
            std::vector<std::string> get_choices_display() {
                std::vector<std::string> display_strs;
                for (size_t idx = 0; idx < this->choices.size(); idx++) {
                    std::string choice = this->choices[idx]->description;
                    std::string idx_str = std::to_string(idx + 1);
                    display_strs.push_back(add_style_to_tag(idx_str) + " " + choice);
                };
                return display_strs;
            };


            /*
            Displays the choices and asks the user for the response.
            Also does an animation after responses.
            */
            SelectResult ask_response(bool show_invalid = false) {
                Console::clear_console();
                Menu::show_header();


                if (show_invalid) {
                    Console::Color::SpecStyle error_specstyle = Console::Color::SpecStyle(false, Console::Color::black, Console::Color::light_red, true);
                    std::cout << error_specstyle.get_str() << "Invalid input, please try again." << Console::Color::SpecStyle().get_str()
                        << std::endl << std::endl;
                }


                std::string base_color_str = Console::Color::SpecStyle().get_str();
                base_color_str += Console::Color::SpecStyle::from_genstyle(this->base_color).get_str();

                std::cout << std::endl
                    << this->choice_text << std::endl
                    << std::endl
                    << std::endl
                    << Console::Color::SpecStyle(false, Console::Color::red, Console::Color::black, false, true).get_str()
                        << this->get_default_choices_display() << std::endl
                    << base_color_str;


                std::vector<short int> option_poss;
                std::vector<std::string> choices_display = this->get_choices_display();
                for (std::string choice_display : choices_display) {
                    option_poss.push_back(Console::Cursor::get_pos().Y);
                    std::cout << choice_display << std::endl;
                };

                option_poss.push_back(Console::Cursor::get_pos().Y);

                std::cout
                    << std::endl
                    << std::endl
                    << Console::Color::SpecStyle(false, Console::Color::light_green, Console::Color::black, true).get_str()
                        << "[]> " << this->typing_color.get_str();


                std::string response;
                Console::flush_streams();
                std::cin >> response;

                if ((response == this->back_tag) && (!this->is_main_menu)) {
                    throw MenuExc::MenuChExit();
                } else if (response == this->exit_tag) {
                    throw MenuExc::MenuExit();
                };


                std::cout << Console::Color::SpecStyle().get_str();

                if (!StrUtils::is_number(response)) {
                    throw MenuExc::MenuChInputNotTag();
                };

                int choice_idx = std::stoi(response) - 1;
                if (((choice_idx + 1) > choices.size()) || (choice_idx < 0)) {
                    throw MenuExc::MenuChInputInvalidTag();
                };

                int row = option_poss[choice_idx];
                int next_row = option_poss[choice_idx + 1];
                return SelectResult(row, next_row - row, choice_idx);
            };


            // Asks the user, but with a loop. This catches invalid inputs then asks again.
            SelectResult ask_response_loop() {
                Console::clear_console();
                bool invalid = false;

                while (true) {
                    try {
                        return ask_response(invalid);
                    } catch (MenuExc::MenuChInvalidInput) {
                        Console::clear_console();
                        invalid = true;
                    };
                };
            };



            void show() {
                while (true) {
                    Console::clear_console();
                    SelectResult response;
                    try {
                        response = this->ask_response_loop();
                        Console::flush_streams();
                    } catch (MenuExc::MenuChExit) {
                        Console::flush_streams();

                        Console::Color::SpecStyle pixel_specstyle = Console::Color::SpecStyle(
                            false,
                            Console::Color::red,
                            Console::Color::black,
                            true
                        );

                        Console::Anim::CornerPixelate(0.1, pixel_specstyle).run();
                        sleep(1);
                        break;
                    } catch (MenuExc::MenuExit exc) {
                        Console::flush_streams();

                        Console::Color::SpecStyle pixel_specstyle = Console::Color::SpecStyle(
                            false,
                            Console::Color::red,
                            Console::Color::red,
                            true
                        );

                        Console::Anim::CornerPixelate(0.5, pixel_specstyle).run();
                        sleep(1);
                        throw exc;
                    };


                    Console::Color::SpecStyle bar_specstyle = Console::Color::SpecStyle(
                        false,
                        Console::Color::green,
                        Console::Color::black,
                        true
                    );
                    Console::Anim::BarHighlight(0.5, response.row, response.width - 1, "=", bar_specstyle).run();

                    sleep(1);

                    Console::Color::SpecStyle wipe_specstyle = Console::Color::SpecStyle(
                        false,
                        Console::Color::light_green,
                        Console::Color::blue,
                        true
                    );
                    Console::Anim::WipeScreen(0.02, "███", wipe_specstyle, true).run();


                    sleep(1);

                    Console::clear_console();
                    this->choices[response.result]->menu->show();
                };
            };
    };
    std::string SelectMenu::back_tag = "x";
    std::string SelectMenu::exit_tag = "xx";
    std::string SelectMenu::tag_open_br = "[";
    std::string SelectMenu::tag_close_br = "]:";
};



#endif