#ifndef ENCODER_H
#define ENCODER_H

#include <fstream>
#include <string>
#include <filesystem>

struct settings
{
    std::string file_type; // a)
    std::string modify_directory; //
    bool deletion_of_input_files; // b)
    std::string output_directory; // c)
    bool overwriting; // d)
    bool on_a_timer; // f)
    std::chrono::seconds timespan; // e)
    char password; // g)
};

class encoder
{
    private:
        char key;
    public:
        encoder(char key_);
        void operator()(const std::string & input_file_, const std::string & output_file_);

};

#endif // ENCODER_H
