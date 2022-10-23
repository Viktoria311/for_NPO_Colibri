#include "encoder.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void customize(settings & s)
{
    //а) указать  тип файлов -это маска
    std::cout << "Enter a type of files which you want to modify. For example: .txt" << std::endl;
    std::getline(std::cin, s.file_type_);

    // из какой директории искать файлы для шифрования/модификации
    std::cout << "From what directory do you want to modify files? For example: /home/dir1/dir2" << std::endl;
    std::getline(std::cin, s.modify_directory_);

    //b) удалять ли первоначальный
    std::cout << "Do you want to delete input files or not? for delete - 1, for leave - 0" << std::endl;
    std::cin >> s.deletion_of_input_files_;
    std::cin.get();

    //c) путь к выходным файлам. куда положить результицующие С:/...
    std::cout << "Enter a path of output files. For example: /home/dir1/dir2" << std::endl;
    std::getline(std::cin, s.output_directory_);

    //d) действия при повторении имени выходного файла:  если коллизия имен ( если второй раз запуск и одинаковые имена у выходных файлов)
            //либо i) перезапись ( Если существует зачем удалять и заново создавать. Просто обнулить и записать новую инфу )
                    // перезапись итогового файла ( то есть если уже есть модифицированный файл с таким именем. то есть если по второму кругу пошел цикл)
                    //Если файл открыть в режиме записи обычно идет пере запись данных
            //либо ii) модификация, если да, то, например, счетчик к имени
                    // Счетчик - если файл уже есть
                    //  Test.txt
                    // Со счетчиков при занятости имени новый файл берет название +1
                    //  Test1.txt

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
            std::cout << "Enter an integer: ";
            std::cin.clear();
            std::cin.ignore(100, '\n');
        }
        sec *= 60;
        s.timespan_ = static_cast<std::chrono::seconds>(sec);
    }

    // g) введите число десятичное в чар: закон маску- пароль -то на что мы делаем ксор. состоит из 8 байлов. ввести 1 чар. являющий
        // закон маска- пароль -то на что мы делаем ксор. состоит из 8 байлов. ввести 1 чар. являющий
        // введите 1 букву или цифру латиницы в чар.
    std::cout << "Enter char which will be a password:" << std::endl;
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
    std::cout << "current_path_ : " << s.modify_directory_  << std::endl;

    if (fs::is_directory(s.modify_directory_))
        std::cout << "директория!: " << std::endl;

    for(const auto & i : fs::recursive_directory_iterator(s.modify_directory_))
    {
        std::cout << "name of file or directory: " << i.path().string() << std::endl;

        //если тип тот и если файл не открыт, не трогать файл 3) Защита от «дурака»: если входной файл не закрыт - не трогать его.
        if (i.path().extension() == s.file_type_ && !is_open_by_user(i.path().string())) // метод extension() есть у path
        {
            std::string output_file_string = i.path().stem(); // "file"
            std::cout << "output_file_string   "  << output_file_string << std::endl;

            fs::path output_dir = s.output_directory_; // "/home/dir1/dir2/dir3/dir4"
            //fs::path output_file {output_file_string}; // "file"
            fs::path file {output_file_string + s.file_type_};
            fs::path output_path = output_dir / file;
            std::cout << "output_path: " << output_path.string() << std::endl;

            if (!s.overwriting_) // если выбрали счетчик имен
            {
                for (int j = 1; fs::exists(output_path); ++j)
                {
                    std::cout << "output_path   "  << output_path.string() << std::endl;
                    std::cout << "output_file_string   "  << output_file_string << std::endl;
                    std::cout << "file "  << i.path().filename() << std::endl;

                    modified_file_name(output_file_string, j);
                    std::cout << "ПОСЛЕ ИЗМЕМЕНИЯ:  J = "  << j<< "  " <<  output_file_string << std::endl;
                    fs::path new_output_file {output_file_string + s.file_type_};
                    output_path = output_dir / new_output_file;
                    std::cout << "output_path: " << output_path.string() << std::endl;
                    output_file_string = i.path().stem();
                }
            }

            if (fs::exists(output_path) && s.overwriting_) // если в итоговой папке уже есть файл с именем И выбрали перезапись
            {
                //обнулить содержание файла; // в классе encoder сделала флаг перезаписи
            }

            encoder Encoder(s.password_);

            //Encoder(путь_где_взять, имя_закодированного_файла)
            Encoder(i.path().string(), output_path.string()); // i.path().extension() - это тип файла
        }
    }
}

void set_all(settings & s)
{
    s.file_type_ = ".txt";
    s.modify_directory_ = "/home/victoria/inp";
    s.deletion_of_input_files_ = false;
    s.output_directory_ = "/home/victoria/outp";
    s.overwriting_ = false; // перезапись
    s.on_a_timer_ = false;
    s.timespan_ = static_cast<std::chrono::seconds>(30*60);
    s.password_ = 'z';
}
int main()
{

    settings settings_;


    //customize(settings_); // настройка
    set_all(settings_);

    //if (settings_.on_a_timer)
    //{
        //std::this_thread::sleep_for(settings_.timespan);

    //}
    //else
    //{
        search_and_encode_files(settings_);
    //}

    // модифицировать : есть чар - 8 байт. в цикле по 8 читать.  XOR  и результат в резуьтир файл


    ///////////////////////////////////
    // ПРИМЕР
    //fs::path root {"/"};
    //fs::path httpd { "etc/httpd/" };
    //fs::path sites {"sites-enabled"};
    //fs::path conf {"site.conf"};

    //fs::path path_to_site_conf = root / httpd / sites / conf;
    //std::cout << "path_to_site_conf: " << path_to_site_conf << std::endl;

/// //////////////////////////////

    return 0;
}
