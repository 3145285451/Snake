#include "../include/game_utils.h"
#include "../include/Snake.h"
#include "../include/game.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <set>
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace sf;

// 全局变量：食物位置与绘制用圆形
pair<int, int> food_position;
CircleShape food_circle;
RectangleShape body(Vector2f(GRID_SIZE, GRID_SIZE));
int score = 0;

//---------------------------------------------------------------------
// 功能：判断鼠标是否悬停在指定的 Sprite 上
bool isMouseOverSprite(const Vector2i &mouse_position, const Sprite &sprite)
{
    FloatRect bounds = sprite.getGlobalBounds();
    return bounds.contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y));
}

//---------------------------------------------------------------------
// 功能：将 RectangleShape 居中显示在窗口中
void centerRectangle(RenderWindow &window, RectangleShape &rect)
{
    Vector2u window_size = window.getSize();
    float rect_x = (window_size.x - rect.getSize().x) / 2;
    float rect_y = (window_size.y - rect.getSize().y) / 2;
    rect.setPosition(rect_x, rect_y);
}

//---------------------------------------------------------------------
// 功能：绘制主菜单界面
// 说明：
// - 绘制渐变背景、标题和 "start" 按钮；
// - start 按钮具有鼠标悬停效果（缩放和颜色变化）
// 注意：此函数只负责绘制，不调用 window.clear() 和 window.display()
void drawMainMenu(RenderWindow &window, Sprite &titleSprite, Sprite &startSprite)
{
    // 绘制渐变背景（使用 VertexArray 构造一个四边形）
    VertexArray bg(Quads, 4);
    bg[0].position = Vector2f(0, 0);
    bg[1].position = Vector2f(GAME_PIXEL_WIDTH, 0);
    bg[2].position = Vector2f(GAME_PIXEL_WIDTH, GAME_PIXEL_HEIGHT);
    bg[3].position = Vector2f(0, GAME_PIXEL_HEIGHT);
    bg[0].color = Color(20, 20, 40);
    bg[1].color = Color(30, 30, 60);
    bg[2].color = Color(50, 50, 80);
    bg[3].color = Color(40, 40, 70);

    // 设置标题颜色
    titleSprite.setColor(Color(255, 0, 255, 220));

    // 检测鼠标悬停，给 start 按钮反馈
    Vector2i mousePos = Mouse::getPosition(window);
    if (startSprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        startSprite.setScale(1.02f, 1.02f);
    else
    {
        startSprite.setScale(1.0f, 1.0f);
        startSprite.setColor(Color::White);
    }

    // 绘制背景、标题和按钮（display由主循环统一调用）
    window.draw(bg);
    window.draw(titleSprite);
    window.draw(startSprite);
}

//---------------------------------------------------------------------
// 功能：根据食物坐标在指定区域绘制食物
// 说明：
// - 使用一个 CircleShape 表示食物，位置基于 square 的位置和配置的网格参数
void drawFood(RenderWindow &window, RectangleShape &square, CircleShape &circle,
              pair<int, int> &food_position)
{
    circle.setRadius(GRID_SIZE / 2);
    circle.setFillColor(FOOD_COLOR);

    circle.setOutlineThickness(1.0f);
    circle.setOutlineColor(Color(255, 255, 255, 200));

    auto [food_x, food_y] = food_position;
    auto [x, y] = square.getPosition();
    // 将食物绘制在 square 区域对应网格位置
    circle.setPosition(Vector2f(x + GRID_SIZE * food_x,
                                y + MAP_PIXEL_SIZE - GRID_SIZE * food_y));
    window.draw(circle);
}

//---------------------------------------------------------------------
// 功能：处理键盘输入，更新蛇的下次移动方向
// 说明：
// - 当检测按键（W,S,A,D）且不反向时，更新 nextDir 参数
void handleKeyboardInput(Snake &s, int &nextDir)
{
    if (Keyboard::isKeyPressed(Keyboard::W) && s.dir != Down)
        nextDir = Up;
    if (Keyboard::isKeyPressed(Keyboard::S) && s.dir != Up)
        nextDir = Down;
    if (Keyboard::isKeyPressed(Keyboard::A) && s.dir != Right)
        nextDir = Left;
    if (Keyboard::isKeyPressed(Keyboard::D) && s.dir != Left)
        nextDir = Right;
}

//---------------------------------------------------------------------
// 功能：生成一个新的食物位置
// 说明：
// - 先构建整个地图的坐标集合，然后去除蛇身体占用的部分，随机选择一个剩余位置
void getFoodPosition(const Snake &s, pair<int, int> &food_position)
{
    vector<pair<int, int>> available_positions;
    for (int i = 0; i < MAP_SIZE; ++i)
    {
        for (int j = 1; j <= MAP_SIZE; ++j)
        {
            available_positions.emplace_back(i, j);
        }
    }

    set<pair<int, int>> snake_body(s.snake.begin(), s.snake.end());
    available_positions.erase(
        remove_if(available_positions.begin(),
                  available_positions.end(),
                  [&snake_body](const pair<int, int> &pos)
                  {
                      return snake_body.count(pos) > 0;
                  }),
        available_positions.end());

    if (!available_positions.empty())
    {
        int index = rand() % available_positions.size();
        food_position = available_positions[index];
    }
}

//---------------------------------------------------------------------
// 功能：更新食物状态
// 说明：
// - 若蛇头与食物重合，则增长蛇体，并重置食物计时及位置
// - 否则若等待时间超过设定间隔，则重新绘制当前食物
void updateFood(RenderWindow &window, RectangleShape &square, Snake &s,
                pair<int, int> &food_position, Clock &food_clock,
                float food_update_interval)
{
    if (s.snake.front() == food_position)
    {
        ++score;
        s.increaseSnakeLength(PER_FOOD_INCREMENT);
        food_clock.restart();
        getFoodPosition(s, food_position);
    }
    else if (food_clock.getElapsedTime().asSeconds() >= food_update_interval)
    {
        drawFood(window, square, food_circle, food_position);
    }
}

//---------------------------------------------------------------------
// 功能：绘制蛇及其细节
// 说明：
// - 根据 snake 列表中各个格子的位置绘制蛇体
// - 蛇头部分额外绘制眼睛以区分
void drawSnake(RenderWindow &window, RectangleShape &square,
               const deque<pair<int, int>> &snake)
{
    auto [x, y] = square.getPosition();

    for (size_t i = 0; i < snake.size(); i++)
    {
        auto [grid_x, grid_y] = snake[i];
        float posX = x + GRID_SIZE * grid_x;
        float posY = y + MAP_PIXEL_SIZE - GRID_SIZE * grid_y;

        body.setPosition(Vector2f(posX, posY));
        window.draw(body);

        // 仅在蛇头绘制眼睛
        if (i == 0)
        {
            CircleShape eye(GRID_SIZE / 10);
            eye.setFillColor(Color::White);

            float eyeOffsetX = GRID_SIZE * 0.2f;
            float eyeOffsetY = GRID_SIZE * 0.2f;

            // 左眼
            eye.setPosition(Vector2f(posX + eyeOffsetX, posY + eyeOffsetY));
            window.draw(eye);

            // 右眼
            eye.setPosition(Vector2f(posX + GRID_SIZE - eyeOffsetX * 2, posY + eyeOffsetY));
            window.draw(eye);
        }
    }
}

//---------------------------------------------------------------------
// 功能：数字绘制函数
// 说明：
// - 根据预定义的数字像素矩阵逐块绘制数字效果
// - 可用于显示各种数字信息（如得分）
void drawDigit(RenderWindow &window, int digit, float startX, float startY)
{
    const float shadow_offset = 2.f; // 阴影偏移量
    RectangleShape block(Vector2f(BLOCK_SIZE, BLOCK_SIZE));

    Color main_color = Color::Yellow;
    Color shadow_color = Color(50, 50, 50, 150);

    // 遍历数字的像素矩阵，遇 1 就绘制一个小块（先绘制阴影，再绘制主块）
    for (int r = 0; r < DIGIT_ROWS; ++r)
    {
        for (int c = 0; c < DIGIT_COLS; ++c)
        {
            if (digitPatterns[digit][r][c] == 1)
            {
                float x = startX + c * (BLOCK_SIZE + BLOCK_GAP);
                float y = startY + r * (BLOCK_SIZE + BLOCK_GAP);

                // 绘制阴影块（偏移一定距离）
                block.setPosition(x + shadow_offset, y + shadow_offset);
                block.setFillColor(shadow_color);
                window.draw(block);

                // 绘制主块
                block.setPosition(x, y);
                block.setFillColor(main_color);
                window.draw(block);
            }
        }
    }
}

//---------------------------------------------------------------------
// 功能：美化版绘制得分函数
// 说明：
// - 将当前得分转换为字符串后，依次调用数字绘制函数顺序显示
// - 数字之间采用适当间距，确保显示美观
void drawScorePixel(RenderWindow &window, const int &score)
{
    // 将分数转换为字符串，便于逐位绘制
    std::string score_str = std::to_string(score);
    float digit_distance = 8.f; // 数字之间的水平间距
    float start_x = 40.f;
    float start_y = 30.f;

    // 计算单个数字的宽度
    float digit_width = DIGIT_COLS * (BLOCK_SIZE + BLOCK_GAP) - BLOCK_GAP;
    // 计算整个得分显示区域的宽度
    float total_width = score_str.size() * digit_width + (score_str.size() - 1) * digit_distance;

    // 依次绘制每个数字，调用单个数字绘制函数
    for (char c : score_str)
    {
        int d = c - '0';
        drawDigit(window, d, start_x, start_y);
        start_x += digit_width + digit_distance;
    }
}

//---------------------------------------------------------------------
// 功能：绘制游戏结束界面
// 说明：
// - 绘制渐变背景，并为 game over、replay 与 exit 图像设置淡入动画
// - 提供鼠标悬停反馈，并响应点击动作
// 注意：此处内部形成一个循环，直到玩家点击 replay 或 exit
void drawEndGameScreen(RenderWindow &window, const Sprite &game_over,
                       const Sprite &replaySprite, const Sprite &exitSprite)
{
    // 克隆 sprite，便于动态调整（颜色透明度、缩放等）
    Sprite over = game_over;
    Sprite replay = replaySprite;
    Sprite exit = exitSprite;

    // 配置渐变背景
    VertexArray bg(Quads, 4);
    bg[0].position = Vector2f(0, 0);
    bg[1].position = Vector2f(GAME_PIXEL_WIDTH, 0);
    bg[2].position = Vector2f(GAME_PIXEL_WIDTH, GAME_PIXEL_HEIGHT);
    bg[3].position = Vector2f(0, GAME_PIXEL_HEIGHT);
    bg[0].color = Color(20, 20, 40);
    bg[1].color = Color(30, 30, 60);
    bg[2].color = Color(50, 50, 80);
    bg[3].color = Color(40, 40, 70);

    float alpha = 0.f; // 透明度
    Clock clock;

    // 循环直至玩家选择结束界面动作
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                if (isMouseOverSprite(mousePos, replay))
                {
                    return; // 点击 replay 退出结束界面，重新开始游戏
                }
                if (isMouseOverSprite(mousePos, exit))
                {
                    sf::sleep(sf::milliseconds(100));
                    window.close();
                    return;
                }
            }
        }

        // 鼠标悬停反馈：按钮轻微放大
        Vector2i mousePos = Mouse::getPosition(window);
        if (isMouseOverSprite(mousePos, replay))
            replay.setScale(1.02f, 1.02f);
        else
            replay.setScale(1.0f, 1.0f);

        if (isMouseOverSprite(mousePos, exit))
            exit.setScale(1.02f, 1.02f);
        else
            exit.setScale(1.0f, 1.0f);

        // 淡入动画：逐渐提高透明度
        float elapsed = clock.getElapsedTime().asSeconds();
        if (alpha < 255.f)
            alpha = min(255.f, elapsed * 100.f);
        Uint8 a = static_cast<Uint8>(alpha);

        over.setColor(Color(64, 224, 208, a));
        replay.setColor(Color(255, 255, 255, a));
        exit.setColor(Color(255, 255, 255, a));

        window.clear();
        window.draw(bg);
        window.draw(over);
        window.draw(replay);
        window.draw(exit);
        window.display();
    }
}

