#ifndef CPPU_CONSOLE
#define CPPU_CONSOLE



#include <iostream>
#include <stdio.h>
#include <cmath>
#include <windows.h>

#include "./string.hpp"
#include "./sleep.hpp"



namespace Console {
    namespace Color {
        enum ColorValue {
            black, red, green, yellow, blue, purple, aqua, white,
            light_black, light_red, light_green, light_yellow, light_blue, light_purple, light_aqua, bold_white
        };


        const std::string color_values_general[] = {
            "0", "1", "2", "3", "4", "5", "6", "7",
            "8", "9", "A", "B", "C", "D", "E", "F"
        };
        inline std::string get_color_general(ColorValue console_color) {
            return color_values_general[console_color];
        };


        class GenStyle {
            public:
                ColorValue text_color;
                ColorValue bg_color;

                GenStyle(ColorValue _text_color = ColorValue::white, ColorValue _bg_color = ColorValue::black) {
                    this->bg_color = _bg_color;
                    this->text_color = _text_color;
                };


                void set_console_color() {
                    std::string cmd = "color " + get_color_general(this->bg_color) + get_color_general(this->text_color);
                    system(cmd.c_str());
                };
        };


        const std::string color_val_spec_text[] = {
            "30", "31", "32", "33", "34", "35", "36", "37",
            "90", "91", "92", "93", "94", "95", "96", "97"
        };
        inline std::string get_color_spec_text(ColorValue console_color) {
            return color_val_spec_text[console_color];
        };

        const std::string color_val_spec_bg[] = {
            "40", "41", "42", "43", "44", "45", "46", "47",
            "100", "101", "102", "103", "104", "105", "106", "107"
        };
        inline std::string get_color_spec_bg(ColorValue console_color) {
            return color_val_spec_bg[console_color];
        };



        enum TextStyle {reset, bold, italic, underline};
        const std::string text_style_str[] = {"0", "1", "3", "4"};
        inline std::string get_text_style(TextStyle text_style) {
            return text_style_str[text_style];
        };


        std::string specstyle_header = "\u001b[";
        std::string specstyle_footer = "m";
        class SpecStyle {
            public:
                ColorValue text_color = ColorValue::bold_white;
                ColorValue bg_color = ColorValue::light_black;

                bool bold = false;
                bool italic = false;
                bool underline = false;
                bool reset = true;



                SpecStyle(
                    bool _reset = true,
                    ColorValue _text_color = ColorValue::bold_white,
                    ColorValue _bg_color = ColorValue::black,
                    bool _bold = false,
                    bool _italic = false,
                    bool _underline = false
                ) {
                    this->text_color = _text_color;
                    this->bg_color = _bg_color;
                    this->bold = _bold;
                    this->italic = _italic;
                    this->underline = _underline;
                    this->reset = _reset;
                };
                SpecStyle(
                    bool _reset,
                    int _text_color,
                    int _bg_color,
                    bool _bold = false,
                    bool _italic = false,
                    bool _underline = false
                ) {
                    this->text_color = static_cast<ColorValue>(_text_color);
                    this->bg_color = static_cast<ColorValue>(_bg_color);
                    this->bold = _bold;
                    this->italic = _italic;
                    this->underline = _underline;
                    this->reset = _reset;
                };
                static SpecStyle from_genstyle(GenStyle &gen_style) {
                    return SpecStyle(false, gen_style.text_color, gen_style.bg_color);
                };


                std::string get_str() {
                    if (this->reset) {
                        return specstyle_header + "0" + specstyle_footer;
                    };

                    std::vector<std::string> textstyles;

                    textstyles.push_back(get_color_spec_text(this->text_color));
                    textstyles.push_back(get_color_spec_bg(this->bg_color));

                    if (this->bold) {
                        textstyles.push_back(get_text_style(TextStyle::bold));
                    };
                    if (this->italic) {
                        textstyles.push_back(get_text_style(TextStyle::italic));
                    };
                    if (this->underline) {
                        textstyles.push_back(get_text_style(TextStyle::underline));
                    };
                    return specstyle_header + StrUtils::join_strs(textstyles, ";") + specstyle_footer;
                }
        };
    };



    CONSOLE_SCREEN_BUFFER_INFO get_csbi() {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            return csbi;
        };

    namespace Size {
        class ConsoleSize {
            public:
                int columns = 0;
                int rows = 0;

                ConsoleSize(int _columns = 0, int _rows = 0) {
                    this->columns = _columns;
                    this->rows = _rows;
                };
        };

        ConsoleSize get_size() {
            CONSOLE_SCREEN_BUFFER_INFO csbi = get_csbi();
            return ConsoleSize(
                csbi.srWindow.Right - csbi.srWindow.Left + 1,
                csbi.srWindow.Bottom - csbi.srWindow.Top + 1
            );
        };


