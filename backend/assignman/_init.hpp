#ifndef BACKEND_ASSIGNMENTS__INIT
#define BACKEND_ASSIGNMENTS__INIT


#include <ctime>
#include <string>
#include <vector>
#include <optional>



namespace AssignMan {
    // Represents an assignment.
    class Todo {
        public:
            std::string name;
            time_t time_created;
            time_t deadline;
            bool is_finished;
            std::optional<time_t> time_finished;

            Todo(
                std::string _name,
                time_t& _time_created,
                time_t& _deadline,
                bool _is_finished = false,
                std::optional<time_t> _time_finished = std::nullopt
            ) {
                this->name = _name;
                this->time_created = _time_created;
                this->deadline = _deadline;
                this->is_finished = _is_finished;
                this->time_finished = _time_finished;
            }
    };

    // Represents a subject.
    class Subject {
        public:
            std::string subject_name;
            std::string subject_abbr;
            std::string subject_code;
            std::optional<std::string> teacher_name;

            std::vector<Todo> todos;


            Subject(std::string _subject_name, std::string _subject_abbr, std::string _subject_code, std::optional<std::string> _teacher_name = std::nullopt) {
                this->subject_name = _subject_name;
                this->subject_abbr = _subject_abbr;
                this->subject_code = _subject_code;
                this->teacher_name = _teacher_name;
            }


            std::string get_display_str() {
                return 
                    "Subject name:\t" + this->subject_name + "\n" +
                    "Abbreviation:\t" + this->subject_abbr + "\n" +
                    "Subject code:\t" + this->subject_code + "\n" +
                    "Teacher name:\t" + this->teacher_name.value_or("Not set!");
            }
    };

    // Represents a period of time where the subjects are effective.
    class Period {
        public:
            int index;
            std::vector<Subject> subjects;

            time_t start_date;
            time_t end_date;

            Period(int _index, std::vector<Subject> _subjects) {
                this->index = _index;
                this->subjects = _subjects;
            }

            void set_start_date_now() {
                this->start_date = std::time(0);
            }

            void set_end_date_now() {
                this->end_date = std::time(0);
            }
    };

    std::optional<Period> current_period = std::nullopt;
}





#endif