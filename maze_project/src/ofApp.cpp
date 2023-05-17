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
	isDFS = 0; //DFS�� �Ϸ���� �ʾҴٴ� flag
	isBFS = 0; //BFS�� �Ϸ���� �ʾҴٴ� flag
	isOpen = 0; //������ ���� ������ �ʾҴٴ� �ǹ��� flag
	HEIGHT = 0; //�̷��� ����(�ʱ�ȭ)
	WIDTH = 0; //�̷��� �ʺ�(�ʱ�ȭ)

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
		isDFS = 0; //DFS ����� ���ּ� DFS�� ��µȴٸ� �̸� ���ִ� ����
		isBFS = 0; //BFS ����� ���ּ� BFS�� ��µȴٸ� �̸� ���ִ� ����
		cout << "Cleared" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	if (title == "Random ENTRANCE") {
		entrance = rand() % num_of_terms; //�������� num_of_terms �� �ϳ��� �����ϰ� �����Ѵ�
		cout << "New Entrance is "<< entrance << endl;
	}

	if (title == "Random EXIT") {
		exit = rand() % num_of_terms; //Ż�ⱸ(������)�� num_of_terms �� �ϳ��� �����ϰ� �����Ѵ�
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
	int i, j; int number = 0; //number�� �̷� ���� index ������ �Ѵ�
	int x = 30, y = 5; //starting point
	int line_length = 70; //�̷��� �� ������ �Ÿ�, �� ���� ���̸� �ǹ��Ѵ�
	
	// TO DO : DRAW MAZE; 
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	if (isOpen) {
		for (i = 0; i < height_graph; i++) {
			for (j = 0; j < width_graph; j++) {
				graph[number]->middle_x = x + line_length/2; //3���� �߰�
				graph[number]->middle_y = y + line_length/2; //3���� �߰�
				if (graph[number]->link[0] == NULL) ofDrawLine(x, y, x + line_length, y); //���� ���� ���� ������ ���� �� �׸���
				if (graph[number]->link[1] == NULL) ofDrawLine(x + line_length, y, x + line_length, y + line_length); //���� ���� �������� �� ���� �� �׸���
				if (graph[number]->link[2] == NULL) ofDrawLine(x, y + line_length, x + line_length, y + line_length); //�ϴ� ���� �������� �� �ϴ� �� �׸���
				if (graph[number]->link[3] == NULL) ofDrawLine(x, y, x, y + line_length); //���� ���� ���� ���� �� ���� �� �׸���
				x += line_length; //�� ���� ���(Ȥ�� vertex)�� ó���ϰ� ���� ���� �Ѿ�� ���� ������ �Ǵ� x��ǥ�� ���� ���� ��ŭ ������Ų��
				number++; //number�� �̷� ���� index ������ �ϱ� ������ ���� ������ �Ѿ �� 1�� ������Ų��
			}
			x = 30; //leftmost(starting point)
			y += line_length; //�� ���� ������ ���� �ٷ� �������� ������ y�� ��ǥ�� 1�� �� ���̸�ŭ ������Ų��
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

//2���� �ǽ�
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

			//������ �ѹ� �о HEIGHT�� WIDTH�� �����Ѵ�
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) { //������ ù�ٺ��� �������ٱ���
				HEIGHT++; //���̸� 1�� ���ؼ� ���� ���̸� ���
				string line = *it; //������ ���� string ������ �����ؼ�0
				WIDTH = line.size(); //������ ���� ���̸� WIDTH�� �����Ѵ�
			}

			//int�� 2���� �迭 maze�� ���� �Ҵ�
			maze = new int* [HEIGHT];
			for (i = 0; i < HEIGHT; i++) {
				maze[i] = new int[WIDTH];
			}
			i = 0;
			
			cout << "HEIGHT: " << HEIGHT << " WIDTH: " << WIDTH << endl;

			//������ �ٽ� �о ���� ������ maze�� ����
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it, i++) {
				string line = *it;
				for (j = 0; j < WIDTH; j++) {
					if (line[j] == '+') maze[i][j] = 1; //�𼭸��� 1�� ����
					else if (line[j] == '-') maze[i][j] = 2; //���κ��� 2�� ����
					else if (line[j] == '|') maze[i][j] = 3; //���κ��� 3���� ����
					else if ((i % 2 == 1) && (j % 2 == 0) && line[j] == ' ') maze[i][j] = 4; //������ ���κ�(���̴� Ȧ��, �ʺ�� ¦��)
					else if ((i % 2 == 0) && (j % 2 == 1) && line[j] == ' ') maze[i][j] = 4; //������ ���κ�(���̴� ¦��, �ʺ�� Ȧ��)
					else maze[i][j] = 0; //���� �ش����� �ʴ� ���� �� ��, �� �̷��� ������ �����ϰ� 0���� ����
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

			//���� adjacency matrix�� ������ ������ �о�Դٸ� ������ �������� ���̸� ���� adjacency list�� ��ȯ�Ѵ�.
			
			//graph�� ����� ���� �ʿ��� ���۾�
			int number = 0;
			height_graph = HEIGHT / 2;
			width_graph = WIDTH / 2;
			num_of_terms = height_graph * width_graph; //adjacency list������ ��ü �̷��� ���� ������ �߿��ϹǷ� �ʺ�x���̷� ���� ������ ���Ѵ�
			graph = new node_pointer[num_of_terms]; //���� �̷��� ���� ������ŭ graph�� ���� �Ҵ�

			//setup�� �����س����� num_of_terms�� ���� ��� exit�� �̻��� ���� �����ȴ�. ���� ������ �а� num_of_terms�� ������ ���� �ٷ� entrance�� exit�� default���� �����Ѵ�
			entrance = 0; //ó������ ������� ���� ����̰�
			exit = num_of_terms - 1; //�������� ���� �ϴ��̴�

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

	//3���� dfs�� �ʿ��� �͵�
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


//3���� �ǽ�
bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	int i;  bool found = false; int dir = 0;
	int current_pos; node_pointer ptr; 	top = -1; top_all = -1;

	visited = new int[num_of_terms];
	stack = new int[num_of_terms];
	all = new int[num_of_terms * 4];

	for (i = 0; i < num_of_terms; i++) visited[i] = 0;
	visited[entrance] = 1; //�������� �湮�� ������ ǥ��
	current_pos = entrance;//�������� ��尡 �� ��ġ
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

//3���� ����
bool ofApp::BFS()
{
	int i, j, current_pos; int dir; bool found = false; int flag = 0;
	front = -1; rear = -1; rear_all = -1;
	node_pointer ptr;

	visited = new int[num_of_terms]; //�湮�ߴ����� �ľ��ϱ� ����
	queue = new int[num_of_terms]; //BFS�� queue�� ���� ����
	all = new int[num_of_terms * 4]; //��� ��θ� ��� ���� Ȱ��
	stack = new int[num_of_terms]; //���߿� �ִ� �Ÿ� ��θ� �����ϱ� ���� ���

	for (i = 0; i < num_of_terms; i++) visited[i] = 0;

	visited[entrance] = 1; //�������� 0��° ���(���� ���)
	current_pos = entrance;//0��° ��尡 �� ��ġ
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
					visited[ptr->link[dir]->index] = 1; //���� �� ��带 �湮�� ǥ�ø� �����
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
		stack[++top] = queue[i - 1]; //���ÿ� �߰��ϰ�
		target = i - 1; //�ش� �κ����� �Ѿ��
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

		x1 = graph[number]->middle_x; //�̹� �ܰ��� ��ǥ
		y1 = graph[number]->middle_y;
		x2 = graph[all[i + 1]]->middle_x;
		y2 = graph[all[i + 1]]->middle_y; //1�ܰ� ������ ��ǥ
		ofDrawLine(x1, y1, x2, y2);
	}

	ofSetColor(0, 0, 0);
	//�� ���� �ִ� ��� ���뼭 �׸���
	for (i = top; i > 0; i--) {
		number = stack[i];
		x1 = graph[number]->middle_x; //�̹� ������ ��ǥ(x,y)
		y1 = graph[number]->middle_y;
		x2 = graph[stack[i - 1]]->middle_x; //1 �ܰ� ���� ��ǥ
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

//----------------------���� �� ����----------------------------//

//1. �̷� 1������ ���� �̷� ������ ������ �ȿ��� �� �� �ֵ��� �����Ͽ� ����
void ofApp::Init_maze(int temp_height, int temp_width, int** maze) {
	int i, j;
	//maze �ʱ�ȭ
	for (i = 0; i < temp_height; i++) { //height * 2 + 1
		for (j = 0; j < temp_width; j++) { //width * 2 + 1
			if ((i % 2) == 0 && (j % 2) == 0) maze[i][j] = 1; //���� ¦��, ���� ¦���� ���� �𼭸�
			if ((i % 2) == 0 && (j % 2) == 1) maze[i][j] = 2; //���� ¦��, ���� Ȧ���� ���� ���κ�
			if ((i % 2) == 1 && (j % 2) == 0) maze[i][j] = 3; //���� Ȧ��, ���� ¦���� ���� ���κ�
			if ((i % 2) == 1 && (j % 2) == 1) maze[i][j] = 0; //���� Ȧ��, ���� Ȧ���� ��쿡�� ��(room)
			//���κ��̳� ���κ��� ������ �ڸ��� 4���� �Է� --> ' '(��ĭ)���� ���
		}
	}
}

void ofApp::print_maze(int temp_height, int temp_width, int** maze) {
	int i, j, command;

	for (i = 0; i < temp_height; i++) {
		for (j = 0; j < temp_width; j++) {
			command = maze[i][j];
			switch (command) {
			case 1: //1�� �𼭸��� �ǹ��Ѵ�
				cout << "+"; break;
			case 2: //2�� ���κ��� �ǹ��Ѵ�
				cout << "-"; break;
			case 3: //3�� ���κ��� �ǹ��Ѵ�
				cout << "|"; break;
			case 4: //4�� ���κ��̳� ���κ� �߿� �㹰������ ��ĭ�� ��츦 �ǹ��Ѵ�
				cout << " "; break;
			default: //�������� ���� �̷��� ���� �ǹ��ϸ� 0�� ���̴�
				cout << " ";
			}
		}
		cout << endl;
	}
}

void ofApp::fwrite_maze(int temp_height, int temp_width, int** maze) {
	int i, j, command;
	ofstream writeFile;
	writeFile.open("./data/maze.maz"); //����θ� ����ؼ� bin ���� data�� 'maze.maz'���� ������ �����Ѵ�
	if (writeFile.is_open())  //������ ���ȴ��� Ȯ��
	{
		for (i = 0; i < temp_height; i++) {
			for (j = 0; j < temp_width; j++) {
				command = maze[i][j];
				switch (command) {
				case 1: //1�� �𼭸��� �ǹ��Ѵ�
					writeFile << "+"; break; 
				case 2: //2�� ���κ��� �ǹ��Ѵ�
					writeFile << "-"; break;
				case 3: //3�� ���κ��� �ǹ��Ѵ�
					writeFile << "|"; break;
				case 4: //4�� ���κ��̳� ���κ� �߿� �㹰������ ��ĭ�� ��츦 �ǹ��Ѵ�
					writeFile << " "; break;
				default: //�������� ���� �̷��� ���� �ǹ��ϸ� 0�� ���̴�
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
	int select; //�������� 0, 1�� �����ؼ� ���� ������ ���� ������ �� ����ϴ� ����
	int number = 10; //�̷��� ���� ������ �����ϱ� ���� ����ϴ� ����(10���� �����ؼ� ������ ���� ������ ������ 1�� �����ؼ� �������ش�)
	int flag = 0; //�� �࿡�� ���� ���տ� ���� ���� ���� ��θ� ���ȴ��� �Ǵ��ϴ� ����
	int possible = 0; //���� �ܰ迡�� ���� ��θ� ���� �� �ִ��� �Ǵ��ϴ� ���� 
	int temp; //�� ������ �̷��� ���� ���� �㹰�� ������ ��ĥ �� �ӽ÷� ���� �����ϱ� ���� ����ϴ� ����
	srand((unsigned int)time(NULL));
	
	//ù��° ��, ���� �ٸ� ���տ� ���ϰ� �ʱ�ȭ
	for (i = 0; i < temp_width; i++) {
		if ((i % 2) == 1) {
			maze[1][i] = number++;
		}
	}
	//ù��° ��, �� �������� ���� or ����
	for (j = 1; j < temp_width - 1; j++) { //���� ���̶� ������ ���� ���� ����� �ƴϱ� ����
		if ((j % 2) == 0) {
			select = rand() % 2; //0 �Ǵ� 1�� ���� (0�̸� �� �����, 1�̸� �� ����)
			if (select == 0) {
				maze[1][j] = 4; //���κ� �����(������ ���� 4�� ����)
				maze[1][j + 1] = maze[1][j - 1]; //���� �������� �� ���� ���ʰ� ���� ���տ� ���ϵ��� ����
			}
		}
	}

	for (i = 1; i < temp_height - 3; i += 2) { //������ �� ������ �ݺ�(���� ��� & �� ����)
		//���� ��� �����(1. ������ ���մ� 1���� ���� ��δ� �־���Ѵ�/ 2. �ִٸ� �� �������ʹ� ���Ƿ� ���Ѵ�)
		//1. ������ �˻��ؼ� �� �ٿ� 1���� �ִ� �ٸ� �����̶��, ������ ���� ��θ� �����
		//2. ���� ���� ������ �ִ� ��쿡��, ���Ƿ� ���� ��θ� �����. ���������� �ȸ�������� ���������� ���� ��θ� �ʼ������� �����Ѵ�
		for (j = 1; j < temp_width - 3; j += 2) {
			if (maze[i][j] == maze[i][j + 2] && possible == 0) {
				select = rand() % 2;
				if (select == 0) {
					maze[i + 1][j] = 4; //���κ� ����
					maze[i + 2][j] = maze[i][j]; //�Ʒ�ĭ�� ��ĭ�� ���� �������� ����
					flag = 1; possible = 1;
				}

				if (j + 2 == temp_width - 2) { //������ĭ ó��
					if (flag == 1 && possible == 0) {
						select = rand() % 2;
						if (select == 0) {
							maze[i + 1][j + 2] = 4; //�㹰���� ������ ���(4)
							maze[i + 2][j + 2] = maze[i][j + 2]; //���� ���տ� ���ϵ��� ����
						}
					}
					else if (flag == 0) { //���� �� ���� ��ΰ� ���� ����
						maze[i + 1][j + 2] = 4; //���� ��ΰ� ������ ������ ���� ��ΰ� ������
						maze[i + 2][j + 2] = maze[i][j + 2];
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
				maze[i + 1][j] = 4; //���κ� ����
				maze[i + 2][j] = maze[i][j]; //�Ʒ�ĭ�� ��ĭ�� ���� �������� ����

				flag = 0;
				possible = 0;

				if (j + 2 == temp_width - 2) { //������ĭ ó��
					maze[i + 1][j + 2] = 4; //���κ� ����(1���ۿ� ���� �����̴ϱ�)
					maze[i + 2][j + 2] = maze[i][j + 2];
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

				if (j + 2 == temp_width - 2) { //������ ĭ ó��
					maze[i + 1][j + 2] = 4; //���κ� ����(1���ۿ� ���� �����̴ϱ�)
					maze[i + 2][j + 2] = maze[i][j + 2];
				}
			}
		}
		flag = 0;
		possible = 0;

		//�Ʒ� ���� �о 0���� �ʱ�ȭ�� ���� �� �ٲ������, ������ΰ� ���� ���̰� �̰�쿡�� ���ο� �������� �����
		for (j = 1; j < temp_width - 1; j += 2) {
			if (maze[i + 2][j] == 0) maze[i + 2][j] = number++;
		}

		if (i + 2 == temp_height - 2) break; //������ ���� ��쿡�� �� ������ ��Ģ�� �ٸ��� ������ ����
		//�࿡�� ���Ƿ� �� ����
		for (j = 1; j < temp_width - 3; j += 2) {
			if (maze[i + 2][j] != maze[i + 2][j + 2]) {
				select = rand() % 2; //���� ������ ���� ����
				if (select == 0) { //���� ����ٸ�
					maze[i + 2][j + 1] = 4; //���κ� �㹰��(����)
					temp = maze[i + 2][j + 2]; //������ ���հ� ������ ���յ��� ã�� ���ؼ� temp�� �ӽ� ����
					maze[i + 2][j + 2] = maze[i + 2][j];//������ ������ ���� ���տ� ����
					for (p = 1; p < temp_height - 1; p += 2) { //�̷θ� Ž���ϸ鼭
						for (t = 1; t < temp_width - 1; t += 2) {
							if (maze[p][t] == temp) maze[p][t] = maze[i + 2][j];//temp�� ���� �����̾��� �͵� ���� �������� ����
						}
					}
				}
			}

		}
	}

	//�������� ó��: ������ ������, ���� �ٸ� ���տ� ���� ��� ������ ��� ���� ����
	for (j = 1; j < temp_width - 3; j += 2) {
		if (maze[temp_height - 2][j] != maze[temp_height - 2][j + 2]) {
			maze[temp_height - 2][j + 1] = 4; //���κ� ����

			temp = maze[temp_height - 2][j + 2]; //������ ���հ� ������ ���յ��� ã�� ���ؼ� temp�� �ӽ� ����
			maze[temp_height - 2][j + 2] = maze[temp_height - 2][j]; //������ ������ ���� ���տ� ����
			for (p = 0; p < temp_height; p++) { //�̷θ� Ž���ϸ鼭
				for (t = 0; t < temp_width; t++) {
					if (maze[p][t] == temp) maze[p][t] = maze[temp_height - 2][j]; //temp�� ���� �����̾��� �͵� ���� �������� ����
				}
			} 

		}
	}
}

void ofApp::make_maze() {
	int i, j, temp_height, temp_width;
	int** temp_maze; //���� ���� �̷θ� ���� ������ ����� ���̱� ������ ���� ������ ������ �迭�� temp_maze�� ����Ѵ�
	cout << "HEIGHT: ";
	cin >> temp_height;
	cout << "WIDTH: ";
	cin >> temp_width;
	
	//adjacency matrix�� �����̱� ������ �Է¹��� hegiht�� width�� ���� 2�迡 1�� ������� �Ѵ�
	temp_width = temp_width * 2 + 1;
	temp_height = temp_height * 2 + 1;

	temp_maze = new int* [temp_height]; //�Է¹��� ���̿� �ʺ� �°� ũ�⸦ �����Ҵ� �Ѵ�
	for (i = 0; i < temp_height; i++) {
		temp_maze[i] = new int[temp_width];
	}

	//Init_maze�Լ��� ���� adjacency matrix ������ temp_maze�� �ʱ�ȭ�Ѵ�
	Init_maze(temp_height, temp_width, temp_maze);

	//���� �˰����� ���� ���� �̷θ� �����Ѵ�
	Eller_algorithm(temp_height, temp_width, temp_maze);

	//������ ���̸� ���� ������ ���� �ֿܼ� ������� �����̷θ� ����Ѵ�
	print_maze(temp_height, temp_width, temp_maze);

	//maze.maz ������ �����ϰ� ������� ���� �̷θ� �����Ѵ�
	fwrite_maze(temp_height, temp_width, temp_maze);

	//���� �Ҵ��ߴ� temp_maze�� �����Ѵ�
	for (i = 0; i < temp_height; i++) {
		delete[] temp_maze[i];
	}
	delete[] temp_maze;
}