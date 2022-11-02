#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "filebuf.h"
#include "objbuf.h"
#include "stdafx.h"
using namespace std;

//opengl
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction_Right_Y(int value);
GLvoid TimerFunction(int value);

//shader
GLchar* vertexSource;
GLchar* fragmentSource;
GLuint s_program;
GLuint vertexShader;
GLuint fragmentShader;
GLuint triangleShaderProgramID;

void InitShader();
void InitBuffer();
void make_fragmentShaders();
void make_vertexShaders();

int winWidth = 800;
int winHeight = 800;
float Br = 1.0; float Bg = 1.0; float Bb = 1.0;

int polygon_mode = 1;

//vao vbo
GLuint vao[4];
GLuint vbo_position[4];
GLuint vbo_normal[4];
GLuint vbo_color[4];
//obj
objRead objSphere_red;
objRead objSphere_green;
objRead objSphere_blue;
GLuint red = objSphere_red.loadObj_normalize_center("sphere_red.obj");
GLuint green = objSphere_green.loadObj_normalize_center("sphere_green.obj");
GLuint blue = objSphere_blue.loadObj_normalize_center("sphere_blue.obj");

//키보드 관련 선언
bool depth = false; //은면
bool yRotate = false;
bool YRotate = false;
bool solid = false;

//회전 관련 선언
float Radian_Right_Y = 0.0f;

//투영변환 관련 선언
bool ortho_bool = false;
bool perspective_bool = false;

//궤도 관련 선언
const double PI = 3.1415926;
std::vector<glm::vec3> orbit;
std::vector<glm::vec3> orbit_color;
void make_orbit();

//궤도 공전 라디안, 속도
float second_orbit_radians[3];
float third_orbit_radians[3];
float second_orbit_speed[3];
float third_orbit_speed[3];

//전체 이동변환
glm::vec3 move_all = glm::vec3(0.0, 0.0, 0.0);

