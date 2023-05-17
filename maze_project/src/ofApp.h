#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon

typedef struct _node* node_pointer;
typedef struct _node {
	int row;
	int col;
	int index;
	int middle_x;
	int middle_y;
	node_pointer link[4]; //0은 up, 1은 right, 2는 down, 3은 left를 의미한다
}node;

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key); // Traps escape key if exit disabled
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		
		int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
		int isDFS;//DFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
		int isBFS;//BFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.

		//adjacency matrix로 구현한 미로
		int** maze; //파일의 미로 정보를 담는 이차원 배열
		int HEIGHT;//미로의 높이
		int WIDTH;//미로의 너비
		
		//adjacency list로 구현한 미로
		node_pointer* graph; //graph[MAX_VERTICES]   MAX_VERTICES == height * width
		int height_graph; //미로 방 기준, 미로의 높이
		int width_graph; //미로 방 기준, 미로의 너비
		int num_of_terms; //미로 방의 개수
		
		//2주차 실습
		bool readFile();
		void freeMemory();

		//3주차 실습(DFS에 필요한 변수 및 함수)
		bool DFS();
		void dfsdraw();
		int* visited;
		int* stack;// num_of_terms만큼 동적 할당해서 경로 저장 용도로 활용
		int top;
		int* all; // 돌아서 나온 경로도 모두 저장하는 용도
		int top_all;
		void push(int current_pos);
		int pop();
		
		//3주차 과제(BFS에 필요한 변수 및 함수)
		bool BFS();
		void bfsdraw();
		int* queue;
		void addq(int current_pos);
		int deleteq();
		int front, rear;
		int rear_all;

		//최종 프로젝트를 위해 추가한 변수 및 함수
		void make_maze(); //아래 함수들을 포함하여 완전 미로를 만들고 maze.maz 파일 안에 저장하는 함수
		void Init_maze(int temp_height, int temp_width, int** maze); //2차원 배열의 미로를 초기화
		void print_maze(int temp_height, int temp_width, int** maze); //디버깅 목적으로 미로의 결과물을 콘솔에 출력
		void fwrite_maze(int temp_height, int temp_width, int** maze); //미로의 결과물을 maze.maz 이름의 파일을 생성하고 저장
		void Eller_algorithm(int temp_height, int temp_width, int** maze); //엘러의 알고리즘으로 완전 미로를 생성하는 알고리즘
		
		ofImage Jerry; //제리의 이미지
		ofImage Cheese; //치즈의 이미지

		int entrance; //미로의 시작점을 의미
		int exit; //미로의 도착점을 의미

		int current_top; //stack의 top과 같은 역할을 하는 변수
		int current; //stack[current_top]을 대입해서 현재 위치해야 하는 미로의 방을 가리키는 index
		int isDFS_Animation; //1이면 DFS_Animation을 실행시키는 것
		int isBFS_Animation; //1이면 BFS_Animation을 실행시키는 것


		// Menu
		ofxWinMenu * menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		// Used by example app
		ofTrueTypeFont myFont;
        ofImage myImage;
		float windowWidth, windowHeight;
		HWND hWnd; // Application window
		HWND hWndForeground; // current foreground window

		// Example menu variables
		bool bShowInfo;
		bool bFullscreen;
		bool bTopmost;

		// Example functions
 		void doFullScreen(bool bFull);
		void doTopmost(bool bTop);
};
