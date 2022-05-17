#include "Board.h"
#include <random>

using namespace std;

Board::Board(int size)
	: _size(size)
	, _board(new Node* [size] {})
{
	if (_size % 2 == 0)
		exit(1);
	for (int i = 0; i < size; i++) {
		_board[i] = new Node[size];
	}
	visit = new int*[_size];
	for (int i = 0; i < _size; i++) {
		visit[i] = new int[_size] {0};
	}
	GenerateByBinaryTree();
	ScreenInit();
}
Board::~Board()
{
	for (int i = 0; i < _size; i++) {
		delete _board[i];
	}
	delete _board;
	for (int i = 0; i < _size; i++) {
		delete visit[i];
	}
	delete visit;
	ScreenRelease();
	cout << str << endl;
}
void Board::GenerateByBinaryTree()
{
	for (int y = 0; y < _size; y++) {
		for (int x = 0; x < _size; x++) {
			if (x % 2 == 0 || y % 2 == 0) {
				_board[y][x].type = TileType::Wall;
			}
			else {
				_board[y][x].type = TileType::Empty;
				_board[y][x].came = false;
			}
		}
	}
	RecursiveBackTraing(1, 1);
	_board[_size - 2][_size - 2].type = TileType::GOAL;
}

void Board::shuffleArray(int* array, int size) {
	int i, r, temp;
	for (i = 0; i < (size - 1); ++i)
	{
		r = i + (rand(0, 4) % (size - i));
		temp = array[i];
		array[i] = array[r];
		array[r] = temp;
	}
}
void Board::RecursiveBackTraing(int y, int x) {
	int r, nx, ny;
	int directions[4] = {
			Direction::UP,
			Direction::DOWN,
			Direction::LEFT,
			Direction::RIGHT
	};
	_board[y][x].came = true;
	shuffleArray(directions, 4);
	for (r = 0; r < 4; r++) {
		nx = x + DIR[directions[r]][0];
		ny = y + DIR[directions[r]][1];
		if (inRange(ny, nx) && (_board[ny][nx].type == TileType::Empty) && !_board[ny][nx].came) {
			RecursiveBackTraing(ny, nx);
			if (ny != y) {
				_board[(ny + y) / 2][nx].type = TileType::Empty;
			}
			else {
				_board[ny][(nx + x) / 2].type = TileType::Empty;
			}
		}
	}
}
int Board::inRange(int y, int x)
{
	return (y > 0 && y < _size - 1) && (x > 0 && x < _size - 1);
}
int Board::rand(int a, int b) {
	std::random_device rd;
	// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
	std::mt19937 gen(rd());
	// 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
	std::uniform_int_distribution<int> dis(a, b);
	return dis(gen);
}
void Board::BFS(int a, int b) {
	queue<pair<int, int>> q;
	q.push(make_pair(a, b));
	while (!q.empty()) { // ť�� ���� ������� ��� �ݺ� ����  // ť�� ���� �ִ�. => ���� �湮���� ���� ��尡 ���� �Ѵ�. 
		int x = q.front().second;
		int y = q.front().first;
		q.pop();
		for (int i = 0; i < 4; i++) {
			int next_y = y + dy[i];
			int next_x = x + dx[i];
			// ���� �̵��� ������ Queue�� �ִ´�.
			if (next_x > 0 && next_y > 0 && next_x < _size-1 && next_y < _size-1 
				&& _board[next_y][next_x].type == TileType::Empty 
				&& visit[next_y][next_x] == 0) {
				visit[next_y][next_x] = 1;
				q.push(make_pair(next_y, next_x));
			}
			if (_board[next_y][next_x].type == TileType::GOAL) {
				visit[next_y][next_x] = 1;
				Render();
				return;
			}
		}
		//Setting(x, y);
		Render();
		this_thread::sleep_for(0.025s);
	}
}

void Board::ScreenInit()
{
	CONSOLE_CURSOR_INFO cci;

	//ȭ�� ���� 2���� �����.
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	//Ŀ���� �����.
	cci.dwSize = 1;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

void Board::ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
	g_nScreenIndex = !g_nScreenIndex;
}

void Board::ScreenClear()
{
	for (int i = 0; i < _size; i++) {
		COORD Coor = { i, 0 };
		DWORD dw;
		FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', 80 * 25, Coor, &dw);
	}
}

void Board::ScreenRelease()
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);
}

void Board::ScreenPrint(int x, int y, std::string s)
{
	DWORD dw;
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
	WriteFile(g_hScreen[g_nScreenIndex], s.c_str(), s.size(), &dw, NULL);
}

void Board::Render()
{
	ScreenClear();
	str = "";
	for (int y = 0; y < _size; y++) {
		for (int x = 0; x < _size; x++) {
			if (_board[y][x].type == TileType::Wall) {//���϶�                                           //�� ���� ��� ��
				str += "��";
			}
			else { //�̵����ɰ����� ��
				if (visit[y][x] == 1)
					str += "��";
				else
					str += "  ";
			}
		}
		str += "\n";
	}
	ScreenPrint(0, 0, str);
	ScreenFlipping();
}

//void Board::Setting(int a, int b)
//{
//	for (int y = 0; y < _size; y++) {
//		for (int x = 0; x < _size; x++) {
//			if (_board[y][x].tipe == TileVisible::Visible) {//������ �̺�Ʈ ��, �þ߿� ���Դ� ����(������ ����)��
//				if (_board[y][x].type == TileType::Empty)//������� ��쿡��
//					_board[y][x].tipe = TileVisible::noVisible;//1���������� ����
//				else                                      //���� ��쿡��
//					_board[y][x].tipe = TileVisible::nonVisible;//2���������κ���
//															//(�������� ������ ���̶� ���̶� ����)
//			}
//		}
//	}
//	double c, d;
//	for (int i = 0; i < 360; i++) {
//		c = (double)a + 0.5, d = (double)b + 0.5;
//		while (true) {
//			c += sin(i) / 100;
//			d += cos(i) / 100;
//			_board[(int)c][(int)d].tipe = TileVisible::Visible;
//			if (_board[(int)c][(int)d].type == TileType::Wall) {
//				_board[(int)c][(int)d].tipe = TileVisible::Visible;
//				break;
//			}
//		}//���� ��ġ �������� 360���� 1���� ���ư��鼭 ����, 0.1ĭ�� ������ �����ϸ鼭
//		//������ ��� ������ �� �� �ִ� ����(Visible)�� �����ϰ�
//		//���� ������ ���� visible�� �ٲٸ鼭 ��������
//		//(360�� �ݺ�)
//	}
//}