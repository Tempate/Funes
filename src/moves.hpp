#ifndef MOVES_HPP_
#define MOVES_HPP_

#include <string>

#include "main.hpp"
#include "bitboard.hpp"

enum { SINGLE, DOUBLE, NULL_MOVE };

struct Move {
    int from;
    int to;

    int type;
    int score;
    int captures;

    Move() : type(NULL_MOVE) {}

    Move(int to) : to(to), from(-1), type(SINGLE) {}

    Move(int from, int to, int type): 
    from(from), to(to), type(type) {}

    Move(std::string moveString);

    friend std::ostream& operator<<(std::ostream& os, const Move &move);
    void print() const;

    constexpr bool operator==(const Move &move) const {
        if (type == SINGLE)
            return to == move.to;
        
        return from == move.from && to == move.to;
    }

    constexpr bool operator!=(const Move &move) const {
        if (type == SINGLE)
            return to != move.to;
        
        return !(from == move.from && to == move.to);
    }

    private:
    int coordToSqr(std::string coord) const {
        return Bitboard{}.getSquare(coord[0] - 'a', coord[1] - '1');
    }
};

#endif // #ifndef MOVES_HPP_