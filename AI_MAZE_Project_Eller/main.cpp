#include<iostream>
#include<vector>
#include<random>

using namespace std;
struct tile {
	int group;
	int type;
};

struct Open {
	int last_x;
	int down;
	Open(int a, int b) : last_x(a), down(b) {}
};

vector<Open> open;
int _size = 21;
tile board[21][21];
int groups = 0;

void Init();
void Set_Row(int y);
bool Check();
bool Group_open(int y, int x);
int Group_lastx(int y, int x);
void Setting_lastx(int y, int x, int a);
void Setting_open(int y, int x, int a);

void Init() {
	for (int y = 0; y < _size; y++) {
		for (int x = 0; x < _size; x++) {
			if (x % 2 == 0 || y % 2 == 0)
				board[y][x].type = 0;
			else {
				board[y][x].type = 1;
				board[y][x].group = -1;
			}
		}
	}
}
void Set_Row(int y) {
	if (board[y][1].group == -1) {
		board[y][1].group = groups++;
		open.push_back(Open(1,0));
	}
	for (int x = 1; x < _size; x+=2) {
		if (board[y][x + 2].group != board[y][x].group && x + 2 <= _size - 2) {
			if (Check()) {
				board[y][x + 1].type = 1;
				if (board[y][x + 2].group != -1) {
					board[y][x].group = board[y][x + 2].group;
					board[y][x + 1].group = board[y][x + 2].group;
					Setting_lastx(y, x + 2, x + 2);
				}
				else {
					board[y][x + 1].group = board[y][x].group;
					board[y][x + 2].group = board[y][x].group;
					Setting_lastx(y, x, x + 2);
				}
			}
			else if(board[y][x+2].group == -1){
				board[y][x + 2].group = groups++;
				open.push_back(Open(x + 2, 0));
			}
		}
		Setting_open(y, x, 0);
	}
}

void Set_Col(int y) {
	for (int x = 1; x < _size; x+=2) {
		if (y + 2 < _size - 1) {
			if (Check()) {
				board[y + 1][x].type = 1;
				board[y + 1][x].group = board[y][x].group;
				board[y + 2][x].group = board[y][x].group;

				open[board[y][x].group].down++;
			}
			else if (Group_open(y, x) == 0 && Group_lastx(y, x) == x) {
				board[y + 1][x].type = 1;
				board[y + 1][x].group = board[y][x].group;
				board[y + 2][x].group = board[y][x].group;
			}
		}
	}
}
void Last() {
	for (int x = 1; x < _size-2; x+=2) {
		if (board[_size - 2][x].group != board[_size - 2][x + 2].group) {
			board[_size - 2][x + 1].type = 1;
		}
	}
}
void println() {
	for (int y = 0; y < _size; y++) {
		for (int x = 0; x < _size; x++) {
			if (board[y][x].type == 1)
				cout << "  ";
			else
				cout << "бс";
		}
		cout << endl;
	}
}
bool Group_open(int y, int x) {
	return open[board[y][x].group].down;
}
int Group_lastx(int y, int x) {
	return open[board[y][x].group].last_x;
}
void Setting_lastx(int y, int x, int a) {
	open[board[y][x].group].last_x = a;
}
void Setting_open(int y, int x, int a) {
	for (int i = 0; i < open.size(); i++) {
		open[i].down = 0;
	}
}
bool Check() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 1);
	return dis(gen);
}

int main() {
	Init();
	
	for (int i = 1; i < _size-1; i+=2) {
		Set_Row(i);
		Set_Col(i);
	}
	Last();
	println();
}