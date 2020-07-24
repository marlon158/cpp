#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#define BOARD_HEIGHT 8	//�{�[�h�̍���
#define BOARD_WIDTH 8	//�{�[�h�̕�

enum {
	CHECK_TOP,
	CHECK_TOP_RIGHT,
	CHECK_RIGHT,
	CHECK_LOWER_RIGHT,
	CHECK_LOWER,
	CHECK_LOWER_LEFT,
	CHECK_LEFT,
	CHECK_TOP_LEFT,
	CHECK_MAX,
};

char board[BOARD_HEIGHT][BOARD_WIDTH];			//�{�[�h�̏�����
char dispBoard[BOARD_HEIGHT][BOARD_WIDTH];		//�\���p�̃{�[�h
char testBoardA[BOARD_HEIGHT][BOARD_WIDTH] = {	//�e�X�g�p�{�[�h
	{0, 0, 2, 0, 0, 0, 0, 2},
	{0, 0, 2, 0, 0, 1, 2, 2},
	{0, 0, 0, 1, 2, 1, 2, 2},
	{0, 0, 2, 1, 2, 1, 2, 2},
	{0, 2, 2, 1, 2, 1, 2, 2},
	{2, 2, 2, 1, 2, 1, 2, 2},
	{2, 2, 2, 1, 2, 1, 2, 0},
	{2, 2, 1, 0, 0, 0, 0, 0},
};

//�S�����̍��W
char checkPoint[CHECK_MAX][2] = {
	{-1, 0},	// CHECK_TOP,
	{-1, 1},	// CHECK_TOP_RIGHT,
	{0, 1},		// CHECK_RIGHT,
	{1, 1},		// CHECK_LOWER_RIGHT,
	{1, 0},		// CHECK_LOWER,
	{1, -1},	// CHECK_LOWER_LEFT,
	{0, -1},	// CHECK_LEFT,
	{-1, -1},	// CHECK_TOP_LEFT,
};

int pointerHeight = 3;		//�|�C���^�̏�������
int pointerWidth = 2;		//�|�C���^�̏�����
int userTurn = 1;			//1 -> �� / 2 -> ��
int flg = 0;				//�����Ŏg���t���O(����)
int whiteStone, blackStone;	//�{�[�h��̐����m�F���邽�߂̕ϐ�

//�{�[�h�̌��ݏ󋵂�\������
void display() {
	memcpy(dispBoard, board, sizeof(board));	//�X�V
	system("cls");								//�O�񕪍폜
	dispBoard[pointerHeight][pointerWidth] = 3;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			switch (dispBoard[i][j]){
			case 0:	printf("�E");	break;	//��
			case 1:	printf("��");	break;	//��
			case 2:	printf("�Z");	break;	//��
			case 3: printf("��");	break;	//�|�C���^
			}
			printf("�@");
		}
		printf("\n\n");
	}
	printf("Player -> ");
	switch (userTurn){
	case 1:	printf("White/��");	break;
	case 2:	printf("Black/�Z");	break;
	}
	printf("\nWhite:[%d] -- Black:[%d]", whiteStone, blackStone);
}

//�΂�u���邩�ǂ���
bool putStone(int _pointerHeight, int _pointerWidth) {
	if (board[_pointerHeight][_pointerWidth] == 0) {
		switch (userTurn) {
		case 1:	board[_pointerHeight][_pointerWidth] = 1;	break;
		case 2:	board[_pointerHeight][_pointerWidth] = 2;	break;
		}
		userTurn = (userTurn + 2) % 2 + 1;	//�^�[���̌��
		return true;
	}
	return false;
}

//�{�[�h��ɂ����ӎv������̂�
void checkStone() {
	whiteStone = 0;
	blackStone = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int j = 0; j < BOARD_WIDTH; j++) {
			switch (board[i][j]){
			case 1:	whiteStone++;	break;
			case 2:	blackStone++;	break;
			}

		}
}

