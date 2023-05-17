#include "ofApp.h"
#include <iostream>

using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = 0;
	isBFS = 0;
	isOpen = 0;
	HEIGHT = 0;
	WIDTH = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		if (isOpen) {
			freeMemory();
			cout << "Dynamically allocated memories freed" << endl;
		}
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		//doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}
	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	char str[256];
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j; int number = 0;
	int x = 30, y = 5; //starting point
	int line_length = 35;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	if (isOpen) {
		for (i = 0; i < height_graph; i++) {
			for (j = 0; j < width_graph; j++) {
				graph[number]->middle_x = x + line_length/2; //3주차 추가
				graph[number]->middle_y = y + line_length/2; //3주차 추가
				if (graph[number]->link[0] == NULL) ofDrawLine(x, y, x + line_length, y);
				if (graph[number]->link[1] == NULL) ofDrawLine(x + line_length, y, x + line_length, y + line_length);
				if (graph[number]->link[2] == NULL) ofDrawLine(x, y + line_length, x + line_length, y + line_length);
				if (graph[number]->link[3] == NULL) ofDrawLine(x, y, x, y + line_length);
				x += line_length;
				number++;
			}
			x = 30; //leftmost(starting point)
			y += line_length;
		}


	}

	if (isDFS)
	{
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (isBFS)
	{
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw

void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen

void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

bool ofApp::readFile()
{
	int i, j, command;
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << endl;
		//printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				HEIGHT++;
				string line = *it;
				WIDTH = line.size();
			}

			//int형 2차원 배열 maze를 동적 할당
			maze = new int* [HEIGHT];
			for (i = 0; i < HEIGHT; i++) {
				maze[i] = new int[WIDTH];
			}
			i = 0;
			
			cout << "HEIGHT: " << HEIGHT << " WIDTH: " << WIDTH << endl;

			//파일의 내용을 읽어서 maze에 저장
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it, i++) {
				string line = *it;
				for (j = 0; j < WIDTH; j++) {
					if (line[j] == '+') maze[i][j] = 1;
					else if (line[j] == '-') maze[i][j] = 2;
					else if (line[j] == '|') maze[i][j] = 3;
					else if ((i % 2 == 1) && (j % 2 == 0) && line[j] == ' ') maze[i][j] = 4; //없어진 세로벽(높이는 홀수, 너비는 짝수)
					else if ((i % 2 == 0) && (j % 2 == 1) && line[j] == ' ') maze[i][j] = 4; //없어진 가로벽(높이는 짝수, 너비는 홀수)
					else maze[i][j] = 0;
				}
			}
			
			//for debugging
			for (i = 0; i < HEIGHT; i++) {
				for (j = 0; j < WIDTH; j++) {
					command = maze[i][j];
					switch (command) {
					case 1:
						cout << "+"; break;
					case 2:
						cout << "-"; break;
					case 3:
						cout << "|"; break;
					case 4:
						cout << " "; break;
					default:
						cout << " ";
					}
				}
				cout << endl;
			}

			//graph를 만들기 위해 필요한 선작업
			int number = 0;
			height_graph = HEIGHT / 2;
			width_graph = WIDTH / 2;
			num_of_terms = height_graph * width_graph;
			graph = new node_pointer[num_of_terms];

			//maze를 읽어서 새로운 자료구조인 graph의 모든 vertex를 구성 및 초기화
			for (i = 0; i < HEIGHT; i++) {
				for (j = 0; j < WIDTH; j++) {
					if ((i % 2 == 1) && (j % 2 == 1)) { //둘다 홀수인 경우가 maze의 방에 해당한다
						node_pointer temp = new node;
						temp->row = i; temp->col = j; //새로만든 node의 기본 정보 초기화
						temp->index = number; 
						temp->link[0] = NULL; temp->link[1] = NULL;
						temp->link[2] = NULL; temp->link[3] = NULL;
						temp->middle_x = 0; temp->middle_y = 0; //3주차에서 추가된 부분
						graph[number++] = temp;
					}
				}
			}

			//초기화된 graph의 vertex를 비교하면서 edge를 연결
			number = 0;
			for (i = 0; i < num_of_terms; i++) {
				node_pointer ptr = graph[number];
				if (maze[ptr->row - 1][ptr->col] == 4) ptr->link[0] = graph[ptr->index - width_graph]; //위의 벽이 뚫려있으면
				if (maze[ptr->row][ptr->col + 1] == 4) ptr->link[1] = graph[ptr->index + 1]; //우측 벽이 뚫려있으면
				if (maze[ptr->row + 1][ptr->col] == 4) ptr->link[2] = graph[ptr->index + width_graph]; //아래 벽이 뚫려있으면
				if (maze[ptr->row][ptr->col - 1] == 4) ptr->link[3] = graph[ptr->index - 1]; //좌측 벽이 뚫려있으면
				number++;
			}
			
		}
		else {
			cout << "Needs a '.maz' extension" << endl;
			return false;
		}
	}
	else {
		cout << "File open error" << endl;
		return false;
	}
}
void ofApp::freeMemory() {
	int i;
	for (i = 0; i < HEIGHT; i++) {
		delete[] maze[i]; 
	}
	delete[] maze; //2차원 배열
	delete[] graph; //1차원 배열

	//3주차 dfs에 필요한 것들(조건부인데 삭제하는게 맞나? dfs를 한 경우에만 삭제)
	if (isDFS) {
		delete[] stack;
		delete[] visited;
		delete[] all;
	}
	
	if (isBFS) {
		delete[] queue;
		delete[] visited;
		delete[] all;
		delete[] stack;
	}
	
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	int i;  bool found = false; int dir = 0;
	int current_pos; node_pointer ptr; 	top = -1; top_all = -1;
	int exit = num_of_terms - 1; //node의 index기준 마지막 노드가 미로 종점

	visited = new int[num_of_terms];
	stack = new int[num_of_terms];
	all = new int[num_of_terms * 4];

	for (i = 0; i < num_of_terms; i++) visited[i] = 0;
	visited[0] = 1; //시작점은 0번째 노드(좌측 상단)
	current_pos = 0;//0번째 노드가 현 위치
	push(current_pos);

	while (top > -1 && found == false) {
		while (current_pos == stack[top] && top > 0) {
			current_pos = pop();
		}
		current_pos = pop(); all[++top_all] = current_pos;
		ptr = graph[current_pos];
		while (dir < 4 && found == false) {
			if (ptr->link[dir] == NULL || visited[ptr->link[dir]->index] == 1) { //벽이라서 or 이미 지나온 곳이라서 못가는 방향일 때
				dir++;
			}
			else if (ptr->link[dir]->index == exit) { //탈출구에 도착한 경우
				if(stack[top] != ptr->index) push(current_pos);
				found = true;
				ptr = ptr->link[dir];
				current_pos = ptr->index;
				push(current_pos);
			}
			else { //이동 할 수 있는 경우
				if (stack[top] != ptr->index) push(current_pos);
				ptr = ptr->link[dir];
				current_pos = ptr->index;
				push(current_pos);
				visited[current_pos] = 1;
				dir = 0; //새로운 지점이니깐 dir도 초기화
			}
		}
		dir = 0;
		//found가 true가 되었으면 마지막 지점에 ptr이 위치하고 stack도 마지막 지점의 index까지 쌓아져 있는 상황에서 while loop 바로 탈출
	}

	if (found == true) {
		cout << "The path is ";
		for (i = 0; i <= top; i++) {
			current_pos = stack[i];
			cout << current_pos << " ";
		}
		cout << endl;
	}
	isDFS = 1; //DFS 경로 찾기 완료

	return true;
}
void ofApp::dfsdraw()
{
	int i, j, number, x1, y1, x2, y2;
	ofSetColor(200);
	ofSetLineWidth(5);

	//전체 경로 먼저 그리고
	for (i = 0; i < top_all; i++) { //마지막 전까지(2개씩 묶으니깐)
		number = all[i];
		x1 = graph[number]->middle_x;
		y1 = graph[number]->middle_y;
		x2 = graph[all[i + 1]]->middle_x;
		y2 = graph[all[i + 1]]->middle_y;
		ofDrawLine(x1, y1, x2, y2);
	}

	ofSetColor(0, 0, 0);
	//그 위에 최단 경로 덧대서 그리기
	for (i = 0; i < top; i++) {
		number = stack[i];
		x1 = graph[number]->middle_x;
		y1 = graph[number]->middle_y;
		x2 = graph[stack[i + 1]]->middle_x;
		y2 = graph[stack[i + 1]]->middle_y;
		ofDrawLine(x1, y1, x2, y2);
	}
	
}

