#include "gomoku.hpp"

bool is_adjacent(data_t *data, ushort x, ushort y)
{
    if (x > 0 && data->goban[(x - 1) * data->goban_size + y] != '0') {
        return true;
    }
    if (x > 0 && x < data->goban_size - 1 && data->goban[(x + 1) * data->goban_size + y] != '0') {
        return true;
    }
    if (y > 0 && data->goban[x * data->goban_size + (y - 1)] != '0') {
        return true;
    }
    if (y > 0 && y < data->goban_size - 1 && data->goban[x * data->goban_size + (y + 1)] != '0') {
        return true;
    }
    if (x > 0 && y > 0 && data->goban[(x - 1) * data->goban_size + (y - 1)] != '0') {
        return true;
    }
    if (x > 0 && y > 0 && x < data->goban_size - 1 && y < data->goban_size - 1 && data->goban[(x + 1) * data->goban_size + (y + 1)] != '0') {
        return true;
    }
    if (x > 0 && y > 0 && y < data->goban_size - 1 && data->goban[(x - 1) * data->goban_size + (y + 1)] != '0') {
        return true;
    }
    if (x > 0 && y > 0 && x < data->goban_size - 1 && data->goban[(x + 1) * data->goban_size + (y - 1)] != '0') {
        return true;
    }
    return false;
}

bool is_move_already_played(pos_t move, std::vector<pos_t> moves)
{
    for (ushort i = 0; i < moves.size(); ++i) {
        if (moves[i].x == move.x && moves[i].y == move.y) {
            return true;
        }
    }
    return false;
}

std::vector<pos_t> get_all_adjacent_moves(data_t *data)
{
    std::vector<pos_t> moves;
    pos_t p = {0, 0};

    for (ushort i = 0; i < data->goban_size; ++i) {
        for (ushort j = 0; j < data->goban_size; j++) {
            if (data->goban[i * data->goban_size + j] == '0' && is_adjacent(data, i, j)) {
                p.x = i;
                p.y = j;
                moves.push_back(p);
            }
        }
    }
    return moves;
}

std::vector<pos_t> get_all_valid_moves(char *goban, ushort goban_size)
{
    std::vector<pos_t> moves;

    pos_t p = {0, 0};
    for (ushort i = 0; i < goban_size; ++i) {
        for (ushort j = 0; j < goban_size; j++) {
            if (goban[i * goban_size + j] == '0') {
                p = {i, j};
                moves.push_back(p);
            }
        }
    }
    return moves;
}

void shuffle(std::vector<pos_t> &v)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
}

static bool check_horizontal(char *goban, ushort goban_size, pos_t pos)
{
    short count = 0;
    char player = goban[pos.x * goban_size + pos.y];

    for (ushort i = 0; i < goban_size; ++i) {
        if (goban[pos.x * goban_size + i] == player) {
            count++;
        } else {
            count = 0;
        }
        if (count >= 5) {
            return true;
        }
    }
    return false;
}

static bool check_vertical(char *goban, ushort goban_size, pos_t pos)
{
    short count = 0;
    char player = goban[pos.x * goban_size + pos.y];

    for (ushort i = 0; i < goban_size; ++i) {
        if (goban[i * goban_size + pos.y] == player) {
            count++;
        } else {
            count = 0;
        }
        if (count >= 5) {
            return true;
        }
    }
    return false;
}

static bool check_diagonal(char *goban, ushort goban_size, pos_t pos)
{
    short count = 0;
    pos_t tmp = pos;
    char player = goban[pos.x * goban_size + pos.y];

    // go up left until we reach the edge
    while (tmp.x > 0 && tmp.y > 0) {
        tmp.x--;
        tmp.y--;
    }
    // check the diagonal
    while (tmp.x < goban_size && tmp.y < goban_size) {
        if (goban[tmp.x * goban_size + tmp.y] == player) {
            count++;
        } else {
            count = 0;
        }
        if (count >= 5) {
            return true;
        }
        tmp.x++;
        tmp.y++;
    }
    count = 0;
    tmp = pos;
    // go up down until we reach the edge
    while (tmp.x > 0 && tmp.y <= goban_size) {
        tmp.x--;
        tmp.y++;
    }
    // check the diagonal
    while (tmp.x < goban_size && tmp.y > 0) {
        if (goban[tmp.x * goban_size + tmp.y] == player) {
            count++;
        } else {
            count = 0;
        }
        if (count >= 5) {
            return true;
        }
        tmp.x++;
        tmp.y--;
    }
    return false;
}

bool is_game_over(char *goban, ushort goban_size, pos_t pos)
{    
    if (check_horizontal(goban, goban_size, pos)) {
        return true;
    }
    if (check_vertical(goban, goban_size, pos)) {
        return true;
    }
    if (check_diagonal(goban, goban_size, pos)) {
        return true;
    }
    return false;
}

char *prepare_goban(node_t *node)
{
    data_t *data = node->data;
    char *goban = new char[data->goban_size * data->goban_size];
    node_t *tmp = node;

    memcpy(goban, data->goban, data->goban_size * data->goban_size);
    while (tmp->parent != NULL) {
        if (tmp->player_one == true) {
            goban[tmp->pos.x * data->goban_size + tmp->pos.y] = '1';
        } else {
            goban[tmp->pos.x * data->goban_size + tmp->pos.y] = '2';
        }
        tmp = tmp->parent;
    }
    return goban;
}

node_t *create_node(data_t *data, node_t *parent, pos_t pos, bool is_player_one)
{
    static ushort id = 0;
    node_t *node = new node_t;

    node->id = ++id;
    node->data = data;
    node->parent = parent;
    node->player_one = is_player_one;
    node->nb_visits = 0;
    node->nb_wins = 0;
    node->pos = pos;
    return node;
}

void create_children(node_t *node, data_t *data, bool is_player_one)
{
    std::vector<pos_t> moves = get_all_adjacent_moves(data);
    std::vector<pos_t> already_played;
    node_t *tmp = node;

    while (tmp->parent != NULL) {
        already_played.push_back(tmp->pos);
        tmp = tmp->parent;
    }
    for (ushort i = 0; i < moves.size(); ++i) {
        if (is_move_already_played(moves[i], already_played)) {
            continue;
        }
        node_t *child = create_node(data, node, moves[i], is_player_one);
        node->children.push_back(child);
    }
}
