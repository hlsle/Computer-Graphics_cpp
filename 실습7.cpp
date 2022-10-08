#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* vertexsource, * fragmentsource; //--- 소스코드 저장 변수
GLuint vertexshader, fragmentshader; //--- 세이더 객체
GLuint s_program;
void InitBuffer();
void InitShader();
char* filetobuf(const char* file);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void changeOpenGL(int x, int y, float* ox, float* oy);
void timer(int value);
GLfloat triShape[12][3] = { //--- 삼각형 위치 값

};
const GLfloat colors[12][3] = { //--- 삼각형 꼭지점 색상
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } ,
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },
	{ 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 }
};
GLuint vao, vbo[2];
int cnt = 0;
int dir[4][2] = {
	{1, 1},
	{-1, 1},
	{-1, -1},
	{1, -1}
};
int wh[12] = { -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };
int n = 0;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Example7");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();
	InitShader();
	InitBuffer();
	glutTimerFunc(100, timer, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
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
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), triShape, GL_DYNAMIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), colors, GL_DYNAMIC_DRAW);
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
	//--- 변경된 배경색 설정
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	InitShader();
	InitBuffer();
	for (int i = 0; i < cnt; i += 3) {
		if (wh[i] == -1) {
			n = rand() % 4;
			wh[i] = n;
		}
		switch (i) {
		case 0:
			triShape[i][0] += dir[wh[i]][0] * 0.01f; triShape[i][1] += dir[wh[i]][1] * 0.01f;
			triShape[i + 1][0] += dir[wh[i]][0] * 0.01f; triShape[i + 1][1] += dir[wh[i]][1] * 0.01f;
			triShape[i + 2][0] += dir[wh[i]][0] * 0.01f; triShape[i + 2][1] += dir[wh[i]][1] * 0.01f;
			break;
		case 3:
			triShape[i][0] += dir[wh[i]][0] * 0.05f; triShape[i][1] += dir[wh[i]][1] * 0.05f;
			triShape[i + 1][0] += dir[wh[i]][0] * 0.05f; triShape[i + 1][1] += dir[wh[i]][1] * 0.05f;
			triShape[i + 2][0] += dir[wh[i]][0] * 0.05f; triShape[i + 2][1] += dir[wh[i]][1] * 0.05f;
			break;
		case 6:
			triShape[i][0] += dir[wh[i]][0] * 0.1f; triShape[i][1] += dir[wh[i]][1] * 0.1f;
			triShape[i + 1][0] += dir[wh[i]][0] * 0.1f; triShape[i + 1][1] += dir[wh[i]][1] * 0.1f;
			triShape[i + 2][0] += dir[wh[i]][0] * 0.1f; triShape[i + 2][1] += dir[wh[i]][1] * 0.1f;
			break;
		default:
			triShape[i][0] += dir[wh[i]][0] * 0.15f; triShape[i][1] += dir[wh[i]][1] * 0.15f;
			triShape[i + 1][0] += dir[wh[i]][0] * 0.15f; triShape[i + 1][1] += dir[wh[i]][1] * 0.15f;
			triShape[i + 2][0] += dir[wh[i]][0] * 0.15f; triShape[i + 2][1] += dir[wh[i]][1] * 0.15f;
			break;
		}

		//왼쪽
		if (triShape[i][0] <= -1.0f) {
			triShape[i][0] = -1.0f; triShape[i][1] += 0.1f;
			triShape[i + 1][0] = -1.0f; triShape[i + 1][1] = triShape[i][1] - 0.2f;
			triShape[i + 2][0] = triShape[i][0] + 0.3f; triShape[i + 2][1] = triShape[i][1] - 0.1f;
			wh[i] = -1;
		}
		if (triShape[i][0] >= 1.0f) {
			triShape[i][0] = 1.0f; triShape[i][1] = triShape[i][1] - 0.1f;
			triShape[i + 1][0] = triShape[i][0]; triShape[i + 1][1] = triShape[i][1] + 0.2f;
			triShape[i + 2][0] = triShape[i][0] + 0.3f; triShape[i + 2][1] = triShape[i][1] + 0.1f;
			wh[i] = -1;
		}
		if (triShape[i][1] >= 1.0f) {
			triShape[i][0] += 0.1f; triShape[i][1] = 1.0f;
			triShape[i + 1][0] = triShape[i][0] - 0.2f; triShape[i + 1][1] = 1.0f;
			triShape[i + 2][0] = triShape[i][0] - 0.1f; triShape[i + 2][1] = 0.7f;
			wh[i] = -1;
		}
		if (triShape[i][1] <= -1.0f) {
			triShape[i][0] -= 0.1f; triShape[i][1] = -1.0f;
			triShape[i + 1][0] = triShape[i][0] + 0.2f; triShape[i + 1][1] = -1.0f;
			triShape[i + 2][0] = triShape[i][0] + 0.1f; triShape[i + 2][1] = -0.7f;
			wh[i] = -1;
		}

		//오른쪽
		if (triShape[i + 1][0] <= -1.0f) {
			triShape[i + 1][0] = -1.0f; triShape[i + 1][1] = triShape[i + 1][1] - 0.1f;
			triShape[i + 2][0] = -0.7f; triShape[i + 2][1] = triShape[i + 1][1] + 0.1f;
			triShape[i][0] = -1.0f; triShape[i][1] = triShape[i + 1][1] + 0.2f;
			wh[i] = -1;
		}
		if (triShape[i + 1][0] >= 1.0f) {
			triShape[i + 1][0] = 1.0f; triShape[i + 1][1] = triShape[i + 1][1] + 0.1f;
			triShape[i + 2][0] = 0.7f; triShape[i + 2][1] = triShape[i + 1][1] - 0.1f;
			triShape[i][0] = 1.0f; triShape[i][1] = triShape[i + 1][1] - 0.2f;
			wh[i] = -1;
		}
		if (triShape[i + 1][1] >= 1.0f) {
			triShape[i + 1][0] = triShape[i + 1][0] - 0.1f; triShape[i + 1][1] = 1.0f;
			triShape[i + 2][0] = triShape[i + 1][0] + 0.1f; triShape[i + 2][1] = 0.7f;
			triShape[i][0] = triShape[i + 1][0] + 0.2f; triShape[i][1] = 1.0f;
			wh[i] = -1;
		}
		if (triShape[i + 1][1] <= -1.0f) {
			triShape[i + 1][0] = triShape[i + 1][0] + 0.1f; triShape[i + 1][1] = -1.0f;
			triShape[i + 2][0] = triShape[i + 1][0] - 0.1f; triShape[i + 2][1] = -0.7f;
			triShape[i][0] = triShape[i + 1][0] - 0.2f; triShape[i][1] = -1.0f;
			wh[i] = -1;
		}

		//위
		/*if (triShape[i + 2][0] <= -1.0f) {
			triShape[i + 1][0] = -1.0f; triShape[i + 1][1] = triShape[i + 1][1] - 0.1f;
			triShape[i + 2][0] = -0.7f; triShape[i + 2][1] = triShape[i + 1][1] + 0.1f;
			triShape[i][0] = -1.0f; triShape[i][1] = triShape[i + 1][1] + 0.2f;
		}
		if (triShape[i + 2][0] >= 1.0f) {
			triShape[i + 1][0] = 1.0f; triShape[i + 1][1] = triShape[i + 1][1] + 0.1f;
			triShape[i + 2][0] = 0.7f; triShape[i + 2][1] = triShape[i + 1][1] - 0.1f;
			triShape[i][0] = 1.0f; triShape[i][1] = triShape[i + 1][1] - 0.2f;
		}*/
		if (triShape[i + 2][1] >= 1.0f) {
			triShape[i + 2][1] = 0.7f;
			triShape[i + 1][0] = triShape[i + 2][0] - 0.1f; triShape[i + 1][1] = 1.0f;
			triShape[i][0] = triShape[i + 2][0] + 0.1f; triShape[i][1] = 1.0f;
			wh[i] = -1;
		}
		if (triShape[i + 2][1] <= -1.0f) {
			triShape[i + 2][1] = -0.7f;
			triShape[i + 1][0] = triShape[i + 2][0] + 0.1f; triShape[i + 1][1] = -1.0f;
			triShape[i][0] = triShape[i + 2][0] - 0.1f; triShape[i][1] = -1.0f;
			wh[i] = -1;
		}
	}
	glUseProgram(s_program);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	}
	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y) {
	float ox, oy;
	if (cnt == 12)
		return;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		changeOpenGL(x, y, &ox, &oy);
		triShape[cnt][0] = ox - 0.1f; triShape[cnt][1] = oy - 0.1f; triShape[cnt][2] = 0.0;
		triShape[cnt + 1][0] = ox + 0.1f; triShape[cnt + 1][1] = oy - 0.1f; triShape[cnt + 1][2] = 0.0;
		triShape[cnt + 2][0] = ox; triShape[cnt + 2][1] = oy + 0.2f; triShape[cnt + 2][2] = 0.0;
		cnt += 3;
	}
}
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(100, timer, 1);
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