//���܂ꂽ�Ƃ��̏���
void changeStone(int _pointHeight, int _pointWidth) {
	//Sleep(1000); //�e�X�g�p�ɂ�����~
	int _putUser = (userTurn + 2) % 2 + 1;	//���ۂɒu�����F
	for (int i = 0; i < CHECK_MAX; i++) {
		//��̍��W���玞�v���Ŋm�F
		int _x = _pointHeight + checkPoint[i][0];
		int _y = _pointWidth + checkPoint[i][1];
		int _enemy = userTurn;
		while (1){
			if (BOARD_WIDTH <= _y || _y <= 0)	//�����������{�[�h��蒴�����Ƃ��ɂ������Ȃ��ƂɂȂ����̂�
				break;
			if (board[_x][_y] == _enemy){
				switch (i) {
				case 0:	_x--;	break;
				case 1: _x--;	_y++;	break;
				case 2: _y++;	break;
				case 3: _x++;	_y++;	break;
				case 4: _x++;	break;
				case 5: _x++;	_y--;	break;
				case 6: _y--;	break;
				case 7:	_x--;	_y--;	break;
				}
				if (board[_x][_y] == _putUser) {
					int _x2 = _x;
					int _y2 = _y;
					switch (i) {
					case 0:
						for (int x = _x2 + 1; x < _pointHeight; x++) {
							board[x][_pointWidth] = _putUser;
						}
						break;
					case 1:
						for (int x = _x2 + 1; x < _pointHeight; x++) {
							for (int y = _y2 - 1; _pointWidth < y; y--) {
								board[x][y] = _putUser;
								_y2--;
								break;
							}
						}
						break;
					case 2:
						for (int y = _y2 - 1; _pointWidth < y; y--) {
							board[_pointHeight][y] = _putUser;
						}
						break;
					case 3:
						for (int x = _x2 - 1; _pointHeight < x; x--) {
							for (int y = _y2 - 1; _pointWidth < y; y--) {
								board[x][y] = _putUser;
								_y2--;
								break;
							}
						}
						break;
					case 4:
						for (int x = _x2 - 1; _pointHeight < x; x--) {
							board[x][_pointWidth] = _putUser;
						}
						break;
					case 5:
						for (int x = _x2 - 1; _pointHeight < x; x--) {
							for (int y = _y2 + 1; y < _pointWidth; y++) {
								board[x][y] = _putUser;
								_y2++;
								break;
							}
						}
						break;
					case 6:
						for (int y = _y2 + 1; y < _pointWidth; y++) {
							board[_pointHeight][y] = _putUser;
						}
						break;
					case 7:
						for (int x = _x2 + 1; x < _pointHeight; x++) {
							for (int y = _y2 + 1; y < _pointWidth; y++) {
								board[x][y] = _putUser;
								_y2++;
								break;
							}
						}
						break;
					}
				}
			}
			else if (board[_x][_y] == 0) {
				break;
			}
			else {
				break;
			}
		}
	}
}

bool isHit(int _pointerHeight, int _pointerWidth) {
	if (_pointerHeight < -1 || BOARD_HEIGHT + 1 <= _pointerHeight ||
		_pointerWidth < -1 || BOARD_WIDTH + 1 <= _pointerWidth)
		return true;
	return false;
}

bool checkPut(int _heighCheck, int _widthCheck) {
	for (int i = 0; i < CHECK_MAX; i++) {
		int _x = _heighCheck + checkPoint[i][0];
		int _y = _widthCheck + checkPoint[i][1];
		int _enemy = (userTurn + 2) % 2 + 1;
		while (1) {
			if (BOARD_WIDTH <= _y || _y <= 0)
				break;
			if (board[_x][_y] == _enemy) {
				switch (i) {
				case 0:	_x--;	break;
				case 1: _x--;	_y++;	break;
				case 2: _y++;	break;
				case 3: _x++;	_y++;	break;
				case 4: _x++;	break;
				case 5: _x++;	_y--;	break;
				case 6: _y--;	break;
				case 7:	_x--;	_y--;	break;
				}
				if (board[_x][_y] == userTurn)
					return true;
			}
			else {
				break;
			}
		}
	}
	return false;
}

bool checkBoard() {
	for (int i = 0; i < BOARD_HEIGHT; i++)
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (board[i][j] == 0)
				return true;
			if (checkPut(i, j))
				return true;
		}
	return false;
}

int passAction(int pointer) {
	switch (pointer){
	case -1:
		return 7;
	case 8:
		return 0;
	default:
		return pointer;
	}
}

int main() {
	memset(board, 0, sizeof(board));
	//�{�[�h�쐬
	/*for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if ((i == 3 || i == 4) && i == j)
				board[i][j] = 1;	//WhiteStone
			else if ((i == 3 && j == 4) || (i == 4 && j == 3))
				board[i][j] = 2;	//BlackStone
		}
	}*/
	//�e�X�g���p�̃{�[�h
	memcpy(board, testBoardA, sizeof(testBoardA));
	while (1){
		checkStone();
		flg = 0;
		if (_kbhit()) {
			switch (_getch()){
			case 'w':
				if(!isHit(pointerHeight - 1, pointerWidth))
					pointerHeight--;
					pointerHeight = passAction(pointerHeight);
				break;
			case 'd':
				if (!isHit(pointerHeight, pointerWidth + 1))
					pointerWidth++;
					pointerWidth = passAction(pointerWidth);
				break;
			case 's':
				if (!isHit(pointerHeight + 1, pointerWidth))
					pointerHeight++;
					pointerHeight = passAction(pointerHeight);
				break;
			case 'a':
				if (!isHit(pointerHeight, pointerWidth - 1))
					pointerWidth--;
					pointerWidth = passAction(pointerWidth);
				break;
			case 0x20:
				if (checkPut(pointerHeight, pointerWidth) == true)
					if (putStone(pointerHeight, pointerWidth) == true)
						changeStone(pointerHeight, pointerWidth);
						//continue;
			}
		}
		display();
	}
	return 0;
}