#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "helper.h"

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

int setCommand(char **command, char *board, int x, int y) {
	int setX, setY;
	if (command[1] == 0x0 || command[2] == 0x0) return -1;
	if ((setX = parseInt(command[1])) < 0)
		return x - 10;
	if ((setY = parseInt(command[2])) < 0)
		return y - 10;
	if (setX >= x || setY >= y) return -2;
	*(board + setX * y + setY) = (*(board + setX * y + setY) + 1) % 2;
	return 0;
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
	for (int i = y - 1; i > -1; i--) {
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
	if (!strcmp(command[0], "turn") || !strcmp(command[0], "t"))
		turn(board, x, y);
	if (!strcmp(command[0], "print") || !strcmp(command[0], "p"))
		printBoard(board, x, y);
	return 0;
}

void setGun(char *board, int x, int y, int posX, int posY) {
	if (posX < 0 || posX >= x || posY < 0 || posY >= y)
		return;
	*(board + ((posX + x - 0) % x) * y + (y - 1 - posY + 0) % y) = 1;
	*(board + ((posX + x - 0) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 1) % x) * y + (y - 1 - posY + 0) % y) = 1;
	*(board + ((posX + 1) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 10) % x) * y + (y - 1 - posY + 0) % y) = 1;
	*(board + ((posX + 10) % x) * y + (y - 1 - posY - 1) % y) = 1;
	*(board + ((posX + 11) % x) * y + (y - 1 - posY - 2) % y) = 1;
	*(board + ((posX + 12) % x) * y + (y - 1 - posY - 3) % y) = 1;
	*(board + ((posX + 13) % x) * y + (y - 1 - posY - 3) % y) = 1;
	*(board + ((posX + 10) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 11) % x) * y + (y - 1 - posY + 2) % y) = 1;
	*(board + ((posX + 12) % x) * y + (y - 1 - posY + 3) % y) = 1;
	*(board + ((posX + 13) % x) * y + (y - 1 - posY + 3) % y) = 1;
	*(board + ((posX + 14) % x) * y + (y - 1 - posY - 0) % y) = 1;
	*(board + ((posX + 15) % x) * y + (y - 1 - posY - 2) % y) = 1;
	*(board + ((posX + 15) % x) * y + (y - 1 - posY + 2) % y) = 1;
	*(board + ((posX + 16) % x) * y + (y - 1 - posY - 1) % y) = 1;
	*(board + ((posX + 16) % x) * y + (y - 1 - posY - 0) % y) = 1;
	*(board + ((posX + 16) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 17) % x) * y + (y - 1 - posY - 0) % y) = 1;
	*(board + ((posX + 20) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 20) % x) * y + (y - 1 - posY + 2) % y) = 1;
	*(board + ((posX + 20) % x) * y + (y - 1 - posY + 3) % y) = 1;
	*(board + ((posX + 21) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 21) % x) * y + (y - 1 - posY + 2) % y) = 1;
	*(board + ((posX + 21) % x) * y + (y - 1 - posY + 3) % y) = 1;
	*(board + ((posX + 22) % x) * y + (y - 1 - posY + 4) % y) = 1;
	*(board + ((posX + 22) % x) * y + (y - 1 - posY + 0) % y) = 1;
	*(board + ((posX + 24) % x) * y + (y - 1 - posY + 4) % y) = 1;
	*(board + ((posX + 24) % x) * y + (y - 1 - posY + 5) % y) = 1;
	*(board + ((posX + 24) % x) * y + (y - 1 - posY + 0) % y) = 1;
	*(board + ((posX + 24) % x) * y + (y - 1 - posY - 1) % y) = 1;
	*(board + ((posX + 34) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 34) % x) * y + (y - 1 - posY + 2) % y) = 1;
	*(board + ((posX + 35) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + 35) % x) * y + (y - 1 - posY + 2) % y) = 1;

}

void setShattle(char *board, int x, int y, int posX, int posY) {
	if (posX < 0 || posX >= x || posY < 0 || posY >= y)
		return;
	*(board + posX * y + (y - 1 - posY) % y) = 1;
	*(board + posX * y + (y - 1 - posY + 1) % y) = 1;
	*(board + posX * y + (y - 1 - posY + 2) % y) = 1;
	*(board + ((posX + x - 1) % x) * y + (y - 1 - posY + 3) % y) = 1;
	*(board + ((posX + x - 1) % x) * y + (y - 1 - posY) % y) = 1;
	*(board + ((posX + x - 2) % x) * y + (y - 1 - posY) % y) = 1;
	*(board + ((posX + x - 3) % x) * y + (y - 1 - posY) % y) = 1;
	*(board + ((posX + x - 4) % x) * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + x - 4) % x) * y + (y - 1 - posY + 3) % y) = 1;
}

void setGlider(char *board, int x, int y, int posX, int posY) {
	if (posX < 0 || posX >= x || posY < 0 || posY >= y)
		return;
	*(board + posX * y + (y - 1 - posY)) = 1;
	*(board + posX * y + (y - 1 - posY + 1) % y) = 1;
	*(board + ((posX + x - 1 + x) % x) * y + (y - 1 - posY + 2) % y) = 1;
	*(board + ((posX + x - 1 + x) % x) * y + y - 1 - posY) = 1;
	*(board + ((posX + x - 2 + x) % x) * y + y - 1 - posY) = 1;
}

#ifdef TERMINAL_VERSION__

int main() {
	int x, y, result;

	// Mamy dwa obszary w pamięci.
	char *input = (char *)malloc(sizeof(char) * 29);
	char **command = malloc(sizeof(char *) * 14);
	for (int i = 0; i < 14; i++)
		command[i] = 0x0;
	input[29] = 0;

	// Myślę, że nie masz pytań.
	printf("Halo! Witamy w grze \"Życie\"!\n\nNajpierw, napisz liczbę kratków dla x i y (że są ograniczone typem int):\n\n> ");
	scanf("%d%d", &x, &y);
	
	// Blok generowania pola.
	char *board = (char *)malloc(sizeof(char) * x * y);
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++)
			*(board + i * y + j) = 0;

	printf("Następne, napisz numer kratki, w której chcesz utworzyć życie! Albo możesz popatrzeć na wszystkie komandy.\n\n");
	while (1) {
		printf("> ");
		scanf(" "); // fgets czyta pierwszy znak jako "\n" i powtarza ">"
		fgets(input, 28, stdin);
		result = executeCommand(input, command, board, x, y);
	}
	return 0;
}

#endif
