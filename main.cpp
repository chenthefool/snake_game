#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>

const int N = 10;    // 行数
const int M = 10;    // 列数
const int L = 20;    // 像素点大小
const int T = 400;   // 间隔

// 方向枚举
enum Direction
{
	RIGHT = 77,
	LEFT = 75,
	DOWN = 80,
	UP = 72,
};

// 点结构体
struct Point
{
	int x, y;
	int X() { return x * L; }
	int Y() { return y * L; }
};

// 蛇结构体
struct Snake
{
	int num;
	Point body[N * M];
	Direction dir;
	Point end;
} snake;

// 食物结构体
struct Food
{
	Point pos;
} food;

// 功能函数
void InitSnake();     // 初始化小蛇
void InitFood();      // 初始化食物
void DrawBoard();     // 绘制网格
void DrawSnake();     // 绘制小蛇
void DrawFood();      // 绘制食物
void DrawAll();       // 全部绘制
void SnakeMove();     // 小蛇移动
void EatFood();       // 吃到食物
bool SnakeDie();      // 小蛇撞死
void KeyDown();       // 按键处理


// ---------------------------主函数---------------------------
void main()
{
	while (true) {
		HWND win = initgraph(M * L, N * L);
		InitSnake();
		InitFood();
		while (true) {
			if (_kbhit()) {
				KeyDown();
			}
			SnakeMove();
			if (SnakeDie()) {
				MessageBox(GetHWnd(), L"Game Over!", L"rePlay", MB_OK);
				break;
			}
			EatFood();
			DrawAll();
			Sleep(T);
		}
	}
	closegraph();
}


void InitSnake()
{
	snake.num = 3;
	snake.dir = RIGHT;
	snake.body[2].x = 1;
	snake.body[2].y = 1;
	snake.body[1].x = 2;
	snake.body[1].y = 1;
	snake.body[0].x = 3;
	snake.body[0].y = 1;
}

void InitFood()
{
	bool flag;
	do {
		flag = false;
		food.pos.x = rand() % (M - 2) + 1;
		food.pos.y = rand() % (N - 2) + 1;
		// 如果食物在蛇身上则重新投放
		for (int i = 0; i < snake.num; i++) {
			if (snake.body[i].x == food.pos.x && snake.body[i].y == food.pos.y) {
				flag = true;
				break;
			}
		}
	} while (flag);
}

void DrawBoard()
{
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			setcolor(BLUE);
			setfillcolor(BLACK);
			fillrectangle(i * L, j * L, i * L + L, j * L + L);
		}
	}
	for (int i = 0; i < N; i++) {
		setcolor(BLUE);
		setfillcolor(RGB(0, 0, 139));
		fillrectangle(0, i * L, L, i * L + L);
		fillrectangle((M - 1) * L, i * L, M * L, i * L + L);
	}
	for (int i = 0; i < M; i++) {
		setcolor(BLUE);
		setfillcolor(RGB(0, 0, 139));
		fillrectangle(i * L, 0, i * L + L, L);
		fillrectangle(i * L, (N - 1) * L, i * L + L, N * L);
	}
}

void DrawSnake()
{
	for (int i = 0; i < snake.num; i++) {
		setcolor(BLUE);
		setfillcolor(RGB(0, 100, 0));
		fillrectangle(snake.body[i].X(), snake.body[i].Y(), snake.body[i].X()+L, snake.body[i].Y()+L);
	}
}

void DrawFood()
{
	setcolor(BLUE);
	setfillcolor(RED);
	fillrectangle(food.pos.X(), food.pos.Y(), food.pos.X() + L, food.pos.Y() + L);
}

void DrawAll()
{
	DrawBoard();
	DrawFood();
	DrawSnake();
}

void SnakeMove()
{	
	// 记录蛇尾
	snake.end = snake.body[snake.num - 1];

	// 更新蛇身
	for (int i = snake.num - 1; i > 0; i--) {
		snake.body[i].x = snake.body[i - 1].x;
		snake.body[i].y = snake.body[i - 1].y;
	}
	// 更新蛇头
	switch (snake.dir) {
	case RIGHT:
		snake.body[0].x++;
		break;
	case LEFT:
		snake.body[0].x--;
		break;
	case DOWN:
		snake.body[0].y++;
		break;
	case UP:
		snake.body[0].y--;
		break;
	}
}

void EatFood()
{
	int x = snake.body[0].x;
	int y = snake.body[0].y;
	if (food.pos.x == x && food.pos.y == y) {
		// 食物重新投放
		bool flag;
		do {
			flag = false;
			food.pos.x = rand() % (M - 2) + 1;
			food.pos.y = rand() % (N - 2) + 1;
			for (int i = 0; i < snake.num; i++) {
				int x = snake.body[i].x;
				int y = snake.body[i].y;
				if (x == food.pos.x && y == food.pos.y) {
					flag = true;
					break;
				}
			}
		} while (flag);
		// 蛇身增长
		snake.body[snake.num].x = snake.end.x;
		snake.body[snake.num].y = snake.end.y;
		snake.num++;
	} 
}

bool SnakeDie()
{
	int x = snake.body[0].x;
	int y = snake.body[0].y;
	// 超出边框
	if (x >= (M - 1) || y >= (N - 1) || x <= 0 || y <= 0) {
		return true;
	}
	// 吃到自身
	for (int i = 1; i < snake.num; i++) {
		if (x == snake.body[i].x && y == snake.body[i].y) {
			return true;
		}
	}
	return false;
}

void KeyDown()
{
	int ch1, ch2;
	ch1 = _getch();
	if (ch1 == 224) {
		ch2 = _getch();
		switch (ch2) {
		case LEFT:
			if (snake.dir != RIGHT) {
				snake.dir = LEFT;
			}
			break;
		case RIGHT:
			if (snake.dir != LEFT) {
				snake.dir = RIGHT;
			}
			break;
		case UP:
			if (snake.dir != DOWN) {
				snake.dir = UP;
			}
			break;
		case DOWN:
			if (snake.dir != UP) {
				snake.dir = DOWN;
			}
			break;
		}
	}	
}












