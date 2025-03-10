#include "../include/Snake.h"
#include "../include/game.h"
#include <set>
#include <algorithm>

// 构造时将蛇初始化在网格中点附近，确保每局游戏的起始状态平衡
Snake::Snake(sf::RectangleShape &square, int num, int n)
    : p(square, num), dir(Up)
{
    int x = p.num / 2;
    int y = x;
    for (int i = 0; i < n; i++)
        snake.push_front({x, y - n + i});
}

// 为确保不会退出游戏区域，通过模拟下一步位置来判断移动合法性
bool Snake::canMove(int new_dir)
{
    auto [x, y] = snake.front();
    switch (new_dir)
    {
    case Up:
        y += 1;
        break;
    case Down:
        y -= 1;
        break;
    case Left:
        x -= 1;
        break;
    case Right:
        x += 1;
        break;
    }
    // 此处保证蛇头始终位于有效网格内
    return x >= 0 && x < p.num && y > 0 && y <= p.num;
}

// 使用集合检查重复坐标，以此判断蛇是否自碰撞
bool Snake::isOverlapping()
{
    std::set<std::pair<int, int>> seen;
    for (const auto &segment : snake)
    {
        if (!seen.insert(segment).second)
            return true;
    }
    return false;
}

// 更新蛇的位置：通过移除尾部并在头部添加新位置，实现连续移动
void Snake::move(int new_dir)
{
    snake.pop_back();
    auto [x, y] = snake.front();
    switch (new_dir)
    {
    case Up:
        y += 1;
        break;
    case Down:
        y -= 1;
        break;
    case Left:
        x -= 1;
        break;
    case Right:
        x += 1;
        break;
    }
    snake.push_front({x, y});
}

// 延长蛇体时，根据尾部的延伸方向保持增长自然；若超界，则尝试交换方向
void Snake::increaseSnakeLength(int n)
{
    int dx = 0, dy = 0;
    if (snake.size() < 2)
    {
        auto head = snake.front();
        if (head.first + 1 < p.num)
            dx = 1;
        else if (head.first - 1 >= 0)
            dx = -1;
        else if (head.second + 1 < p.num)
            dy = 1;
        else if (head.second - 1 >= 0)
            dy = -1;
    }
    else
    {
        auto tail = snake.back();
        auto beforeTail = snake[snake.size() - 2];
        dx = tail.first - beforeTail.first;
        dy = tail.second - beforeTail.second;
    }

    for (int i = 0; i < n; i++)
    {
        int new_x = snake.back().first + dx;
        int new_y = snake.back().second + dy;
        // 保证扩展时仍在网格内，不合适时交换方向
        if (new_x < 0 || new_x >= p.num || new_y <= 0 || new_y > p.num)
        {
            std::swap(dx, dy);
            new_x = snake.back().first + dx;
            new_y = snake.back().second + dy;
        }
        snake.push_back({new_x, new_y});
    }
}

// 返回蛇的当前长度，用于后续判断游戏状态
int Snake::getSnakeLength() const { return snake.size(); }