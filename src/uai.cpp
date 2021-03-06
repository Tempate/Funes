#include <sstream>
#include <string>

#include "main.hpp"
#include "board.hpp"
#include "hashtables.hpp"
#include "alphabeta/search.hpp"

#include "uai.hpp"

void isready(void);
void position(Board &board, const std::string &s);
void go(Board &board, const std::string &s);
void perft(const Board &board, const int depth);
Move bestmove(const Board &board, Settings &settings);

pthread_t worker;
int working = 0;

void uai() {
    Board board;

    std::string cmd;
    std::string msg;

    while (true) {
        getline(std::cin, msg);

        if (msg.compare("uai") == 0) {
            std::cout << "id name " << ENGINE_NAME << std::endl;
            std::cout << "id author " << ENGINE_AUTHOR << std::endl;
            std::cout << "uaiok" << std::endl;
            break;
        }
    }

    while (true) {
        getline(std::cin, msg);
        std::stringstream ss(msg);
        ss >> cmd;

        if (cmd.compare("isready") == 0)
            isready();

        else if (cmd.compare("uainewgame") == 0) {
            board.startpos();
            tt.clear();
        }
        
        else if (cmd.compare("position") == 0)
            position(board, msg.substr(9));
        else if (cmd.compare("go") == 0)
            go(board, msg.substr(3));
        else if (cmd.compare("print") == 0)
            board.print();
        else if (cmd.compare("perft") == 0)
            perft(board, std::stoi(msg.substr(6)));
        else if (cmd.compare("quit") == 0)
            break;
    }
}

void isready() { 
    std::cout << "readyok" << std::endl; 
}

// Sets the board to a certain position
// position (startpos | fen? <fen>) (moves e2e4 c7c5)?
void position(Board &board, const std::string &s) {
    std::string cmd;
    std::stringstream ss(s);
    ss >> cmd;

    if (cmd.compare("startpos") == 0)
        board.startpos();
    else if (cmd.compare("fen") == 0)
        board.fromFen(s.substr(4));
    else
        board.fromFen(s);

    const size_t n = s.find("moves");

    if (n != std::string::npos)
        board.playSequence(s.substr(n + 6));
}

void go(Board &board, const std::string &s) {
    Settings settings;
    settings.init();

    std::stringstream ss(s);
    std::string cmd;

    while (ss >> cmd) {
        if (cmd.compare("infinite") == 0) {
            settings.timed = false;
            settings.depth = MAX_DEPTH;
        } else if (cmd.compare("depth") == 0) {
            settings.timed = false;
            settings.depth = std::stoi(s.substr(6));
        } 
        
        else if (cmd.compare("wtime") == 0) {
            ss >> cmd;
            settings.wtime = std::stoi(cmd);
        } else if (cmd.compare("btime") == 0) {
            ss >> cmd;
            settings.btime = std::stoi(cmd);
        } else if (cmd.compare("winc") == 0) {
            ss >> cmd;
            settings.winc = std::stoi(cmd);
        } else if (cmd.compare("binc") == 0) {
            ss >> cmd;
            settings.binc = std::stoi(cmd);
        }  else if (cmd.compare("movetime") == 0) {
            ss >> cmd;
            settings.movetime = std::stoi(cmd);
        }
    };

    const Move move = bestmove(board, settings);

    std::cout << "bestmove " << move << std::endl;
}

void perft(const Board &board, const int depth) {
    for (int d = 1; d <= depth; ++d) {
        const auto start = std::chrono::steady_clock::now();
        const uint64_t nodes = board.perft(d);
        const auto end = std::chrono::steady_clock::now();

        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "info depth " << d << " nodes " << nodes << " time "
                  << elapsed << "ms";

        if (elapsed > 0) {
            const double nps = 1000 * (nodes / elapsed);
            std::cout << " nps " << nps;
        }

        std::cout << std::endl;
    }
}

Move bestmove(const Board &board, Settings &settings) {
    Move bestMove;

    if (TYPE == RANDOM_PLAYER) {
        std::vector<Move> moves = board.genMoves();
        bestMove = moves[rand() % moves.size()];
    }

    else if (TYPE == MOST_CAPTURES) {
        std::vector<Move> moves = board.genMoves();

        int record = 0;
        bestMove = moves[0];

        for (const Move &move : moves) {
            const int captures = board.countCaptures(move);

            if (captures > record) {
                record = captures;
                bestMove = move;
            }
        }
    }

    else if (TYPE == ALPHABETA)
        bestMove = alphabeta::search(board, settings);

    return bestMove;
}
