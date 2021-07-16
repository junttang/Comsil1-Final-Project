#include "ofApp.h"
#include <string>
#include <iostream>
// Comsil Project : Where is my Cheese ?
// Mady by Junhyeok-Park 20171643 CSE / Sogang
// Period : 2021.06.18 ~ 2021.06.21
// Brief Description : Maze Game with some unique features
/*
���� ���̵��
4. ���� �ۼ�
5. ���� �Կ�
*/
using namespace std;
clock_t inittime;

//------ Basic Setup Settings for Openframeworks Program ------//
void ofApp::setup(void) {
	ofSetWindowTitle("Where is my Cheese?");	// ���α׷� �ֻ�� �������� ȭ�� ����
	ofSetFrameRate(15);		// OFS�� ������ ������ ����
	ofBackground(200, 200, 0);	// ���ȭ�� ���� ���� - ���� �����
	windowWidth = ofGetWidth()*1.5;	// ���� ȭ�� ũ�� ����
	windowHeight = ofGetHeight()*1.5;
	isdfs = false;	// isdfs �÷��� 0 �ʱ�ȭ
	isOpen = 0;		// isOpen �÷��� 0 �ʱ�ȭ

	//------ GUI Settings (Offered from TA when MAZE Practice Class) ------//
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);
	ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
	ofShowCursor();
	myFont.loadFont("verdana.ttf", 12, true, true);
	hWnd = WindowFromDC(wglGetCurrentDC());
	ofSetEscapeQuitsApp(false);
	menu = new ofxWinMenu(this, hWnd);
	menu->CreateMenuFunction(&ofApp::appMenuFunction);
	HMENU hMenu = menu->CreateWindowMenu();
	HMENU hPopup;;
	hPopup = menu->AddPopupMenu(hMenu, "Info");
	menu->AddPopupItem(hPopup, "About", false, false);
	menu->SetWindowMenu();

	//------ External Data Loading Part ------//
	gamelogo.load("gamelogo.png");		// GUI �� ���� bgm ������ ���� ����
	playbutton.load("playbutton.png");	// �ܺε����͸� �ε��ϰ�, ũ��, ����, ����,
	rankbutton.load("rankbutton.png");	// �ӵ� ���� ������ �����ϴ� �κ�
	exitbutton.load("exitbutton.png");
	returnbutton.load("returnbutton.png");
	quitbutton.load("quitbutton.png");
	menubgm.load("menubgm.mp3");
	menubgm.setVolume(0.1);
	menusound.load("menusound.mp3");
	menusound.setVolume(0.6);
	menusound.setSpeed(2);
	if (initscrflag)
		menubgm.play();
	endsound.load("endsound.mp3");
	endsound.setVolume(0.4);
	gamebgm.load("gamebgm.mp3");
	gamebgm.setVolume(0.2);
	ratup.load("ratup.png");
	ratdown.load("ratdown.png");
	ratright.load("ratright.png");
	ratleft.load("ratleft.png");
	cheese.load("cheese.png");
	brickwall.load("brickwall.jpg");
	whitewall.load("whitewall.jpg");
	scoreboard.load("scoreboard.png");
	loadone.load("loadone.png");
	loadtwo.load("loadtwo.png");
	loadthree.load("loadthree.png");
	loadfour.load("loadfour.png");
	rankboard.load("rankboard.png");
	rankplace.load("verdana.ttf", 12);
	rankplace2.load("verdana.ttf", 24);
	gamescore.load("verdana.ttf", 50);
	clocktimer1.load("clocktimer1.png");
	clocktimer2.load("clocktimer2.png");
	clocktimer3.load("clocktimer3.png");
	clocktimer4.load("clocktimer4.png");
	clocktimer5.load("clocktimer5.png");
	clocktimer6.load("clocktimer6.png");
	clocktimer7.load("clocktimer7.png");
	clocktimer8.load("clocktimer8.png");
	clocktimer9.load("clocktimer9.png");
	clocktimer10.load("clocktimer10.png");
	clocktimer11.load("clocktimer11.png");
	clocktimer12.load("clocktimer12.png");
	clockboard.load("clockboard.png");
	gameover.load("gameover.png");
	whoosh.load("whoosh.mp3");
	whoosh.setVolume(0.2);
	numberone.load("numberone.png");
	hooray.load("hooray.mp3");
	hooray.setSpeed(1.4);
	hooray.setVolume(0.2);
	decoration.load("decoration.png");

	inittime = clock();	// ���� ���� �ð��� ���
	createRankList();	// rank.txt ������ �о� �̸� ��ŷ ����
	bShowInfo = 1;
}

//------ Graphic User Interface Setting Part ------//
void ofApp::doFullScreen(bool bFull) {
	if (bFull) {
		menu->RemoveWindowMenu();
		ofHideCursor();
		ofSetFullscreen(true);
	}
	else {
		ofSetFullscreen(false);
		menu->SetWindowMenu();
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		ofShowCursor();
		if (bTopmost) doTopmost(true);
	}
}

