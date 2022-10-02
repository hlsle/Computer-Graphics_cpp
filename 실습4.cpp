#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void changeOpenGL(int x, int y, float* ox, float* oy);
void Move(int value);
void ZigZag(int value);
void Scale(int value);
int cnt = 0;
float xmov = 0.0f, ymov = 0.0f;
bool animMove = false, animZig, animScale;
struct Rect {
	float length = 0.2f;
	float x;
	float y;
	float x2;
	float y2;
	float x_direct = 1.0f;
	float y_direct = 1.0f;
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;
	bool make = false;
	float xinit = 0.0f;
	float yinit = 0.0f;
};
Rect rect[5];
void main(int argc, char** argv) {
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(800, 800); // �������� ũ�� ����
	glutCreateWindow("Example4"); // ������ ���� (������ �̸�)
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl
			;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	glClearColor(0.15, 0.15, 0.15, 0);
	srand((unsigned int)time(NULL));
	glutDisplayFunc(drawScene);// ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}
GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClear(GL_COLOR_BUFFER_BIT);
	// ������ ������ ��ü�� ĥ�ϱ�
   // �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	for (int i = 0; i < cnt; i++) {
		if (rect[i].make) {
			glColor3f(rect[i].r, rect[i].g, rect[i].b);
			glRectf(rect[i].x, rect[i].y, rect[i].x2, rect[i].y2);
		}
	}
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (rect[cnt].make == false) {
			rect[cnt].make = true;
			changeOpenGL(x, y, &rect[cnt].xinit, &rect[cnt].yinit);
			rect[cnt].x = rect[cnt].xinit - rect[cnt].length / 2;
			rect[cnt].y = rect[cnt].yinit - rect[cnt].length / 2;
			rect[cnt].x2 = rect[cnt].xinit + rect[cnt].length / 2;
			rect[cnt].y2 = rect[cnt].yinit + rect[cnt].length / 2;
			cnt++;
		}
	}
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		if (animMove)
			animMove = false;
		else {
			animMove = true;
			glutTimerFunc(100, Move, 1);
		}
		break;
	case 'i':
		if (animZig)
			animZig = false;
		else {
			animZig = true;
			glutTimerFunc(100, ZigZag, -1);
		}
		break;
	case 'c':
		if (animScale)
			animScale = false;
		else {
			animScale = true;
			glutTimerFunc(200, Scale, -1);
		}
		break;
	case 's':
		animScale = false;
		animZig = false;
		animMove = false;
		break;
	case 'm':
		for (int i = 0; i < cnt; i++) {
			rect[i].x = rect[i].xinit - rect[i].length / 2;
			rect[i].y = rect[i].yinit - rect[i].length / 2;
			rect[i].x2 = rect[i].xinit + rect[i].length / 2;
			rect[i].y2 = rect[i].yinit + rect[i].length / 2;
		}
		break;
	case 'r':
		for (int i = 0; i < cnt; i++)
			rect[i].make = false;
		cnt = 0;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}
void changeOpenGL(int x, int y, float* ox, float* oy)
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}
void Move(int value) {
	if (animMove) {
		for (int i = 0; i < cnt; i++) {
			rect[i].x += rect[i].x_direct * 0.03f; rect[i].y += rect[i].y_direct * 0.03f;
			rect[i].x2 += rect[i].x_direct * 0.03f; rect[i].y2 += rect[i].y_direct * 0.03f;
			if (rect[i].x <= -1.0f || rect[i].x2 >= 1.0f) {
				rect[i].x_direct *= -1;
				if (rect[i].x < -1.0f) {
					rect[i].x += rect[i].x_direct * (-1.0f - rect[i].x);
					rect[i].x2 += rect[i].x_direct * (-1.0f - rect[i].x);
				}
				if (rect[i].x2 > 1.0f) {
					rect[i].x += rect[i].x_direct * (rect[i].x2 - 1.0f);
					rect[i].x2 += rect[i].x_direct * (rect[i].x2 - 1.0f);
				}
			}
			if (rect[i].y <= -1.0f || rect[i].y2 >= 1.0f) {
				rect[i].y_direct *= -1;
				if (rect[i].y < -1.0f) {
					rect[i].y += rect[i].y_direct * (-1.0f - rect[i].y);
					rect[i].y2 += rect[i].y_direct * (-1.0f - rect[i].y);
				}
				if (rect[i].y2 > 1.0f) {
					rect[i].y += rect[i].x_direct * (rect[i].y2 - 1.0f);
					rect[i].y2 += rect[i].x_direct * (rect[i].y2 - 1.0f);
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(100, Move, 1);
	}
}
void ZigZag(int value) {
	if (animZig) {
		for (int i = 0; i < cnt; i++) {
			rect[i].x += value * 0.03f; rect[i].y += rect[i].y_direct * 0.03f;
			rect[i].x2 += value * 0.03f; rect[i].y2 += rect[i].y_direct * 0.03f;
			if (rect[i].y2 >= 1.0f) {
				rect[i].y_direct *= -1;
				if (rect[i].y2 > 1.0f) {
					rect[i].x += value * 0.03f; rect[i].y += rect[i].y_direct * (rect[i].y2 - 1.0f);
					rect[i].x2 += value * 0.03f; rect[i].y2 += rect[i].y_direct * (rect[i].y2 - 1.0f);
				}
				else {
					rect[i].x += value * 0.03f; rect[i].y += rect[i].y_direct * 0.03f;
					rect[i].x2 += value * 0.03f; rect[i].y2 += rect[i].y_direct * 0.03f;
				}
			}
			if (rect[i].y <= -1.0f)
				rect[i].y_direct *= -1;
		}
		glutPostRedisplay();
		glutTimerFunc(100, ZigZag, -1 * value);
	}
}
void Scale(int value) {
	if (animScale) {
		for (int i = 0; i < cnt; i++) {
			float m = (float)rand() / (float)RAND_MAX;
			float n = (float)rand() / (float)RAND_MAX;
			rect[i].x -= value * m; rect[i].y -= value * n;
			rect[i].x2 += value * m; rect[i].y2 += value * n;
		}
		glutPostRedisplay();
		glutTimerFunc(200, Scale, -1 * value);
	}
}