void main(int argc, char** argv) {
	srand(time(NULL));
	for (int i = 0; i < 3; i++) {
		second_orbit_radians[i] = rand() % 360;
		third_orbit_radians[i] = rand() % 360;
		second_orbit_speed[i] = rand() % 10+1;
		third_orbit_speed[i] = rand() % 10 + 1;
	}
	make_orbit();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, TimerFunction, 1);

	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(Br, Bg, Bb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (solid)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	if (depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	glUseProgram(s_program);

	//뷰잉 변환
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	//투영 변환
	glm::mat4 projection = glm::mat4(1.0f);
	if (ortho_bool) {//직각 투영
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	}
	if (perspective_bool) {//원근 투영
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -4.0));
	}
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	//모델링 변환
	//축 그리기
	glm::mat4 All = glm::mat4(1.0f);
	All = glm::rotate(All, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));//전체를 x축에 대해 30도 회전
	//All = glm::rotate(All, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));//전체를 y축에 대해 30도 회전
	glm::mat4 line_T1 = glm::mat4(1.0f);
	glm::mat4 line_R1 = glm::mat4(1.0f);
	line_T1 = glm::translate(line_T1, glm::vec3(move_all.x, move_all.y, move_all.z)); //전체 이동
	line_R1 = glm::rotate(line_R1,glm::radians(Radian_Right_Y), glm::vec3(0.0,1.0,0.0)); //전체 자전
	glm::mat4 line_TR = glm::mat4(1.0f); 
	line_TR = line_T1 * line_R1 * All;
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");

	//첫번째-파란 구체와 궤도
	glm::mat4 first_R1 = glm::mat4(1.0f);
	glm::mat4 first_R2 = glm::mat4(1.0f);
	first_R1 = glm::rotate(first_R1, glm::radians(45.0f), glm::vec3(0.0,0.0,1.0)); //z축 기준으로 45도 기울어진 궤도
	first_R2 = glm::rotate(first_R2, glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));//          -45도
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(line_TR));
	glBindVertexArray(vao[3]);
	glDrawArrays(GL_TRIANGLES, 0, blue);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, orbit.size());

	/*for (int i = 0; i < orbit[0].size(); i++)
		glDrawArrays(GL_LINES, i, 2);*/
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(line_TR*first_R1));
	glDrawArrays(GL_LINES, 0, orbit.size());

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(line_TR*first_R2));
	glDrawArrays(GL_LINES, 0, orbit.size());
	
	//두번째-초록구체와 궤도
	glm::mat4 second_TR_1 = glm::mat4(1.0f); //정상궤도의 통합 행렬
	glm::mat4 second_TR_2 = glm::mat4(1.0f); //45도궤도
	glm::mat4 second_TR_3 = glm::mat4(1.0f); //-45도궤도
	glm::mat4 second_T1 = glm::mat4(1.0f); //공전을 위한 이동
	glm::mat4 second_R1 = glm::mat4(1.0f); //정상궤도의 공전
	glm::mat4 second_R2 = glm::mat4(1.0f); //45도궤도
	glm::mat4 second_R3 = glm::mat4(1.0f); //-45도궤도
	glm::mat4 second_S1 = glm::mat4(1.0f); //신축
	
	second_T1 = glm::translate(second_T1, glm::vec3(0.7f,0.0f,0.0f));
	second_R1 = glm::rotate(second_R1, glm::radians(second_orbit_radians[0]), glm::vec3(0.0, 1.0, 0.0));
	second_R2 = glm::rotate(second_R2, glm::radians(second_orbit_radians[1]), glm::vec3(0.0, 1.0, 0.0));
	second_R3 = glm::rotate(second_R3, glm::radians(second_orbit_radians[2]), glm::vec3(0.0, 1.0, 0.0));
	second_S1 = glm::scale(second_S1, glm::vec3(0.5,0.5,0.5));

	second_TR_1 = line_TR * second_R1 * second_T1 * second_S1; //정상궤도

	//이전에 했던 행렬변환식을 vec3(0.0,0.0,0.0)에 적용시켜 모은다
	//나온 vec3값으로 translate시켜 해당 좌표에 obj를 이동시킨다
	//회전행렬에 의해 obj의 기울어짐을 방지할 수 있다
	glm::vec3 second_TRvec_2 = glm::vec3(0.0, 0.0, 0.0); //궤도 공전으로 후 위치 저장 벡터 선언
	glm::vec3 second_TRvec_3 = glm::vec3(0.0, 0.0, 0.0);

	second_TRvec_2 = first_R1 * second_R2 * second_T1 * glm::vec4(second_TRvec_2, 1.0);
	glm::mat4 second_T2 = glm::mat4(1.0f);
	second_T2 = glm::translate(second_T2, glm::vec3(second_TRvec_2));
	second_TR_2 = line_TR * second_T2 * second_S1; //45도궤도

	second_TRvec_3 = first_R2 * second_R3 * second_T1 * glm::vec4(second_TRvec_3, 1.0);
	glm::mat4 second_T3 = glm::mat4(1.0f);
	second_T3 = glm::translate(second_T3, glm::vec3(second_TRvec_3));
	second_TR_3 = line_TR * second_T3 * second_S1; //-45도궤도

	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(second_TR_1));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, green);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, orbit.size());

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(second_TR_2));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, green);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, orbit.size());

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(second_TR_3));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, green);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, orbit.size());
	
	//세번째-
	glm::mat4 third_TR_1 = glm::mat4(1.0f); //정상궤도
	glm::mat4 third_TR_2 = glm::mat4(1.0f); //45
	glm::mat4 third_TR_3 = glm::mat4(1.0f); //-45
	glm::mat4 third_T1 = glm::mat4(1.0f); //공전을 위한 이동행렬
	glm::mat4 third_R1 = glm::mat4(1.0f); //공전 행렬
	glm::mat4 third_R2 = glm::mat4(1.0f); 
	glm::mat4 third_R3 = glm::mat4(1.0f);
	glm::mat4 third_S1 = glm::mat4(1.0f); //신축
	third_T1 = glm::translate(third_T1, glm::vec3(0.35f, 0.0f, 0.0f));
	third_R1 = glm::rotate(third_R1, glm::radians(third_orbit_radians[0]), glm::vec3(0.0, 1.0, 0.0));
	third_R2 = glm::rotate(third_R2, glm::radians(third_orbit_radians[1]), glm::vec3(0.0, 1.0, 0.0));
	third_R3 = glm::rotate(third_R3, glm::radians(third_orbit_radians[2]), glm::vec3(0.0, 1.0, 0.0));
	third_S1 = glm::scale(third_S1, glm::vec3(0.25, 0.25, 0.25));

	third_TR_1 = line_TR * second_R1 * second_T1 * third_R1 * third_T1 * third_S1;
	third_TR_2 = line_TR * second_T2 * third_R2 * third_T1 * third_S1;
	third_TR_3 = line_TR * second_T3 * third_R3 * third_T1 * third_S1;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(third_TR_1));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, red);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(third_TR_2));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, red);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(third_TR_3));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, red);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}