void ofApp::doTopmost(bool bTop) {
	if (bTop) {
		hWndForeground = GetForegroundWindow();

		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);

		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void ofApp::appMenuFunction(string title, bool bChecked) {
	ofFileDialogResult result;
	string filePath;
	size_t pos;

	if (title == "About")
		ofSystemAlertDialog("ID : 20171643\nName : Junhyeok Park\nComsil Project\n");
}

//------ Some Empty OFS Functions ------//
void ofApp::update() {

}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {

}

void ofApp::mousePressed(int x, int y, int button) {

}

void ofApp::mouseReleased(int x, int y, int button) {

}

void ofApp::windowResized(int w, int h) {

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//------ Core Functions Operating on frame-by-frame basis ------//
void ofApp::keyPressed(int key) {	// Ű���� �Է¿� ���� ó�� �κ�
	if (key == VK_ESCAPE) {	// esc ������ ���� �ǵ��� ����
		ofExit();
	}
	// �ʱ�ȭ�鿡�� play ������ ������ ��Ȳ���� ����Ű�� ������ ��� : ���� ����
	if (key == OF_KEY_RETURN && selectIdx == 0 && initscrflag == 1) {
		initscrflag = 0;	// �ʱ�ȭ�� ����
		reachtargetflag = 0;	// ������ ���� �÷��� 0 �ʱ�ȭ
		menubgm.stop();	// �ʱ�ȭ�� bgm ����
		freeMemory();	// �ʱ� �������� ����
		readFile();	// maze.maz ������ �о� �̷� ����

		if (isOpen) {	// maze.maz ���� ���� ���� ��
			isload = 1;	// �ε�ȭ�� ǥ�� (������ ������ Ư���� ����.)
			coordX = coordY = 0;	// ���� �ʱ� ��ġ ����
			targetX = WIDTH - 3;	// ġ���� �ʱ� ��ġ ����
			targetY = HEIGHT - 3;
		}
	}
	// �ʱ�ȭ�鿡�� rank ������ ������ ��Ȳ���� ����Ű�� ������ ��� : ��ŷ ǥ��
	if (key == OF_KEY_RETURN && selectIdx == 1 && initscrflag == 1) {
		menusound.play();	// ���� ���� ����
		initscrflag = 0;	// �ʱ�ȭ�� ����
		rankscrflag = 1;	// ��ŷȭ�� ǥ��
	}
	// ��ŷ ǥ�� ȭ�鿡�� �ٽ� ����Ű�� ������ ��� : �ʱ�ȭ������ ����
	else if (key == OF_KEY_RETURN && selectIdx == 1 && rankscrflag == 1) {
		menusound.play();	// ���� ���� ����
		initscrflag = 1;	// �ʱ�ȭ�� ǥ��
		rankscrflag = 0;	// ��ŷȭ�� ����
	}
	// �ʱ�ȭ�鿡�� exit ������ ������ ��Ȳ���� ����Ű�� ������ ��� : ���α׷� ����
	if (key == OF_KEY_RETURN && selectIdx == 2 && initscrflag == 1) {
		menusound.play();	// ���� ���� ����
		gameexit = 1;		// heap �޸� ������ ���� ���� �÷���
		freeMemory();	// ��� �� �Ҵ� �޸� ����
		ofExit();	// ���α׷� ����
	}

	if (initscrflag && !isgame) {	// �ʱ�ȭ�鿡��
		if (key == OF_KEY_UP) {	// '��'Ű�� ������ ���
			menusound.play();	// ���� �̵� ����
			selectIdx--;	// ���� ǥ�� �ε��� ����
			if (selectIdx < 0)	// ������ ����
				selectIdx = 0;
		}

		if (key == OF_KEY_DOWN) {	// '��'Ű�� ������ ���
			menusound.play();	// ���� ������ �ݴ�޺� ����
			selectIdx++;
			if (selectIdx == 3)
				selectIdx = 2;
		}
	}
	// ���� ���� ���� �� ���� �������� �������� ���� ���¿����� Ű�Է� ó��
	if (isgame && !isload && !(coordX == targetX && coordY == targetY)) {
		if (key == 'q' || key == 'Q') {	// qŰ�� ������ ��� 1ȸ ���� Ż��
			initscrflag = 1;	// �ٽ� �ʱ�ȭ�� ǥ��
			isgame = 0;		// ���� ȭ�� ����
			gamebgm.stop();	// ���� bgm ����
			menubgm.play();	// �ʱ�ȭ�� bgm ����
		}

		if (key == OF_KEY_RIGHT) {	// '��'Ű�� ������ ���
			if (coordX < WIDTH - 4) {	// �̷� �ֿ����� ��� �̵� �Ұ�
				if (Maze[1 + coordY][1 + coordX + 1] == ' ') { // �̷� �շ�������
					coordX += 2;	// �̵�
					rightflag = 1;	// ���� �÷��� ���� -> ���� ��� ������ ����
					leftflag = upflag = downflag = 0; // �ٸ� ���� �÷��� ����
				}
			}
		}

		if (key == OF_KEY_LEFT) {	// '��'Ű�� ������ ���
			if (coordX) {	// ���� ���Ǻο� �����ϰ� �۵�
				if (Maze[1 + coordY][1 + coordX - 1] == ' ') {
					coordX -= 2;
					leftflag = 1;
					rightflag = upflag = downflag = 0;
				}
			}
		}

		if (key == OF_KEY_UP) {
			if (coordY) {
				if (Maze[1 + coordY - 1][1 + coordX] == ' ') {
					coordY -= 2;
					upflag = 1;
					leftflag = rightflag = downflag = 0;
				}
			}
		}

		if (key == OF_KEY_DOWN) {
			if (coordY < HEIGHT - 4) {
				if (Maze[1 + coordY + 1][1 + coordX] == ' ') {
					coordY += 2;
					downflag = 1;
					leftflag = upflag = rightflag = 0;
				}
			}
		}
	}
}

void ofApp::draw() {	// �� ������ ���� ȭ���� �׸��� �Լ�
	int i, sec;	// �ε��� ���� i��, �ʸ� ����� sec����
	char str[256];	// ���۱� ���ڿ� ǥ��
	Point temp;	// ���� �� ������ ��ǥ�� ���Ϲ��� ����
	ofSetColor(255, 255, 255);	// ��� �ε� �̹��������� ��� �ִ� ����
	ofSetLineWidth(5);	// ���� ���⸦ 5�� ����
	brickwall.draw(0, 0, windowWidth, windowHeight);	// ������ ����ȭ�� :����

	if (initscrflag) {	// �ʱ�ȭ�� �÷��� ���� ��
		initScreen();	// �ʱ�ȭ�� ǥ�� �Լ�
		loadframe = ofGetFrameNum();	// �ε�ȭ�� ǥ�� �� �ε� ���� ������ ���
	}	// �� ���� ����

	if (rankscrflag) {	// ��ŷȭ�� �÷��� ���� ��
		rankboard.draw(33, 0, 900, 600);	// ��ŷ���� �׸�
		returnbutton.draw(670, 580, 210, 110);	// ���͹�ư �׸�
		showRankBoard();	// ��ŷ ȭ�鿡 ǥ�� �Լ�
	}

	if (isload)	// �ε�ȭ�� �÷��� ���� ��
		loadingScreen(loadframe);	// �ε�ȭ���� ���� ������ ���� ǥ��
	// ������ ������ ��Ҹ� ���߱� ���� �ε�ȭ���� ��, ������ ������ �ε����� ����
	if (isgame) {	// ���� �÷��� �÷��� ���� ��
		whitewall.draw(10, 10, 600, 600);	// GUI���� �̹��� ���� ǥ��
		quitbutton.draw(650, 580, 180, 80);	// ���� ��ư
		scoreboard.draw(637, 75, 300, 200);	// ���ھ��
		clockboard.draw(710, 350, 150, 150);// �ð�
		decoration.draw(800, 420, 150, 250);// ���� �ϴ� �� �׸�

		if (!reachtargetflag) {	// �������� �������� ������ ��
			if (resetinitflag) {	// 10�� �������� �÷��� ���� �� ����
				resetcriteria = clock();	// 10�� ���� ��� ������
				resetinitflag = 0;
			}
			resetfull = clock();	// 10�� ���� ��� ����
			drawclock(resetfull);	// �� �ʸ��� �ð�׸� ȸ�� ���
			sec = (int)((resetfull - resetcriteria) / (double)1000);
			// ���������κ��� �帥 �ð� �ʸ� ����ϴ� sec����
			if (sec > 9) {	// 10�ʰ� ���� ���
				temp = maximumDist();	// �� ��ġ���� ���� �� ������ Ȯ��
				targetX = temp.x;	// �ش� ���������� ������ ����
				targetY = temp.y;
				whoosh.play();	// ġ� �����̴� ���� ȿ��
				resetinitflag = 1;	// �ٽ� 10�ʸ� ���� ���� �÷��� ����
			}
		}
		else
			drawclock(resetfull);	// ������ ������ ��� �ð� ���ߵ��� ����

		ofSetColor(0, 0, 0);	// �̷θ� �׸��� ���� ��İ� ǥ��
		for (i = 0; i < linesSize; i++)
			ofDrawLine(20 * lines[i].x1, 20 * lines[i].y1, 20 * lines[i].x2, 20 * lines[i].y2);
		// �̷θ� �׸��� �ݺ���
		ofSetColor(255, 255, 255);	// �̷� �����ϰ�� ��� ��� �ִ�
		if (isOpen) {	// ���� ó��
			if (isdfs)	// DFS�� ���������� ����Ǿ��ٸ�,
				dfsdraw();	// DFS ����� �ִܰ�θ� ȭ�鿡 ǥ��
			PlayGame();	// ���� �÷��̸� �����ϴ� ���� �÷��� ���� �Լ�(���� ����)
		}
	}

	if (bShowInfo) {
		ofSetColor(255);	// ī�� ����Ʈ ���ڿ� ǥ��
		sprintf(str, "Copyright 2021. joon hyeok comsil project. all rights reserved.");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}
}

//------ Assistant Functions for draw() in 'Game Play Mode' ------//
// ġ���� 10�� ���� ���ο� ��ġ�� ã���ִ� �Լ�. �̷��� 4���� ����������
// �ĺ��̴�. �Ʒ��� temp�迭�� �̵��� ����Ų��.
// ����ġ�� �������� ���� �Ÿ��� ���� ����Ͽ� �Ǵ��Ѵ�.
Point ofApp::maximumDist(void) {
	int maxIdx;	// ���� �Ÿ��� �� �������� �ε��� ���
	int maximum;// �ִ� �񱳸� ���� ����
	Point temp[4] = { {WIDTH - 3,HEIGHT - 3},{WIDTH - 3,0},{0,HEIGHT - 3},{0,0} };
	Point tmp = temp[0];	// �ʱ� ����

	maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
	maxIdx = 0;
	// ���� �Ÿ��� �� ���� ã�� ���� : ���� �� ������ �Ÿ��� ��Ÿ��������� �̿���
	tmp = temp[1];	// '�������� �Ÿ�'�� ����. ����� ������ �ƴ�.
	if (maximum < (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y)) {
		maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
		maxIdx = 1;
	}
	tmp = temp[2];
	if (maximum < (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y)) {
		maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
		maxIdx = 2;
	}
	tmp = temp[3];
	if (maximum < (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y)) {
		maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
		maxIdx = 3;
	}

	return temp[maxIdx];	// ����Ÿ��� ���� �� �������� ��ȯ
}

void ofApp::drawclock(double current) {	// �����÷��� ���� �ð踦 �׸��� �Լ�
	switch ((int)((resetfull - inittime) / (double)1000) % 12) { // �ʺ���
	case 0:
		clocktimer1.draw(710, 350, 150, 150);
		break;
	case 1:	// ���� �������� �ð谡 �������� �����̴� ��ó�� ���̰� �ϴ� �Լ��̴�.
		clocktimer2.draw(710, 350, 150, 150);
		break;	// �����δ� ��ó�� 12���� ��Ȳ�� �׸��� �����ִ� �����̴�.
	case 2:
		clocktimer3.draw(710, 350, 150, 150);
		break;
	case 3:
		clocktimer4.draw(710, 350, 150, 150);
		break;
	case 4:
		clocktimer5.draw(710, 350, 150, 150);
		break;
	case 5:
		clocktimer6.draw(710, 350, 150, 150);
		break;
	case 6:
		clocktimer7.draw(710, 350, 150, 150);
		break;
	case 7:
		clocktimer8.draw(710, 350, 150, 150);
		break;
	case 8:
		clocktimer9.draw(710, 350, 150, 150);
		break;
	case 9:
		clocktimer10.draw(710, 350, 150, 150);
		break;
	case 10:
		clocktimer11.draw(710, 350, 150, 150);
		break;
	case 11:
		clocktimer12.draw(710, 350, 150, 150);
		break;
	}
}

//------ Create a random basis maze of WIDTH 15, HEIGHT 15 ------//
void ofApp::makemazefile(void) {	// �� ���� ȸ���� ���� ������ �̷θ� �����ϴ� �Լ�
	FILE*fp = fopen("maze.maz", "wt");	// ������ �ۼ��ϵ��� ��Ʈ�� ����
	int width, height;	// ���� ���̸� ���
	int set = 0, i;	// ���� �ð��� ����ߴ� Eller's Algorithm�� ����Ѵ�.
	width = 15;
	height = 15;

	if (first_heap_alloc_flag) {	// ���� ���� ���� �ÿ��� �޸𸮸� �Ҵ��Ѵ�.
		rows = (int*)malloc(sizeof(int)*width);
		vert = (int*)malloc(sizeof(int)*(width - 1));
		horiz = (int*)malloc(sizeof(int)*width);
	}	// �� ���� ���� ȸ�� ���� �� �޸𸮰� ������ �ʰ� �ϱ� ���� �޸𸮸� �� ����
	// �Ҵ��ϰ� ���� ���� ���� ������ʹ� �ش� �޸𸮸� �����ϴ� ���̴�.

	for (i = 0; i < width; i++)
		fprintf(fp, "+-");	// �ֻ�� �׵θ� �׸�
	fprintf(fp, "+\n");

	for (i = 0; i < height; i++) {	// ���̺��� ��ȸ�ϸ�,
		if (first_line_flag)	// ù ��° ���� �׸� ����
			randomize(width, &set);	// Randomize �Լ� ȣ��
		else {	// �� ���� �ٿ� ���ؼ���,
			randomizeH(width, &set);	// ���� ���� �����ϰ� ����
			if (i == (height - 1))	// ������ ������ ���� ���� �б�
				last_line_flag = 1;
			randomizeV(width);	// ������ ���� �����ϰ� ����
		}
		drawMaze(fp, width);	// �̷θ� maze.maz ���Ͽ� ���ڵ�� �׸���.
		first_line_flag = 0;	// �÷��� ���� ����
	}
	for (i = 0; i < width; i++)
		fprintf(fp, "+-");	// ���ϴ� �׵θ� �׸�
	fprintf(fp, "+\n");

	fclose(fp);
}

void ofApp::randomize(int n, int *set) {	// ���� seed ���� �Լ�
	int prevIdx = -1;	// �ʱ� set �ʱ�ȭ
	int i, j;

	srand(time(NULL));	// seed ����
	for (i = 0; i < n - 1; i++) {
		vert[i] = rand() % 2;	// �������� �����ϰ� ����

		if (vert[i]) {	// �������� �ִ� ���
			for (j = ++prevIdx; j <= i; j++)	// set�� ������.
				rows[j] = *set;
			prevIdx = i;	// prevIdx�� i�� ���
			(*set)++;	// set�� �þ����. (�翬)
		}
	}

	for (i = ++prevIdx; i < n; i++)
		rows[i] = *set;	// �� �ٿ� ���Ͽ� set ������ ����
	(*set)++;
}

void ofApp::randomizeV(int n) {	// ������ ������ ���� ���� �Լ�
	int prevSet;
	int flag = 0;	// ���� �Ҵ� �÷���
	int i, j;

	for (i = 0; i < n - 1; i++) {
		if (rows[i] != rows[i + 1]) {	// set�� �ٲ�� ��������
			if (last_line_flag) flag = 1;	// ������ ������ ��쿡�� Ư�� �б�
			else flag = rand() % 2;	// �� ���� ������ ��� �����ϰ� ����

			if (flag) {	// �����÷��װ� 1�� ��
				vert[i] = 0;	// �������� ���ٰ� ����

				prevSet = rows[i + 1];	// set�� ����ͼ�
				rows[i + 1] = rows[i];

				for (j = 0; j < n; j++)	// set�� ��ģ��.
					if (rows[j] == prevSet)
						rows[j] = rows[i];
			}
			else
				vert[i] = 1;	// �� �ܿ��� �������� ����
		}
		else
			vert[i] = 1;	// set�� ���� ��쿣 vert[i]=1�� ����
	}
}

void ofApp::randomizeH(int n, int *set) {	// ������ ���� ���� ���� �Լ�
	int prevSet;
	int flag = 0;	// ���� ���� ������ ����ų �÷��� ����
	int i;

	prevSet = rows[0];	// �ʱ� set
	for (i = 0; i < n; i++) {
		horiz[i] = rand() % 2;	// ���� ���� ����

		if (!horiz[i])	// ���� ��쿣 �÷��� ������ ����
			flag = 1;

		if (i < n - 1) {	// ������ ��ȸ�ϴµ�,
			if (rows[i + 1] != prevSet) {	// set�� �ٸ��ٸ�,
				if (!flag)	// �̶� ������ ���� ���
					horiz[i] = 0;	// �״�� ���� ���� ���ΰ�
				else	// ������ �ִ� ���
					flag = 0;	// �÷��� ���� ����

				prevSet = rows[i + 1];	// ���� set�� ������Ʈ
			}
		}
		if ((i == n - 1) && !flag)	// �÷��װ� �����Ǿ� �ְ�, ������ �ε����� ��
			horiz[i] = 0;	// Ư���ϰ� ���� ���� ����

		if (horiz[i]) {	// ������ ���� ���� set�� �״�� ����� �ϸ� �ȴ�.
			rows[i] = *set;
			(*set)++;
		}
	}
}

//------ Read 'Maze.maz' file to create a random basis graphic maze ------//
// ���� �̷� ���� ����� ���� ������ maze.maz ������ �о� OFS �󿡼� ������ �̷θ�
// �����ϴ� �Լ�
bool ofApp::readFile(void) {
	ifstream inputFile, tempFile;
	string line;
	int i, j;
	makemazefile();	// �켱 �ռ��� ���� �̷θ� �����س���(maze.maz�� �����س���)
	HEIGHT = 0;

	inputFile.open("maze.maz");	// ������ ���� �д´�.
	tempFile.open("maze.maz");

	if (inputFile.is_open()) {	// ������ ���������� ���ȴٸ�,
		ofFile file("maze.maz"); // �� �� �� Ȯ��
		string tempStr;

		if (!file.exists()) {	// ������ �ȿ��ȴٸ� ���� ó��
			cout << "Target file does not exists." << endl;
			return false;
		}
		else {	// ������ ã�� ��� isOpen �÷��� ����
			cout << "We found the target file." << endl;
			isOpen = 1;
		}

		while (getline(inputFile, line)) {	// maze.maz������ �ٺ��� ����
			WIDTH = line.length();	// �� ���� ���̸� ������ ���
			HEIGHT++;	// ���̸� ī��Ʈ
		}

		if (first_heap_alloc_flag) {	// ���� ���� ���� �ÿ��� �޸� �Ҵ�
			Maze = new char*[HEIGHT];	// �� �ܿ��� �޸� ȿ������ ���� ���Ҵ�
			for (i = 0; i < HEIGHT; i++)	// �޸𸮸� ������ ��
				Maze[i] = new char[WIDTH];
			lines = new struct Line[HEIGHT*WIDTH + 10];	// �����ϰ� ũ�� ����
		}

		for (i = 0; i < HEIGHT; i++) {	// ���� ����
			getline(tempFile, tempStr);	// ���ڿ��� �о�
			for (j = 0; j < WIDTH; j++)
				Maze[i][j] = tempStr[j];	// �ش� ���ڿ��� �̷� �迭�� ���ں� ���
		}
		shortRoute = new struct Point[10000];	// �ִ� ��� �迭 �Ҵ� (����)
		allRoute = new struct Point[10000];	// ��ü ��� �迭 �Ҵ� (����)
		// �Ʒ��� �̷� �迭�� ������� �̷� �׸��⸦ ���� ��� ���� ����
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (Maze[i][j] == '-')
					lines[linesSize++] = pathFind(i, j, 1); // ������ ũ�� ����
				else if (Maze[i][j] == '|')
					lines[linesSize++] = pathFind(i, j, 0);
			}
		}
		return true;
	}
	else {
		printf("  Needs a '.maz' extension\n");	// ����ó��
		return false;	// �ǽ� �ڵ��� ������
	}
}

