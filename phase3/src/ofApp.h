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
	node_pointer link[4]; //0�� uo, 1�� right, 2�� down, 3�� left�� �ǹ��Ѵ�
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


		int HEIGHT;//�̷��� ����
		int WIDTH;//�̷��� �ʺ�
		int** maze; //������ �̷� ������ ��� ������ �迭
		int height_graph; //�̷� �� ����, �̷��� ����
		int width_graph; //�̷� �� ����, �̷��� �ʺ�
		int num_of_terms; //�̷� ���� ����
		node_pointer* graph; //graph[MAX_VERTICES]   MAX_VERTICES == height * width
		
		//DFS�� �ʿ��� ���� �� �Լ�
		bool DFS();
		void dfsdraw();
		int* visited;
		int* stack;// num_of_terms��ŭ ���� �Ҵ��ؼ� ��� ���� �뵵�� Ȱ��
		int top;
		int* all; // ���Ƽ� ���� ��ε� ��� �����ϴ� �뵵
		int top_all;
		void push(int current_pos);
		int pop();

		//BFS�� �ʿ��� ���� �� �Լ�
		int* queue;
		void addq(int current_pos);
		int deleteq();
		int front, rear;
		int rear_all;

		int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�.
		int isDFS;//DFS�Լ��� ������״��� �Ǵ��ϴ� ����. 0�̸� ������߰� 1�̸� �����ߴ�.
		int isBFS;//BFS�Լ��� ������״��� �Ǵ��ϴ� ����. 0�̸� ������߰� 1�̸� �����ߴ�.
		
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
