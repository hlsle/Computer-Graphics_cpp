#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void main(int argc, char** argv) {
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(600, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름)
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
	glColor3f(1, 0, 0);
	glutDisplayFunc(drawScene);// 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작 
}
GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
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
