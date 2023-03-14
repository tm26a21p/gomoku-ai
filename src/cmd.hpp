#pragma once

#include "core.hpp"

typedef struct map_s {
    std::string key;
    void (*func)(data_t *data);
} map_t;

void start(data_t *data);
void end(data_t *data);
void turn(data_t *data);
void begin(data_t *data);
void board(data_t *data);

static const map_t cmd_map[] = {
    {"START", start},
    {"END", end},
    {"BEGIN", begin},
    {"TURN", turn},
    {"BOARD", board},
    {"INFO", NULL},
    {"", NULL}
};