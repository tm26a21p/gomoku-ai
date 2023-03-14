#include "gomoku.hpp"

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

node_t *traverse_uct(node_t *node)
{
    node_t *best = nullptr;
    ushort size = node->children.size();
    if (size == 0) {
        best = node;
    } else {
        best = node->children[0];
    }
    double best_score = 0;
    double score = 0;
    static double c = sqrt(2);

    for (ushort i = 0; i < size; ++i) {
        if (node->children[i]->nb_visits == 0) {
            score = 1000000000;
            best = node->children[i];
            break;
        }
        else {
            score = (double)node->children[i]->nb_wins / (double)node->children[i]->nb_visits +
            c * sqrt(log(node->nb_visits) / (double)node->children[i]->nb_visits);
        }
        if (score > best_score) {
            best_score = score;
            best = node->children[i];
        }
    }

    if (best->children.size() == 0) {
        create_children(best, best->data, !best->player_one);
        return best;
    }
    return traverse_uct(best);
}

node_t *best_child(node_t *node)
{
    node_t *best_child = node->children[0];
    double best_score = 0;
    double score = 0;
    ushort size = node->children.size();

    for (ushort i = 0; i < size; ++i) {
        score = ((double)node->children[i]->nb_wins / (double)node->children[i]->nb_visits);
        if (score > best_score) {
            best_score = score;
            best_child = node->children[i];
        }
    }
    return best_child;
}

void futures_backpropagate(std::vector<std::future<bool>> &futures_sim, node_t *leaf, size_t &nb_simulations)
{
    for (size_t i = 0; i < futures_sim.size(); ++i) {
        if (futures_sim[i].valid()) {
            if (futures_sim[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                backpropagate_recursive(leaf, futures_sim[i].get());
                futures_sim.erase(futures_sim.begin() + i);
                ++nb_simulations;
            }
        }
    }
}



void free_node(node_t *node)
{
    if (node == NULL)
        return;
    for (ushort i = 0; i < node->children.size(); i++) {
        free_node(node->children[i]);
    }
}

pos_t monte_carlo_tree_search(data_t *data, pos_t last_move)
{
    node_t *root = create_node(data, NULL, last_move, false);
    std::chrono::time_point<std::chrono::system_clock> end = \
    std::chrono::system_clock::now() + std::chrono::seconds(4);
    std::vector<std::future<bool>> futures_sim;
    node_t *leaf = nullptr;
    size_t nb_simulations = 0;

    while (true) {
        leaf = traverse_uct(root);
        futures_sim.push_back(std::async(std::launch::async, simulate, leaf));
        futures_backpropagate(futures_sim, leaf, nb_simulations);
        if (std::chrono::system_clock::now() > end)
            break;
    }
    pos_t res = best_child(root)->pos;
    free_node(root);
    return res;
}
