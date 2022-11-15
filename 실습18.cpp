#include "objRead.h"
#define VAO_SIZE 4
#define VBO_SIZE 8

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
void subTimer(int value);
void subTimer2(int value);
void subTimer3(int value);
float x_init, y_init, ox, oy;
GLuint vao[VAO_SIZE], vbo[VBO_SIZE];
glm::vec3 cameraPos = glm::vec3(0.0f, 0.4f, 1.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
unsigned int projectionLocation;
unsigned int viewLocation;
objRead plane;
objRead body;
objRead arm;
int object = plane.loadObj_normalize_center("plane.obj");
int object2 = body.loadObj_normalize_center("body.obj");
int object3 = arm.loadObj_normalize_center("arm.obj");

float line[2][3] = {
	{0.0, 5.0, 0.0},
	{0.0, -5.0, 0.0},
};
float line_cal[2][3];
float plane_col[6][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.5, 0.5, 0.5},
	{0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0}
};
float crain_col[36][3] = {
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

glm::mat4 T_body = glm::mat4(1.0);
glm::mat4 T_all = glm::mat4(1.0);
glm::mat4 T_arm = glm::translate(glm::mat4(1.0), glm::vec3(0.13, body.scaleY * 1.7, 0.0));
glm::mat4 T_arm2 = glm::translate(glm::mat4(1.0), glm::vec3(-0.13, body.scaleY * 1.7, 0.0));

glm::mat4 R_middleBody = glm::mat4(1.0);
glm::mat4 R_arm = glm::mat4(1.0);
glm::mat4 R_arm2 = glm::mat4(1.0);

glm::mat4 S_body = glm::mat4(1.0);
glm::mat4 S_plane = glm::scale(glm::mat4(1.0), glm::vec3(3.0, 3.0, 3.0));

vector<pair<int, bool>> v;
glm::vec3 n;

int arm_angle = 0;
float cam_angle = glm::radians(0.1f);
float cam_directangle = glm::radians(180.0f);
float r = 1.0;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example18");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	glewInit();
	for (int i = 0; i < 8; i++) {
		v.push_back(make_pair(0, false));
	}
	InitShader();
	InitBuffer();
	//원근 투영
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -4.0));

	S_body = glm::scale(glm::mat4(1.0), glm::vec3(0.7, 0.7, 0.7));
	T_body = glm::translate(glm::mat4(1.0), glm::vec3(0.0, body.scaleY, 0.0));
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
	glGenVertexArrays(VAO_SIZE, vao);
	glGenBuffers(VBO_SIZE, vbo);

	//plane
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, plane.outvertex.size() * sizeof(glm::vec3), &plane.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_col), plane_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//body
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, body.outvertex.size() * sizeof(glm::vec3), &body.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crain_col), crain_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//arm
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, arm.outvertex.size() * sizeof(glm::vec3), &arm.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crain_col), crain_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_cal), line_cal, GL_STATIC_DRAW);
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

	glUseProgram(s_program);
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	
	viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));
	
	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(S_plane));
	glBindVertexArray(vao[3]);
	glDrawArrays(GL_LINES, 0, 2);
	//plane
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, object);

	//body
	glBindVertexArray(vao[1]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_all));
	glDrawArrays(GL_TRIANGLES, 0, object2);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_all * T_body * R_middleBody * S_body));
	glDrawArrays(GL_TRIANGLES, 0, object2);

	//arm
	glBindVertexArray(vao[2]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_all * R_middleBody * T_arm * R_arm));
	glDrawArrays(GL_TRIANGLES, 0, object3);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_all * R_middleBody * T_arm2 * R_arm2));
	glDrawArrays(GL_TRIANGLES, 0, object3);

	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		// crain move x
	case 'b': 
		if (v[0].second) {
			v[0].first = 0;
			v[0].second = false;
			break;
		}
		v[0].first = 1;
		v[1].first = 0;
		v[0].second = true;
		v[1].second = false;
		break;
	case 'B': 
		if (v[1].second) {
			v[1].first = 0;
			v[1].second = false;
			break;
		}
		v[0].first = 0;
		v[1].first = 1;
		v[1].second = true;
		v[0].second = false;
		break;

		//middle crain rotate y
	case 'm': 
		if (v[2].second || arm_angle == 18) {
			v[2].first = 0;
			v[2].second = false;
			break;
		}
		v[3].first = 0;
		v[2].first = 1;
		v[2].second = true;
		v[3].second = false;
		break;
	case 'M':
		if (v[3].second || arm_angle == -18) {
			v[3].first = 0;
			v[3].second = false;
			break;
		}
		v[2].first = 0;
		v[3].first = 1;
		v[3].second = true;
		v[2].second = false;
		break;

		//arm rotate
	case 't': 
		if (v[4].second) {
			v[4].first = 0;
			v[4].second = false;
			break;
		}
		v[5].first = 0;
		v[4].first = 1;
		v[4].second = true;
		v[5].second = false;
		break;
	case 'T':
		if (v[5].second) {
			v[5].first = 0;
			v[5].second = false;
			break;
		}
		v[4].first = 0;
		v[5].first = 1;
		v[5].second = true;
		v[4].second = false;
		break;

		//camera x, z move
	case 'z': 
		n = glm::normalize(cameraDirection - cameraPos);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos += n;
		cameraDirection += n;
		break;
	case 'Z': 
		n = glm::normalize(cameraDirection - cameraPos);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos -= n;
		cameraDirection -= n;
		break;
	case 'x': 
		n = glm::normalize(glm::cross(cameraUp, cameraDirection - cameraPos));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos += n;
		cameraDirection += n;
		break;
	case 'X': 
		n = glm::normalize(glm::cross(cameraUp, cameraDirection - cameraPos));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos -= n;
		cameraDirection -= n;
		break;

		//camera y rotate
	case 'y':  
		if(cam_directangle>3.1f)
		cam_directangle = glm::radians(360.0f) -
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));
		if(cam_directangle>6.14f)
			cam_directangle = 
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));
		cam_directangle += 0.1;
		cameraDirection.x = sin(cam_directangle) + cameraPos.x;
		cameraDirection.z = cos(cam_directangle) + cameraPos.z;
		break;
	case 'Y': 
		if (cam_directangle > 3.1f)
			cam_directangle = glm::radians(360.0f) -
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));
		if (cam_directangle > 6.14f)
			cam_directangle =
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));
		cam_directangle -= 0.1;
		cameraDirection.x = sin(cam_directangle) + cameraPos.x;
		cameraDirection.z = cos(cam_directangle) + cameraPos.z;
		break;

		//camera revolution y
	case 'r': 
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1)
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
		if(cameraPos.x<0)
				cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);

		cam_angle += 0.1; 
		cameraPos.x = sin(cam_angle)*r;
		cameraPos.z = cos(cam_angle)*r;
		
		break;
	case 'R': 
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1)
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);
		cam_angle -= 0.1;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;
		break;


	case 'a':
		if (v[6].second) {
			v[6].first = 0;
			v[6].second = false;
			break;
		}
		v[6].first = 1;
		v[7].first = 0;
		v[6].second = true;
		v[7].second = false;
		break;
	case 'A':
		if (v[7].second) {
			v[7].first = 0;
			v[7].second = false;
			break;
		}
		v[6].first = 0;
		v[7].first = 1;
		v[7].second = true;
		v[6].second = false;
		break;

		// all stop
	case 's':
		v[0].second = false; v[1].second = false;
		v[2].second = false; v[3].second = false;
		v[4].second = false; v[5].second = false;
		v[6].second = false; v[7].second = false;
		break;
	case 'S':
		for (int i = 0; i < v.size(); i++) {
			if (v[i].first == 1)
				v[i].second = true;
		}
		break;

		// all init
	case 'c':
		T_all = glm::mat4(1.0);
		T_arm = glm::translate(glm::mat4(1.0), glm::vec3(0.13, body.scaleY * 1.7, 0.0));
		T_arm2 = glm::translate(glm::mat4(1.0), glm::vec3(-0.13, body.scaleY * 1.7, 0.0));
		S_body = glm::scale(glm::mat4(1.0), glm::vec3(0.7, 0.7, 0.7));
		T_body = glm::translate(glm::mat4(1.0), glm::vec3(0.0, body.scaleY, 0.0));
		R_middleBody = glm::mat4(1.0);
		R_arm = glm::mat4(1.0);
		R_arm2 = glm::mat4(1.0);
		v[0].second = false;  v[1].second = false;
		v[2].second = false; v[3].second = false;
		v[4].second = false; v[5].second = false;
		v[6].second = false; v[7].second = false;
		arm_angle = 0; cam_angle = glm::radians(2.0f);
		cameraPos = glm::vec3(0.0f, 0.4f, 1.0f);
		cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		arm_angle = 0;
		cam_angle = glm::radians(0.1f);
		cam_directangle = glm::radians(180.0f);
		r = 1.0;

		break;

	case 'Q': glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {
	//translate all
	if (v[0].second) 
		T_all = glm::translate(T_all, glm::vec3(0.1, 0.0, 0.0));
	if (v[1].second)
		T_all = glm::translate(T_all, glm::vec3(-0.1, 0.0, 0.0));

	//rotate mid body
	if (v[2].second)
		R_middleBody = glm::rotate(R_middleBody, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	if (v[3].second)
		R_middleBody = glm::rotate(R_middleBody, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));

	//rotate arm
	if (v[4].second) {
		R_arm = glm::rotate(R_arm, glm::radians(5.0f), glm::vec3(0.0, 0.0, 1.0));
		R_arm2 = glm::rotate(R_arm2, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
		arm_angle++;
		if (arm_angle >= 19) {
			v[4].second = false; v[4].first = 0;
			v[5].second = true; v[5].first = 1;
		}
	}
	if (v[5].second) {
		R_arm = glm::rotate(R_arm, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
		R_arm2 = glm::rotate(R_arm2, glm::radians(5.0f), glm::vec3(0.0, 0.0, 1.0));
		arm_angle--;
		if (arm_angle <= -18) {
			v[5].second = false; v[5].first = 0;
			v[4].second = true; v[4].first = 1;
		}
	}

	//rotate cam
	if (v[6].second) {
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1)
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);
		cam_angle += 0.1;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;
	}
	if (v[7].second) {
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1)
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);
		cam_angle -= 0.1;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;
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