#ifndef MENU_EXCS
#define MENU_EXCS



#include "../exc.hpp"


namespace ConsMenu {
    /*
    Contains the exceptions used.
    */
    namespace MenuExc {
        class BaseMenuExc : public Exc::Exception {
            public:
                BaseMenuExc(std::string message) : Exc::Exception(message) {};
        };


            //Raised whenever all the menus are exited.
            class MenuExit : public BaseMenuExc {
                public:
                    MenuExit() : BaseMenuExc("Exited all menus.") {};
            };

            class MenuChExit : public BaseMenuExc {
                public:
                    MenuChExit() : BaseMenuExc("Exited choice menu.") {};
            };


            class MenuChInvalidInput : public BaseMenuExc {
                public:
                    MenuChInvalidInput(std::string message) : BaseMenuExc(message) {};
            };

                class MenuChInputNotTag : public MenuChInvalidInput {
                    public:
                        MenuChInputNotTag() : MenuChInvalidInput("Input is not a valid tag.") {};
                };

                class MenuChInputInvalidTag : public MenuChInvalidInput {
                    public:
                        MenuChInputInvalidTag() : MenuChInvalidInput("Input is not a tag listed in the choices.") {};
                };
    };
};



#endif