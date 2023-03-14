# include "gomoku.hpp"

std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}

void print_node(node_t *node)
{
    std::cout << "DEBUG " << bold_on << "node: " << node->id << " - ";
    if (node->parent != NULL) {
        if (node->parent->id == 1) {
            std::cout << "parent: " << "root" << " - ";
        }
        else {
            std::cout << "parent: " << node->parent->id << " - ";
        }
    }
    std::cout << "(" << node->pos.x << ", " << node->pos.y << ")" << " - ";
    std::cout << "(" << node->nb_wins << " / " << node->nb_visits << ")" << " - ";
    if (node->player_one) {
        std::cout << " player one - ";
    } else {
        std::cout << " player two - ";
    }
    std::cout << "childrens: " << node->children.size();
    std::cout << bold_off << std::endl;
}

void print_children(node_t *node)
{
    // std::cout << "DEBUG " << bold_on << "childrens of node " << node->id << bold_off << std::endl;

    for (ushort i = 0; i < node->children.size(); ++i) {
        print_node(node->children[i]);
    }
    // exit(0);
}

void print_goban(char *goban, ushort goban_size)
{
    std::string str = "DEBUG GOBAN\n";
    str += "    ";
    for (ushort i = 0; i < goban_size; ++i) {
        if (i < 10) {
            str += " ";
        }
        str += std::to_string(i) + " ";
    }
    str += "\n\n";
    for (ushort i = 0; i < goban_size; ++i) {
        if (i < 10) {
            str += " ";
        }
        str += std::to_string(i) + " - ";
        for (ushort j = 0; j < goban_size; j++) {
            str += goban[i * goban_size + j];
            // str += "";
            str += "  ";
        }
        str += "\n";
    }
    #ifdef _WIN32
    #else
    dprintf(1, YELLOW "%s\n" RESET, str.c_str());
    #endif
}

void debug(std::string str)
{
    std::cout << "DEBUG " << bold_on << str << bold_off << std::endl;
}