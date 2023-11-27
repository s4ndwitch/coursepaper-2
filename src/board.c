#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "helper.h"
#include "list.h"


int splitInput(char *input, char **command) {
	char word[29];
	int symbolCount = 0, wordCount = 0;
	for (int i = 0; i < 29; i++) {
		if (input[i] != ' ' && input[i] != '\n' && input[i] != 0)
			word[symbolCount++] = input[i];
		else {
			word[symbolCount++] = 0;
			command[wordCount] = (char *)malloc(sizeof(char) * symbolCount);
			strcat(command[wordCount++], word);
			symbolCount = 0;
			
			if (input[i] == '\n') break;
		}
	}
	return wordCount;
}

int setPoint(char *board, int x, int y, int setX, int setY) {
	if (setX < 0 || setX >= x || setY < 0 || setY >= y) return -1;
	*(board + setX * y + y - setY - 1) = (*(board + setX * y + y - setY - 1) + 1) % 2;

	// addElement(board + setX * y + setY, cells);

	return 0;
}

int setCommand(char **command, char *board, int x, int y) {
	int setX, setY;
	if (command[1] == 0x0 || command[2] == 0x0) return -1;
	if ((setX = parseInt(command[1])) < 0)
		return x - 10;
	if ((setY = parseInt(command[2])) < 0)
		return y - 10;
	if (setPoint(board, x, y, setX, setY) == -1)
		return -2;
	else
		return 0;
}

int setGlider(char *board, int x, int y, int posX, int posY) {
	if (posX < 0 || posX >= x || posY < 0 || posY >= y)
		return -1;
	setPoint(board, x, y, posX, posY);
	setPoint(board, x, y, (posX - 1) % x, posY);
	setPoint(board, x, y, (posX - 2) % x, posY);
	setPoint(board, x, y, posX, (posY + 1) % y);
	setPoint(board, x, y, (posX - 1) % x, (posY + 2) % y);

	return 0;
}

void optimised_turn(char *board, int x, int y) {
	int count;
	char *newBoard = (char *)malloc(sizeof(char) * x * y);
	for (int i = y - 1; i > -1; i--) {
		for (int j = 0; j < x; j++) {
			count = 0;
			for (int shiftX = -1; shiftX < 2; shiftX++)
				for (int shiftY = -1; shiftY < 2; shiftY++) {
					*(newBoard + j * y + i) = 0;
					if (shiftY == 0 && shiftX == 0) continue;
					if (*(board + ((j + shiftX + x) % x) * y + ((i + shiftY + y) % y)) == 1) count += 1;
				}
			if (*(board + j * y + i)) {
				if (count == 2 || count == 3) {
					*(newBoard + j * y + i) = 1;
				} else {
					*(newBoard + j * y + i) = 0;
				}
			} else {
				if (count == 3)
					*(newBoard + j * y + i) = 1;
			}
		}
	}
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++)
			*(board + i * y + j) = *(newBoard + i * y + j);
	free(newBoard);
	return;
}

void turn(char *board, int x, int y) {
	int count;
	char *newBoard = (char *)malloc(sizeof(char) * x * y);
	for (int i = y - 1; i > -1; i--) {
		for (int j = 0; j < x; j++) {
			count = 0;
			for (int shiftX = -1; shiftX < 2; shiftX++)
				for (int shiftY = -1; shiftY < 2; shiftY++) {
					*(newBoard + j * y + i) = 0;
					if (shiftY == 0 && shiftX == 0) continue;
					if (*(board + ((j + shiftX + x) % x) * y + ((i + shiftY + y) % y)) == 1) count += 1;
				}
			if (*(board + j * y + i)) {
				if (count == 2 || count == 3) {
					*(newBoard + j * y + i) = 1;
				} else {
					*(newBoard + j * y + i) = 0;
				}
			} else {
				if (count == 3)
					*(newBoard + j * y + i) = 1;
			}
		}
	}
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++)
			*(board + i * y + j) = *(newBoard + i * y + j);
	free(newBoard);
	return;
}

void printBoard(char *board, int x, int y) {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++)
			printf("%d", *(board + i + y * j));
		printf("\n");
	}
	return;
}

int executeCommand(char *input, char **command, char *board, int x, int y) {
	int result;
	if ((result = splitInput(input, command)) < 0) {
		return result - 10;
	}
	if (!strcmp(command[0], "set") || !strcmp(command[0], "s"))
		if ((result = setCommand(command, board, x, y)) < 0)
			return result - 20;
	if (!strcmp(command[0], "glider") || !strcmp(command[0], "g")) {
		int setX, setY;
		if (command[1] == 0x0 || command[2] == 0x0) return -1;
		if ((setX = parseInt(command[1])) < 0)
			return x - 10;
		if ((setY = parseInt(command[2])) < 0)
			return y - 10;
		if ((result = setGlider(board, WIDTH, HEIGHT, setX, setY)) < 0)
			return result - 20;
	}
	if (!strcmp(command[0], "gun") || !strcmp(command[0], "G")) {
		int setX, setY;
		if (command[1] == 0x0 || command[2] == 0x0) return -1;
		if ((setX = parseInt(command[1])) < 0)
			return x - 10;
		if ((setY = parseInt(command[2])) < 0)
			return y - 10;
		if ((result = setGun(board, WIDTH, HEIGHT, setX, setY)) < 0)
			return result - 20;
	}
	if (!strcmp(command[0], "turn") || !strcmp(command[0], "t")) {
		clock_t start_time = clock();
		turn(board, x, y);
		clock_t end_time = clock();
		printf("Ticks passed: %d\n", end_time - start_time);
	}
	if (!strcmp(command[0], "print") || !strcmp(command[0], "p"))
		printBoard(board, x, y);
	return 0;
}

