#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameBoard.h"

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameScene);

private:
    GameBoard gameBoard;
    std::vector<std::vector<cocos2d::Label*>> tileLabels;
    std::vector<std::vector<cocos2d::LayerColor*>> tileLayers;

    cocos2d::Label* scoreLabel;
    int score;

    void updateBoard();
    void gameOver();
    cocos2d::Color4B getTileColor(int number);
    void resetGame();
};

#endif // __GAME_SCENE_H__