Line ofApp::pathFind(int i, int j, int flag) {	// �̷θ� ȭ�� �� �����ϰ� �׸�
	Line path = { j,i,j,i };	// �� ���� ������ ���� ������ �����ϰ� �����ؼ� ��
	// ȯ���ִ� �Լ���, �ش� �۾��� ��ü �ڵ忡�� �� ���� �ݺ������� �Ͼ�� ������
	if (flag) {	// �̸� �Լ����ȭ�Ͽ���.
		path.x1 -= 0.5;
		path.y1 += 0.5;
		path.x2 += 1.5;
		path.y2 += 0.5;
	}
	else {
		path.x1 += 0.5;
		path.y1 -= 0.5;
		path.x2 += 0.5;
		path.y2 += 1.5;
	}

	return path;	// Line ����ü ������ ��ȯ��.
}

//------ Main Function for 'Game Play Mode' ------//
// 1ȸ�� ���� �÷��̸� ��ü������ �����ϴ� �̹� ������Ʈ�� �ٽ� �Լ�
// ������ ����, ��Ȳ �÷��� ����, Ű �Է� ���� ��� �ٸ� ������ �ϴ�
// �Լ��̴�. �Լ��� ���� �ڼ��� ������ �ּ����� �̾��.
void ofApp::PlayGame() {
	Point coord;	// ����ġ�� ����� ����
	coord.x = coordX;	// �� ��ġ ������Ʈ
	coord.y = coordY;
	// �������� ġ� �׸���.
	cheese.draw(20 * targetX + 15, 20 * targetY + 15, 35, 35);

	if (coordX != targetX || coordY != targetY)	// �������� �������� ����
		allRoute[aIdx++] = coord;	// ��� �̵��� allRoute�迭�� ����Ѵ�.

	if (upflag)	// '��'Ű�� ������ ��� �㰡 ���� �ٶ󺸰� �Ѵ�.
		ratup.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	else if (downflag)	// '��'Ű�� ������ ��� �㰡 �Ʒ��� �ٶ󺸰� �Ѵ�.
		ratdown.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	else if (rightflag)	// ���� ���� ���
		ratright.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	else if (leftflag)
		ratleft.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	printf("(%d, %d)\n", coordX, coordY);	// ���� ��ǥ�� �ܼ� �� ǥ��

	if (coordX == targetX && coordY == targetY) {	// ġ� ������ ���
		reachtargetflag = 1;	// ������ ���� �÷��� ����

		gamebgm.stop();	// ���� bgm�� ����.
		if (!endsoundflag) {	// ��Ȯ�� '���� ����'�� ���ؼ� (�÷��� �̿�)
			DFS();	// �ִ� ��� ��� with DFS
			isdfs = 1;	// DFS ����� �׸���� ����� draw�Լ��� ����
			endgame = clock();	// ������ ����� ������ ���
			playtime = (int)((endgame - startgame) / (double)1000); // �ҿ�ð�
			playtime += aIdx / 2;	// �� ��ü��� �ε����� �̿��Ͽ� ������ ���
			if (playtime > 999)	// ������ 999�� �̻��� ���(���� ���� ����)
				playtime = 999;	// ���ǹ��ϴ� �Ǵ��Ͽ� �̶����ʹ� ��� 999������ ����
			if (playtime < currentTopRanker)	// ���� �÷��� ������ 1� �ش��ϸ�
				hooray.play();	// 1�� ��� ȿ���� ���
			else	// 1���� �ƴϸ� ��� ���ϵ� ���� ���� ����
				endsound.play();

			currentframe = ofGetFrameNum() + 104;	// ��� ��⸦ ���� ������ ���
			endsoundflag = 1;	// '���� ����'���� ������ ���� �÷��� ����
		}	// ������ ��Ȯ�� ���� ���� : ���� ���ǹ� ��Ȳ�� �����Ӻ��� �ݺ��ϸ� �ȵ�.
		ofSetColor(255, 255, 255);	// ��� �ִ� �����Ͽ�
		gameover.draw(60, 50, 480, 480);	// ���� ���� ��Ȳ�� �׸�

		sprintf(printScore, "%d", playtime);	// ������ ȭ�鿡 ǥ���� ���ڿ� ����
		ofSetColor(0, 0, 0);	// ���� ���ھ� ���ڿ��� ��Ӱ� ǥ���ϱ� ����
		gamescore.drawString(printScore, 720, 200);	// ȭ�鿡 ǥ��
		ofSetColor(255, 255, 255);	// ������ �����ϰ�� �ٽ� ��� ǥ���ϱ� ����

		if (playtime < currentTopRanker)	// 1���� ���� 1�� ��� �׸��� ǥ����.
			numberone.draw(33, 0, 900, 600);

		if ((currentframe - ofGetFrameNum()) % 105 == 0) {	// ��� ��� �ð��� ������
			newRank(playtime);	// ������ ��ŷ�� ���Խ�Ű��
			initscrflag = 1;	// �ʱ�ȭ������ ����
			isgame = 0;	// 1ȸ�� �÷��� ����
			menubgm.play();	// �ʱ�ȭ�� bgm�� �ٽ� ư��.
		}
	}
	else {	// �������� �������� ���� ��� (�������� ��Ȳ)
		playtime = (int)((clock() - startgame) / (double)1000);
		playtime += aIdx / 2;	// �� ������
		if (playtime > 999)
			playtime = 999;
		sprintf(printScore, "%d", playtime);	// ȭ�鿡 ǥ���Ѵ�.
		ofSetColor(100, 100, 100);	// ���� ���� ǥ�ÿ� ���� �����Ͽ���.
		gamescore.drawString(printScore, 720, 200);
		ofSetColor(255, 255, 255);
	}
}

