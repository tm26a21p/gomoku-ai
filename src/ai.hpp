# pragma once

#include "core.hpp"
#include <thread>
#include <future>
#ifdef _WIN32
#include <windows.h>
#endif

static const ushort MAX_THREAD = std::thread::hardware_concurrency();
static const std::chrono::seconds ZERO(0);
static const std::chrono::seconds FIVE(5);
static const std::future_status DONE = std::future_status::ready;
static const double sqrt2 = sqrt(2);

pos_t monte_carlo_tree_search(data_t *data, pos_t last_move);
void play(data_t *data, pos_t pos);
