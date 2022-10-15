#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void main(int argc, char** argv) {
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(600, 600); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
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
	glColor3f(1, 0, 0);
	glutDisplayFunc(drawScene);// ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutMainLoop(); // �̺�Ʈ ó�� ���� 
}
GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (x >=glutGet(GLUT_WINDOW_WIDTH)/4 && x <= glutGet(GLUT_WINDOW_WIDTH)*3 / 4 && 
			y >= glutGet(GLUT_WINDOW_HEIGHT) / 4 && y <= glutGet(GLUT_WINDOW_HEIGHT) * 3 / 4) {
			glColor3f(rand()%2, rand() % 2, rand() % 2);
		}
		else {
			glClearColor(rand() % 5, rand() % 5, rand() % 5, 0);
		}
		glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		if (x >=glutGet(GLUT_WINDOW_WIDTH)/4 && x <= glutGet(GLUT_WINDOW_WIDTH)*3 / 4 && 
			y >= glutGet(GLUT_WINDOW_HEIGHT) / 4 && y <= glutGet(GLUT_WINDOW_HEIGHT) * 3 / 4) {
			glScalef(1.5, 1.5, 1.5);
		}
		else {
			glScalef(0.8, 0.8, 0.8);
		}
		glutPostRedisplay();
	}
}
