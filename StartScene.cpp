#include "StartScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* StartScene::createScene() {
    return StartScene::create();
}

bool StartScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置背景
    auto bg = LayerColor::create(Color4B(204, 192, 179, 255));
    bg->setContentSize(visibleSize);
    bg->setPosition(origin);
    this->addChild(bg);

    // 添加标题
    auto titleLabel = Label::createWithSystemFont("2048 Game", "Arial", 48);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - 100));
    this->addChild(titleLabel);

    // 添加开始按钮
    auto startItem = MenuItemFont::create("Start Game", [&](Ref* sender) {
        // 切换到 GameScene
        auto gameScene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene));
        });
    startItem->setFontSizeObj(36);

    // 添加退出按钮
    auto exitItem = MenuItemFont::create("Exit", [&](Ref* sender) {
        Director::getInstance()->end();
        });
    exitItem->setFontSizeObj(36);

    // 创建菜单
    auto menu = Menu::create(startItem, exitItem, nullptr);
    menu->alignItemsVerticallyWithPadding(20);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    this->addChild(menu);

    return true;
}
