#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// maze가 10이상인 경우, ' '(빈칸)을 출력
// maze가 1인 경우, '+'(모서리)를 출력
// maze가 2인 경우, '-'(가로벽)를 출력
// maze가 3인 경우, '|'(세로벽)를 출력

int** maze;

void Init_maze(int HEIGHT, int WIDTH) {
	int i, j;
	for (i = 0; i < HEIGHT; i++) { //height * 2 + 1
		for (j = 0; j < WIDTH; j++) { //width * 2 + 1
			if ((i % 2) == 0 && (j % 2) == 0) maze[i][j] = 1; //행이 짝수, 열도 짝수인 경우는 모서리
			if ((i % 2) == 0 && (j % 2) == 1) maze[i][j] = 2; //행은 짝수, 열이 홀수인 경우는 가로벽
			if ((i % 2) == 1 && (j % 2) == 0) maze[i][j] = 3; //행이 홀수, 열이 짝수인 경우는 세로벽
			if ((i % 2) == 1 && (j % 2) == 1) maze[i][j] = 0; //행이 홀수, 열이 홀수인 경우에는 방(room)
			//세로벽이나 가로벽이 없어진 자리는 4으로 입력 --> ' '(빈칸)으로 출력
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

	//첫번째 줄, 서로 다른 집합에 속하게 초기화
	for (i = 0; i < WIDTH; i++) {
		if ((i % 2) == 1) {
			maze[1][i] = number++;
		}
	}
	//첫번째 줄, 벽 랜덤으로 제거 or 유지
	for (j = 1; j < WIDTH - 1; j++) { //왼쪽 벽이랑 오른쪽 벽은 제거 대상이 아니기 때문
		if ((j % 2) == 0) {
			select = rand() % 2; //0 또는 1이 저장 (0이면 벽 지우기, 1이면 벽 유지)
			if (select == 0) {
				maze[1][j] = 4; //세로벽 지우기(지워진 벽은 4로 간주)
				maze[1][j + 1] = maze[1][j - 1]; //벽이 없어지면 벽 기준 왼쪽과 같은 집합에 속하도록 설정
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

	for (i = 1; i < HEIGHT - 3; i += 2) { //마지막 줄 전까지 반복(수직 경로 & 행 정리)

		//수직 경로 만들기(1. 무조건 집합당 1개의 수직 경로는 있어야한다/ 2. 있다면 그 다음부터는 임의로 정한다)
		//1. 한줄을 검색해서 그 줄에 1개만 있는 다른 집합이라면, 무조건 수직 경로를 만든다
		//2. 여러 개의 집합이 있는 경우에는, 임의로 수직 경로를 만든다. 마지막까지 안만들었으면 마지막꺼의 수직 경로를 필수적으로 생성한다
		for (j = 1; j < WIDTH - 3; j += 2) {
			if (maze[i][j] == maze[i][j + 2] && possible == 0) {
				select = rand() % 2;
				if (select == 0) {
					maze[i+1][j] = 4; //가로벽 제거
					maze[i+2][j] = maze[i][j]; //아래칸을 위칸과 같은 집합으로 설정
					flag = 1; possible = 1;
				}

				if (j + 2 == WIDTH - 2) { //마지막칸 처리
					if (flag == 1 && possible == 0) {
						select = rand() % 2;
						if (select == 0) {
							maze[i+1][j + 2] = 4;
							maze[i+2][j + 2] = maze[i][j + 2];
						}
					}
					else if(flag == 0) { //집합 내 수직 경로가 아직 없음
						maze[i+1][j + 2] = 4; //수직 경로가 없으니 무조건 수직 경로가 나야함
						maze[i+2][j + 2] = maze[i][j + 2];
					}
					else if (flag == 1 && possible == 1) {
						possible = 0;
					}
				}
			}

			else if (maze[i][j] == maze[i][j + 2] && possible == 1) { //바로 이전꺼가 수직경로를 내린 경우
				possible = 0; //다음 시행에서는 그릴 가능성 있음, flag는 이미 1
			}

			else if (maze[i][j] != maze[i][j + 2] && flag == 0) { //다음부터 다른 집합인데 기존 집합에서 수직 경로가 없었다면,
				maze[i+1][j] = 4; //가로벽 제거
				maze[i+2][j] = maze[i][j]; //아래칸을 위칸과 같은 집합으로 설정
				
				flag = 0;
				possible = 0;

				if (j + 2 == WIDTH - 2) { //마지막칸 처리
					maze[i+1][j + 2] = 4; //가로벽 제거(1개밖에 없는 집합이니깐)
					maze[i+2][j + 2] = maze[i][j + 2];
				}
			}
			else if (maze[i][j] != maze[i][j + 2] && flag == 1) {
				if (possible == 0) {
					select = rand() % 2;
					if (select == 0) {
						maze[i + 1][j] = 4;
						maze[i + 2][j] = maze[i][j]; //아래칸을 위칸과 같은 집합으로 설정
					}
				}
				flag = 0; //다음 시행에서 집합이 flag 0으로 시작할 수 있도록
				possible = 0;

				if (j + 2 == WIDTH - 2) { //마지막 칸 처리
					maze[i+1][j + 2] = 4; //가로벽 제거(1개밖에 없는 집합이니깐)
					maze[i+2][j + 2] = maze[i][j + 2];
				}
			}
		}
		flag = 0;
		possible = 0;

		//아랫 줄을 읽어서 0으로 초기화된 값이 안 바뀌었으면, 수직경로가 없던 것이고 이경우에는 새로운 집합으로 만든다
		for (j = 1; j < WIDTH - 1; j += 2) {
			if (maze[i + 2][j] == 0) maze[i + 2][j] = number++;
		}

		if (i + 2 == HEIGHT - 2) break; //마지막 줄의 경우에는 벽 제거의 원칙이 다르기 때문에 종료
		//행에서 임의로 벽 제거
		for (j = 1; j < WIDTH - 3; j += 2) {
			if (maze[i + 2][j] != maze[i + 2][j + 2]) {
				select = rand() % 2;
				if (select == 0) {
					maze[i + 2][j + 1] = 4; //세로벽 허물기(우측)
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
	
	
	//마지막줄 처리: 인접해 있으며, 서로 다른 집합에 속한 방들 사이의 모든 벽을 제거
	for (j = 1; j < WIDTH - 3; j += 2) {
		if (maze[HEIGHT - 2][j] != maze[HEIGHT - 2][j + 2]) {
			maze[HEIGHT - 2][j + 1] = 4; //가로벽 제거

			temp = maze[HEIGHT - 2][j + 2];
			maze[HEIGHT - 2][j + 2] = maze[HEIGHT - 2][j];
			for (p = 0; p < HEIGHT ; p++) {
				for (t = 0; t < WIDTH; t++) {
					if (maze[p][t] == temp) maze[p][t] = maze[HEIGHT - 2][j];
				}
			} //왼쪽 집합으로 오른쪽 집합을 변경
		
		}
	}
	
	print_maze(HEIGHT, WIDTH);
	printf("\n");

	//debugging_maze(HEIGHT, WIDTH);

	fwrite_maze(HEIGHT, WIDTH);
	
	for (i = 0; i < HEIGHT; i++) free(maze[i]);
	free(maze);
}