//------ Find the shortest route when user reached the target ------//
// �������� ������ ���, �ش� ������������ �ִܰ�θ� ����� DFS �Լ�.
// �̶�, ���� �÷��� ������ ���µ� ġ���� ���� ��ġ���� �Ǵ��Ѵ�.
// ��, ���� ������ coordX = 0, coordY =0�������� ���� ���������� ���� ġ��
// ��ġ������ �ִ� ��θ� ���̴� ���̴�. �߰��� ����ģ ���������� ������� ����.
bool ofApp::DFS() {
	int moveOrNot;	// �������� ������ ���ϴ� �÷��� ����
	int i, j;
	struct Point p1, p2;

	if (first_heap_alloc_flag) {	// ���� ���� ���� ���� �����Ҵ�
		Visit = new int*[HEIGHT];	// �׿ܿ��� �޸� ������ ��(���)
		for (i = 0; i < HEIGHT; i++)
			Visit[i] = new int[WIDTH];
		first_heap_alloc_flag = 0;	// �� ģ���� ���� ������ ���� ������ �Ҵ��̱�
	}	// ������ ���⿡�� �÷��׸� �����Ѵ�.

	for (i = 0; i < HEIGHT; i++) {	// �̷� �迭�� �������
		for (j = 0; j < WIDTH; j++) {	// ������ �� �ִ� ���� ���� ���� Visit�迭
			if (Maze[i][j] != ' ')	// �� ����صд�.
				Visit[i][j] = 1;
			else
				Visit[i][j] = 0;
		}
	}
	Visit[1][1] = 1;	// ���� �������� �̹� �湮�� ������ �����.
	p1.x = 1;	// ������ : �̶�, �߿��� ����, OFS ȭ�� ǥ�� ��ǥ��, DFS ��� ����
	p1.y = 1;	// ��ǥ�� �ٸ��ٴ� ���� �ν��ϴ� ���̴�.

	shortRoute[sIdx++] = p1;	// ���ÿ� Ǫ��

	while (sIdx) {	// ������ ��� ������
		if (shortRoute[sIdx - 1].x == (targetX + 1) && shortRoute[sIdx - 1].y == (targetY + 1)) {
			printf("Reached the Destination!!\n");
			return true;	// �������� �����ϸ� ����
		}

		moveOrNot = 0;	// �켱 �̵����� �������� ����
		for (i = 0; i < 4; i++) {	// �̵��������� Ȯ���ϴ� �κ�
			p2.x = shortRoute[sIdx - 1].x + move[i].x;	// �� ��� ��ġ
			p2.y = shortRoute[sIdx - 1].y + move[i].y;

			if (!Visit[p2.y][p2.x]) {	// �ش� ��ġ�� ���� �̹湮 ��ġ���
				moveOrNot = 1;	// �̵��ϱ�� ����!
				// �ش� �̵� ��ġ�� �ִܰ�� ���ÿ� Ǫ���Ѵ�. �湮���ε� �翬 ���
				Visit[shortRoute[sIdx - 1].y + move[i].y][shortRoute[sIdx - 1].x + move[i].x] = 1;
				shortRoute[sIdx++] = p2;
				break;
			}
		}
		if (!moveOrNot)	// �̵� ���ɼ��� ���ٸ� �̵����ɼ��� ���� ������ ������
			sIdx--;	// ���Ѵ�.
	}
}

