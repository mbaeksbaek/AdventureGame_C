#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#pragma warning(disable:4996)
#define SIZE_X 100
#define SIZE_Y 100
#define MAX_NAME 128
#define START_FOOD 50
#define START_HEALTH 100
#define START_LUCK 0.01

struct explorer {
	char name[MAX_NAME];
	int score;
	int gold;
	int food;
	int health;
	float luck;
}user;



void initField();	// Field �� �ʱ�ȭ ��Ű�� �Լ�
void displayField();	// Field ��ü�� display ���ִ� �Լ�
void displayUserField(int current_X, int current_Y);	// User�� ��ȸ�ϴ� Field �� display �ϴ� �Լ�
int Dice();	// Ȯ�� �۾��� ���� Dice �ζ��� �Լ�
void getName();	// ���� �Է�(�̸�)�� �޴� �Լ� < �ζ��� �Լ��� ��� >
void initUser();	// ���� �ʱ�ȭ �۾��� �������ִ� �Լ�
void initPoint();

int checkMovement(int d_x, int d_y);	// �������� ���� ���θ� Ȯ������ �Լ�
void movement(char move);	// �������� �˻��ϴ� �Լ�
void initGame();	// ������ �����ϴ� �Լ�
void updateScore();	// ������ ������Ʈ�ϴ� �Լ�
void updateField();	// ��峪 ���̾Ƹ�带 ȹ���Ͽ��� �� �ʵ带 ������Ʈ �����ִ� �Լ�

int isTownAble(char move);	// ������ �����ϴ� ������ �����ϴ��� Ȯ�����ִ� �Լ�
void TownDisplay();	// ������ �����Ͽ��� �� �����ִ� ȭ��
void EnterTown();

int isDungeonAble(char move);	// ������ �����ϴ� ������ �����ϴ��� Ȯ�����ִ� �Լ�
void DungeonDisplay();	// ������ �����Ͽ��� �� �����ִ� ȭ��
void EnterDungeon();	// ������ ������ �� ȣ��� �Լ�

void Monster();	// ���� ������ ���ִ� �Լ�

char field[SIZE_Y][SIZE_X];
char name[MAX_NAME];
int current_X;
int current_Y;
int score;



int main(void) {
	initGame();
	
	return 0;
}




void initField() {
	srand(time(NULL));
	int i, j;
	int dice;
	for (i = 0; i < SIZE_Y; i++) {
		for (j = 0; j < SIZE_X; j++) {
			dice = Dice();
			if (dice <= 80) field[i][j] = '_';
			else if (dice <= 95) field[i][j] = 'T';
			else if (dice <= 97) field[i][j] = '+';
			else if (dice <= 98) field[i][j] = '*';
			else if (dice <= 99) field[i][j] = 'i';
			else if (dice == 100) field[i][j] = '#';

			else {
				printf("[ERROR] �߸��� Ȯ�� ����\n");
			}
		}
	}
}

int Dice() {
	return rand() % 100 + 1;
}

void displayField() {
	int i, j;
	for (i = 0; i < SIZE_Y; i++) {
		for (j = 0; j < SIZE_X; j++) {
			printf("%c", field[i][j]);
		}
		printf("\n");
	}
}

void displayUserField(int current_X, int current_Y) {
	int i, j;
	printf("Name : %s\tScore : %d\tGold : %d\tFood : %d\tHealth : %d\tLuck : %.2f\n<Point> X : %d, Y : %d\n", user.name, user.score, user.gold, user.food, user.health, user.luck, current_X, current_Y);
	for (i = current_Y - 10; i < current_Y + 10; i++) {
		for (j = current_X - 10; j < current_X + 10; j++) {
			if (i == current_Y && j == current_X)
				printf("o");
			else if (i >= SIZE_Y || j >= SIZE_X || i < 0 || j < 0) printf(" ");

			else printf("%c", field[i][j]);
				
		}

		printf("\n");
	}
}

void getName() {
	printf("�Ƶ帮���� �ձ��� ���� ���� ȯ���մϴ� !\n");
	printf("�̸��� �Է����ּ��� : ");
	scanf("%s", user.name);
}

void initUser() {
	getName();
	user.score = 0;
	user.gold = 0;
	user.food = START_FOOD;
	user.health = START_HEALTH;
	user.luck = START_LUCK;
	
	initPoint();
}

void initPoint() {
	int i, j;
	i = Dice();
	j = Dice();

	while (field[i][j] != '_') {
		i = Dice();
		j = Dice();
	}

	current_X = j;
	current_Y = i;
}

int checkMovement(int d_x, int d_y) {
	if (field[current_Y][current_X + d_x] == 'T') {
		user.health -= 10;
		return 0;
	}

	if (field[current_Y + d_y][current_X] == 'T') {
		user.health -= 10;
		return 0;
	}

	if (field[current_Y + d_y][current_X] == 'M') {
		user.health -= 10;
		return 1;
	}

	if (field[current_Y][current_X + d_x] == 'M') {
		user.health -= 10;
		return 1;
	}

	if (current_X + d_x >= SIZE_X || current_X + d_x < 0) return 0;

	if (current_Y + d_y >= SIZE_Y || current_Y + d_y < 0) return 0;

	return 1;
}

