#include "core.hpp"
#include "debug.hpp"
#include "gomoku.hpp"
#include <future>
#include <string>

void play(data_t *data, pos_t opponent_last_move)
{
    pos_t move = {0, 0};

    if (data->turn == 0) {
        ushort x = (ushort) (data->goban_size / 2) - (ushort)1;
        ushort y = (ushort) (data->goban_size / 2) - (ushort)1;
        move = {x, y};
    } else {
        move = monte_carlo_tree_search(data, opponent_last_move);
    }
    data->my_last_move = move;
    data->goban[move.x * data->goban_size + move.y] = '1';
    ++data->turn;
    std::cout << move.x << "," << move.y << std::endl;
}

bool simulate(node_t *node)
{
    char *goban = prepare_goban(node);
    ushort goban_size = node->data->goban_size;
    std::vector<pos_t> moves = get_all_valid_moves(goban, goban_size);
    bool player_one = !node->player_one;

    shuffle(moves);
    for (ushort i = 0; i < moves.size(); ++i) {
        if (player_one == true) {
            goban[moves[i].x * goban_size + moves[i].y] = '1';
        } else {
            goban[moves[i].x * goban_size + moves[i].y] = '2';
        }
        if (is_game_over(goban, goban_size, moves[i]) == true) {
            delete[] goban;
            if (player_one == true) {
                return true;
            }
            return false;
        }
        player_one = !player_one;
    }
    delete[] goban;
    return false;
}

void backpropagate_recursive(node_t *node, bool win)
{
    ++node->nb_visits;
    if (win) {
        ++node->nb_wins;
    }
    if (node->parent != NULL) {
        backpropagate_recursive(node->parent, win);
    }
}

node_t *traverse_uct(node_t *node) {
    for (node_t *child : node->children) {
        if (child->nb_visits == 0) {
            return child;
        }
    }
    double best_score = -1;
    node_t *best_node = nullptr;

    for (node_t *child : node->children) {
        double score = (double)child->nb_wins / (double)child->nb_visits + sqrt(2 * log(node->nb_visits) / (double)child->nb_visits);
        if (score > best_score) {
            best_score = score;
            best_node = child;
        }
    }

    if (best_node->children.empty()) {
        create_children(best_node, best_node->data, !best_node->player_one);
        return best_node->children[0];
    }

    return traverse_uct(best_node);
}

node_t *best_child(node_t *node)
{
    node_t *best_child = node->children[0];
    double best_score = 0;
    double score = 0;
    ushort size = node->children.size();

    for (ushort i = 0; i < size; ++i) {
        score = (double)node->children[i]->nb_visits;
        if (score > best_score) {
            best_score = score;
            best_child = node->children[i];
        }
    }
    return best_child;
}

void handle_futures(std::future<bool> *futures, node_t *leaf, size_t &nb_simulations)
{
    for (ushort i = 0; i < MAX_THREAD; ++i) {
        if (futures[i].valid()) {
            if (futures[i].wait_for(ZERO) == DONE) {
                backpropagate_recursive(leaf, futures[i].get());
                futures[i] = std::future<bool>();
                ++nb_simulations;
            }
        }
    }
}

pos_t monte_carlo_tree_search(data_t *data, pos_t last_move)
{
    node_t *root = create_node(data, NULL, last_move, false);
    create_children(root, data, true);
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now() + FIVE;
    std::future<bool> futures[MAX_THREAD];

    size_t nb_simulations = 0;
    node_t *leaf;
    print_goban(data->goban, data->goban_size);
    while (true) {
        leaf = traverse_uct(root);
        for (ushort i = 0; i < MAX_THREAD; ++i) {
            if (!futures[i].valid())
                futures[i] = std::async(std::launch::async, simulate, leaf);
        }
        handle_futures(futures, leaf, nb_simulations);
        if (std::chrono::system_clock::now() > end)
            break;
    }
    pos_t playin = best_child(root)->pos;
    free_node(root);
    // debug("sim: " + std::to_string(nb_simulations) + " turn " + std::to_string(data->turn) + " nmoves " + std::to_string(root->children.size()));
    // print_children(root);
    // for (node_t *child : root->children) {
    //     print_children(child);
    // }
    return playin;
}
