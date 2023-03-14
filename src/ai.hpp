# pragma once

#include "core.hpp"
#include <thread>
#include <future>
#ifdef _WIN32
#include <windows.h>
#endif


pos_t monte_carlo_tree_search(data_t *data, pos_t last_move);
void play(data_t *data, pos_t pos);