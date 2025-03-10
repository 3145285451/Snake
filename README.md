# 贪吃蛇游戏 (Snake Game)

## 项目概述
基于 **SFML 2.6.2** 开发的经典 **贪吃蛇游戏**，采用 **C++ 17** 编写，支持 **Windows** 和 **Linux** 平台，适合学习 **SFML 开发** 或 **C++ 游戏开发** 。

---

## 功能特点
- **图形界面**：基于 SFML 渲染，包含动态背景、动画效果。
- **游戏逻辑**：
  - **WASD** 控制蛇的移动方向。
  - 吃到食物后蛇身增长，得分增加。
  - 碰撞检测（边界碰撞和自身碰撞）。
- **跨平台支持**：Windows & Linux。

---

## 技术栈
- **编程语言**：C++ 17
- **图形库**：SFML 2.6.X (64 位)
- **构建系统**：CMake 3.10+
- **编译器**：
  - **Windows**：MinGW-w64 13.1 (64 位)
  - **Linux**：GCC 9+ (推荐)

> **重要提示**：Windows环境下，SFML 必须使用 2.6.X 版本，MinGW-w64 必须是 13.1 (64位) 版本，并且两者都需要添加到环境变量中。

---

##  项目结构
```sh
├── assets/             # 游戏资源文件（图片等）
├── include/            # 头文件
│   ├── Snake.h         # 蛇类定义
│   ├── game.h          # 游戏常量和枚举定义
│   ├── game_utils.h    # 游戏工具函数声明
│   └── resource.h      # 资源管理类定义
├── src/                # 源代码文件
│   ├── Snake.cpp       # 蛇类实现
│   ├── game_utils.cpp  # 游戏工具函数实现
│   └── main.cpp        # 主程序入口
├── CMakeLists.txt      # CMake 构建配置文件
├── Snake.zip           # 打包好的可执行文件（Windows）
└── README.md           # 本文件
```

---

##  安装与运行

### Windows（已提供可执行文件）
如果你只想直接玩游戏，无需配置环境：
1. 下载仓库中的 `Snake.zip` 文件
2. 解压到任意位置
3. 双击运行 `SnakeGame.exe`

### Windows 源代码编译

#### 前置要求
- CMake 3.10+
- MinGW-w64 13.1 (64 位)
- SFML 2.6.X (64 位)

> **下载说明**：
> - 官方下载地址：[SFML 2.6.2 官方网站](https://www.sfml-dev.org/download/sfml/2.6.2/index.php)
> - 确保 MinGW 和 SFML 都已添加到系统环境变量中

#### 编译步骤
```sh
# 创建并进入构建目录
mkdir build && cd build

# 生成构建文件
cmake -G "MinGW Makefiles" ..

# 编译
mingw32-make

# 运行
./SnakeGame
```

### 🐧 Linux 下编译

#### 前置要求
- CMake 3.10+
- GCC 9+ (推荐)
- SFML 2.6.X 开发库

#### 编译步骤
```sh
# 安装依赖（Ubuntu/Debian 示例）
sudo apt-get install libsfml-dev cmake g++

# 创建并进入构建目录
mkdir build && cd build

# 生成构建文件
cmake ..

# 编译
make

# 运行
./SnakeGame
```

---