void ofApp::push(int current_pos) {
	if (top == num_of_terms - 1) {
		fprintf(stderr, "Stack is full");
		ofExit(1);
	}
	stack[++top] = current_pos;
	all[++top_all] = current_pos;
}

int ofApp::pop() {
	if (top == -1) {
		fprintf(stderr, "Stack is empty");
		ofExit(1);
	}
	return stack[top--];
}

bool ofApp::BFS()
{
	int i, j, current_pos; int dir; bool found = false; int flag = 0;
	int exit = num_of_terms - 1;
	front = -1; rear = -1; rear_all = -1;
	node_pointer ptr;

	visited = new int[num_of_terms];
	queue = new int[num_of_terms];
	all = new int[num_of_terms * 4];
	stack = new int[num_of_terms];

	for (i = 0; i < num_of_terms; i++) visited[i] = 0;
	visited[0] = 1; //시작점은 0번째 노드(좌측 상단)
	current_pos = 0;//0번째 노드가 현 위치
	addq(current_pos);

	cout << "The path is: 0 ";

	while (front != rear && found == false) {
		current_pos = deleteq();
		ptr = graph[current_pos];
		all[++rear_all] = ptr->index;

		for (dir = 0; dir < 4; dir++) {
			if (ptr->link[dir] != NULL) { //벽이 아니고 
				if (visited[ptr->link[dir]->index] == 0) { //방문된 곳도 아닌 경우(갈 수 있는 방향)
					if (flag == 1) all[++rear_all] = ptr->index;
					addq(ptr->link[dir]->index);
					cout << ptr->link[dir]->index << " ";
					visited[ptr->link[dir]->index] = 1;
					flag = 1;

					if (ptr->link[dir]->index == exit) {//탈출구인 경우
						found = true;
					} 
				}
			}
		}
		if (flag == 1) all[++rear_all] = ptr->index; //다른 위치랑 섞이지 않도록
		flag = 0; //다음 위치로 넘어갔을 때를 위한 초기화
	}
	cout << endl;
	
	//최단 거리를 stack에 넣어서 구하기
	top = -1;
	for (j = rear; j >= 0; j--) if (queue[j] == exit) break;
	stack[++top] = queue[j];
	int target = j;
	for (i = target; i > 0; i--) {
		//4방위를 조사해서 인접해 있지 않다면 다음꺼로 패스
		if (queue[i - 1] != queue[target] - 1 &&
			queue[i - 1] != queue[target] + 1 &&
			queue[i - 1] != queue[target] - width_graph &&
			queue[i - 1] != queue[target] + width_graph) continue;
		//4방위를 조사해서 인접해 있는게 있었어, 그런데 지나갈 수 없는 벽이면
		if (queue[i - 1] == queue[target] - 1 ||
			queue[i - 1] == queue[target] + 1 ||
			queue[i - 1] == queue[target] - width_graph ||
			queue[i - 1] == queue[target] + width_graph) {
			if (queue[i - 1] == queue[target] - 1) {
				if (graph[queue[target]]->link[3] == NULL) continue;
			}
			else if (queue[i - 1] == queue[target] + 1) {
				if (graph[queue[target]]->link[1] == NULL) continue;
			}
			else if (queue[i - 1] == queue[target] - width_graph) {
				if (graph[queue[target]]->link[0] == NULL) continue;
			}
			else if (queue[i - 1] == queue[target] + width_graph) {
				if (graph[queue[target]]->link[2] == NULL) continue;
			}
		}
		stack[++top] = queue[i - 1];
		target = i - 1;
	}
	cout << "The shortest path: ";
	for (i = top; i >= 0; i--) {
		cout << stack[i] << " ";
	}
	cout << endl;
	
	isBFS = 1;
	return true;
}