//---------------------------------------------------------------------
// 功能：绘制获胜界面（调用 game over 界面函数，传入 win 图片）
void drawWinGameScreen(RenderWindow &window, const Sprite &win, const Sprite &replay, const Sprite &exit)
{
    drawEndGameScreen(window, win, replay, exit);
}

//---------------------------------------------------------------------
// 功能：绘制失败界面（调用 game over 界面函数，传入 lose 图片）
void drawFailGameScreen(RenderWindow &window, const Sprite &lose, const Sprite &replay, const Sprite &exit)
{
    drawEndGameScreen(window, lose, replay, exit);
}

//---------------------------------------------------------------------
// 功能：设置游戏界面背景和网格
// 说明：
// - 配置背景色、游戏区域（square）以及基于网格生成的背景线条（grid_lines）
void setGameScreen(RenderWindow &window, RectangleShape &square, VertexArray &grid_lines)
{
    // 配置游戏区域的外观
    square.setFillColor(Color(75, 152, 240));
    square.setOutlineThickness(5);
    square.setOutlineColor(Color(200, 200, 200));
    centerRectangle(window, square);

    grid_lines.clear(); // 每帧前先清空已有顶点
    auto [x, y] = square.getPosition();

    // 构建网格线条
    for (int i = 0; i <= MAP_SIZE; i++)
    {
        grid_lines.append(Vertex(Vector2f(x, y + i * GRID_SIZE), Color(204, 255, 229)));
        grid_lines.append(Vertex(Vector2f(x + MAP_PIXEL_SIZE, y + i * GRID_SIZE), Color(204, 255, 229)));
        grid_lines.append(Vertex(Vector2f(x + i * GRID_SIZE, y), Color(204, 255, 229)));
        grid_lines.append(Vertex(Vector2f(x + i * GRID_SIZE, y + MAP_PIXEL_SIZE), Color(204, 255, 229)));
    }
}

