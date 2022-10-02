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
void Motion(int x, int y);
int cnt = 1;
float xmov = 0.0f, ymov = 0.0f;
struct Rect {
	float x = -0.3f;
	float y = -0.3f;
	float x2 = 0.3f;
	float y2 = 0.3f;
	float r = 1.0f;
	float g = 0.0f;
	float b = 0.0f;
	bool left_button = false;
	float ox = 0.0f;
	float oy = 0.0f;
	float mx = 0.0f;
	float my = 0.0f;
};
Rect rect[5];
void main(int argc, char** argv) {
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
	glutCreateWindow("Example3"); // 윈도우 생성 (윈도우 이름)
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
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 
}
GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClear(GL_COLOR_BUFFER_BIT);
	 // 설정된 색으로 전체를 칠하기
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다
	for (int i = 0; i < cnt; i++) {
		glColor3f(rect[i].r, rect[i].g, rect[i].b);
	
		glRectf(rect[i].x, rect[i].y, rect[i].x2, rect[i].y2);
		//rect[i].x += rect[i].mx - rect[i].ox; rect[i].x2 += rect[i].mx - rect[i].ox;
		//rect[i].y += rect[i].my - rect[i].oy; rect[i].y2 += rect[i].my - rect[i].oy;
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
		for (int i = 0; i < cnt; i++) {
			changeOpenGL(x, y, &rect[i].ox, &rect[i].oy); // 마우스 위치를 지엘좌표로 바꿔 저장
			if (rect[i].ox >= rect[i].x && rect[i].ox <= rect[i].x2 &&
				rect[i].oy >= rect[i].y && rect[i].oy <= rect[i].y2) { //마우스가 사각형 안에 있는지 확인
				rect[i].left_button = true;
				if (i > 0) {
					for(int j= i; j > 0; j--)
						if (rect[j - 1].left_button)
							rect[j - 1].left_button = false;
				}
			}
			else rect[i].left_button = false;
		}
		
	}
	glutPostRedisplay();
}
void Motion(int x, int y) {
	
	for (int i = 0; i < cnt; i++) {
		if (rect[i].left_button) {
			changeOpenGL(x, y, &rect[i].mx, &rect[i].my);
			xmov += rect[i].mx - rect[i].ox;
			ymov += rect[i].my - rect[i].oy;
			rect[i].x += xmov; rect[i].x2 += xmov;
			rect[i].y += ymov; rect[i].y2 += ymov;
			rect[i].ox = rect[i].mx; rect[i].oy = rect[i].my;
		}
		xmov = 0.0f; ymov = 0.0f;
	}
	
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a': 
		if (cnt == 5)
			break;
		else {
			rect[cnt].r = (float)rand() / (float)RAND_MAX; 
			rect[cnt].g = (float)rand() / (float)RAND_MAX;
			rect[cnt].b = (float)rand() / (float)RAND_MAX;
			cnt++;
		}
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