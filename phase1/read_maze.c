#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 100

typedef struct _node* node_pointer;
typedef struct _node {
	int row;
	int col;
	int index;
	node_pointer link[4]; //0은 uo, 1은 right, 2는 down, 3은 left를 의미한다
}node;

int** maze;

void read_maze(int HEIGHT, int WIDTH) {
	int i, j; char line[MAX_LINE]; FILE* fp;
	fp = fopen("maze.maz", "r");
	for (i = 0; i < HEIGHT; i++) {
		fgets(line, MAX_LINE, fp);
		for (j = 0; j < WIDTH; j++) {
			if (line[j] == '+') maze[i][j] = 1;
			else if (line[j] == '-') maze[i][j] = 2;
			else if (line[j] == '|') maze[i][j] = 3;
			else if ((i % 2 == 1) && (j % 2 == 0) && line[j] == ' ') maze[i][j] = 4; //없어진 세로벽(높이는 홀수, 너비는 짝수)
			else if ((i % 2 == 0) && (j % 2 == 1) && line[j] == ' ') maze[i][j] = 4; //없어진 가로벽(높이는 짝수, 너비는 홀수)
			else maze[i][j] = 0;
		}
	}
	fclose(fp);
}

void print_maze(int HEIGHT, int WIDTH) {
	int i, j, command;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			command = maze[i][j];
			switch (command) {
			case 1:
				printf("+"); break;
			case 2:
				printf("-"); break;
			case 3:
				printf("|"); break;
			case 4:
				printf(" "); break;
			default:
				printf(" ");
			}
		}
		printf("\n");
	}
}

int main() {
	FILE* fp; int HEIGHT, WIDTH; int i, j;
	fp = fopen("maze.maz", "r");
	HEIGHT = 0;
	char line[MAX_LINE];
	while (!feof(fp)) { //maz 끝에 빈줄이 하나도 없는 경우
		fgets(line, MAX_LINE, fp);
		HEIGHT++;
	}
	WIDTH = (int)strlen(line);
	fclose(fp);
	
	printf("HEIGHT: %d WIDTH: %d\n", HEIGHT, WIDTH);

	maze = (int**)malloc(sizeof(int*) * HEIGHT);
	for (i = 0; i < HEIGHT; i++) {
		maze[i] = (int*)malloc(sizeof(int) * WIDTH);
	}

	read_maze(HEIGHT, WIDTH);

	print_maze(HEIGHT, WIDTH);

	
}