//---------------------------------------------------------------------
// 功能：游戏过程中主要操作逻辑处理
// 说明：
// - 监听键盘输入、更改蛇运动方向
// - 控制蛇移动、检测非法状态（例如碰撞或越界）
// - 绘制蛇、食物、背景网格
// - 判断游戏获胜/失败状态返回相应标志
GameState handleGameOperation(RenderWindow &window, RectangleShape &square,
                              VertexArray &grid_lines, Snake &s,
                              pair<int, int> &food_position)
{
    // 记录下一次的方向（静态变量保存上次输入）
    static int nextDir = s.dir;
    // 记录最后一次合法状态的蛇体
    static deque<pair<int, int>> last_valid_snake = s.snake;
    // 标识是否处于非法状态
    static bool is_illegal = false;
    // 三个计时器：运动、非法状态检测、食物刷新
    static Clock move_clock, illegal_state_clock, food_clock;
    // 静态标记：是否已完成新游戏的重置
    static bool reset_done = false;

    // 当蛇处于初始状态（新游戏开始）时，仅在首次检测到是新一局游戏时重置静态变量。
    if (s.getSnakeLength() == SNAKE_INIT_LENGTH && (!reset_done || (!last_valid_snake.empty() && s.snake.front() != last_valid_snake.front())))
    {
        nextDir = s.dir;
        last_valid_snake = s.snake;
        is_illegal = false;
        move_clock.restart();
        illegal_state_clock.restart();
        food_clock.restart();
        reset_done = true;
    }

    // 绘制游戏区域（square 由 setGameScreen 配置后绘制）
    window.draw(square);

    // 监听键盘输入，更新 nextDir（不允许反向）
    handleKeyboardInput(s, nextDir);

    // 蛇的移动逻辑：隔一定时间更新移动
    if (move_clock.getElapsedTime().asSeconds() >= move_update_interval)
    {
        if (s.canMove(nextDir) && !s.isOverlapping()) // 合法移动
        {
            s.move(nextDir);
            s.dir = nextDir;
            last_valid_snake = s.snake; // 保存最新合法状态
            is_illegal = false;
        }
        else if (!is_illegal) // 首次进入非法状态时启动计时
        {
            illegal_state_clock.restart();
            is_illegal = true;
        }
        move_clock.restart();
    }

    // 非法状态持续一定时间后判定游戏失败
    if (is_illegal && illegal_state_clock.getElapsedTime().asSeconds() >= check_update_interval)
    {
        score = 0;
        return Lost;
    }
    // 绘制蛇（以最后一次合法状态以保证画面连贯性）
    drawSnake(window, square, last_valid_snake);
    // 更新食物状态（若吃到或等待超时则更新）
    updateFood(window, square, s, food_position, food_clock, food_update_interval);
    // 绘制背景网格
    drawScorePixel(window, score);

    window.draw(grid_lines);

    // 最后刷新屏幕，由主循环的 window.display() 调用统一刷新时机（这里只返回状态）
    return (s.getSnakeLength() >= SNAKE_WIN_LENGTH) ? Won : Running;
}