#include "gomoku.hpp"

void board(data_t *data)
{
    ushort size = data->goban_size;
    int x = 0;
    int y = 0; 
    if (data->goban) {
        delete[] data->goban;
    }
    data->goban = new char[size * size];
    for (ushort i = 0; i < size * size; ++i) {
        data->goban[i] = '0';
    }
    while (true) {
        std::string input = get_input();
        if (input == "DONE") {
            break;
        }
        auto line = split(input, ',');
        x = std::stoi(line[0]);
        y = std::stoi(line[1]);
        int who = std::stoi(line[2]);
        data->goban[y * size + x] = who + '0';
        ++data->turn;
    }
    play(data, {(ushort)x, (ushort)y});
}

void start(data_t *data)
{
    std::string arg = data->args;
    if (arg == "" || has_any_digits(arg) == false) {
        exit(84);
    }
    int size = std::stoi(arg);
    data->goban_size = size;
    if (size < MINIMUM_GOBAN_SIZE || size > MAXIMUM_GOBAN_SIZE) {
        print("ERROR size");
        exit(84);
    }
    data->goban = new char[size * size];
    for (int i = 0; i < size * size; ++i) {
        data->goban[i] = '0';
    }
    print("OK");
}

void end(UNUSED data_t *data)
{
    // we should free everything here (map, tree)
    if (data->goban) {
        delete[] data->goban;
    }
    exit(0);
}

void turn(data_t *data)
{
    std::string arg = data->args;
    std::vector<std::string> line = split(arg, ',');
    int x = std::stoi(line[0]);
    int y = std::stoi(line[1]);
    pos_t last_move = {(ushort)x, (ushort)y};
    data->goban[x * data->goban_size + y] = '2';
    ++data->turn;
    play(data, last_move);
}

void begin(data_t *data)
{
    play(data, {0, 0});
}