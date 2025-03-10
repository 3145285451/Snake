#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

// 网格参数
constexpr int MAP_SIZE = 15;                         // 每行/列的单元数
constexpr int GRID_SIZE = 50;                        // 每个单元的像素尺寸
constexpr int MAP_PIXEL_SIZE = MAP_SIZE * GRID_SIZE; // 游戏区域总尺寸

// 窗口尺寸
constexpr int GAME_PIXEL_WIDTH = 1150;
constexpr int GAME_PIXEL_HEIGHT = 820;

// 初始状态和获胜条件
constexpr int SNAKE_INIT_LENGTH = 3;                        // 初始蛇体长度
constexpr int PER_FOOD_INCREMENT = 1;                       // 吃到食物后增长的单位
constexpr int SNAKE_WIN_LENGTH = (MAP_SIZE * MAP_SIZE / 2); // 获胜条件

// 更新间隔时间
constexpr float move_update_interval = 0.15f; // 每一次移动更新的时间间隔（单位：秒）
constexpr float check_update_interval = 0.1f; // 检查游戏状态更新的时间间隔（单位：秒）
constexpr float food_update_interval = 1.0f;  // 食物刷新更新的时间间隔（单位：秒）

// 蛇身颜色
const sf::Color BODY_COLOR = sf::Color(255, 25, 25);
const sf::Color FOOD_COLOR = sf::Color(255, 255, 51);

// 游戏状态在不同阶段切换界面及逻辑
enum GameState
{
    Running,
    Won,
    Lost
};

// 方向枚举
enum
{
    Up,
    Down,
    Left,
    Right
};

// 定义绘制数字所需参数
const int DIGIT_ROWS = 5; // 数字矩阵行数
const int DIGIT_COLS = 3; // 数字矩阵列数
const int BLOCK_SIZE = 8; // 每个像素块的尺寸（单位：像素）
const int BLOCK_GAP = 2;  // 每个像素块之间的间隙

// 每个数字（0-9）的像素布局，1 表示绘制像素块，0 表示空白
const int digitPatterns[10][DIGIT_ROWS][DIGIT_COLS] = {
    // 0
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1}},
    // 1
    {
        {0, 1, 0},
        {1, 1, 0},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}},
    // 2
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1}},
    // 3
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}},
    // 4
    {
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1}},
    // 5
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}},
    // 6
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}},
    // 7
    {
        {1, 1, 1},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 0},
        {0, 1, 0}},
    // 8
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}},
    // 9
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1}}};

#endif