void ofApp::dfsdraw() {	// DFS ����� ���� �ִܰ�θ� ȭ�鿡 ǥ���ϴ� �Լ�
	int wholePath = aIdx;	// ������ ����Ǵ� �������� �ߵ��Ѵ�.
	int shortestPath = sIdx;	// �ε����� ���
	int i;

	for (i = 0; i < wholePath; i++) {
		allroute.addVertex(20 * allRoute[aIdx - 1].x + 30, 20 * allRoute[aIdx - 1].y + 30);
		aIdx--;	// ��ü ��θ� ȭ�鿡 ���������� ǥ���Ѵ�.
	}
	for (i = 0; i < shortestPath; i++) {
		shortroute.addVertex(20 * shortRoute[sIdx - 1].x + 10, 20 * shortRoute[sIdx - 1].y + 10);
		sIdx--;	// �ִ� ��θ� ȭ�鿡 ������� ǥ���Ѵ�.
	}
	ofSetLineWidth(5);
	ofSetColor(140, 0, 0);
	allroute.draw();	// �迭�� ���� ofPolyLine OFS ������ �̿��� �����ϰ�
	ofSetColor(0, 140, 0);	// �׷��ش�. ofPolyLine�̱� ������ �������̵�Ǹ鼭
	shortroute.draw();	// �׷����ٴ� ���� �ν��Ѵ�.
}

