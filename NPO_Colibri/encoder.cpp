#include "encoder.h"
#include <fstream>

encoder::encoder(char key_) : key(key_) {}

void encoder::operator()(const std::string & input_file_, const std::string & output_file_)
{
    std::fstream fin, fout;

    fin.open(input_file_, std::fstream::in);
    fout.open(output_file_, std::fstream::out | std::ofstream::trunc); // with overwriting

    char c;

    while (fin >> std::noskipws >> c)
    {
        int temp = c ^ key;

        fout << (char)temp;
    }

    // Closing both files
    fin.close();
    fout.close();
}