void ofApp::bfsdraw()
{
	int i, j, number, x1, y1, x2, y2;
	ofSetColor(200);
	ofSetLineWidth(5);

	//다른방법: visited가 1인 지점에서 인접 칸이 1이면 연결하기(전체 경로 그리기)

	//전체 경로 먼저 그리고
	for (i = 0; i < rear_all; i++) { //마지막 전까지(2개씩 묶으니깐)
		number = all[i];
		//다음 경로가 지금 경로의 근접이 아니라면 패스
		if (all[i + 1] != all[i] - 1 &&
			all[i + 1] != all[i] + 1 &&
			all[i + 1] != all[i] - height_graph &&
			all[i + 1] != all[i] + height_graph) continue;

		x1 = graph[number]->middle_x;
		y1 = graph[number]->middle_y;
		x2 = graph[all[i + 1]]->middle_x;
		y2 = graph[all[i + 1]]->middle_y;
		ofDrawLine(x1, y1, x2, y2);
	}

	ofSetColor(0, 0, 0);
	//그 위에 최단 경로 덧대서 그리기
	for (i = top; i > 0; i--) {
		number = stack[i];
		x1 = graph[number]->middle_x;
		y1 = graph[number]->middle_y;
		x2 = graph[stack[i - 1]]->middle_x;
		y2 = graph[stack[i - 1]]->middle_y;
		ofDrawLine(x1, y1, x2, y2);
	}
}

void ofApp::addq(int current_pos) 
{
	if (rear == num_of_terms - 1) {
		fprintf(stderr, "Queue is full\n");
		ofExit(1);
	}
	queue[++rear] = current_pos;
	all[++rear_all] = current_pos;
}

int ofApp::deleteq() 
{
	if (front == rear) {
		fprintf(stderr, "Queue is empty\n");
		ofExit(1);
	}
	return queue[++front];
}