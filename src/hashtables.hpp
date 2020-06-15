#ifndef HASHTABLES_HPP_
#define HASHTABLES_HPP_

#include <string>

#include "board.hpp"
#include "moves.hpp"

#define DEF_SIZE 128
#define DEF_N DEF_SIZE * 1024 * 1024 / sizeof(Entry)

struct Entry {
    uint64_t key;

    Move bestMove;

    int depth;
    int score;
    int flag;
    int nodes;

    Entry() {
        key = 0;
        depth = 0;
        score = 0;
        flag = 0;
        nodes = 0;

        bestMove = Move{};
    }

    Entry(uint64_t key, Move bestMove, int depth, int score, int flag) {
        this->key = key;
        this->bestMove = bestMove;
        this->depth = depth;
        this->score = score;
        this->flag = flag;
    }

    Entry(uint64_t key, int depth, uint64_t nodes) {
        this->key = key;
        this->depth = depth;
        this->nodes = nodes;
    }
};

class TT {
private:
    std::array<Entry, DEF_N> entries;
    int size = DEF_SIZE;
    int n = DEF_N;

public:
    TT();

    void clear();

    uint64_t perft(Board &board, const int depth);

    Entry get(const uint64_t key);
    void add(const uint64_t key, Move &bestMove, const int depth,
             const int score, const int flag);
};

extern TT tt;

#endif // #ifndef HASHTABLES_HPP_