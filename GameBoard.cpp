#include "GameBoard.h"
#include <algorithm>
#include <random>

GameBoard::GameBoard() : score(0) {
    reset();
}

void GameBoard::reset() {
    board = std::vector<std::vector<int>>(4, std::vector<int>(4, 0));
    score = 0;
    addRandomTile();
    addRandomTile();
}

const std::vector<std::vector<int>>& GameBoard::getBoard() const {
    return board;
}

int GameBoard::getScore() const {
    return score;
}

bool GameBoard::slideRowLeft(std::vector<int>& row) {
    std::vector<int> newRow;
    // 移除所有的0
    for(auto num : row) {
        if(num != 0) newRow.push_back(num);
    }
    // 合并相同的数字
    for(int i = 0; i < newRow.size()-1; ++i) {
        if(newRow[i] == newRow[i+1]) {
            newRow[i] *= 2;
            score += newRow[i];
            newRow[i+1] = 0;
        }
    }
    // 移除合并后的0
    std::vector<int> finalRow;
    for(auto num : newRow) {
        if(num != 0) finalRow.push_back(num);
    }
    // 填充0
    while(finalRow.size() < 4) finalRow.push_back(0);
    bool changed = (row != finalRow);
    row = finalRow;
    return changed;
}

bool GameBoard::moveLeft() {
    bool moved = false;
    for(auto& row : board) {
        if(slideRowLeft(row)) {
            moved = true;
        }
    }
    if(moved) {
        addRandomTile();
    }
    return moved;
}

bool GameBoard::moveRight() {
    bool moved = false;
    for(auto& row : board) {
        std::reverse(row.begin(), row.end());
        if(slideRowLeft(row)) {
            moved = true;
        }
        std::reverse(row.begin(), row.end());
    }
    if(moved) {
        addRandomTile();
    }
    return moved;
}

bool GameBoard::moveUp() {
    bool moved = false;
    for(int j = 0; j < 4; ++j) {
        std::vector<int> column;
        for(int i = 0; i < 4; ++i) column.push_back(board[i][j]);
        if(slideRowLeft(column)) {
            moved = true;
            for(int i = 0; i < 4; ++i) board[i][j] = column[i];
        }
    }
    if(moved) {
        addRandomTile();
    }
    return moved;
}

bool GameBoard::moveDown() {
    bool moved = false;
    for(int j = 0; j < 4; ++j) {
        std::vector<int> column;
        for(int i = 0; i < 4; ++i) column.push_back(board[i][j]);
        std::reverse(column.begin(), column.end());
        if(slideRowLeft(column)) {
            moved = true;
            std::reverse(column.begin(), column.end());
            for(int i = 0; i < 4; ++i) board[i][j] = column[i];
        }
    }
    if(moved) {
        addRandomTile();
    }
    return moved;
}

void GameBoard::addRandomTile() {
    std::vector<std::pair<int, int>> empty;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(board[i][j] == 0)
                empty.emplace_back(i, j);
    if(empty.empty()) return;
    // 随机选择一个空位置
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_pos(0, empty.size()-1);
    auto [i, j] = empty[dis_pos(gen)];
    // 生成2或4
    std::uniform_int_distribution<> dis_val(1, 10);
    board[i][j] = (dis_val(gen) == 1) ? 4 : 2;
}

bool GameBoard::canMove() {
    // 检查是否有空格
    for(auto& row : board)
        for(auto num : row)
            if(num == 0) return true;
    // 检查是否可以合并
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j) {
            if(i < 3 && board[i][j] == board[i+1][j]) return true;
            if(j < 3 && board[i][j] == board[i][j+1]) return true;
        }
    return false;
}
