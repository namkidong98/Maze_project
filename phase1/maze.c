#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// maze�� 10�̻��� ���, ' '(��ĭ)�� ���
// maze�� 1�� ���, '+'(�𼭸�)�� ���
// maze�� 2�� ���, '-'(���κ�)�� ���
// maze�� 3�� ���, '|'(���κ�)�� ���

int** maze;

void Init_maze(int HEIGHT, int WIDTH) {
	int i, j;
	for (i = 0; i < HEIGHT; i++) { //height * 2 + 1
		for (j = 0; j < WIDTH; j++) { //width * 2 + 1
			if ((i % 2) == 0 && (j % 2) == 0) maze[i][j] = 1; //���� ¦��, ���� ¦���� ���� �𼭸�
			if ((i % 2) == 0 && (j % 2) == 1) maze[i][j] = 2; //���� ¦��, ���� Ȧ���� ���� ���κ�
			if ((i % 2) == 1 && (j % 2) == 0) maze[i][j] = 3; //���� Ȧ��, ���� ¦���� ���� ���κ�
			if ((i % 2) == 1 && (j % 2) == 1) maze[i][j] = 0; //���� Ȧ��, ���� Ȧ���� ��쿡�� ��(room)
			//���κ��̳� ���κ��� ������ �ڸ��� 4���� �Է� --> ' '(��ĭ)���� ���
		}
	}
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
				//printf("%d", maze[i][j]); //for debugging
			}
		}
		printf("\n");
	}
}

void debugging_maze(int HEIGHT, int WIDTH) {
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
				//printf(" ");
				printf("%d", maze[i][j]); //for debugging
			}
		}
		printf("\n");
	}
}

