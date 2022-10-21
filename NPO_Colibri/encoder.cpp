#include "encoder.h"

encoder::encoder(char key_) : key(key_) {}

void encoder::operator()(const std::string & input_file_, const std::string & output_file_)
{
    std::fstream fin, fout;

    fin.open(input_file_, std::fstream::in); // Open input file
    fout.open(output_file_, std::fstream::out | std::ofstream::trunc);

    char c;

    // ios::binary- reading file character by character
    while (fin >> std::noskipws >> c) // Reading original file till end of file
    {
        int temp = c ^ key;

        // Write temp as char in output file
        fout << (char)temp;
    }

    // Closing both files
    fin.close();
    fout.close();
}
