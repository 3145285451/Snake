#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "game.h"
#include "snake.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <deque>
#include <utility>

// 全局变量用于各函数间共享游戏元素状态，便于状态同步管理
extern std::pair<int, int> food_position;
extern sf::CircleShape food_circle;
extern sf::RectangleShape body;

// 检查鼠标是否悬停于目标 Sprite 上，用于交互反馈
bool isMouseOverSprite(const sf::Vector2i &mouse_position, const sf::Sprite &sprite);

// 根据窗口尺寸调整 RectangleShape 位置，确保居中显示
void centerRectangle(sf::RenderWindow &window, sf::RectangleShape &rect);

// 绘制主菜单界面，整体视觉统一且易于交互
void drawMainMenu(sf::RenderWindow &window, sf::Sprite &titleSprite, sf::Sprite &startSprite);

// 根据当前网格坐标在指定区域内绘制食物，保持视觉同步
void drawFood(sf::RenderWindow &window, sf::RectangleShape &square, sf::CircleShape &circle,
              std::pair<int, int> &food_position);

// 处理键盘输入，更新蛇下一步的移动方向（避免直接反向）
void handleKeyboardInput(Snake &s, int &nextDir);

// 根据现有蛇体位置更新可放置食物的坐标，使游戏更具变化性
void getFoodPosition(const Snake &s, std::pair<int, int> &food_position);

// 按设定间隔更新食物状态，保持游戏节奏，不失连续性
void updateFood(sf::RenderWindow &window, sf::RectangleShape &square, Snake &s,
                std::pair<int, int> &food_position, sf::Clock &food_clock,
                float food_update_interval);

// 绘制蛇体，采用分段绘制保证前后状态一致性，便于观察移动轨迹
void drawSnake(sf::RenderWindow &window, sf::RectangleShape &square,
               const std::deque<std::pair<int, int>> &snake);

// 绘制结束界面，统一过渡动画及交互反馈，便于用户决定下一步操作
void drawEndGameScreen(sf::RenderWindow &window, const sf::Sprite &game_over, const sf::Sprite &replay, const sf::Sprite &exit);

// 针对获胜情况，调用结束界面展示特定获胜画面，突出游戏成果
void drawWinGameScreen(sf::RenderWindow &window, const sf::Sprite &win, const sf::Sprite &replay, const sf::Sprite &exit);

// 针对失败情况，调用结束界面展示特定失败画面，给予用户直观提示
void drawFailGameScreen(sf::RenderWindow &window, const sf::Sprite &win, const sf::Sprite &replay, const sf::Sprite &exit);

// 设置游戏区域背景及网格，统一界面风格并便于后续状态更新
void setGameScreen(sf::RenderWindow &window, sf::RectangleShape &square, sf::VertexArray &grid_lines);

// 处理游戏主逻辑，整合输入检测、移动更新、碰撞检测及界面刷新，便于整体模式控制
GameState handleGameOperation(sf::RenderWindow &window, sf::RectangleShape &square,
                              sf::VertexArray &grid_lines, Snake &s,
                              std::pair<int, int> &food_position);

#endif // GAME_UTILS_H