#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <chrono>
#include <cstring>
#include <random>
#include <thread>
#include <limits>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define UNUSED __attribute__((unused))

#define MINIMUM_GOBAN_SIZE 5
#define MAXIMUM_GOBAN_SIZE 30

#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

typedef unsigned short ushort;
typedef struct node_s node_t;

typedef struct pos_s {
    ushort x;
    ushort y;
} pos_t;

typedef struct data_s {
    std::string args;
    char *goban;
    ushort goban_size;
    ushort turn;
    pos_t my_last_move;
} data_t;

struct node_s {
    size_t id;
    node_t *parent;
    data_t *data;
    bool player_one;
    unsigned int nb_wins;
    unsigned int nb_visits;
    pos_t pos;
    std::vector<node_t *> children;
};

static const double infi = std::numeric_limits<double>::infinity();
