#include "encoder.h"
#include "funcs.h"
#include <thread>

int main()
{
    settings settings_;

    customize(settings_);

    if (settings_.on_a_timer_)
    {
        while(true)
        {
            search_and_encode_files(settings_);
            std::this_thread::sleep_for(settings_.timespan_);
        }
    }
    else
    {
        search_and_encode_files(settings_);
    }

    return 0;
}
