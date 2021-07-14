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



void initField();	// Field 를 초기화 시키는 함수
void displayField();	// Field 전체를 display 해주는 함수
void displayUserField(int current_X, int current_Y);	// User가 조회하는 Field 를 display 하는 함수
int Dice();	// 확률 작업을 위한 Dice 인라인 함수
void getName();	// 유저 입력(이름)을 받는 함수 < 인라인 함수로 사용 >
void initUser();	// 유저 초기화 작업을 진행해주는 함수
void initPoint();

int checkMovement(int d_x, int d_y);	// 움직임의 가능 여부를 확인해줄 함수
void movement(char move);	// 움직임을 검사하는 함수
void initGame();	// 게임을 시작하는 함수
void updateScore();	// 점수를 업데이트하는 함수
void updateField();	// 골드나 다이아몬드를 획득하였을 때 필드를 업데이트 시켜주는 함수

int isTownAble(char move);	// 마을에 입장하는 조건을 만족하는지 확인해주는 함수
void TownDisplay();	// 마을에 도착하였을 때 보여주는 화면
void EnterTown();

int isDungeonAble(char move);	// 던전에 입장하는 조건을 만족하는지 확인해주는 함수
void DungeonDisplay();	// 던전에 도착하였을 때 보여주는 화면
void EnterDungeon();	// 던전에 입장할 때 호출될 함수

void Monster();	// 몬스터 생성을 해주는 함수

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
				printf("[ERROR] 잘못된 확률 변수\n");
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
	printf("아드리아의 왕국에 오신 것을 환영합니다 !\n");
	printf("이름을 입력해주세요 : ");
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
	// 대문자 입력시 소문자로 변환하여 Movement 입력을 진행
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
	printf("마을에 오신 것을 환영합니다 !\n");
	printf("마을에서는 음식을 사거나 체력을 회복할 수 있으며 행운을 구입할 수 있습니다 !\n");
	printf("==============================\n");
	printf("메뉴 목록\n");
	printf("1. Food 구입 ( 1 Gold 당 2 Food )\n");
	printf("2. Health 회복 ( 1 Gold 당 5 Health )\n");
	printf("3. Luck 구입\ ( 100 Gold 당 Luck 2배 )\n");
	printf("4 : Exit\n");
	printf("메뉴 입력 :\n");

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
				printf("골드 부족 !\n");
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
				printf("골드 부족 !\n");
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
				printf("골드 부족 !\n");
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
		printf("던전에 입장하실 수 없습니다 !\n");
		printf("체력이 깎입니다 !\n");
		getch();
		user.health -= 30;
	}
	return 0;
}

void DungeonDisplay() {
	printf("Name : %s\tScore : %d\tGold : %d\tFood : %d\tHealth : %d\tLuck : %.2f\n<Point> X : %d, Y : %d\n", user.name, user.score, user.gold, user.food, user.health, user.luck, current_X, current_Y);
	printf("==============================\n");
	printf("던전에 오신 것을 환영합니다 !\n");
	printf("던전에서는 본인의 행운에 따라 골드와 점수를 획득하실 수 있습니다 !\n");
	printf("==============================\n");
	printf("입장하시겠습니까 ? (Y / N) :\n");
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