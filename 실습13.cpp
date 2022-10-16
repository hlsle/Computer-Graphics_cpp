#include "objRead.h"

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
void Keyboard(unsigned char key, int x, int y);
void SpecialKey(int key, int x, int y);
void Timer(int value);
float x_init, y_init, ox, oy;
GLuint vao[3], vbo[6];
objRead objReader;
objRead objReader2;
GLint Object = objReader.loadObj_normalize_center("cube.obj");
GLint Object2 = objReader2.loadObj_normalize_center("pyra.obj");
float line[4][3] = {
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
};
float line_col[4][3] = {
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 0.0, 0.0}
};
float cube_col[36][3] = {
	{1.0, 0.0, 0.0}, // 1
	{0.0, 1.0, 0.0}, // 2
	{0.0, 0.0, 1.0}, // 3
	{0.0, 0.0, 1.0}, // 3
	{1.0, 1.0, 0.0}, //4
	{1.0, 0.0, 0.0}, // 1
	{0.7, 0.7, 0.7}, //5
	{0.5, 0.5, 0.5}, //6
	{0.0, 1.0, 1.0}, //7 
	{0.0, 1.0, 1.0},
	{1.0,  0.0, 1.0}, //8
	{0.7, 0.7, 0.7},
	{1.0, 0.0, 0.0},
	{0.7, 0.7, 0.7},
	{1.0,  0.0, 1.0},
	{1.0,  0.0, 1.0},
	{0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{1.0,  0.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{1.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{0.0, 255 / 255, 255 / 255},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{1.0, 0.0, 0.0},
	{1.0, 1.0, 0.0}
};
float pyra_col[21][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 0.0, 0.0},
	{1.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
};
// 객체 생성 여부
bool is_cube = false, is_pyra = false;

// 은면 제거 여부
bool is_hid = false;
bool le = false;

// 좌표변환을 위한 행렬
glm::mat4 l = glm::mat4(1.0);
glm::mat4 c = glm::mat4(1.0); // transform
glm::mat4 r = glm::mat4(1.0); // rotate
glm::mat4 t = glm::mat4(1.0); // c * r

// 회전 여부
int ro_x = 0, ro_y = 0;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example13");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();
	InitShader();
	InitBuffer();
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutTimerFunc(100, Timer, 1);
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
	glGenVertexArrays(3, vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(6, vbo); //--- 2개의 VBO를 지정하고 할당하기

	// x, y 축
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_col), line_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 육면체 
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_col), cube_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 사각뿔
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, objReader2.outvertex.size() * sizeof(glm::vec3), &objReader2.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyra_col), pyra_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(s_program);
	//--- 사용할 VAO 불러오기

	//x, y축 그리기
	glBindVertexArray(vao[0]);
	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(l));
	glDrawArrays(GL_LINES, 0, 4);

	glEnable(GL_CULL_FACE);

	t = c * r;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(t));
	// 육면체 그리기
	if (is_cube) {
		glBindVertexArray(vao[1]);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	// 사각뿔 그리기
	if (is_pyra) {
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, 21);
	}


	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		// 육면체 생성
	case 'c': is_cube = true; is_pyra = false; c = glm::mat4(1.0f); t = glm::mat4(1.0f); r = glm::mat4(1.0f);
		break;

		// 사각뿔 생성
	case 'p': is_pyra = true; is_cube = false; c = glm::mat4(1.0f); t = glm::mat4(1.0f); r = glm::mat4(1.0f);
		break;

		// 은면 제거 적용/해제
	case 'h':
		if (!is_hid) {
			glEnable(GL_DEPTH_TEST);
			is_hid = true;
		}
		else {
			glDisable(GL_DEPTH_TEST);
			is_hid = false;
		}
		break;

		// x축 기준 양/음의 방향으로 회전
	case 'x': 
		if (ro_x == 1) {
			ro_x = 0;
			break;
		}
		ro_x = 1;
		break;
	case 'X': 
		if (ro_x == -1) {
			ro_x = 0;
			break;
		}
		ro_x = -1;
		break;

		// y축 기준 양 / 음의 방향으로 회전
	case 'y': 
		if (ro_y == 1) {
			ro_y = 0;
			break;
		}
		ro_y = 1;
		break;
	case 'Y': 
		if (ro_y == -1) {
			ro_y = 0;
			break;
		}
		ro_y = -1;
		break;

		// 와이어 / 솔리드 객체
	case 'w': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'W': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

		// 원위치
	case 's': 
		c = glm::mat4(1.0f);  
		r = glm::mat4(1.0f);
		t = glm::mat4(1.0f); 
		ro_x = 0; 
		ro_y = 0;
		break;
	}
	glutPostRedisplay();
}
void SpecialKey(int key, int x, int y) {

	switch (key) {
		// 방향키로 객체 이동
	case GLUT_KEY_LEFT: c = glm::translate(c, glm::vec3(-0.05, 0.0, 0.0));
		break;
	case GLUT_KEY_RIGHT: c = glm::translate(c, glm::vec3(0.05, 0.0, 0.0));
		break;
	case GLUT_KEY_UP: c = glm::translate(c, glm::vec3(0.0, 0.05, 0.0));
		break;
	case GLUT_KEY_DOWN: c = glm::translate(c, glm::vec3(0.0, -0.05, 0.0));
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {

	if (ro_x == 1) {
		r = glm::rotate(r, glm::radians(5.0f), glm::vec3(1.0, 0.0, 0.0));
	}
	else if (ro_x == -1) {
		r = glm::rotate(r, glm::radians(-5.0f), glm::vec3(1.0, 0.0, 0.0));
	}

	if (ro_y == 1) {
		r = glm::rotate(r, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (ro_y == -1) {
		r = glm::rotate(r, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
	}

	glutTimerFunc(100, Timer, 1);
	glutPostRedisplay();
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