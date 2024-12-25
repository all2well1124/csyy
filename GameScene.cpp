#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) return false;

    // ��ʼ������
    score = 0;

    // ���ñ���
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = LayerColor::create(Color4B(250, 248, 239, 255));
    this->addChild(bg);

    // ��ӷ�����ʾ
    scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 24);
    scoreLabel->setPosition(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 50);
    this->addChild(scoreLabel);

    // ��ʼ������
    tileLabels.resize(4, std::vector<Label*>(4, nullptr));
    tileLayers.resize(4, std::vector<LayerColor*>(4, nullptr));
    float tileSize = 100.0f;
    float gap = 10.0f;
    float startX = origin.x + (visibleSize.width - (tileSize * 4 + gap * 3)) / 2;
    float startY = origin.y + (visibleSize.height - (tileSize * 4 + gap * 3)) / 2;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            // �������鱳��
            auto tile = LayerColor::create(getTileColor(0), tileSize, tileSize);
            tile->setPosition(startX + j * (tileSize + gap), startY + i * (tileSize + gap));
            this->addChild(tile);
            tileLayers[i][j] = tile;

            // �������ֱ�ǩ
            auto label = Label::createWithSystemFont("", "Arial", 36);
            label->setPosition(tileSize / 2, tileSize / 2);
            label->setVisible(false); // ��ʼ����
            tile->addChild(label);
            tileLabels[i][j] = label;
        }

    updateBoard();

    // �����¼�����
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) -> bool {
        return true;
        };
    touchListener->onTouchEnded = [&](Touch* touch, Event* event) {
        Vec2 start = touch->getStartLocation();
        Vec2 end = touch->getLocation();
        Vec2 delta = end - start;

        if (delta.length() < 50) return; // ��������̫�̺���

        if (std::abs(delta.x) > std::abs(delta.y)) {
            if (delta.x > 0) {
                // �һ�
                gameBoard.moveRight();
            }
            else {
                // ��
                gameBoard.moveLeft();
            }
        }
        else {
            if (delta.y > 0) {
                // �ϻ�
                gameBoard.moveUp();
            }
            else {
                // �»�
                gameBoard.moveDown();
            }
        }

        updateBoard();
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // ������ð�ť
    auto resetItem = MenuItemFont::create("Reset", [&](Ref* sender) {
        resetGame();
        });
    resetItem->setFontSizeObj(24);
    auto menu = Menu::create(resetItem, nullptr);
    menu->setPosition(origin.x + 100, origin.y + visibleSize.height - 50);
    this->addChild(menu);

    return true;
}

Color4B GameScene::getTileColor(int number) {
    switch (number) {
    case 0: return Color4B(204, 192, 179, 255);
    case 2: return Color4B(238, 228, 218, 255);
    case 4: return Color4B(237, 224, 200, 255);
    case 8: return Color4B(242, 177, 121, 255);
    case 16: return Color4B(245, 149, 99, 255);
    case 32: return Color4B(246, 124, 95, 255);
    case 64: return Color4B(246, 94, 59, 255);
    case 128: return Color4B(237, 207, 114, 255);
    case 256: return Color4B(237, 204, 97, 255);
    case 512: return Color4B(237, 200, 80, 255);
    case 1024: return Color4B(237, 197, 63, 255);
    case 2048: return Color4B(237, 194, 46, 255);
    default: return Color4B(60, 58, 50, 255); // ����2048����ɫ
    }
}

void GameScene::updateBoard() {
    const auto& board = gameBoard.getBoard();
    bool changed = false;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            int num = board[i][j];
            if (num != 0) {
                tileLabels[i][j]->setString(std::to_string(num));
                tileLabels[i][j]->setVisible(true);
                tileLayers[i][j]->setColor(Color3B(getTileColor(num)));
                tileLabels[i][j]->setTextColor((num <= 4) ? Color4B::BLACK : Color4B::WHITE);

                // ��ӵ�������
                auto scaleUp = ScaleTo::create(0.1f, 1.2f);
                auto scaleDown = ScaleTo::create(0.1f, 1.0f);
                tileLayers[i][j]->runAction(Sequence::create(scaleUp, scaleDown, nullptr));
            }
            else {
                tileLabels[i][j]->setString("");
                tileLabels[i][j]->setVisible(false);
                tileLayers[i][j]->setColor(Color3B(getTileColor(0)));
            }
        }

    // ���·���
    score = gameBoard.getScore();
    scoreLabel->setString("Score: " + std::to_string(score));

    if (!gameBoard.canMove()) {
        gameOver();
    }
}

void GameScene::gameOver() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto gameOverLabel = Label::createWithSystemFont("Game Over!", "Arial", 48);
    gameOverLabel->setPosition(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2);
    gameOverLabel->setColor(Color3B::RED);
    this->addChild(gameOverLabel);
}

void GameScene::resetGame() {
    gameBoard.reset();
    updateBoard();

    // �Ƴ� "Game Over" ��ǩ���������
    auto children = this->getChildren();
    for (auto child : children) {
        if (auto label = dynamic_cast<Label*>(child)) {
            if (label->getString() == "Game Over!") {
                this->removeChild(label);
                break;
            }
        }
    }

    scoreLabel->setString("Score: 0");
}
