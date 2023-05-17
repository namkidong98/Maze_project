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
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	if (isOpen) {
		for (i = 0; i < height_graph; i++) {
			for (j = 0; j < width_graph; j++) {
				graph[number]->middle_x = x + line_length/2; //3���� �߰�
				graph[number]->middle_y = y + line_length/2; //3���� �߰�
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

			//int�� 2���� �迭 maze�� ���� �Ҵ�
			maze = new int* [HEIGHT];
			for (i = 0; i < HEIGHT; i++) {
				maze[i] = new int[WIDTH];
			}
			i = 0;
			
			cout << "HEIGHT: " << HEIGHT << " WIDTH: " << WIDTH << endl;

			//������ ������ �о maze�� ����
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it, i++) {
				string line = *it;
				for (j = 0; j < WIDTH; j++) {
					if (line[j] == '+') maze[i][j] = 1;
					else if (line[j] == '-') maze[i][j] = 2;
					else if (line[j] == '|') maze[i][j] = 3;
					else if ((i % 2 == 1) && (j % 2 == 0) && line[j] == ' ') maze[i][j] = 4; //������ ���κ�(���̴� Ȧ��, �ʺ�� ¦��)
					else if ((i % 2 == 0) && (j % 2 == 1) && line[j] == ' ') maze[i][j] = 4; //������ ���κ�(���̴� ¦��, �ʺ�� Ȧ��)
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

			//graph�� ����� ���� �ʿ��� ���۾�
			int number = 0;
			height_graph = HEIGHT / 2;
			width_graph = WIDTH / 2;
			num_of_terms = height_graph * width_graph;
			graph = new node_pointer[num_of_terms];

			//maze�� �о ���ο� �ڷᱸ���� graph�� ��� vertex�� ���� �� �ʱ�ȭ
			for (i = 0; i < HEIGHT; i++) {
				for (j = 0; j < WIDTH; j++) {
					if ((i % 2 == 1) && (j % 2 == 1)) { //�Ѵ� Ȧ���� ��찡 maze�� �濡 �ش��Ѵ�
						node_pointer temp = new node;
						temp->row = i; temp->col = j; //���θ��� node�� �⺻ ���� �ʱ�ȭ
						temp->index = number; 
						temp->link[0] = NULL; temp->link[1] = NULL;
						temp->link[2] = NULL; temp->link[3] = NULL;
						temp->middle_x = 0; temp->middle_y = 0; //3�������� �߰��� �κ�
						graph[number++] = temp;
					}
				}
			}

			//�ʱ�ȭ�� graph�� vertex�� ���ϸ鼭 edge�� ����
			number = 0;
			for (i = 0; i < num_of_terms; i++) {
				node_pointer ptr = graph[number];
				if (maze[ptr->row - 1][ptr->col] == 4) ptr->link[0] = graph[ptr->index - width_graph]; //���� ���� �շ�������
				if (maze[ptr->row][ptr->col + 1] == 4) ptr->link[1] = graph[ptr->index + 1]; //���� ���� �շ�������
				if (maze[ptr->row + 1][ptr->col] == 4) ptr->link[2] = graph[ptr->index + width_graph]; //�Ʒ� ���� �շ�������
				if (maze[ptr->row][ptr->col - 1] == 4) ptr->link[3] = graph[ptr->index - 1]; //���� ���� �շ�������
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
	delete[] maze; //2���� �迭
	delete[] graph; //1���� �迭

	//3���� dfs�� �ʿ��� �͵�(���Ǻ��ε� �����ϴ°� �³�? dfs�� �� ��쿡�� ����)
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

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	int i;  bool found = false; int dir = 0;
	int current_pos; node_pointer ptr; 	top = -1; top_all = -1;
	int exit = num_of_terms - 1; //node�� index���� ������ ��尡 �̷� ����

	visited = new int[num_of_terms];
	stack = new int[num_of_terms];
	all = new int[num_of_terms * 4];

	for (i = 0; i < num_of_terms; i++) visited[i] = 0;
	visited[0] = 1; //�������� 0��° ���(���� ���)
	current_pos = 0;//0��° ��尡 �� ��ġ
	push(current_pos);

	while (top > -1 && found == false) {
		while (current_pos == stack[top] && top > 0) {
			current_pos = pop();
		}
		current_pos = pop(); all[++top_all] = current_pos;
		ptr = graph[current_pos];
		while (dir < 4 && found == false) {
			if (ptr->link[dir] == NULL || visited[ptr->link[dir]->index] == 1) { //���̶� or �̹� ������ ���̶� ������ ������ ��
				dir++;
			}
			else if (ptr->link[dir]->index == exit) { //Ż�ⱸ�� ������ ���
				if(stack[top] != ptr->index) push(current_pos);
				found = true;
				ptr = ptr->link[dir];
				current_pos = ptr->index;
				push(current_pos);
			}
			else { //�̵� �� �� �ִ� ���
				if (stack[top] != ptr->index) push(current_pos);
				ptr = ptr->link[dir];
				current_pos = ptr->index;
				push(current_pos);
				visited[current_pos] = 1;
				dir = 0; //���ο� �����̴ϱ� dir�� �ʱ�ȭ
			}
		}
		dir = 0;
		//found�� true�� �Ǿ����� ������ ������ ptr�� ��ġ�ϰ� stack�� ������ ������ index���� �׾��� �ִ� ��Ȳ���� while loop �ٷ� Ż��
	}

	if (found == true) {
		cout << "The path is ";
		for (i = 0; i <= top; i++) {
			current_pos = stack[i];
			cout << current_pos << " ";
		}
		cout << endl;
	}
	isDFS = 1; //DFS ��� ã�� �Ϸ�

	return true;
}
void ofApp::dfsdraw()
{
	int i, j, number, x1, y1, x2, y2;
	ofSetColor(200);
	ofSetLineWidth(5);

	//��ü ��� ���� �׸���
	for (i = 0; i < top_all; i++) { //������ ������(2���� �����ϱ�)
		number = all[i];
		x1 = graph[number]->middle_x;
		y1 = graph[number]->middle_y;
		x2 = graph[all[i + 1]]->middle_x;
		y2 = graph[all[i + 1]]->middle_y;
		ofDrawLine(x1, y1, x2, y2);
	}

	ofSetColor(0, 0, 0);
	//�� ���� �ִ� ��� ���뼭 �׸���
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
	visited[0] = 1; //�������� 0��° ���(���� ���)
	current_pos = 0;//0��° ��尡 �� ��ġ
	addq(current_pos);

	cout << "The path is: 0 ";

	while (front != rear && found == false) {
		current_pos = deleteq();
		ptr = graph[current_pos];
		all[++rear_all] = ptr->index;

		for (dir = 0; dir < 4; dir++) {
			if (ptr->link[dir] != NULL) { //���� �ƴϰ� 
				if (visited[ptr->link[dir]->index] == 0) { //�湮�� ���� �ƴ� ���(�� �� �ִ� ����)
					if (flag == 1) all[++rear_all] = ptr->index;
					addq(ptr->link[dir]->index);
					cout << ptr->link[dir]->index << " ";
					visited[ptr->link[dir]->index] = 1;
					flag = 1;

					if (ptr->link[dir]->index == exit) {//Ż�ⱸ�� ���
						found = true;
					} 
				}
			}
		}
		if (flag == 1) all[++rear_all] = ptr->index; //�ٸ� ��ġ�� ������ �ʵ���
		flag = 0; //���� ��ġ�� �Ѿ�� ���� ���� �ʱ�ȭ
	}
	cout << endl;
	
	//�ִ� �Ÿ��� stack�� �־ ���ϱ�
	top = -1;
	for (j = rear; j >= 0; j--) if (queue[j] == exit) break;
	stack[++top] = queue[j];
	int target = j;
	for (i = target; i > 0; i--) {
		//4������ �����ؼ� ������ ���� �ʴٸ� �������� �н�
		if (queue[i - 1] != queue[target] - 1 &&
			queue[i - 1] != queue[target] + 1 &&
			queue[i - 1] != queue[target] - width_graph &&
			queue[i - 1] != queue[target] + width_graph) continue;
		//4������ �����ؼ� ������ �ִ°� �־���, �׷��� ������ �� ���� ���̸�
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

	//�ٸ����: visited�� 1�� �������� ���� ĭ�� 1�̸� �����ϱ�(��ü ��� �׸���)

	//��ü ��� ���� �׸���
	for (i = 0; i < rear_all; i++) { //������ ������(2���� �����ϱ�)
		number = all[i];
		//���� ��ΰ� ���� ����� ������ �ƴ϶�� �н�
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
	//�� ���� �ִ� ��� ���뼭 �׸���
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