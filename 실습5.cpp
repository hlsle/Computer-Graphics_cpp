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
	glClearColor(0.4, 0.5, 0.6, 0);
	srand((unsigned int)time(NULL));
	glutDisplayFunc(drawScene);// ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}
GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
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
	
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
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