//------ Functions for Graphic User Interface ------//
// �ռ�, ���� �̷� ���� �Լ����� �̷θ� maze.maz ���Ͽ� ���� ������ ����ϴ� �Լ�
void ofApp::drawMaze(FILE* fp, int n) {
	int i;

	if (!first_line_flag) {	// ù ° ������ �ƴ� ���� ���
		fprintf(fp, "+");

		for (i = 0; i < n; i++) {
			if (horiz[i])	// ������ ������ �Ʒ��� ���
				fprintf(fp, "-+");
			else	// ������ �Ʒ��� ���
				fprintf(fp, " +");
		}
		fprintf(fp, "\n");	// �ٳѱ�
	}
	fprintf(fp, "|");	// �׵θ��� �׸���.
	for (i = 0; i < n - 1; i++) {	// �������� �׸��� ����
		fprintf(fp, " ");

		if (vert[i])
			fprintf(fp, "|");
		else
			fprintf(fp, " ");	// �� ����
	}
	fprintf(fp, " |\n");
}

void ofApp::initScreen(void) {	// ������ �ʱ�ȭ���� �׸��� �Լ�
	gamelogo.draw(50, 0, 900, 600);	// �ΰ� �׷��ְ�
	switch (selectIdx) {	// ���� ���� �ε����� ����
	case 0:	// ���õ� ������ �׷����� ����ó��, �������� ũ�⸦ Ű���ش�.
		playbutton.draw(375, 345, 210, 110); // ��, ���õǾ����� ���������
		rankbutton.draw(380, 450, 200, 100); // ǥ���ϴ� ���̴�.
		exitbutton.draw(380, 550, 200, 100); // �̼��� ������ �״�� ���
		break;
	case 1:
		playbutton.draw(380, 350, 200, 100);
		rankbutton.draw(375, 445, 210, 110);
		exitbutton.draw(380, 550, 200, 100);
		break;
	case 2:
		playbutton.draw(380, 350, 200, 100);
		rankbutton.draw(380, 450, 200, 100);
		exitbutton.draw(375, 545, 210, 110);
	}
}

