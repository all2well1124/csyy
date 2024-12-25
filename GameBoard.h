#ifndef __GAME_BOARD_H__
#define __GAME_BOARD_H__

#include <vector>

class GameBoard {
public:
    GameBoard();
    void reset();
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    bool canMove();
    void addRandomTile();
    const std::vector<std::vector<int>>& getBoard() const;
    int getScore() const;

private:
    std::vector<std::vector<int>> board;
    int score;

    bool slideRowLeft(std::vector<int>& row);
};

#endif // __GAME_BOARD_H__
