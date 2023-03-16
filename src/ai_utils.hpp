# pragma once

#include "core.hpp"

node_t *create_node(data_t *data, node_t *parent, pos_t pos, bool is_player_one);
void create_children(node_t *node, data_t *data, bool is_player_one);

bool is_adjacent(data_t *data, ushort x, ushort y);
char *prepare_goban(node_t *node);
std::vector<pos_t> get_all_valid_moves(char *goban, ushort goban_size);
std::vector<pos_t> get_all_adjacent_moves(data_t *data);
void shuffle(std::vector<pos_t> &v);
bool is_game_over(char *goban, ushort goban_size, pos_t pos);
void free_node(node_t *node);