        ConsoleSize default_size = get_size();


        std::string get_fill_hor(std::string filler, ConsoleSize size = default_size) {
            int filler_size = filler.size();
            std::string new_str = StrUtils::string_repeat(filler, size.columns / filler_size);
            std::string extra = filler.substr(0, size.columns % filler_size);
            return new_str + extra;
        };
    };

    namespace Cursor {
        COORD get_pos() {
            CONSOLE_SCREEN_BUFFER_INFO csbi = get_csbi();
            return csbi.dwCursorPosition;
        };


        void set_pos(short int x, short int y) {
            COORD pos = {x, y};
            HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleCursorPosition(out, pos);
        };
        void set_pos(COORD coords) {
            set_pos(coords.X, coords.Y);
        };


        void set_pos_rel(short int x, short int y) {
            COORD current_pos = get_pos();
            set_pos(current_pos.X + x, current_pos.Y + y);
        };
        void set_pos_rel(COORD coords) {
            set_pos_rel(coords.X, coords.Y);
        };


        class PosStore {
            public:
                COORD previous_pos;

                PosStore() {};
                PosStore(COORD _previous_pos) {
                    this->previous_pos = _previous_pos;
                };


                static PosStore from_current() {return PosStore(get_pos());};

                void restore() {set_pos(this->previous_pos);};
        };
    };



    namespace Anim {
        // Represents a console animation.
        class Animator {
            public:
                float duration_s = 1;

                Animator(float _duration_s) {
                    this->duration_s = _duration_s;
                };


                // Runs the animation.
                virtual void run() {};


                // Gets the seconds per frame given frames.
                float get_spf(int frames) {return (float)this->duration_s / (float)frames;};
        };



        class BarHighlight : public Animator {
            public:
                int row = 0;
                int width = 1;
                std::string border = "=";
                Color::SpecStyle style{};

                BarHighlight(
                    float duration,
                    int _row,
                    int _width = 1,
                    std::string _bar = "=",
                    Color::SpecStyle _style = Color::SpecStyle()
                ) : Animator(
                    duration
                ) {
                    this->row = _row;
                    this->width = _width;
                    this->border = _bar;
                    this->style = _style;
                };


                // Draws a line across the screen.
                void draw_line(unsigned int _row, std::string _border) {
                    Size::ConsoleSize size = Size::get_size();

                    std::string fill = Size::get_fill_hor(_border, size);
                    Cursor::PosStore pos_store = Cursor::PosStore::from_current();
                    Cursor::set_pos(0, _row);
                    std::cout << fill;
                    pos_store.restore();
                };

                // Draws two lines with a set amount of space away from a target row.
                void draw_surr_lines(unsigned int _row, unsigned int space, std::string _border) {
                    Size::ConsoleSize size = Size::get_size();

                    int top_row = _row - space;
                    if (!(top_row < 0)) this->draw_line(top_row, _border);

                    int bot_row = (_row + this->width) + space;
                    if (!(bot_row > size.rows)) this->draw_line(bot_row, _border);
                };


                // Draws surrounding blanks.
                void draw_surr_blanks(unsigned int _row, unsigned int space) {
                    this->draw_surr_lines(_row, space, " ");
                };

                // Draws surrounding borders.
                void draw_surr_border(unsigned int _row, unsigned int space) {
                    this->draw_surr_lines(_row, space, this->border);
                };

                // Draws both the blank and border.
                void draw_surr_bundle(unsigned int _row, unsigned int space) {
                    this->draw_surr_border(_row, space);
                    this->draw_surr_blanks(_row, space + 1);
                };


                void run() override {
                    Size::ConsoleSize size = Size::get_size();

                    int top_diff = this->row - 0;
                    int bot_diff = size.rows - (this->row + this->width);

                    int initial_space;
                    if (top_diff == bot_diff) initial_space = top_diff;
                    else if (top_diff > bot_diff) initial_space = top_diff;
                    else initial_space = bot_diff;

                    float spf = this->get_spf(initial_space - 1);

                    std::cout << this->style.get_str();

                    for (; initial_space > 0; initial_space--) {
                        this->draw_surr_bundle(this->row, initial_space);
                        sleep(spf);
                    };
                };
        };



        class WipeScreen : public Animator {
            public:
                Color::SpecStyle style{};
                std::string border = "###";
                bool is_fast = false;

                WipeScreen(
                    float _duration_s,
                    std::string _border = "###",
                    Color::SpecStyle _style = Color::SpecStyle(),
                    bool _is_fast = false
                ) : Animator(
                    _duration_s
                ) {
                    this->is_fast = _is_fast;
                    this->border = _border;
                    this->style = _style;
                };


