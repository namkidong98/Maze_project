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
	node_pointer link[4]; //0은 uo, 1은 right, 2는 down, 3은 left를 의미한다
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


		bool readFile();
		void freeMemory();

		bool BFS();
		void bfsdraw();


		int HEIGHT;//미로의 높이
		int WIDTH;//미로의 너비
		int** maze; //파일의 미로 정보를 담는 이차원 배열
		int height_graph; //미로 방 기준, 미로의 높이
		int width_graph; //미로 방 기준, 미로의 너비
		int num_of_terms; //미로 방의 갯수
		node_pointer* graph; //graph[MAX_VERTICES]   MAX_VERTICES == height * width
		
		//DFS에 필요한 변수 및 함수
		bool DFS();
		void dfsdraw();
		int* visited;
		int* stack;// num_of_terms만큼 동적 할당해서 경로 저장 용도로 활용
		int top;
		int* all; // 돌아서 나온 경로도 모두 저장하는 용도
		int top_all;
		void push(int current_pos);
		int pop();

		//BFS에 필요한 변수 및 함수
		int* queue;
		void addq(int current_pos);
		int deleteq();
		int front, rear;
		int rear_all;

		int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
		int isDFS;//DFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
		int isBFS;//BFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
		
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

		//final project
		void make_maze();

};