void fwrite_maze(int HEIGHT, int WIDTH) {
	FILE* fp;
	fp = fopen("maze.maz", "w");
	int i, j, command;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			command = maze[i][j];
			switch (command) {
			case 1:
				fprintf(fp, "+"); break;
			case 2:
				fprintf(fp, "-"); break;
			case 3:
				fprintf(fp, "|"); break;
			case 4:
				fprintf(fp, " "); break;
			default:
				fprintf(fp, " ");
			}
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

int main() {
	int i, j; int number = 10; 	int width, height; int select; int flag = 0; int t; int temp; int p; int flag_circle = 0; int possible = 0;
	srand((unsigned int)time(NULL)); 
	//int command; for debugging


	printf("Put width, height: ");
	scanf("%d %d", &width, &height);
	int WIDTH = width * 2 + 1;
	int HEIGHT = height * 2 + 1;

	maze = (int**)malloc(sizeof(int*) * HEIGHT);
	for (i = 0; i < HEIGHT; i++) {
		maze[i] = (int*)malloc(sizeof(int) * WIDTH);
	}

	Init_maze(HEIGHT, WIDTH);

	//ù��° ��, ���� �ٸ� ���տ� ���ϰ� �ʱ�ȭ
	for (i = 0; i < WIDTH; i++) {
		if ((i % 2) == 1) {
			maze[1][i] = number++;
		}
	}
	//ù��° ��, �� �������� ���� or ����
	for (j = 1; j < WIDTH - 1; j++) { //���� ���̶� ������ ���� ���� ����� �ƴϱ� ����
		if ((j % 2) == 0) {
			select = rand() % 2; //0 �Ǵ� 1�� ���� (0�̸� �� �����, 1�̸� �� ����)
			if (select == 0) {
				maze[1][j] = 4; //���κ� �����(������ ���� 4�� ����)
				maze[1][j + 1] = maze[1][j - 1]; //���� �������� �� ���� ���ʰ� ���� ���տ� ���ϵ��� ����
			}
		}
	}

	/* for debugging(first line)
	for (j = 0; j < WIDTH; j++) {
		command = maze[1][j];
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
			//printf(" ");
			printf("%d", maze[1][j]); //for debugging
		}
	}
	printf("\n");
	*/

	for (i = 1; i < HEIGHT - 3; i += 2) { //������ �� ������ �ݺ�(���� ��� & �� ����)

		//���� ��� �����(1. ������ ���մ� 1���� ���� ��δ� �־���Ѵ�/ 2. �ִٸ� �� �������ʹ� ���Ƿ� ���Ѵ�)
		//1. ������ �˻��ؼ� �� �ٿ� 1���� �ִ� �ٸ� �����̶��, ������ ���� ��θ� �����
		//2. ���� ���� ������ �ִ� ��쿡��, ���Ƿ� ���� ��θ� �����. ���������� �ȸ�������� ���������� ���� ��θ� �ʼ������� �����Ѵ�
		for (j = 1; j < WIDTH - 3; j += 2) {
			if (maze[i][j] == maze[i][j + 2] && possible == 0) {
				select = rand() % 2;
				if (select == 0) {
					maze[i+1][j] = 4; //���κ� ����
					maze[i+2][j] = maze[i][j]; //�Ʒ�ĭ�� ��ĭ�� ���� �������� ����
					flag = 1; possible = 1;
				}

				if (j + 2 == WIDTH - 2) { //������ĭ ó��
					if (flag == 1 && possible == 0) {
						select = rand() % 2;
						if (select == 0) {
							maze[i+1][j + 2] = 4;
							maze[i+2][j + 2] = maze[i][j + 2];
						}
					}
					else if(flag == 0) { //���� �� ���� ��ΰ� ���� ����
						maze[i+1][j + 2] = 4; //���� ��ΰ� ������ ������ ���� ��ΰ� ������
						maze[i+2][j + 2] = maze[i][j + 2];
					}
					else if (flag == 1 && possible == 1) {
						possible = 0;
					}
				}
			}

			else if (maze[i][j] == maze[i][j + 2] && possible == 1) { //�ٷ� �������� ������θ� ���� ���
				possible = 0; //���� ���࿡���� �׸� ���ɼ� ����, flag�� �̹� 1
			}

			else if (maze[i][j] != maze[i][j + 2] && flag == 0) { //�������� �ٸ� �����ε� ���� ���տ��� ���� ��ΰ� �����ٸ�,
				maze[i+1][j] = 4; //���κ� ����
				maze[i+2][j] = maze[i][j]; //�Ʒ�ĭ�� ��ĭ�� ���� �������� ����
				
				flag = 0;
				possible = 0;

				if (j + 2 == WIDTH - 2) { //������ĭ ó��
					maze[i+1][j + 2] = 4; //���κ� ����(1���ۿ� ���� �����̴ϱ�)
					maze[i+2][j + 2] = maze[i][j + 2];
				}
			}
			else if (maze[i][j] != maze[i][j + 2] && flag == 1) {
				if (possible == 0) {
					select = rand() % 2;
					if (select == 0) {
						maze[i + 1][j] = 4;
						maze[i + 2][j] = maze[i][j]; //�Ʒ�ĭ�� ��ĭ�� ���� �������� ����
					}
				}
				flag = 0; //���� ���࿡�� ������ flag 0���� ������ �� �ֵ���
				possible = 0;

				if (j + 2 == WIDTH - 2) { //������ ĭ ó��
					maze[i+1][j + 2] = 4; //���κ� ����(1���ۿ� ���� �����̴ϱ�)
					maze[i+2][j + 2] = maze[i][j + 2];
				}
			}
		}
		flag = 0;
		possible = 0;

		//�Ʒ� ���� �о 0���� �ʱ�ȭ�� ���� �� �ٲ������, ������ΰ� ���� ���̰� �̰�쿡�� ���ο� �������� �����
		for (j = 1; j < WIDTH - 1; j += 2) {
			if (maze[i + 2][j] == 0) maze[i + 2][j] = number++;
		}

		if (i + 2 == HEIGHT - 2) break; //������ ���� ��쿡�� �� ������ ��Ģ�� �ٸ��� ������ ����
		//�࿡�� ���Ƿ� �� ����
		for (j = 1; j < WIDTH - 3; j += 2) {
			if (maze[i + 2][j] != maze[i + 2][j + 2]) {
				select = rand() % 2;
				if (select == 0) {
					maze[i + 2][j + 1] = 4; //���κ� �㹰��(����)
					temp = maze[i + 2][j + 2];
					maze[i + 2][j + 2] = maze[i + 2][j];
					for (p = 1; p < HEIGHT - 1; p += 2) {
						for (t = 1; t < WIDTH - 1; t += 2) {
							if (maze[p][t] == temp) maze[p][t] = maze[i + 2][j];
						}
					}
				}
			}

		}

		/* for debugging
		for (j = 0; j < WIDTH; j++) {
			command = maze[i+2][j];
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
				//printf(" ");
				printf("%d", maze[i+2][j]); //for debugging
			}
		}
		printf("\n");
		*/
	}
	
	
	//�������� ó��: ������ ������, ���� �ٸ� ���տ� ���� ��� ������ ��� ���� ����
	for (j = 1; j < WIDTH - 3; j += 2) {
		if (maze[HEIGHT - 2][j] != maze[HEIGHT - 2][j + 2]) {
			maze[HEIGHT - 2][j + 1] = 4; //���κ� ����

			temp = maze[HEIGHT - 2][j + 2];
			maze[HEIGHT - 2][j + 2] = maze[HEIGHT - 2][j];
			for (p = 0; p < HEIGHT ; p++) {
				for (t = 0; t < WIDTH; t++) {
					if (maze[p][t] == temp) maze[p][t] = maze[HEIGHT - 2][j];
				}
			} //���� �������� ������ ������ ����
		
		}
	}
	
	print_maze(HEIGHT, WIDTH);
	printf("\n");

	//debugging_maze(HEIGHT, WIDTH);

	fwrite_maze(HEIGHT, WIDTH);
	
	for (i = 0; i < HEIGHT; i++) free(maze[i]);
	free(maze);
}