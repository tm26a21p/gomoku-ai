#pragma once

#include "core.hpp"

std::ostream& bold_on(std::ostream& os);
std::ostream& bold_off(std::ostream& os);
void print_node(node_t *node);
void print_children(node_t *node);
void print_goban(char *goban, ushort goban_size);
void debug(std::string str);