GLvoid TimerFunction_Right_Y(int value) {//자전 타이머
	if (yRotate || YRotate) {
		if (yRotate)
			Radian_Right_Y += 7;
		else if (YRotate)
			Radian_Right_Y -= 7;
		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_Right_Y, 1);
	}
}
GLvoid TimerFunction(int value) { //두,세번째 행성의 공전 타이머
	for (int i = 0; i < 3; i++) {
		second_orbit_radians[i] += second_orbit_speed[i];
		third_orbit_radians[i] += third_orbit_speed[i];
	}
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}
GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case 'y':// 전체 obj y축에 대한 양 방향 회전
		if (yRotate) {
			yRotate = false;
		}
		else {
			YRotate = false;
			yRotate = true;
			glutTimerFunc(100, TimerFunction_Right_Y, 1);
		}
		break;
	case 'Y':// 전체 obj y축에 대한 음 방향 회전
		if (YRotate) {
			YRotate = false;
		}
		else {
			yRotate = false;
			YRotate = true;
			glutTimerFunc(100, TimerFunction_Right_Y, 1);
		}
		break;
	case 'h': //깊이 검사 은면제거
		if (depth)
			depth = false;
		else depth = true;
		break;
	case 'm': //솔리드, 와이어
		if (solid)
			solid = false;
		else solid = true;
		break;
	case 'p': //직교투영
		if (ortho_bool)
			ortho_bool = false;
		else
			ortho_bool = true;
		break;
	case 'P': //원근투영
		if (perspective_bool)
			perspective_bool = false;
		else
			perspective_bool = true;
		break;
	case 'w':
		move_all.y += 0.1f;
		break;
	case 'a':
		move_all.x -= 0.1f;
		break;
	case 's':
		move_all.y -= 0.1f;
		break;
	case 'd':
		move_all.x += 0.1f;
		break;
	case 'z':
		move_all.z += 0.1f;
		break;
	case 'x':
		move_all.z -= 0.1f;
		break;
	case 'q': //종료
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void make_vertexShaders() {

	vertexSource = filetobuf("vertex.glsl"); // 버텍스세이더 읽어오기
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader error\n" << errorLog << std::endl;
	}
}
void make_fragmentShaders()
{

	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader error\n" << errorLog << std::endl;
	}
}
void InitBuffer() {
	glGenVertexArrays(4, vao);
	glGenBuffers(4, vbo_position);
	glGenBuffers(4, vbo_normal);
	glGenBuffers(4, vbo_color);

	//s_program[0] 설정
	glUseProgram(s_program);
	//line
	GLint pAttribute = glGetAttribLocation(s_program, "in_Position");
	GLint cAttribute = glGetAttribLocation(s_program, "in_Color");
	GLint nAttribute = glGetAttribLocation(s_program, "in_Normal");
	
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position[0]);
	glBufferData(GL_ARRAY_BUFFER, orbit.size() * sizeof(glm::vec3), &orbit[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[0]);
	glBufferData(GL_ARRAY_BUFFER, orbit_color.size() * sizeof(glm::vec3), &orbit_color[0], GL_STATIC_DRAW);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
	
	
	//red sphere
	//vertex
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position[1]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_red.outvertex.size() * sizeof(glm::vec3), &objSphere_red.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);
	//texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[1]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_red.outuv.size() * sizeof(glm::vec3), &objSphere_red.outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[1]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_red.outnormal.size() * sizeof(glm::vec3), &objSphere_red.outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//green sphere
	//vertex
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position[2]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_green.outvertex.size() * sizeof(glm::vec3), &objSphere_green.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);
	//texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[2]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_green.outuv.size() * sizeof(glm::vec3), &objSphere_green.outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[2]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_green.outnormal.size() * sizeof(glm::vec3), &objSphere_green.outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//blue sphere
	//vertex
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position[3]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_blue.outvertex.size() * sizeof(glm::vec3), &objSphere_blue.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);
	//texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[3]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_blue.outuv.size() * sizeof(glm::vec3), &objSphere_blue.outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[3]);
	glBufferData(GL_ARRAY_BUFFER, objSphere_blue.outnormal.size() * sizeof(glm::vec3), &objSphere_blue.outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

}
void InitShader() {
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(s_program, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(triangleShaderProgramID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
	}

	glUseProgram(s_program);
}

void make_orbit() {//궤도 만들기 함수
	float R = 0.7f;
	float radian = 0.0;//원의 각도 -삼각함수-
	glm::vec3 orbit_vertex;
	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f);
	glm::mat4 angle_mat = glm::mat4(1.0f);

	while (1) {
		if (radian > 360.0)
			return;

		orbit_vertex.x = cos(radian * PI / 180) * R;
		orbit_vertex.y = 0.0f;
		orbit_vertex.z = sin(radian * PI / 180) * R;

		radian += 1.5f;
		orbit.push_back(orbit_vertex);
		orbit_color.push_back(color);
	}
}