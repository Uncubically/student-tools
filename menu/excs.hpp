#ifndef MENU_EXCS
#define MENU_EXCS


#include <exception>



namespace ConsMenu {
    /*
    Contains the exceptions used.
    */
    namespace MenuExc {
        class BaseMenuExc : public std::exception {
            public:
                const char* what() const noexcept override {return "Base menu exception.";}
        };


            //Raised whenever all the menus are exited.
            class MenuExit : public BaseMenuExc {
                public:
                    const char* what() const noexcept override {return "Exited from menu.";}
            };

            class MenuChExit : public BaseMenuExc {
                public:
                    const char* what() const noexcept override {return "Exited from choice menu.";}
            };


            class MenuChInvalidInput : public BaseMenuExc {
                public:
                    const char* what() const noexcept override {return "Invalid choice input.";}
            };

                class MenuChInputNotTag : public MenuChInvalidInput {
                    public:
                        const char* what() const noexcept override {return "Input is not a valid tag.";}
                };

                class MenuChInputInvalidTag : public MenuChInvalidInput {
                    public:
                        const char* what() const noexcept override {return "Input is not a tag listed in the choices.";}
                };
    };
};



#endif