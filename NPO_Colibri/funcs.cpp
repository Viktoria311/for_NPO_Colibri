#include "encoder.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void customize(settings & s)
{
    //а) указать  тип файлов - это маска
    std::cout << "Enter a type of files which you want to modify. For example: .txt" << std::endl;
    std::getline(std::cin, s.file_type_);

    // из какой директории искать файлы для шифрования/модификации
    std::cout << "From what directory do you want to modify files? For example: /home/dir1/dir2" << std::endl;
    std::getline(std::cin, s.modify_directory_);

    //b) удалять ли первоначальный
    std::cout << "Do you want to delete input files or not? for delete - 1, for leave - 0" << std::endl;
    std::cin >> s.deletion_of_input_files_;
    std::cin.get();

    //c) путь к выходным файлам. куда положить результицующие
    std::cout << "Enter a path of output files. For example: /home/dir1/dir2" << std::endl;
    std::getline(std::cin, s.output_directory_);

    //d) действия при повторении имени выходного файла:  если коллизия имен ( если второй раз запуск и одинаковые имена у выходных файлов)
            // либо i) перезапись: обнулить и записать новую инфу в уже существующий файл
            // либо ii) модификация, если да, то, например, счетчик к имени

    std::cout << "Actions when the output file name repeats: for overwrite output files - 1,  for apply a counter to a name - 0" << std::endl;
    std::cin >> s.overwriting_;

    //f) работа по таймеру или разовый запуск. sleep()в цикле. периодичность опроса наличия входного файла (таймер)
    std::cout << "Run on a timer - 1, one-time start - 0" << std::endl;
    std::cin >> s.on_a_timer_;

    //e) указать период на сколько sleep. периодичность опроса наличия входного файла (таймер)
    if (s.on_a_timer_)
    {
        long int sec;

        std::cout << "Entes a quantity of min to sleep" << std::endl;

        while(!(std::cin >> sec))
        {
            std::cout << "Try again to enter an integer: ";
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }

        sec *= 60;
        s.timespan_ = static_cast<std::chrono::seconds>(sec);
    }
    else
    {
        s.timespan_ = static_cast<std::chrono::seconds>(0);
    }

    // g) введите пароль: 1 букву или цифру латиницы в чар
    std::cout << "Enter char which will be a password: " << std::endl;
    std::cin.get(s.password_);
    std::cin.get();
}

void modified_file_name(std::string & file_name_, int i)
{
    file_name_ += std::to_string(i);
}

bool is_open_by_user(const std::string & file_)
{
       return fs::exists(file_) && !std::fstream(file_);
}

void search_and_encode_files(const settings & s)
{
    for(const auto & i : fs::recursive_directory_iterator(s.modify_directory_))
    {
        //если тип тот и если файл открыт, не трогать файл 3) Защита от «дурака»: если входной файл не закрыт - не трогать его.
        if (i.path().extension() == s.file_type_ && !is_open_by_user(i.path().string()))
        {
            std::string output_file_string = i.path().stem();
            fs::path output_dir = s.output_directory_;
            fs::path file {output_file_string + s.file_type_};
            fs::path output_path = output_dir / file;

            if (!s.overwriting_) // если выбрали счетчик имен
            {
                for (int j = 1; fs::exists(output_path); ++j)
                {
                    modified_file_name(output_file_string, j);
                    fs::path new_output_file {output_file_string + s.file_type_};
                    output_path = output_dir / new_output_file;
                    output_file_string = i.path().stem();
                }
            }

            encoder Encoder(s.password_);

            Encoder(i.path().string(), output_path.string());
        }
    }
}

void set_all(settings & s)
{
    s.file_type_ = ".txt";
    s.modify_directory_ = "/home/victoria/inp";
    s.deletion_of_input_files_ = false;
    s.output_directory_ = "/home/victoria/outp";
    s.overwriting_ = false;
    s.on_a_timer_ = false;
    s.timespan_ = static_cast<std::chrono::seconds>(30*60);
    s.password_ = 'z';
}
