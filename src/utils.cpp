# include "gomoku.hpp"


std::string get_input(void)
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void print(std::string str) 
{
    std::cout << str << std::endl;
}

void cross_platform_sleep(int ms)
{
    #ifdef _WIN32
    Sleep(ms);
    #else
    usleep(ms * 1000);
    #endif
}

std::vector<std::string> split(std::string str, char delim)
{
    std::vector<std::string> res;
    std::string tmp = "";
    for (ushort i = 0; i < str.size(); ++i) {
        if (str[i] == delim) {
            res.push_back(tmp);
            tmp = "";
        } else {
            tmp += str[i];
        }
    }
    res.push_back(tmp);
    return res;
}

bool has_any_digits(const std::string& s)
{
    return std::any_of(s.begin(), s.end(), ::isdigit);
}

