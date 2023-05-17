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
	node_pointer link[4]; //0�� up, 1�� right, 2�� down, 3�� left�� �ǹ��Ѵ�
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

		
		int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�.
		int isDFS;//DFS�Լ��� ������״��� �Ǵ��ϴ� ����. 0�̸� ������߰� 1�̸� �����ߴ�.
		int isBFS;//BFS�Լ��� ������״��� �Ǵ��ϴ� ����. 0�̸� ������߰� 1�̸� �����ߴ�.

		//adjacency matrix�� ������ �̷�
		int** maze; //������ �̷� ������ ��� ������ �迭
		int HEIGHT;//�̷��� ����
		int WIDTH;//�̷��� �ʺ�
		
		//adjacency list�� ������ �̷�
		node_pointer* graph; //graph[MAX_VERTICES]   MAX_VERTICES == height * width
		int height_graph; //�̷� �� ����, �̷��� ����
		int width_graph; //�̷� �� ����, �̷��� �ʺ�
		int num_of_terms; //�̷� ���� ����
		
		//2���� �ǽ�
		bool readFile();
		void freeMemory();

		//3���� �ǽ�(DFS�� �ʿ��� ���� �� �Լ�)
		bool DFS();
		void dfsdraw();
		int* visited;
		int* stack;// num_of_terms��ŭ ���� �Ҵ��ؼ� ��� ���� �뵵�� Ȱ��
		int top;
		int* all; // ���Ƽ� ���� ��ε� ��� �����ϴ� �뵵
		int top_all;
		void push(int current_pos);
		int pop();
		
		//3���� ����(BFS�� �ʿ��� ���� �� �Լ�)
		bool BFS();
		void bfsdraw();
		int* queue;
		void addq(int current_pos);
		int deleteq();
		int front, rear;
		int rear_all;

		//���� ������Ʈ�� ���� �߰��� ���� �� �Լ�
		void make_maze(); //�Ʒ� �Լ����� �����Ͽ� ���� �̷θ� ����� maze.maz ���� �ȿ� �����ϴ� �Լ�
		void Init_maze(int temp_height, int temp_width, int** maze); //2���� �迭�� �̷θ� �ʱ�ȭ
		void print_maze(int temp_height, int temp_width, int** maze); //����� �������� �̷��� ������� �ֿܼ� ���
		void fwrite_maze(int temp_height, int temp_width, int** maze); //�̷��� ������� maze.maz �̸��� ������ �����ϰ� ����
		void Eller_algorithm(int temp_height, int temp_width, int** maze); //������ �˰������� ���� �̷θ� �����ϴ� �˰���
		
		ofImage Jerry; //������ �̹���
		ofImage Cheese; //ġ���� �̹���

		int entrance; //�̷��� �������� �ǹ�
		int exit; //�̷��� �������� �ǹ�

		int current_top; //stack�� top�� ���� ������ �ϴ� ����
		int current; //stack[current_top]�� �����ؼ� ���� ��ġ�ؾ� �ϴ� �̷��� ���� ����Ű�� index
		int isDFS_Animation; //1�̸� DFS_Animation�� �����Ű�� ��
		int isBFS_Animation; //1�̸� BFS_Animation�� �����Ű�� ��


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
