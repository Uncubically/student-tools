#ifndef BACKEND_ASSIGNMENTS__INIT
#define BACKEND_ASSIGNMENTS__INIT


#include <ctime>
#include <string>
#include <optional>



namespace Assignment {
    class Assignment {
        public:
            std::string name;
            time_t time_created;
            time_t deadline;
            bool is_finished;
            std::optional<time_t> time_finished;

            Assignment(
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

    class Subject {
        public:
            std::string subject_name;
            std::string subject_abbr;
            std::string class_code;
            std::optional<std::string> teacher_name;


            Subject(std::string _subject_name, std::string _subject_abbr, std::string _class_code, std::optional<std::string> _teacher_name = std::nullopt) {
                this->subject_name = _subject_name;
                this->subject_abbr = _subject_abbr;
                this->class_code = _class_code;
                this->teacher_name = _teacher_name;
            }
    };
}



#endif