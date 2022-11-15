#include "objRead.h"
#define VAO_SIZE 4
#define VBO_SIZE 8
#define RANDNUM 6

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* vertexsource, * fragmentsource; //--- 소스코드 저장 변수
GLuint vertexshader, fragmentshader; //--- 세이더 객체
GLuint s_program;
void InitBuffer();
void InitShader();
char* filetobuf(const char* file);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void changeOpenGL(int x, int y, float* ox, float* oy);
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);
float x_init, y_init, ox, oy, mx, my;
GLuint vao[VAO_SIZE], vbo[VBO_SIZE];
glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 0.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f); //--- 카메라 위쪽 방향
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
unsigned int projectionLocation;
unsigned int viewLocation;
objRead box;
objRead ball;
objRead cube;
int object = box.loadObj_normalize_center("box.obj");
int object2 = ball.loadObj_normalize_center("ball.obj");
int object3 = cube.loadObj_normalize_center("cube.obj");

float box_col[36][3] = {
	{0.2, 0.2, 0.2},
	{0.2, 0.2, 0.2},
	{0.2, 0.2, 0.2},
	{0.2, 0.2, 0.2},
	{0.2, 0.2, 0.2},
	{0.2, 0.2, 0.2},

	{0.1, 0.1, 0.1},
	{0.1, 0.1, 0.1},
	{0.1, 0.1, 0.1},
	{0.1, 0.1, 0.1},
	{0.1, 0.1, 0.1},
	{0.1, 0.1, 0.1},

	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},

	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},

	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},
	{0.4, 0.4, 0.4},

	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7},
	{0.7, 0.7, 0.7}
};

