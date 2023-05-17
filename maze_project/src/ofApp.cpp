#include "ofApp.h"
#include <iostream>
#include <time.h>
#include <fstream>


using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = 0; //DFS가 완료되지 않았다는 flag
	isBFS = 0; //BFS가 완료되지 않았다는 flag
	isOpen = 0; //파일이 아직 열리지 않았다는 의미의 flag
	HEIGHT = 0; //미로의 높이(초기화)
	WIDTH = 0; //미로의 너비(초기화)

	isDFS_Animation = 0;

	Jerry.load("./Jerry.png");
	Cheese.load("./cheese.png");
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
	menu->AddPopupItem(hPopup, "Make random maze", false, false);
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
	menu->AddPopupItem(hPopup, "Show BFS",false, false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check
	menu->AddPopupItem(hPopup, "Clear", false, false);
	
	hPopup = menu->AddPopupMenu(hMenu, "Edit");
	menu->AddPopupItem(hPopup, "Random ENTRANCE", false, false); // Checked
	menu->AddPopupItem(hPopup, "Random EXIT", false, false); // Checked

	hPopup = menu->AddPopupMenu(hMenu, "Animation");
	menu->AddPopupItem(hPopup, "DFS Animation", false, false); // Checked
	menu->AddPopupItem(hPopup, "BFS Animation", false, false); // Checked

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
	srand((unsigned int)time(NULL));

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}

	if (title == "Make random maze") {
		make_maze();
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

	if (title == "Clear") {
		isDFS = 0; //DFS 결과를 없애서 DFS가 출력된다면 이를 없애는 역할
		isBFS = 0; //BFS 결과를 없애서 BFS가 출력된다면 이를 없애는 역할
		cout << "Cleared" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	if (title == "Random ENTRANCE") {
		entrance = rand() % num_of_terms; //시작점을 num_of_terms 중 하나로 랜덤하게 설정한다
		cout << "New Entrance is "<< entrance << endl;
	}

	if (title == "Random EXIT") {
		exit = rand() % num_of_terms; //탈출구(도착점)을 num_of_terms 중 하나로 랜덤하게 설정한다
		cout << "New Exit is " << exit << endl;
	}

	if (title == "DFS Animation") {
		if (isDFS) {
			isDFS_Animation = 1;
			current_top = 0;
		}
	}
	if (title == "BFS Animation") {
		if (isBFS) {
			isBFS_Animation = 1;
			current_top = top;
		}
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
	if (isDFS_Animation) {
		ofSetFrameRate(3);
		current = stack[current_top++];
		if (current_top > top) {
			isDFS_Animation = 0;
			ofSetFrameRate(60);
		}
	}
	if (isBFS_Animation) {
		ofSetFrameRate(3);
		current = stack[current_top--];
		if (current_top < 0) {
			isBFS_Animation = 0;
			ofSetFrameRate(60);
		} 
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	char str[256];
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j; int number = 0; //number는 미로 방의 index 역할을 한다
	int x = 30, y = 5; //starting point
	int line_length = 70; //미로의 방 사이의 거리, 즉 벽의 길이를 의미한다
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	if (isOpen) {
		for (i = 0; i < height_graph; i++) {
			for (j = 0; j < width_graph; j++) {
				graph[number]->middle_x = x + line_length/2; //3주차 추가
				graph[number]->middle_y = y + line_length/2; //3주차 추가
				if (graph[number]->link[0] == NULL) ofDrawLine(x, y, x + line_length, y); //위쪽 벽이 막혀 있으면 위쪽 벽 그리기
				if (graph[number]->link[1] == NULL) ofDrawLine(x + line_length, y, x + line_length, y + line_length); //우측 벽이 막혀있을 때 우측 벽 그리기
				if (graph[number]->link[2] == NULL) ofDrawLine(x, y + line_length, x + line_length, y + line_length); //하단 벽이 막혀있을 때 하단 벽 그리기
				if (graph[number]->link[3] == NULL) ofDrawLine(x, y, x, y + line_length); //좌측 벽이 막혀 있을 때 좌측 벽 그리기
				x += line_length; //한 개의 노드(혹은 vertex)를 처리하고 다음 노드로 넘어가기 위해 기준이 되는 x좌표를 벽의 길이 만큼 증가시킨다
				number++; //number는 미로 방의 index 역할을 하기 때문에 다음 방으로 넘어갈 때 1을 증가시킨다
			}
			x = 30; //leftmost(starting point)
			y += line_length; //한 줄이 끝나고 다음 줄로 내려가기 때문에 y축 좌표를 1개 벽 길이만큼 증가시킨다
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

	if (isOpen) {
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofSetColor(255, 255, 255);
		if (isDFS_Animation || isBFS_Animation) {
			Jerry.draw(graph[current]->middle_x, graph[current]->middle_y, 50, 50);
			Cheese.draw(graph[exit]->middle_x, graph[exit]->middle_y, 50, 50);
		}
		else {
			Jerry.draw(graph[entrance]->middle_x, graph[entrance]->middle_y, 50, 50);
			Cheese.draw(graph[exit]->middle_x, graph[exit]->middle_y, 50, 50);
		}

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

//2주차 실습
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

			//파일을 한번 읽어서 HEIGHT와 WIDTH를 설정한다
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) { //파일의 첫줄부터 마지막줄까지
				HEIGHT++; //높이를 1씩 더해서 최종 높이를 계산
				string line = *it; //파일의 줄을 string 변수로 설정해서0
				WIDTH = line.size(); //파일의 줄의 길이를 WIDTH에 저장한다
			}

			//int형 2차원 배열 maze를 동적 할당
			maze = new int* [HEIGHT];
			for (i = 0; i < HEIGHT; i++) {
				maze[i] = new int[WIDTH];
			}
			i = 0;
			
			cout << "HEIGHT: " << HEIGHT << " WIDTH: " << WIDTH << endl;

			//파일을 다시 읽어서 파일 내용을 maze에 저장
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it, i++) {
				string line = *it;
				for (j = 0; j < WIDTH; j++) {
					if (line[j] == '+') maze[i][j] = 1; //모서리는 1로 저장
					else if (line[j] == '-') maze[i][j] = 2; //가로벽은 2로 저장
					else if (line[j] == '|') maze[i][j] = 3; //세로벽은 3으로 저장
					else if ((i % 2 == 1) && (j % 2 == 0) && line[j] == ' ') maze[i][j] = 4; //없어진 세로벽(높이는 홀수, 너비는 짝수)
					else if ((i % 2 == 0) && (j % 2 == 1) && line[j] == ' ') maze[i][j] = 4; //없어진 가로벽(높이는 짝수, 너비는 홀수)
					else maze[i][j] = 0; //위에 해당하지 않는 것은 빈 방, 즉 미로의 방으로 생각하고 0으로 저장
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

			//위에 adjacency matrix로 파일의 정보를 읽어왔다면 이후의 구현들의 편이를 위해 adjacency list로 변환한다.
			
			//graph를 만들기 위해 필요한 선작업
			int number = 0;
			height_graph = HEIGHT / 2;
			width_graph = WIDTH / 2;
			num_of_terms = height_graph * width_graph; //adjacency list에서는 전체 미로의 방의 개수가 중요하므로 너비x높이로 방의 개수를 구한다
			graph = new node_pointer[num_of_terms]; //구한 미로의 방의 개수만큼 graph를 동적 할당

			//setup에 설정해놓으면 num_of_terms가 아직 없어서 exit에 이상한 값이 설정된다. 따라서 파일을 읽고 num_of_terms를 결정한 다음 바로 entrance와 exit의 default값을 설정한다
			entrance = 0; //처음에는 출발점이 좌측 상단이고
			exit = num_of_terms - 1; //도착점은 우측 하단이다

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

	//3주차 dfs에 필요한 것들
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


//3주차 실습
bool ofApp::DFS()//DFS탐색을 하는 함수
{
	int i;  bool found = false; int dir = 0;
	int current_pos; node_pointer ptr; 	top = -1; top_all = -1;

	visited = new int[num_of_terms];
	stack = new int[num_of_terms];
	all = new int[num_of_terms * 4];

	for (i = 0; i < num_of_terms; i++) visited[i] = 0;
	visited[entrance] = 1; //시작점을 방문한 것으로 표시
	current_pos = entrance;//시작점의 노드가 현 위치
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

//3주차 과제
bool ofApp::BFS()
{
	int i, j, current_pos; int dir; bool found = false; int flag = 0;
	front = -1; rear = -1; rear_all = -1;
	node_pointer ptr;

	visited = new int[num_of_terms]; //방문했는지를 파악하기 위해
	queue = new int[num_of_terms]; //BFS는 queue를 통해 구현
	all = new int[num_of_terms * 4]; //모든 경로를 담기 위해 활용
	stack = new int[num_of_terms]; //나중에 최단 거리 경로를 추적하기 위해 사용

	for (i = 0; i < num_of_terms; i++) visited[i] = 0;

	visited[entrance] = 1; //시작점은 0번째 노드(좌측 상단)
	current_pos = entrance;//0번째 노드가 현 위치
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
					visited[ptr->link[dir]->index] = 1; //새로 간 노드를 방문한 표시를 남기고
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
		stack[++top] = queue[i - 1]; //스택에 추가하고
		target = i - 1; //해당 부분으로 넘어간다
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

		x1 = graph[number]->middle_x; //이번 단계의 좌표
		y1 = graph[number]->middle_y;
		x2 = graph[all[i + 1]]->middle_x;
		y2 = graph[all[i + 1]]->middle_y; //1단계 다음의 좌표
		ofDrawLine(x1, y1, x2, y2);
	}

	ofSetColor(0, 0, 0);
	//그 위에 최단 경로 덧대서 그리기
	for (i = top; i > 0; i--) {
		number = stack[i];
		x1 = graph[number]->middle_x; //이번 차례의 좌표(x,y)
		y1 = graph[number]->middle_y;
		x2 = graph[stack[i - 1]]->middle_x; //1 단계 전의 좌표
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

//----------------------수정 및 개선----------------------------//

//1. 미로 1주차의 완전 미로 생성을 윈도우 안에서 할 수 있도록 변형하여 포함
void ofApp::Init_maze(int temp_height, int temp_width, int** maze) {
	int i, j;
	//maze 초기화
	for (i = 0; i < temp_height; i++) { //height * 2 + 1
		for (j = 0; j < temp_width; j++) { //width * 2 + 1
			if ((i % 2) == 0 && (j % 2) == 0) maze[i][j] = 1; //행이 짝수, 열도 짝수인 경우는 모서리
			if ((i % 2) == 0 && (j % 2) == 1) maze[i][j] = 2; //행은 짝수, 열이 홀수인 경우는 가로벽
			if ((i % 2) == 1 && (j % 2) == 0) maze[i][j] = 3; //행이 홀수, 열이 짝수인 경우는 세로벽
			if ((i % 2) == 1 && (j % 2) == 1) maze[i][j] = 0; //행이 홀수, 열이 홀수인 경우에는 방(room)
			//세로벽이나 가로벽이 없어진 자리는 4으로 입력 --> ' '(빈칸)으로 출력
		}
	}
}

void ofApp::print_maze(int temp_height, int temp_width, int** maze) {
	int i, j, command;

	for (i = 0; i < temp_height; i++) {
		for (j = 0; j < temp_width; j++) {
			command = maze[i][j];
			switch (command) {
			case 1: //1은 모서리를 의미한다
				cout << "+"; break;
			case 2: //2는 가로벽을 의미한다
				cout << "-"; break;
			case 3: //3은 세로벽을 의미한다
				cout << "|"; break;
			case 4: //4는 가로벽이나 세로벽 중에 허물어져서 빈칸인 경우를 의미한다
				cout << " "; break;
			default: //나머지의 경우는 미로의 방을 의미하며 0의 값이다
				cout << " ";
			}
		}
		cout << endl;
	}
}

void ofApp::fwrite_maze(int temp_height, int temp_width, int** maze) {
	int i, j, command;
	ofstream writeFile;
	writeFile.open("./data/maze.maz"); //상대경로를 사용해서 bin 안의 data에 'maze.maz'으로 파일을 생성한다
	if (writeFile.is_open())  //파일이 열렸는지 확인
	{
		for (i = 0; i < temp_height; i++) {
			for (j = 0; j < temp_width; j++) {
				command = maze[i][j];
				switch (command) {
				case 1: //1은 모서리를 의미한다
					writeFile << "+"; break; 
				case 2: //2는 가로벽을 의미한다
					writeFile << "-"; break;
				case 3: //3은 세로벽을 의미한다
					writeFile << "|"; break;
				case 4: //4는 가로벽이나 세로벽 중에 허물어져서 빈칸인 경우를 의미한다
					writeFile << " "; break;
				default: //나머지의 경우는 미로의 방을 의미하며 0의 값이다
					writeFile << " ";
				}
			}
			if (i < temp_height - 1) writeFile << endl;
		}
		cout << "Making Random Maze Succeed" << endl;
	}
	writeFile.close();
}

void ofApp::Eller_algorithm(int temp_height, int temp_width, int** maze) {
	int i, j, t, p; 
	int select; //랜덤으로 0, 1을 설정해서 벽을 지울지 말지 결정할 때 사용하는 변수
	int number = 10; //미로의 방의 집합을 구분하기 위해 사용하는 변수(10부터 시작해서 집합이 새로 등장할 때마다 1씩 증가해서 설정해준다)
	int flag = 0; //한 행에서 같은 집합에 속한 방이 수직 경로를 내렸는지 판단하는 변수
	int possible = 0; //다음 단계에서 수직 경로를 내릴 수 있는지 판단하는 변수 
	int temp; //두 인접한 미로의 방이 벽을 허물고 집합을 합칠 때 임시로 값을 저장하기 위해 사용하는 변수
	srand((unsigned int)time(NULL));
	
	//첫번째 줄, 서로 다른 집합에 속하게 초기화
	for (i = 0; i < temp_width; i++) {
		if ((i % 2) == 1) {
			maze[1][i] = number++;
		}
	}
	//첫번째 줄, 벽 랜덤으로 제거 or 유지
	for (j = 1; j < temp_width - 1; j++) { //왼쪽 벽이랑 오른쪽 벽은 제거 대상이 아니기 때문
		if ((j % 2) == 0) {
			select = rand() % 2; //0 또는 1이 저장 (0이면 벽 지우기, 1이면 벽 유지)
			if (select == 0) {
				maze[1][j] = 4; //세로벽 지우기(지워진 벽은 4로 간주)
				maze[1][j + 1] = maze[1][j - 1]; //벽이 없어지면 벽 기준 왼쪽과 같은 집합에 속하도록 설정
			}
		}
	}

	for (i = 1; i < temp_height - 3; i += 2) { //마지막 줄 전까지 반복(수직 경로 & 행 정리)
		//수직 경로 만들기(1. 무조건 집합당 1개의 수직 경로는 있어야한다/ 2. 있다면 그 다음부터는 임의로 정한다)
		//1. 한줄을 검색해서 그 줄에 1개만 있는 다른 집합이라면, 무조건 수직 경로를 만든다
		//2. 여러 개의 집합이 있는 경우에는, 임의로 수직 경로를 만든다. 마지막까지 안만들었으면 마지막꺼의 수직 경로를 필수적으로 생성한다
		for (j = 1; j < temp_width - 3; j += 2) {
			if (maze[i][j] == maze[i][j + 2] && possible == 0) {
				select = rand() % 2;
				if (select == 0) {
					maze[i + 1][j] = 4; //가로벽 제거
					maze[i + 2][j] = maze[i][j]; //아래칸을 위칸과 같은 집합으로 설정
					flag = 1; possible = 1;
				}

				if (j + 2 == temp_width - 2) { //마지막칸 처리
					if (flag == 1 && possible == 0) {
						select = rand() % 2;
						if (select == 0) {
							maze[i + 1][j + 2] = 4; //허물어진 벽으로 기록(4)
							maze[i + 2][j + 2] = maze[i][j + 2]; //같은 집합에 속하도록 설정
						}
					}
					else if (flag == 0) { //집합 내 수직 경로가 아직 없음
						maze[i + 1][j + 2] = 4; //수직 경로가 없으니 무조건 수직 경로가 나야함
						maze[i + 2][j + 2] = maze[i][j + 2];
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
				maze[i + 1][j] = 4; //가로벽 제거
				maze[i + 2][j] = maze[i][j]; //아래칸을 위칸과 같은 집합으로 설정

				flag = 0;
				possible = 0;

				if (j + 2 == temp_width - 2) { //마지막칸 처리
					maze[i + 1][j + 2] = 4; //가로벽 제거(1개밖에 없는 집합이니깐)
					maze[i + 2][j + 2] = maze[i][j + 2];
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

				if (j + 2 == temp_width - 2) { //마지막 칸 처리
					maze[i + 1][j + 2] = 4; //가로벽 제거(1개밖에 없는 집합이니깐)
					maze[i + 2][j + 2] = maze[i][j + 2];
				}
			}
		}
		flag = 0;
		possible = 0;

		//아랫 줄을 읽어서 0으로 초기화된 값이 안 바뀌었으면, 수직경로가 없던 것이고 이경우에는 새로운 집합으로 만든다
		for (j = 1; j < temp_width - 1; j += 2) {
			if (maze[i + 2][j] == 0) maze[i + 2][j] = number++;
		}

		if (i + 2 == temp_height - 2) break; //마지막 줄의 경우에는 벽 제거의 원칙이 다르기 때문에 종료
		//행에서 임의로 벽 제거
		for (j = 1; j < temp_width - 3; j += 2) {
			if (maze[i + 2][j] != maze[i + 2][j + 2]) {
				select = rand() % 2; //벽을 지울지 말지 선택
				if (select == 0) { //벽을 지운다면
					maze[i + 2][j + 1] = 4; //세로벽 허물기(우측)
					temp = maze[i + 2][j + 2]; //오른쪽 집합과 동일한 집합들을 찾기 위해서 temp에 임시 저장
					maze[i + 2][j + 2] = maze[i + 2][j];//오른쪽 집합을 왼쪽 집합에 병합
					for (p = 1; p < temp_height - 1; p += 2) { //미로를 탐색하면서
						for (t = 1; t < temp_width - 1; t += 2) {
							if (maze[p][t] == temp) maze[p][t] = maze[i + 2][j];//temp와 같은 집합이었던 것도 왼쪽 집합으로 병합
						}
					}
				}
			}

		}
	}

	//마지막줄 처리: 인접해 있으며, 서로 다른 집합에 속한 방들 사이의 모든 벽을 제거
	for (j = 1; j < temp_width - 3; j += 2) {
		if (maze[temp_height - 2][j] != maze[temp_height - 2][j + 2]) {
			maze[temp_height - 2][j + 1] = 4; //가로벽 제거

			temp = maze[temp_height - 2][j + 2]; //오른쪽 집합과 동일한 집합들을 찾기 위해서 temp에 임시 저장
			maze[temp_height - 2][j + 2] = maze[temp_height - 2][j]; //오른쪽 집합을 왼쪽 집합에 병합
			for (p = 0; p < temp_height; p++) { //미로를 탐색하면서
				for (t = 0; t < temp_width; t++) {
					if (maze[p][t] == temp) maze[p][t] = maze[temp_height - 2][j]; //temp와 같은 집합이었던 것도 왼쪽 집합으로 병합
				}
			} 

		}
	}
}

void ofApp::make_maze() {
	int i, j, temp_height, temp_width;
	int** temp_maze; //새로 완전 미로를 만들 때에만 사용할 것이기 때문에 지역 변수로 이차원 배열의 temp_maze를 사용한다
	cout << "HEIGHT: ";
	cin >> temp_height;
	cout << "WIDTH: ";
	cin >> temp_width;
	
	//adjacency matrix의 형태이기 때문에 입력받은 hegiht와 width의 값에 2배에 1을 더해줘야 한다
	temp_width = temp_width * 2 + 1;
	temp_height = temp_height * 2 + 1;

	temp_maze = new int* [temp_height]; //입력받은 높이와 너비에 맞게 크기를 동적할당 한다
	for (i = 0; i < temp_height; i++) {
		temp_maze[i] = new int[temp_width];
	}

	//Init_maze함수를 통해 adjacency matrix 형태의 temp_maze를 초기화한다
	Init_maze(temp_height, temp_width, temp_maze);

	//엘러 알고리즘을 통해 완전 미로를 생성한다
	Eller_algorithm(temp_height, temp_width, temp_maze);

	//디버깅과 편이를 위해 윈도우 말고 콘솔에 만들어진 완전미로를 출력한다
	print_maze(temp_height, temp_width, temp_maze);

	//maze.maz 파일을 생성하고 만들어진 완전 미로를 저장한다
	fwrite_maze(temp_height, temp_width, temp_maze);

	//동적 할당했던 temp_maze를 해제한다
	for (i = 0; i < temp_height; i++) {
		delete[] temp_maze[i];
	}
	delete[] temp_maze;
}