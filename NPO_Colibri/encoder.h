#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <chrono>

struct settings
{
    std::string file_type_; // a)
    std::string modify_directory_;
    bool deletion_of_input_files_; // b)
    std::string output_directory_; // c)
    bool overwriting_; // d)
    bool on_a_timer_; // f)
    std::chrono::seconds timespan_; // e)
    char password_; // g)
};

class encoder
{
    private:
        char key;
    public:
        encoder(char key_);
        void operator()(const std::string & input_file_, const std::string & output_file_);

};

#endif
