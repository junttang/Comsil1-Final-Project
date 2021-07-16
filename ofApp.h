#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"

//------ Fundamental C Structures for Implementaion ------//
struct Line {			// ������ ǥ���ϱ� ���� ����ü
	float x1, y1;		// ������ �� �� �� (x1, y1)�� (x2, y2)
	float x2, y2;
};

struct Point {			// ���� ǥ���ϱ� ���� ����ü
	int x, y;			// (x, y)
};

struct Node {			// ���Ḯ��Ʈ�� �����ϱ� ���� ��� ����ü
	char date[15];		// ���� �÷��̰� ����� ��¥ �� �ð��� ���
	int score;			// 1ȸ�� ���ӿ����� ���� ������ ���
	struct Node *link;	// link������
};

class ofApp : public ofBaseApp {
public:
	//------ Fundamental Data Structures Declaration ------//
	struct Line *lines;	// �̷θ� ȭ�� �� �׸��� ���� ���� �迭
	struct Point *shortRoute;	// �ִ� ��θ� �� ���� ����� ����
	struct Point *allRoute;		// ��ü ��θ� �� ���� ����� �迭
	char **Maze;		// maze.maz�κ��� �̷θ� �о���� 2���� �迭
	int **Visit;		// �̷��� Ư�� ĭ �湮 ���θ� ����� 2���� �迭
	int linesSize = 0;	// lines �迭�� ũ�⸦ ����� ����
	int sIdx = 0;		// �ִ� ��� ������ top�� ��Ī�ϴ� ����
	int aIdx = 0;		// ��ü ��� �迭�� ũ�⸦ ����� ����
	struct Point move[4] = {	// DFS �ִܰŸ� Ž���� ���� ���� ����ü
		{ 1,0 }, { 0,1 }, { -1,0 }, { 0,-1 }
	};
	int *rows, *vert, *horiz;	// ������ �̷θ� �����ϴµ� ���Ǵ� �迭��
	ofPolyline shortroute;	// �ִܰ�θ� �׸��� �������� ���� ofPolyLine
	ofPolyline allroute;	// ��ü��θ� �׸��� �������� ���� ofPolyLine

	//------ Fundamental Coordinates Representation ------//
	int HEIGHT;			// �̷��� ���̸� ���
	int WIDTH;			// �̷��� ���� ���
	int coordX;			// ���� ���� ��ġ�� ����� ����
	int coordY;
	int targetX;		// ġ��(������)�� ��ġ�� ����� ����
	int targetY;

	//------ Essential Variables for Implementation ------//
	int selectIdx = 0;	// ����ȭ�鿡�� ���� ���� ��ġ�� �Ǵ��ϴ� �ε�������
	int currentframe;	// ���� �����Ӽ��� �Է¹��� ����
	int playtime;		// ���� �÷��� �ҿ� �ð��� ����� ����
	int loadframe;		// �ε� ȭ�� �ð� delay�� ���� ������ ��� ����
	struct Node * head = NULL;	// ��ŷ ���Ḯ��Ʈ�� ���
	int numOfRanks = 0;	// ���Ḯ��Ʈ�� ��� ���� ���
	char printScore[10];// ���� �÷��� �� ���ھ� ǥ�ø� ���� ���ڿ�
	char date[10];		// ���� �÷��� ���� �� ��¥ �� �ð��� ����� ���ڿ�
	int currentTopRanker = 999;	// ���� ��ŷ �ý��ۿ��� �ְ����� ����� ����

	//------ Flag Variables for Efficient Branching ------//
	int isOpen;			// maze.maz ���� ���� ���� ���� �÷���
	int first_heap_alloc_flag = 1;	// ���� ���� ���� ���� �޸� �Ҵ� �÷���
	int first_line_flag = 1;	// �̷� ���� �� ù ��° ������ ������ �÷���
	int last_line_flag = 0;		// �̷� ���� �� ������ ������ ������ �÷���
	int initscrflag = 1;// ���� �ʱ�ȭ�� ǥ�� �÷���
	int isload = 0;		// �ε�ȭ�� ǥ�� �÷���
	bool isgame;		// ���� ���� �÷���
	int upflag = 0;		// '��'Ű�� ������ ���� �÷���
	int downflag = 1;	// '��'Ű�� ������ ���� �÷���
	int rightflag = 0;	// '��'Ű�� ������ ���� �÷���
	int leftflag = 0;	// '��'Ű�� ������ ���� �÷���
	int resetinitflag = 1;	// 10�� ���� ������ ���� 10�� �ҿ� ���� �÷���
	int reachtargetflag = 0;// ������ ���� ���� �÷���
	int endsoundflag = 0;	// ���� ���� �÷���
	bool isdfs;			// DFS ��� ǥ�� �÷���
	int gameexit = 0;	// ���� ���� �÷���
	int rankscrflag = 0;// ��ŷ ǥ�� �÷���

	//------ Fundamental Functions Declaration ------//
	void freeMemory();	// �� ȸ ���� �� �缳�� �� ���� ���� ���� �� �޸� ����
	bool readFile();	// maze.maz ������ �о�鿩 �̷� ���� �� ȭ�� ǥ�� �غ�
	Line pathFind(int i, int j, int flag);	// �̷� ���� �� ���� �Լ�
	void randomize(int n, int *set);	// ���� �̷� ���� �� �����ϰ� �ʱ� ����
	void randomizeH(int n, int *set);	// �̷� ���� �� ���� �����ϰ� ����
	void randomizeV(int n);	// �̷� ���� �� ������ �����ϰ� ����
	void makemazefile(void);// ������ �̷θ� ���� maze.maz ������ ����
	void drawMaze(FILE* fp, int n);	// �̷θ� maze.maz�� �׸��� �Լ�
	void createRankList(void);	// rank.txt ������ �о� ��ŷ ����
	void writeRankFile(void);	// rank.txt ���Ͽ� ������ ��ŷ�� ������Ʈ
	void newRank(int score);	// �� ���� ���� �� ���ο� ������ ��ŷ�� ����
	void initScreen(void);	// ������ �ʱ�ȭ���� ǥ���ϴ� �Լ�
	void loadingScreen(int loadframe);	// �ε�ȭ���� ǥ���ϴ� �Լ�
	void showRankBoard(void);	// ��ŷ�� ȭ�鿡 ǥ���ϴ� �Լ�
	bool DFS();			// �ִ� ��� ����� �����ϴ� DFS�Լ�
	void dfsdraw();		// DFS ��� �ִ� ��θ� ȭ�鿡 ǥ���ϴ� �Լ�
	void drawclock(double current);	// ���� �� �ð� ������ ǥ���ϴ� �Լ�
	void PlayGame(void);// 1ȸ�� ������ ��ü������ �����ϴ� �Լ�
	Point maximumDist(void);	// 10�� ���� ������ ������ ���� �Լ�

	//------ Base OFW & GUI Interface Declaration ------//
	ofTrueTypeFont myFont;
	float windowWidth, windowHeight;
	HWND hWnd;
	HWND hWndForeground;
	ofxWinMenu * menu;
	bool bShowInfo;
	bool bFullscreen;
	bool bTopmost;

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void appMenuFunction(string title, bool bChecked);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void doFullScreen(bool bFull);
	void doTopmost(bool bTop);

	//------ External Data Declaration for Importing ------//
	ofTrueTypeFont rankplace;	// GUI �� ���� �� ���� ������ ���� �ʿ���
	ofTrueTypeFont rankplace2;	// �ܺ� ���� ������ ���α׷��� �ε��ϱ� ����
	ofTrueTypeFont gamescore;	// ���� �����

	clock_t startgame;
	clock_t endgame;
	clock_t resetcriteria;
	clock_t resetfull;
	
	ofSoundPlayer menubgm;
	ofSoundPlayer menusound;
	ofSoundPlayer gamebgm;
	ofSoundPlayer endsound;
	ofSoundPlayer whoosh;
	ofSoundPlayer hooray;

	ofImage gamelogo;
	ofImage gameover;
	ofImage loadone;
	ofImage loadtwo;
	ofImage loadthree;
	ofImage loadfour;
	ofImage ratup;
	ofImage ratdown;
	ofImage ratright;
	ofImage ratleft;
	ofImage cheese;
	ofImage playbutton;
	ofImage rankbutton;
	ofImage exitbutton;
	ofImage returnbutton;
	ofImage quitbutton;
	ofImage brickwall;
	ofImage whitewall;
	ofImage decoration;
	ofImage numberone;
	ofImage scoreboard;
	ofImage rankboard;
	ofImage clockboard;
	ofImage clocktimer1;
	ofImage clocktimer2;
	ofImage clocktimer3;
	ofImage clocktimer4;
	ofImage clocktimer5;
	ofImage clocktimer6;
	ofImage clocktimer7;
	ofImage clocktimer8;
	ofImage clocktimer9;
	ofImage clocktimer10;
	ofImage clocktimer11;
	ofImage clocktimer12;
};