void movement(char move) {
	// �빮�� �Է½� �ҹ��ڷ� ��ȯ�Ͽ� Movement �Է��� ����
	strlwr(&move);
	switch (move) {
	case 'w': {
		if (checkMovement(0, -1)) {
			user.score++;
			current_Y--;
		}
		else {
			printf("Cannot Move to That Way !\n");
			getch();
		}
		user.food--;
		break;
	}
	case 'a': {
		if (checkMovement(-1, 0)) {
			user.score++;
			current_X--;
		}
		else {
			printf("Cannot Move to That Way !\n");
			getch();
		}
		user.food--;
		break;
	}
	case 's': {
		if (checkMovement(0, +1)) {
			user.score++;
			current_Y++;
		}
		else {
			printf("Cannot Move to That Way !\n");
			getch();
		}
		user.food--;
		break;
	}
	case 'd': {
		if (checkMovement(+1, 0)) {
			user.score++;
			current_X++;
		}
		else {
			printf("Cannot Move to That Way !\n");
			getch();
		}
		user.food--;
		break;
	}
	default:
		break;
	}
}

void updateScore() {
	user.score += user.gold * 2;
}

void updateField() {
	if (field[current_Y][current_X] == '+') {
		user.gold += 10;
		field[current_Y][current_X] = '_';
		user.score += 5;
	}

	else if (field[current_Y][current_X] == '*') {
		user.gold += 50;
		field[current_Y][current_X] = '_';
		user.score += 20;
	}

	Monster();
}

int isTownAble(char move) {
	if (move == 'e' && field[current_Y][current_X] == '#') return 1;

	return 0;
}

void TownDisplay() {
	printf("Name : %s\tScore : %d\tGold : %d\tFood : %d\tHealth : %d\tLuck : %.2f\n<Point> X : %d, Y : %d\n", user.name, user.score, user.gold, user.food, user.health, user.luck, current_X, current_Y);
	printf("==============================\n");
	printf("������ ���� ���� ȯ���մϴ� !\n");
	printf("���������� ������ ��ų� ü���� ȸ���� �� ������ ����� ������ �� �ֽ��ϴ� !\n");
	printf("==============================\n");
	printf("�޴� ���\n");
	printf("1. Food ���� ( 1 Gold �� 2 Food )\n");
	printf("2. Health ȸ�� ( 1 Gold �� 5 Health )\n");
	printf("3. Luck ����\ ( 100 Gold �� Luck 2�� )\n");
	printf("4 : Exit\n");
	printf("�޴� �Է� :\n");

}

void EnterTown() {
	while (1) {
		system("cls");
		TownDisplay();
		char input;
		input = getch();

		switch (input) {
		case '1': {
			if (user.gold < 1) {
				printf("��� ���� !\n");
				break;
			}
			else {
				if (user.food <= 98) {
					user.gold--;
					user.food += 2;
					break;
				}
				else break;
			}
		}

		case '2': {
			if (user.gold < 1) {
				printf("��� ���� !\n");
				break;
			}
			else {
				if (user.health <= 95) {
					user.gold--;
					user.health += 5;
					break;
				}
				else break;

			}

		}

		case '3': {
			if (user.gold < 100) {
				printf("��� ���� !\n");
				break;
			}
			else {
				if (user.luck <= 0.5) {
					user.luck *= 2;
					break;
				}
				else break;
			}
		}

		case '4': {
			return;
		}

		case 'x': {
			return;
		}

		default: {
			printf("Invalid Menu\n");
			printf("Please Try Again\n");
			break;
		}

		}
	}
}

int isDungeonAble(char move) {
	if (move == 'e' && field[current_Y][current_X] == 'i' && user.luck >= 0.08) return 1;

	else {
		printf("������ �����Ͻ� �� �����ϴ� !\n");
		printf("ü���� ���Դϴ� !\n");
		getch();
		user.health -= 30;
	}
	return 0;
}

void DungeonDisplay() {
	printf("Name : %s\tScore : %d\tGold : %d\tFood : %d\tHealth : %d\tLuck : %.2f\n<Point> X : %d, Y : %d\n", user.name, user.score, user.gold, user.food, user.health, user.luck, current_X, current_Y);
	printf("==============================\n");
	printf("������ ���� ���� ȯ���մϴ� !\n");
	printf("���������� ������ �� ���� ���� ������ ȹ���Ͻ� �� �ֽ��ϴ� !\n");
	printf("==============================\n");
	printf("�����Ͻðڽ��ϱ� ? (Y / N) :\n");
}

void EnterDungeon() {
	while (1) {
		system("cls");
		DungeonDisplay();
		char input;
		input = getch();

		if (input == 'Y' || input == 'y') {
			user.gold += user.luck * 1000;
			user.score += user.luck * 2000;
			field[current_Y][current_X] = '!';
			return;
		}

		else if (input == 'N' || input == 'n') {
			return;
		}
	}
}

void Monster() {
	srand(time(NULL));
	int Mon_X;
	int Mon_Y;
	int tmp;
	int dice = Dice();
	// - 10 ~ 10 
	if (dice <= 2) {
		// -9 ~ 9
		Mon_X = Dice() % 19 - 9;
		Mon_Y = Dice() % 19 - 9;

		Mon_X += current_X;
		Mon_Y += current_Y;

		for (int i = Mon_X - 1; i <= Mon_X + 1; i++) {
			for (int j = Mon_Y - 1; j <= Mon_Y + 1; j++) {
				field[i][j] = 'M';
			}
		}

	}
	return;
}

void initGame() {
	char move;
	score = 0;
	initField();
	initUser();
	//displayField();
	system("cls");
	while (1) {
		if (user.food <= 0) break;
		else if (user.health <= 0) break;
		updateField();
		system("cls");
		displayUserField(current_X, current_Y);
		printf("Input Move : ");
		move = getch();
		if (move == 'x') break;
		
		if (move == 'e') {
			if (isTownAble(move)) EnterTown();

			else if (isDungeonAble(move)) EnterDungeon();
		}
		movement(move);
	}
	// END GAME
	system("cls");
	updateScore();
	printf("==============================\n");
	printf("Game Over\n");
	printf("Name : %s, Score : %d\n", user.name, user.score);
	printf("==============================\n");

	return;
}