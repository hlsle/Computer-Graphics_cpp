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
void Timer(int value);
float x_init, y_init, ox, oy;
GLuint vao[5], vbo[5];
objRead cube;
objRead cone;
objRead pyra;
objRead sphere;
GLint Object = cube.loadObj_normalize_center("cube.obj");
GLint Object2 = cone.loadObj_normalize_center("cone.obj");
GLint Object3 = pyra.loadObj_normalize_center("pyra.obj");
GLint Object4 = sphere.loadObj_normalize_center("sphere.obj");
float line[6][3] = {
	{0.0, 0.8, 0.0},
	{0.0, -0.8, 0.0},
	{0.8, -0.2, 0.0},
	{-0.8, 0.2, 0.0},
	{0.6, 0.6, 0.0},
	{-0.6, -0.6, 0.0}
};
// 객체 생성 여부
bool is_cube = true, is_cone = true, is_pyra = false, is_sphere = false;

// 은면 제거 여부
bool is_hid = false;
bool le = false;

// 좌표변환을 위한 행렬
glm::mat4 l = glm::mat4(1.0);
glm::mat4 cc = glm::translate(glm::mat4(1.0), glm::vec3(-0.5, 0.25, 0.0)); // cube or cone transform
glm::mat4 ps = glm::translate(glm::mat4(1.0), glm::vec3(0.5, -0.25, 0.0)); // pyra or sphere transform

glm::mat4 rcc = glm::mat4(1.0); // cube or cone rotate
glm::mat4 rps = glm::mat4(1.0); // pyra or sphere rotate
glm::mat4 ry = glm::mat4(1.0); // y rotate

glm::mat4 t = glm::mat4(1.0); // total

// 회전 여부
int ro_y = 0, cc_rx = 0, cc_ry = 0, ps_rx = 0, ps_ry = 0;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example14");
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
	glGenVertexArrays(5, vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(5, vbo); //--- 2개의 VBO를 지정하고 할당하기

	// x, y 축
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 육면체 
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, cube.outvertex.size() * sizeof(glm::vec3), &cube.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// 원뿔
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, cone.outvertex.size() * sizeof(glm::vec3), &cone.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// 사각뿔
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, pyra.outvertex.size() * sizeof(glm::vec3), &pyra.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// 구
	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sphere.outvertex.size() * sizeof(glm::vec3), &sphere.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
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
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(l * ry));
	glDrawArrays(GL_LINES, 0, 6);

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ry * cc * rcc));

	// 육면체 그리기
	if (is_cube) {
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, Object);
	}

	// 사각뿔 그리기
	if (is_pyra){
		glBindVertexArray(vao[3]);
		glDrawArrays(GL_TRIANGLES, 0, Object3);
	}
	

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ry * ps * rps));
	// 원뿔 그리기
	if (is_cone) {
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, Object2);
	}

	// 구 그리기
	if (is_sphere) {
		glBindVertexArray(vao[4]);
		glDrawArrays(GL_TRIANGLES, 0, Object4);
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
		// 다른 도형으로 바꾸기
	case 'c': 
		if (is_cube) {
			is_cube = false;
			is_cone = false;
			is_pyra = true;
			is_sphere = true;
		}
		else if (is_pyra) {
			is_cube = true;
			is_cone = true;
			is_pyra = false;
			is_sphere = false;
		}
		break;

		// 우측 객체 중심을 기준으로 x축  양/음의 방향으로 회전
	case 'x':
		if (ps_rx == 1) {
			ps_rx = 0;
			break;
		}
		ps_rx = 1;
		break;
	case 'X':
		if (ps_rx == -1) {
			ps_rx = 0;
			break;
		}
		ps_rx = -1;
		break;

		// 우측 객체 중심을 기준으로 y축 양 / 음의 방향으로 회전
	case 'y':
		if (ps_ry == 1) {
			ps_ry = 0;
			break;
		}
		ps_ry = 1;
		break;
	case 'Y':
		if (ps_ry == -1) {
			ps_ry = 0;
			break;
		}
		ps_ry = -1;
		break;

		// 좌측 객체 중심을 기준으로 x축  양/음의 방향으로 회전
	case 'a':
		if (cc_rx == 1) {
			cc_rx = 0;
			break;
		}
		cc_rx = 1;
		break;
	case 'A':
		if (cc_rx == -1) {
			cc_rx = 0;
			break;
		}
		cc_rx = -1;
		break;

		// 좌측 객체 중심을 기준으로 y축 양 / 음의 방향으로 회전
	case 'b':
		if (cc_ry == 1) {
			cc_ry = 0;
			break;
		}
		cc_ry = 1;
		break;
	case 'B':
		if (cc_ry == -1) {
			cc_ry = 0;
			break;
		}
		cc_ry = -1;
		break;

		// 두 객체를 y축 기준으로 양/음 방향으로 회전
	case 'r':
		if (ro_y == 1) {
			ro_y = 0;
			break;
		}
		ro_y = 1;
		break;
	case 'R':
		if (ro_y == -1) {
			ro_y = 0;
			break;
		}
		ro_y = -1;
		break;

		// 원위치
	case 's':
		ro_y = 0;
		cc_rx = 0;
		cc_ry = 0;
		ps_rx = 0;
		ps_ry = 0;
		cc = glm::translate(glm::mat4(1.0), glm::vec3(-0.5, 0.25, 0.0));
		ps = glm::translate(glm::mat4(1.0), glm::vec3(0.5, -0.25, 0.0));
		rcc = glm::mat4(1.0);
		rps = glm::mat4(1.0);
		ry = glm::mat4(1.0);
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {

	if (ro_y == 1) {
		ry = glm::rotate(ry, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (ro_y == -1) {
		ry = glm::rotate(ry, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
	}

	if (cc_rx == 1) {
		rcc = glm::rotate(rcc, glm::radians(5.0f), glm::vec3(1.0, 0.0, 0.0));
	}
	else if (cc_rx == -1) {
		rcc = glm::rotate(rcc, glm::radians(-5.0f), glm::vec3(1.0, 0.0, 0.0));
	}

	if (cc_ry == 1) {
		rcc = glm::rotate(rcc, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (cc_ry == -1) {
		rcc = glm::rotate(rcc, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
	}

	if (ps_rx == 1) {
		rps = glm::rotate(rps, glm::radians(5.0f), glm::vec3(1.0, 0.0, 0.0));
	}
	else if (ps_rx == -1) {
		rps = glm::rotate(rps, glm::radians(-5.0f), glm::vec3(1.0, 0.0, 0.0));
	}

	if (ps_ry == 1) {
		rps = glm::rotate(rps, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (ps_ry == -1) {
		rps = glm::rotate(rps, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
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