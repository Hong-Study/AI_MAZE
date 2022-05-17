#pragma once
#include <iostream>
#include <random>
#include <time.h>
#include <queue>
#include <thread>
#include <Windows.h>
#include <vector>

class Board {
private:
	int _size;
	std::string str;
	int **visit;
	int g_nScreenIndex;
	HANDLE g_hScreen[2];
public:
	Board() = delete;
	Board(int size);
	~Board();

	//void Setting(int a, int b);
	void BFS(int a, int b);
private:
	enum class TileType {
		Empty,
		Wall,
		GOAL
	};//타일을 공간, 벽, 유저 3개로 나눔

	enum class sight {
		sight = false
	};
	enum Direction {
		LEFT,
		RIGHT,
		UP,
		DOWN//움직임 감지
	};
	struct Node {
		TileType type;
		bool came;// 타일타입, 캠
	};
	const int DIR[4][2] = { {0,-2},{0,+2},{-2,0},{+2,0} };
	const int dx[4] = { 0, 0, 1, -1 }; // x 축 이동 (열 이동)
	const int dy[4] = { 1, -1, 0, 0 }; // y 축 이동 (행 이동)
	Node** _board;// 이동공간(?)

	void GenerateByBinaryTree();	// 초기 맵 설정
	void RecursiveBackTraing(int start_X, int start_Y);		// Maze Algorithm
	void shuffleArray(int* array, int size);
	int inRange(int y, int x);

	void ScreenInit();
	void ScreenFlipping();
	void ScreenClear();
	void ScreenRelease();
	void ScreenPrint(int x, int y, std::string s);
	void Render();
	int rand(int a, int b);
};