void ofApp::loadingScreen(int loadframe) { // �ε�ȭ���� �׸��� �Լ�
	int currentframe = ofGetFrameNum();	// �ε�ȭ���� ������ ���� ���� ������ �ٸ���.
	int frameN = currentframe - loadframe;
	// ������ �ε� ����� ���� �κ�����, �Ϲ����� ������ �ε�ȭ�鿡�� ���̵� �����.
	if (frameN < 15)	// �����Ӻ��� ������ �׸��� �ִϸ��̼�ó�� ���̴� �Ϳ� �Ұ���.
		loadone.draw(0, 0, windowWidth, windowHeight);
	if (frameN >= 15 && frameN < 30)	// ��, �������� �ε������� �ƴϴ�.
		loadtwo.draw(0, 0, windowWidth, windowHeight);
	if (frameN >= 30 && frameN < 45)
		loadthree.draw(0, 0, windowWidth, windowHeight);
	if (frameN >= 45 && frameN < 60)
		loadfour.draw(0, 0, windowWidth, windowHeight);

	if (frameN >= 60) {	// 60���� �������� ��������, �ε��� ���߰�, ��¥ ���� �÷���
		isload = 0;	// ���� ����. isload�� �����ϰ�, isgame�� �����Ѵ�.
		isgame = 1;
		gamebgm.play();	// ���� bgm�� �����ϸ�,
		startgame = clock();	// �ҿ�ð� ������ ���� �������� ����ְ� �Լ� ����
	} // �ε�ȭ���� ������ ������, �ڿ������� ������ ���¸� ���߰� �ϱ� ����.
}

