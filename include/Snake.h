#ifndef SNAKE_H
#define SNAKE_H

#include "game.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

// Point 用于计算游戏区域内的网格坐标，确保蛇以固定单位移动。
class Point
{
    int x, y;                             // 区域左上角坐标，由 square 定位
    int num;                              // 网格维度，即每行/列单元格数
    int unit;                             // 单元格大小（由 GRID_SIZE 定义）
    std::vector<std::vector<int>> points; // 存储所有网格坐标

    friend class Snake; // 允许 Snake 访问网格数据

public:
    // 构造时利用 square 的位置和网格维度 n 构造完整网格，
    // 保证所有位置计算基于一致网格单位。
    Point(sf::RectangleShape &square, int n)
        : x(static_cast<int>(square.getPosition().x)), y(static_cast<int>(square.getPosition().y)), num(n), unit(GRID_SIZE)
    {
        for (int i = 0; i < num; i++)
        {
            for (int j = 0; j < num; j++)
            {
                points.push_back({i, j});
            }
        }
    }
};

// Snake 类封装了蛇的逻辑，如移动、碰撞检测、增长等，
// 实现和渲染分离，便于维护和扩展。
class Snake
{
public:
    Point p;                               // 基于网格坐标运动参考
    int dir;                               // 当前移动方向（定义在 game.h 中）
    std::deque<std::pair<int, int>> snake; // 蛇体各部分在网格中的坐标

    // 初始化蛇，使初始状态与游戏区域和长度匹配
    Snake(sf::RectangleShape &square, int num, int initLength);

    // 判断指定方向是否可移动，以防反向或碰撞
    bool canMove(int dir);

    // 检测蛇体是否自碰撞
    bool isOverlapping();

    // 根据 direction 更新蛇体位置，实现平滑移动
    void move(int dir);

    // 吃到食物后扩展蛇体，保证增长过程合理
    void increaseSnakeLength(int n);

    // 返回蛇当前长度，用于判断获胜条件等
    int getSnakeLength() const;
};

#endif