#include "gomoku.hpp"

data_t init_data()
{
    data_t data;
    data.args = "";
    data.goban_size = 0;
    data.turn = 0;
    data.goban = NULL;
    return data;
}

void gomoku(void) 
{
    data_t data = init_data();

    while (1) {
        std::string input = get_input();
        if (input.find(" ") != std::string::npos) {
            data.args = input.substr(input.find(" ") + 1);
            input = input.substr(0, input.find(" "));
        }
        for (int i = 0; cmd_map[i].func != NULL; ++i) {
            if (input == cmd_map[i].key) {
                cmd_map[i].func(&data);
            }
        }
    }
}

int main(UNUSED int ac, UNUSED char **av)
{
    gomoku();
    // monte_carlo_search_tree(NULL);
    return 0;
}