//------ Functions for Implementing Ranking System ------//
// �Ʒ��� �Լ���, rank.txt ������ �̿��� ��ŷ�� �����ϴ� �Լ��̴�.
// ��, �ܺο� �������� �����ν� ������ ����Ǿ��� �ٽ� ����Ǿ ������ ��ŷ��
// �����ȴ�.
void ofApp::createRankList(void) {
	int score;	// ���� ���
	char temp[15];	// ��ϳ�¥ ���
	FILE*fp = fopen("rank.txt", "rt");	// rank.txt ������ ����.
	struct Node* newNode;	// ���Ḯ��Ʈ ������ ���� ���� �����
	struct Node* currNode = head;	// currNode�� ��带 ���

	if (fp == NULL || fscanf(fp, "%d", &numOfRanks) == EOF) {//����ó��
		printf("Ranking File Open Error !\n");
		return;
	}// ����, rank.txt ������ �����ϴ��� ���� ���࿡ ������ ������ �ʴ´�.
	// ��ŷ�� ���� ������ ���� ���� ������ ����Ͽ���.
	while (fscanf(fp, "%s %d", temp, &score) != EOF) {	// ������ �ٺ��� �о�
		newNode = (Node*)malloc(sizeof(Node));	// ��� ���� �Ҵ�
		newNode->score = score;
		strcpy(newNode->date, temp);	// ��� ����
		newNode->link = NULL;

		if (head == NULL) {	// ���� ������ ��
			currentTopRanker = newNode->score;	// ž ��Ŀ�� ���. �� ���� 1���̹Ƿ�
			head = newNode;
			currNode = newNode;	// �Ӹ� ����
		}
		else {	// ����� ���� ���԰���
			currNode->link = newNode;
			currNode = newNode;
		}
	}
	fclose(fp);
}

void ofApp::writeRankFile(void) {	// �� �÷��� ����ɶ� rank.txt�� ������Ʈ�� �Լ�
	FILE* fp = fopen("rank.txt", "wt");	// ��Ʈ���� �����Ѵ�.
	Node* currNode = head;
	currentTopRanker = currNode->score;	// ž ��Ŀ�� ����Ѵ�.

	fprintf(fp, "%d\n", numOfRanks);
	while (currNode) {	// rank.txt ���Ͽ� ����.
		fprintf(fp, "%s %d\n", currNode->date, currNode->score);
		currNode = currNode->link;
	}
	fclose(fp);
}

void ofApp::newRank(int score) {	// �� �÷��� ����ø��� ������ ��ŷ�� ������.
	Node* newNode;
	Node* currNode = head;	// �Ӹ��������� ��ȸ ������ ��
	Node* prevNode = NULL;

	newNode = (Node*)malloc(sizeof(Node)); // �ش� ������ ����Ѵ�.
	sprintf(newNode->date, "%d/%d,%d:%d", ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes());
	newNode->score = score;	// �̸� ��� �������� ����ڸ� �����ϴ� ��

	while (currNode) {	// ������ ��ġ�� ���������� Ž��
		if (score <= currNode->score)
			break;
		prevNode = currNode;
		currNode = currNode->link;
	}

	newNode->link = currNode;	// ������ ��ġ�� ����
	if (prevNode == NULL)
		head = newNode;
	else
		prevNode->link = newNode;

	numOfRanks++;		// ���Ḯ��Ʈ ũ�⸦ ������Ʈ
	writeRankFile();	// rank.txt ���Ͽ� ������Ʈ
}

void ofApp::showRankBoard(void) {	// ��ŷ�� ȭ�鿡 ǥ���ϴ� �Լ�
	char score[30];
	Node* currNode = head;
	int i = 0;

	while (currNode) {
		if (i == 5)	// ���� 5������� �����ش�.
			break;
		sprintf(score, "%s", currNode->date);	// ����Ѵ�.
		rankplace.drawString(score, windowWidth / 2 - 68, 250 + 52 * i);
		sprintf(score, "%d", currNode->score);
		rankplace2.drawString(score, windowWidth / 2 + 75, 250 + 52 * i);
		currNode = currNode->link;
		i++;
	}
}

//------ Free all heap allocated memories during the process ------//
// �Ʒ��� �Լ��� ���α׷� ���� ���� �� �÷��׷� ���� �б�Ǿ� �����Ҵ�Ǿ���
// ��� ������ ���� �޸� ������ �����ϴ� �Լ��̴�. ����, ���� ��Ȳ�� �ƴ� ��Ȳ
// ���� �� �Լ��� ȣ��Ǹ�, �����Ҵ� ������ �����ϰ�, �÷��� ���� �ʱ⼳�� ���� ��
// ���� �ε��� ���� �ʱ�ȭ �۾��� �̷������.
void ofApp::freeMemory() {
	int i;
	struct Node * delNode;
	struct Node * currNode = head;

	first_line_flag = 1;	// �÷��׸� ��� ���� ù ���·� ����
	last_line_flag = 0;
	endsoundflag = 0;

	if (gameexit) {	// ���α׷� ���� �ÿ��� �����Ҵ� ����. ������ ����.
		for (i = 0; i < HEIGHT; i++)
			delete[] Maze[i];
		delete[] Maze;
		for (i = 0; i < HEIGHT; i++)
			delete[] Visit[i];
		delete[] Visit;
		delete[] lines;
		delete[] shortRoute;
		delete[] allRoute;
		delete[] rows;
		delete[] vert;
		delete[] horiz;
		while (currNode) {
			delNode = currNode;
			currNode = currNode->link;
			free(delNode);
		}
		head = NULL;
	}
	linesSize = 0;
	sIdx = aIdx = isdfs = isOpen = 0;	// �ε��� ���� �� �÷��� ���� ����
	allroute.clear();	// ofPolyLine�� ����ش�.
	shortroute.clear();
}