                // Gets the width of the input line.
                int get_line_width(std::string line) {
                    Size::ConsoleSize size = Size::get_size();
                    float line_length = (float)(line.length());
                    float cons_height = (float)(size.rows);
                    return std::ceil(cons_height / line_length);
                };



                // Draws a part of the line.
                void print_line_part(COORD top_coords, std::string line) {
                    Size::ConsoleSize size = Size::get_size();
                    if (top_coords.X > size.columns) return;

                    Cursor::PosStore pos_store = Cursor::PosStore::from_current();
                    int line_length = line.length();
                    for (int i_line = 0; i_line < line_length; i_line++) {
                        Cursor::set_pos(top_coords.X, top_coords.Y + i_line);
                        std::cout << line[i_line];
                    };
                    pos_store.restore();
                };


                // Draws the entire line.
                void print_line(short int top_column, std::string line) {
                    Size::ConsoleSize size = Size::get_size();
                    int line_length = line.length();

                    short int row = 0;
                    COORD part_coord = {top_column, row};
                    while (true) {
                        this->print_line_part(part_coord, line);

                        part_coord.X--;
                        part_coord.Y += line_length;

                        if (part_coord.Y + line_length > size.rows) break;

                    };

                    int remaining = size.rows - part_coord.Y;
                    if (remaining == 0) return;

                    this->print_line_part(part_coord, line.substr(0, remaining));
                };



                // Draws a blank line.
                void print_blank(short int top_column) {
                    this->print_line(top_column, StrUtils::string_repeat(" ", this->border.length()));
                };

                // Draws a solid line.
                void print_border(short int top_column) {
                    this->print_line(top_column, this->border);
                };


                // Prints both the blank and solid line.
                void print_bundle(short int tl_column) {
                    if (!this->is_fast) {
                        this->print_border(tl_column);
                        this->print_blank(tl_column + 1);
                    } else {
                        this->print_blank(tl_column);
                    }
                }


                void run() override {
                    Size::ConsoleSize size = Size::get_size();
                    int line_width = this->get_line_width(this->border);

                    int start_col = size.columns + line_width + 1;
                    float spf = this->get_spf(start_col);

                    std::cout << this->style.get_str();

                    for (int current_col = start_col; current_col >= -1; current_col -= 1) {
                        this->print_bundle(current_col);
                        sleep(spf);
                    };
                };
        };



        class CornerPixelate : public Animator {
            public:
                Color::SpecStyle style{};

                CornerPixelate(float _duration_s, Color::SpecStyle _style = Color::SpecStyle()) : Animator(_duration_s) {
                    this->style = _style;
                };


                // Draws the diagonal line.
                void draw_line(short int top_column) {
                    Size::ConsoleSize size = Size::get_size();

                    Cursor::PosStore pos_store = Cursor::PosStore::from_current();

                    for (short int row = 0; (top_column < size.columns && row < size.rows); (top_column++, row++)) {
                        if (top_column < 0) continue;
                        Cursor::set_pos(top_column, row);
                        std::cout << ".";
                    };

                    pos_store.restore();
                };


                void run() override {
                    Size::ConsoleSize size = Size::get_size();
                    int column = size.columns - 1;

                    float spf = this->get_spf(size.columns);

                    std::cout << this->style.get_str();

                    for (; column > -(size.rows); column--) {
                        this->draw_line(column);
                        sleep(spf);
                    };
                }
        };
    
    
    
        class Typewriter : public Animator {
            public:
                std::string source;

                Typewriter(float _duration_s, std::string _source) : Animator(_duration_s) {
                    this->source = _source;
                };


                void run() override {
                    int source_length = this->source.length();
                    float spf = this->get_spf(source_length);
                    for (int idx = 0; idx < this->source.length(); idx++) {
                        std::cout << this->source[idx];
                        sleep(spf);
                    };
                };
        };
    };



    // Flushes the cout and cin streams for reuse regardless of errors.
    void flush_streams() {
        std::cout.flush();
        std::cin.sync();
        std::cin.clear();
    }

    // Clears the console. Taken from stackoverflow.com.
    void clear_console() {
        flush_streams();
        std::cout << Console::Color::SpecStyle().get_str();

        COORD tl = {0,0};
        CONSOLE_SCREEN_BUFFER_INFO s;
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacterW(console, ' ', cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
    };

    // Prompts the user to press enter to exit.
    void enter_to_exit(bool display_text = true) {
        Color::SpecStyle specstyle(false, Color::light_black, Color::black, true);
        if (display_text) std::cout << specstyle.get_str() << "Press enter to exit.";

        flush_streams();
        std::cin.get();

        std::cout << Color::SpecStyle().get_str();
    };


    // Attempts to maximize the console window (deprecated).
    void maximize() {
        SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, NULL);
    };
};



#endif