int setGun(char *board, int x, int y, int posX, int posY) {
	if (posX < 0 || posX >= x || posY < 0 || posY >= y)
		return -1;
	setPoint(board, x, y, (posX - 0) % x, (posY + 0) % y);
	setPoint(board, x, y, (posX - 0) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 1) % x, (posY + 0) % y);
	setPoint(board, x, y, (posX + 1) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 10) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 10) % x, (posY + 0) % y);
	setPoint(board, x, y, (posX + 10) % x, (posY - 1) % y);
	setPoint(board, x, y, (posX + 11) % x, (posY - 2) % y);
	setPoint(board, x, y, (posX + 11) % x, (posY + 2) % y);
	setPoint(board, x, y, (posX + 12) % x, (posY - 3) % y);
	setPoint(board, x, y, (posX + 12) % x, (posY + 3) % y);
	setPoint(board, x, y, (posX + 13) % x, (posY - 3) % y);
	setPoint(board, x, y, (posX + 13) % x, (posY + 3) % y);
	setPoint(board, x, y, (posX + 14) % x, (posY - 0) % y);
	setPoint(board, x, y, (posX + 15) % x, (posY - 2) % y);
	setPoint(board, x, y, (posX + 15) % x, (posY + 2) % y);
	setPoint(board, x, y, (posX + 16) % x, (posY - 0) % y);
	setPoint(board, x, y, (posX + 16) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 16) % x, (posY - 1) % y);
	setPoint(board, x, y, (posX + 17) % x, (posY + 0) % y);
	setPoint(board, x, y, (posX + 20) % x, (posY + 2) % y);
	setPoint(board, x, y, (posX + 20) % x, (posY + 3) % y);
	setPoint(board, x, y, (posX + 20) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 21) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 21) % x, (posY + 2) % y);
	setPoint(board, x, y, (posX + 21) % x, (posY + 3) % y);
	setPoint(board, x, y, (posX + 22) % x, (posY + 0) % y);
	setPoint(board, x, y, (posX + 22) % x, (posY + 4) % y);
	setPoint(board, x, y, (posX + 24) % x, (posY + 4) % y);
	setPoint(board, x, y, (posX + 24) % x, (posY + 5) % y);
	setPoint(board, x, y, (posX + 24) % x, (posY + 0) % y);
	setPoint(board, x, y, (posX + 24) % x, (posY - 1) % y);
	setPoint(board, x, y, (posX + 34) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 34) % x, (posY + 2) % y);
	setPoint(board, x, y, (posX + 35) % x, (posY + 1) % y);
	setPoint(board, x, y, (posX + 35) % x, (posY + 2) % y);

	return 0;
}

void setShattle(char *board, int x, int y, int posX, int posY) {
	if (posX < 0 || posX >= x || posY < 0 || posY >= y)
		return;
	setPoint(board, x, y, posX, posY);
	setPoint(board, x, y, posX, posY + 1);
	setPoint(board, x, y, posX, posY + 2);
	setPoint(board, x, y, posX - 1, posY + 3);
	setPoint(board, x, y, posX - 1, posY);
	setPoint(board, x, y, posX - 2, posY);
	setPoint(board, x, y, posX - 3, posY);
	setPoint(board, x, y, posX - 4, posY + 1);
	setPoint(board, x, y, posX - 4, posY + 3);
}


#ifdef TERMINAL_VERSION__


int main() {

	int x, y, result;
	
	cells = createList();

	char *input = (char *)malloc(sizeof(char) * 29);
	char **command = malloc(sizeof(char *) * 14);
	for (int i = 0; i < 14; i++)
		command[i] = 0x0;
	input[29] = 0;

	printf("Write number of cells in x and y (bordered by int size):\n\n> ");
	scanf("%d%d", &x, &y);

	WIDTH = x;
	HEIGHT = y;

	char *board = (char *)malloc(sizeof(char) * x * y);
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++)
			*(board + i * y + j) = 0;

	printf("set x, y; turn; print;\n\n");
	while (1) {
		printf("> ");
		scanf(" ");
		fgets(input, 28, stdin);
		result = executeCommand(input, command, board, x, y);
	}
	return 0;
}


#endif

