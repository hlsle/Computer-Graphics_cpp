#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void changeOpenGL(int x, int y, float* ox, float* oy);
float xmov = 0.0f, ymov = 0.0f;
struct Rect {
	float x = ((float)rand() / (float)RAND_MAX) * 1.8f - 0.9f;
	float y = ((float)rand() / (float)RAND_MAX) * 1.8f - 0.9f;
	float x2 = x + 0.1f;
	float y2 = y + 0.1f;
	float x_direct = 1.0f;
	float y_direct = 1.0f;
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;
	bool make = true;
};
struct Re {
	bool make = false;
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;
	float x;
	float y;
	float xinit = 0.0f;
	float yinit = 0.0f;
	float xm;
	float ym;
};
Rect rect[100];
Re re;
void main(int argc, char** argv) {
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
	glutCreateWindow("Example4"); // 윈도우 생성 (윈도우 이름)
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl
			;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	glClearColor(0.4, 0.5, 0.6, 0);
	srand((unsigned int)time(NULL));
	glutDisplayFunc(drawScene);// 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작 
}
GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (re.make) {
		glColor3f(0.2f, 0.2f, 0.2f);
		glRectf(re.x - 0.1f, re.y - 0.1f, re.x + 0.1f, re.y + 0.1f);
	}
	for (int i = 0; i < 100; i++) {
		if (rect[i].x >= re.x - 0.1f && rect[i].x <= re.x+0.1f &&
			rect[i].y >= re.y - 0.1f && rect[i].y <= re.y + 0.1f) {
			rect[i].make = false;
		}
		if (rect[i].x2 >= re.x - 0.1f && rect[i].x2 <= re.x + 0.1f &&
			rect[i].y2 >= re.y - 0.1f && rect[i].y2 <= re.y + 0.1f) {
			rect[i].make = false;
		}
		if (rect[i].x >= re.x - 0.1f && rect[i].x <= re.x + 0.1f &&
			rect[i].y2 >= re.y - 0.1f && rect[i].y2 <= re.y + 0.1f) {
			rect[i].make = false;
		}
		if (rect[i].x2 >= re.x - 0.1f && rect[i].x2 <= re.x + 0.1f &&
			rect[i].y >= re.y - 0.1f && rect[i].y <= re.y + 0.1f) {
			rect[i].make = false;
		}
		if (rect[i].make) {
			glColor3f(rect[i].r, rect[i].g, rect[i].b);
			glRectf(rect[i].x, rect[i].y, rect[i].x2, rect[i].y2);
		}
	}
	
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		re.make = true;
		changeOpenGL(x, y, &re.xinit, &re.yinit);
		re.x = re.xinit; re.y = re.yinit;
	}
	else re.make = false;
	glutPostRedisplay();
}
void changeOpenGL(int x, int y, float* ox, float* oy)
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}
void Motion(int x, int y) {
	if (re.make) {
		changeOpenGL(x, y, &re.xm, &re.ym);
		re.x += re.xm - re.xinit; re.y += re.ym - re.yinit;
		re.xinit = re.xm; re.yinit = re.ym;
	}
	glutPostRedisplay();
}