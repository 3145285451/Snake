#include "../include/Snake.h"
#include "../include/game.h"
#include "../include/game_utils.h"
#include "../include/resource.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace sf;

int main()
{
    // 创建窗口并限制帧率
    RenderWindow window(VideoMode(GAME_PIXEL_WIDTH, GAME_PIXEL_HEIGHT), "Snake", Style::Default);
    window.setFramerateLimit(60);

    // 使用资源管理类加载外部资源
    ResourceManager resManager;
    if (!resManager.loadResources())
        return -1;

    // 设置主菜单所需的 Sprite（居中及位置调整）
    Sprite titleSprite(resManager.titleTexture);
    {
        FloatRect titleBounds = titleSprite.getLocalBounds();
        titleSprite.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        titleSprite.setPosition(GAME_PIXEL_WIDTH / 2, 150);
    }
    Sprite startSprite(resManager.startTexture);
    {
        FloatRect startBounds = startSprite.getLocalBounds();
        startSprite.setOrigin(startBounds.width / 2, startBounds.height / 2);
        startSprite.setPosition(GAME_PIXEL_WIDTH / 2, GAME_PIXEL_HEIGHT / 2 + 100);
    }
    Sprite winSprite(resManager.winTexture);
    {
        FloatRect bounds = winSprite.getLocalBounds();
        winSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        winSprite.setPosition(GAME_PIXEL_WIDTH / 2, GAME_PIXEL_HEIGHT / 2 - 150);
    }
    Sprite loseSprite(resManager.loseTexture);
    {
        FloatRect bounds = loseSprite.getLocalBounds();
        loseSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        loseSprite.setPosition(GAME_PIXEL_WIDTH / 2, GAME_PIXEL_HEIGHT / 2 - 150);
    }
    Sprite replaySprite(resManager.replayTexture);
    {
        FloatRect bounds = replaySprite.getLocalBounds();
        replaySprite.setOrigin(bounds.width / 2, bounds.height / 2);
        replaySprite.setPosition(GAME_PIXEL_WIDTH / 2 - 150, GAME_PIXEL_HEIGHT / 2 + 150);
    }
    Sprite exitSprite(resManager.exitTexture);
    {
        FloatRect bounds = exitSprite.getLocalBounds();
        exitSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        exitSprite.setPosition(GAME_PIXEL_WIDTH / 2 + 150, GAME_PIXEL_HEIGHT / 2 + 150);
    }

    bool inMainMenu = true;

    // 主循环：按状态分离主菜单与游戏
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (inMainMenu)
        {
            window.clear(Color(30, 30, 30));
            drawMainMenu(window, titleSprite, startSprite);
            window.display();

            // 检测鼠标按下与释放
            static bool left_mouse_down = false;
            if (!Mouse::isButtonPressed(Mouse::Left))
                // 鼠标已释放，重置标志
                left_mouse_down = false;
            else if (!left_mouse_down && isMouseOverSprite(Mouse::getPosition(window), startSprite))
            {
                // 检测到鼠标从未按下状态转为按下，并且在 start 按钮范围内
                left_mouse_down = true;
                inMainMenu = false;
                sf::sleep(sf::milliseconds(200));
            }
        }
        else
        {
            // 初始化游戏区域和对象，确保每局游戏的开始状态独立且随机
            RectangleShape square(Vector2f(MAP_PIXEL_SIZE, MAP_PIXEL_SIZE));
            VertexArray grid_lines(Lines);
            setGameScreen(window, square, grid_lines);

            Snake s(square, MAP_SIZE, SNAKE_INIT_LENGTH);
            body.setFillColor(BODY_COLOR);

            srand(static_cast<unsigned>(time(0)));
            food_position = {rand() % MAP_SIZE, rand() % MAP_SIZE + 1};

            GameState gameState = Running;

            while (gameState == Running)
            {
                while (window.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                    {
                        window.close();
                        return 0;
                    }
                }

                // 更新游戏状态并稳定刷新屏幕，呈现最新画面
                window.clear(Color(0, 0, 70));
                setGameScreen(window, square, grid_lines);
                gameState = handleGameOperation(window, square, grid_lines, s, food_position);
                window.display();
            }

            // 根据游戏结果展示不同的结束界面
            if (gameState == Won)
                drawWinGameScreen(window, winSprite, replaySprite, exitSprite);
            else
                drawFailGameScreen(window, loseSprite, replaySprite, exitSprite);

            // 结束界面后返回主菜单状态
            inMainMenu = true;
        }
    }

    return 0;
}