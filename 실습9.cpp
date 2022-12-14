#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <math.h>
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* vertexsource, * fragmentsource; //--- 소스코드 저장 변수
GLuint vertexshader, fragmentshader; //--- 세이더 객체
GLuint s_program;
void InitBuffer();
void InitShader();
char* filetobuf(const char* file);
void changeOpenGL(int x, int y, float* ox, float* oy);
void Mouse(int button, int state, int x, int y);
void timer(int value);
int bg = 0, dr, d = 0;
float angle = 0.1, r = 0.01;
bool f = false;
GLfloat dot[150][3] = { 
	
};
GLfloat colors[150][3] = { 
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }
};
GLfloat background[5][3] = {
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.5, 0.5, 0.5 },
};
GLuint vao, vbo[2];
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Example9");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();
	InitShader();
	InitBuffer();
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glutTimerFunc(50, timer, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMainLoop();
}
void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(dot), dot, GL_STATIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}
void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}
GLvoid drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	InitShader();
	InitBuffer();

	glUseProgram(s_program);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
	glPointSize(6.0);
	if (f)
		glDrawArrays(GL_POINTS, 0, d+1); // 150번째 점까지 계속 그리기
	
	d++;
	if (d == 150) // 인덱스 초과하면 마지막 인덱스로 고정
		d = 149;
	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// 변수들 초기화
		f = false; 
		angle = 0.1; 
		r = 0.01; 
		dr = rand() % 2; 
		d = 0;

		if (!f) {
			changeOpenGL(x, y, &dot[0][0], &dot[0][1]);
			f = true;
		}
		// 배경색 바꾸기
		glClearColor(background[bg % 5][0], background[bg % 5][1], background[bg % 5][2], 1.0f);
		bg++;

		if (dr == 0) { //반시계
			for (int i = 1; i < 75; i++) {
				dot[i][0] = r * cos(angle) + dot[0][0];
				dot[i][1] = r * sin(angle) + dot[0][1];
				dot[i][2] = 0.0f;
				r += 0.0055f;
				angle += 0.256f;
			}

			dot[75][0] = dot[74][0]; dot[75][1] = dot[74][1];
			for (int i = 76; i < 150; i++) {
				if (dot[74][0] - dot[149 - i][0] < 0)
					dot[i][0] = dot[74][0] - (dot[74][0] - dot[149 - i][0]);
				else if (dot[74][0] - dot[149 - i][0] > 0)
					dot[i][0] = dot[74][0] + (dot[74][0] - dot[149 - i][0]);

				dot[i][1] = dot[74][1] + (dot[74][1] - dot[149 - i][1]);

				dot[i][2] = 0.0f;
			}
		}
		else { // 시계
			for (int i = 1; i < 75; i++) {
				dot[i][0] = r * cos(angle) + dot[0][0];
				dot[i][1] = r * sin(angle) + dot[0][1];
				dot[i][2] = 0.0f;
				r -= 0.0055f;
				angle -= 0.256f;
			}

			dot[75][0] = dot[74][0]; dot[75][1] = dot[74][1];
			for (int i = 76; i < 150; i++) {
				if (dot[74][0] - dot[149 - i][0] < 0)
					dot[i][0] = dot[74][0] + (dot[74][0] - dot[149 - i][0]);
				else if (dot[74][0] - dot[149 - i][0] > 0)
					dot[i][0] = dot[74][0] - (dot[74][0] - dot[149 - i][0]);
			
				dot[i][1] = dot[74][1] + (dot[74][1] - dot[149 - i][1]);
			
				dot[i][2] = 0.0f;
			}
		}
	}
	
}
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(50, timer, 1);
}
//opengl 좌표로 변환
void changeOpenGL(int x, int y, float* ox, float* oy)
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}