vector<glm::vec3> ball_col(2880, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> cube_col(36, glm::vec3(1.0, 0.0, 0.0));
vector<glm::vec3> cube_col2(36, glm::vec3(0.0, 0.0, 0.0));

// box size = 1 ball r=0.05 cube size = 0.1
vector<bool> is_ball;
vector<glm::vec3> ball_pos(5, glm::vec3(0.0, 0.0, 0.0));
vector<glm::vec3> ball_dir;
vector<glm::vec3> cube_pos;
glm::vec3 gravity;
glm::vec3 u(0.0, 1.0, 0.0);
glm::vec3 d(0.0, 0.0, 1.0);

float speed[10] = {
	1.0, 
};

glm::mat4 T_box = glm::mat4(1.0);
vector<glm::mat4> T_ball(5, glm::mat4(1.0));
vector<glm::mat4> T_cube(3, glm::mat4(1.0));

glm::mat4 R_box = glm::mat4(1.0);
glm::mat4 R_cube = glm::mat4(1.0);

vector<glm::mat4> S_cube(3, glm::mat4(1.0));

int n = 0, p = -1;
float angle = 0.0;
bool l = false, open = false;
float random_dir[RANDNUM]{
	0.05, -0.05,
	0.1, -0.1,
	0.02, -0.02
};
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example21");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 5; i++) {
		float x = random_dir[rand() % RANDNUM];
		float y = random_dir[rand() % RANDNUM];
		float z = random_dir[rand() % RANDNUM];
		ball_dir.push_back(glm::vec3(x, y, z));
	}

	//0=0.425 1=0.45 2=0.475
	S_cube[0] = glm::scale(S_cube[0], glm::vec3(1.5, 1.5, 1.5));
	S_cube[2] = glm::scale(S_cube[2], glm::vec3(0.5, 0.5, 0.5));

	T_cube[0] = glm::translate(T_cube[0], glm::vec3(0.0, 0.01, 0.425));
	T_cube[1] = glm::translate(T_cube[1], glm::vec3(0.0, cube.scaleY * 2, 0.45));
	T_cube[2] = glm::translate(T_cube[2], glm::vec3(0.0, cube.scaleY * 3.5, 0.475));

	cube_pos.push_back(glm::vec3(0.0, 0.0, 0.425));
	cube_pos.push_back(glm::vec3(0.0, 0.0, 0.45));
	cube_pos.push_back(glm::vec3(0.0, 0.0, 0.475));

	//원근 투영
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, 0.0));

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
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
	glGenVertexArrays(VAO_SIZE, vao);
	glGenBuffers(VBO_SIZE, vbo);

	//box
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, box.outvertex.size() * sizeof(glm::vec3), &box.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_col), box_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//ball
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, ball.outvertex.size() * sizeof(glm::vec3), &ball.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, ball_col.size() * sizeof(glm::vec3), &ball_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//cube
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, cube.outvertex.size() * sizeof(glm::vec3), &cube.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, cube_col.size() * sizeof(glm::vec3), &cube_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, cube.outvertex.size() * sizeof(glm::vec3), &cube.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, cube_col2.size() * sizeof(glm::vec3), &cube_col2[0], GL_STATIC_DRAW);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	
	viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));

	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_box * R_box));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// make box
	glBindVertexArray(vao[0]);
	if(!open)
		glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawArrays(GL_TRIANGLES, 12, 24);

	glFrontFace(GL_CCW);
	//make ball
	glBindVertexArray(vao[1]);
	for (int i = 0; i < is_ball.size(); i++) {
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_box * R_box * T_ball[i]));
		glDrawArrays(GL_TRIANGLES, 0, object2);
	}
	
	// make cube
	for (int i = 0; i < 3; i++) {
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_box * R_box * T_cube[i]  * S_cube[i]));
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, object3);
	}
	for (int i = 0; i < 3; i++) {
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_box * R_box * T_cube[i]  * S_cube[i]));
		glBindVertexArray(vao[3]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(2.0);
		glDrawArrays(GL_TRIANGLES, 0, object3);
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
		// z축 이동 (카메라 위치가 y축에 있으므로 y로 움직여야 가까워졌다 멀어졌다함)
	case 'z':
		T_box = glm::translate(T_box, glm::vec3(0.0, 0.1, 0.0));
		break;
	case 'Z':
		T_box = glm::translate(T_box, glm::vec3(0.0, -0.1, 0.0));
		break;

		// y축 공전
	case 'y':
		R_box = glm::rotate(R_box, glm::radians(9.0f), glm::vec3(0.0, 1.0, 0.0));
		angle -= 9.0f;
		//cameraUp.x = sin(glm::radians(180.0f + angle));
		//cameraUp.z = cos(glm::radians(180.0f + angle));
		d.x = sin(glm::radians(angle));
		d.z = cos(glm::radians(angle));
		break;
	case 'Y':
		R_box = glm::rotate(R_box, glm::radians(-9.0f), glm::vec3(0.0, 1.0, 0.0));
		angle += 9.0f;
		d.x = sin(glm::radians(angle));
		d.z = cos(glm::radians(angle));
		break;
		
		// 공 추가하고 튕기기 최대 5개
	case 'B':
		if (is_ball.size() == 5)
			break;
		is_ball.push_back(true);
		break;

	case 'o':
		open = true;
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {
	if (!open) {
		// 공 이동
		for (int i = 0; i < is_ball.size(); i++) {
			T_ball[i] = glm::translate(T_ball[i], glm::vec3(ball_dir[i]));
			ball_pos[i] += ball_dir[i];

			if (ball_pos[i].x > 0.45) {
				T_ball[i] = glm::translate(T_ball[i], glm::vec3(-ball_dir[i].x, 0.0, 0.0));
				ball_pos[i].x -= ball_dir[i].x;
				ball_dir[i].x = random_dir[rand() % RANDNUM];
			}
			if (ball_pos[i].x < -0.45) {
				T_ball[i] = glm::translate(T_ball[i], glm::vec3(-ball_dir[i].x, 0.0, 0.0));
				ball_pos[i].x -= ball_dir[i].x;
				ball_dir[i].x = random_dir[rand() % RANDNUM];
			}

			if (ball_pos[i].y > 0.95) {
				T_ball[i] = glm::translate(T_ball[i], glm::vec3(0.0, -ball_dir[i].y, 0.0));
				ball_pos[i].y -= ball_dir[i].y;
				ball_dir[i].y = random_dir[rand() % RANDNUM];
			}
			if (ball_pos[i].y < 0.05) {
				T_ball[i] = glm::translate(T_ball[i], glm::vec3(0.0, -ball_dir[i].y, 0.0));
				ball_pos[i].y -= ball_dir[i].y;
				ball_dir[i].y = random_dir[rand() % RANDNUM];
			}

			if (ball_pos[i].z > 0.45) {
				T_ball[i] = glm::translate(T_ball[i], glm::vec3(0.0, 0.0, -ball_dir[i].z));
				ball_pos[i].z -= ball_dir[i].z;
				ball_dir[i].z = random_dir[rand() % RANDNUM];
			}
			if (ball_pos[i].z < -0.45) {
				T_ball[i] = glm::translate(T_ball[i], glm::vec3(0.0, 0.0, -ball_dir[i].z));
				ball_pos[i].z -= ball_dir[i].z;
				ball_dir[i].z = random_dir[rand() % RANDNUM];
			}
		}
		
		// 자유 낙하?
		for (int i = 0; i < T_cube.size(); i++) {
				T_cube[i] = glm::translate(T_cube[i], glm::vec3(d.x * 0.02, 0.0, d.z * 0.02));
				cube_pos[i].x += d.x * 0.02;
				cube_pos[i].z += d.z * 0.02;
				switch (i) {
				case 0:
					if (cube_pos[i].x < -0.425) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(-0.425 - cube_pos[i].x, 0.0, 0.0));
						cube_pos[i].x += -0.425 - cube_pos[i].x;
					}
					if (cube_pos[i].x > 0.425) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.425 - cube_pos[i].x, 0.0, 0.0));
						cube_pos[i].x += 0.425 - cube_pos[i].x;
					}
					if (cube_pos[i].z < -0.425) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.0, 0.0, -0.425 - cube_pos[i].z));
						cube_pos[i].z += -0.425 - cube_pos[i].z;
					}
					if (cube_pos[i].z > 0.425) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.0, 0.0, 0.425 - cube_pos[i].z));
						cube_pos[i].z += 0.425 - cube_pos[i].z;
					}
					break;
				case 1:
					if (cube_pos[i].x < -0.45) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(-0.45 - cube_pos[i].x, 0.0, 0.0));
						cube_pos[i].x += -0.45 - cube_pos[i].x;
					}
					if (cube_pos[i].x > 0.45) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.45 - cube_pos[i].x, 0.0, 0.0));
						cube_pos[i].x += 0.45 - cube_pos[i].x;
					}
					if (cube_pos[i].z < -0.45) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.0, 0.0, -0.45 - cube_pos[i].z));
						cube_pos[i].z += -0.45 - cube_pos[i].z;
					}
					if (cube_pos[i].z > 0.45) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.0, 0.0, 0.45 - cube_pos[i].z));
						cube_pos[i].z += 0.45 - cube_pos[i].z;
					}
					break;
				case 2:
					if (cube_pos[i].x < -0.475) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(-0.475 - cube_pos[i].x, 0.0, 0.0));
						cube_pos[i].x += -0.475 - cube_pos[i].x;
					}
					if (cube_pos[i].x > 0.475) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.475 - cube_pos[i].x, 0.0, 0.0));
						cube_pos[i].x += 0.475 - cube_pos[i].x;
					}
					if (cube_pos[i].z < -0.475) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.0, 0.0, -0.475 - cube_pos[i].z));
						cube_pos[i].z += -0.475 - cube_pos[i].z;
					}
					if (cube_pos[i].z > 0.475) {
						T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.0, 0.0, 0.475 - cube_pos[i].z));
						cube_pos[i].z += 0.475 - cube_pos[i].z;
					}
					break;
				}
			}
	}
	else {
		for (int i = 0; i < is_ball.size(); i++) 
			T_ball[i] = glm::translate(T_ball[i], glm::vec3(0.0, -0.05, 0.0));
		for (int i = 0; i < T_cube.size(); i++)
			T_cube[i] = glm::translate(T_cube[i], glm::vec3(0.0, -0.05, 0.0));
	}
	
	glutTimerFunc(70, Timer, 1);
	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		changeOpenGL(x, y, &x_init, &y_init);
		cout << x_init << "   " << y_init << endl;
		l = true;
	}
	else l = false;

	glutPostRedisplay();
}
void Motion(int x, int y) {
	if (l) {
		changeOpenGL(x, y, &ox, &oy);

		float mx = ox - x_init;
		float mz = -oy + y_init;

		float rx = cos(glm::radians(angle)) * mx + sin(glm::radians(angle)) * mz;
		float rz = -sin(glm::radians(angle)) * mx + cos(glm::radians(angle)) * mz;

		for (int i = 0; i < T_cube.size(); i++) {
			T_cube[i] = glm::translate(T_cube[i], glm::vec3(rx, 0.0, rz));
			cube_pos[i] += glm::vec3(rx, 0.0, rz);
		}
		
		x_init = ox; y_init = oy;
	}
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