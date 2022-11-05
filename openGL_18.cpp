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
float Br = 0.0; float Bg = 0.0; float Bb = 0.0;

int polygon_mode = 1;

//vao vbo
GLuint vao[3];
GLuint vbo_position[3];
GLuint vbo_normal[3];
GLuint vbo_color[3];
//obj
objRead objLine;
objRead objCube;
objRead objCylinder;
GLuint line = objLine.loadObj_normalize_center("line.obj");
GLuint cube = objCube.loadObj_normalize_center("cube.obj");
GLuint cylinder = objCylinder.loadObj_normalize_center("cylinder.obj");

//키보드 관련 선언
bool depth = true; //은면
bool yRotate = false;
bool YRotate = false;
bool solid = true;

//투영변환 관련 선언
bool ortho_bool = false;
bool perspective_bool = true;

//카메라(뷰잉) 관련 선언
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 10.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0f, 0.0f);
//라디안 계산을 마친 카메라 위치
glm::vec3 cameraPos_now = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 cameraDirection_now = glm::vec3(0.0, 0.0, 0.0);
void convert_camera();
//크레인 이동 위치
glm::vec3 crainPos = glm::vec3(0.0f, 0.0f, 0.0f);

//중단 큐브 회전 라디안
float mid_cube_radian = 0.0f;
//상단 실린더 회전 라디안
float top_cylinder_radian = 0.0f;

//크레인 애니메이션 관련 선언
bool crainpos_xmove_p = false;
bool crainpos_xmove_m = false;
bool mid_rotate_p = false;
bool mid_rotate_m = false;
bool top_rotate_p = false;
bool top_rotate_m = false;
GLvoid TimerFunction_crainpos_x_p(int value);
GLvoid TimerFunction_crainpos_x_m(int value);
GLvoid TimerFunction_midrotate_p(int value);
GLvoid TimerFunction_midrotate_m(int value);
GLvoid TimerFunction_toprotate_p(int value);
GLvoid TimerFunction_toprotate_m(int value);

//카메라 애니메이션 관련 선언
bool cameraView_rotate_p = false;
bool cameraView_rotate_m = false;
bool cameraPos_rotate_p = false;
bool cameraPos_rotate_m = false;
float cameraView_rotate_radian = 0.0f;
float cameraPos_rotate_radian = 0.0f;
GLvoid TimerFunction_camerapos_rotate_p(int value);
GLvoid TimerFunction_camerapos_rotate_m(int value);
GLvoid TimerFunction_cameraview_rotate_p(int value);
GLvoid TimerFunction_cameraview_rotate_m(int value);

void DrawScene();

void main(int argc, char** argv) {
	srand(time(NULL));
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

	glViewport(0, 0, 400, 400);
	//glPushMatrix();
	//뷰잉 변환
	glm::mat4 lookat = glm::mat4(1.0f);
	convert_camera();
	lookat = glm::lookAt(cameraPos_now,cameraDirection_now,cameraUp);
	unsigned int viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &lookat[0][0]);

	//투영 변환
	glm::mat4 projection = glm::mat4(1.0f);
	if (ortho_bool) {//직각 투영
		projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
	}
	if (perspective_bool) {//원근 투영
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -4.0));
	}
	unsigned int projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	DrawScene();
	//glPopMatrix();


	glViewport(450, 400, 300, 300);
	//glPushMatrix();
	glm::mat4 lookat_1 = glm::mat4(1.0f);
	convert_camera();
	lookat_1 = glm::lookAt(glm::vec3(0.0f,10.0f,0.01f), glm::vec3(0.0f,0.0f,0.0f), cameraUp);
	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &lookat_1[0][0]);

	glm::mat4 projection_1 = glm::mat4(1.0f);
	//직각 투영
	projection_1 = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -4.0f, 50.0f);
	
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection_1[0][0]);
	DrawScene();
	//glPopMatrix();

	glViewport(450, 50, 300, 300);
	//glPushMatrix();
	glm::mat4 lookat_2 = glm::mat4(1.0f);
	convert_camera();
	lookat_2 = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), cameraUp);
	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &lookat_2[0][0]);

	glm::mat4 projection_2 = glm::mat4(1.0f);
	//직각 투영
	projection_2 = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -4.0f, 50.0f);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection_2[0][0]);
	DrawScene();
	//glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	
}

void DrawScene() {
	//모델링 변환
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	//바닥 그리기
	glm::mat4 floor_TR = glm::mat4(1.0f);
	glm::mat4 floor_S1 = glm::mat4(1.0f);
	glm::mat4 floor_T1 = glm::mat4(1.0f);

	floor_S1 = glm::scale(floor_S1, glm::vec3(100.0, 1.0, 100.0));
	floor_T1 = glm::translate(floor_T1, glm::vec3(0.0, -0.5, 0.0));
	floor_TR = floor_T1 * floor_S1 * floor_T1;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(floor_TR));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, cube);

	//하단 중단 상단
	glm::mat4 all_T1 = glm::mat4(1.0f);
	all_T1 = glm::translate(all_T1, crainPos);
	glm::mat4 S_T1 = glm::mat4(1.0f);//신축을 위한 좌표이동
	glm::mat4 S_T2 = glm::mat4(1.0f);
	S_T1 = glm::translate(S_T1, glm::vec3(0.0, -0.5, 0.0));
	S_T2 = glm::translate(S_T2, glm::vec3(0.0, 0.5, 0.0));

	//하단 큐브 그리기
	glm::mat4 bottom_TR = glm::mat4(1.0f);
	glm::mat4 bottom_S1 = glm::mat4(1.0f);
	bottom_S1 = glm::scale(bottom_S1, glm::vec3(7.0, 4.0, 7.0));
	bottom_TR = all_T1 * bottom_S1;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(bottom_TR));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, cube);

	//중단 큐브 그리기
	glm::mat4 mid_TR = glm::mat4(1.0f);
	glm::mat4 mid_S1 = glm::mat4(1.0f);
	glm::mat4 mid_R1 = glm::mat4(1.0f);
	glm::mat4 mid_T1 = glm::mat4(1.0f);
	mid_T1 = glm::translate(mid_T1, glm::vec3(0.0, 4.0, 0.0));
	mid_R1 = glm::rotate(mid_R1, glm::radians(mid_cube_radian), glm::vec3(0.0, 1.0, 0.0));
	mid_S1 = glm::scale(mid_S1, glm::vec3(4.0, 2.0, 4.0));
	mid_TR = all_T1 * mid_T1 * mid_R1 * mid_S1;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mid_TR));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, cube);
	//상단 실린더 그리기
	glm::mat4 top_TR_1 = glm::mat4(1.0f);
	glm::mat4 top_TR_2 = glm::mat4(1.0f);
	glm::mat4 top_S1 = glm::mat4(1.0f);
	glm::mat4 top_T1 = glm::mat4(1.0f);//첫번째 실린더
	glm::mat4 top_R1 = glm::mat4(1.0f);
	glm::mat4 top_T2 = glm::mat4(1.0f);//두번째 실린더
	glm::mat4 top_R2 = glm::mat4(1.0f);
	glm::mat4 top_T3 = glm::mat4(1.0f);
	top_S1 = glm::scale(top_S1, glm::vec3(1.0, 6.0, 1.0));
	top_T1 = glm::translate(top_T1, glm::vec3(0.0, 0.0, 1.0));
	top_R1 = glm::rotate(top_R1, glm::radians(top_cylinder_radian), glm::vec3(0.0, 0.0, 1.0));
	top_T2 = glm::translate(top_T2, glm::vec3(0.0, 0.0, -1.0));
	top_R2 = glm::rotate(top_R2, glm::radians(-top_cylinder_radian), glm::vec3(0.0, 0.0, 1.0));
	top_T3 = glm::translate(top_T3, glm::vec3(0.0, 6.0, 0.0));
	top_TR_1 = all_T1 * top_T3 * mid_R1 * top_T1 * top_R1 * top_S1;
	top_TR_2 = all_T1 * top_T3 * mid_R1 * top_T2 * top_R2 * top_S1;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(top_TR_1));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, cylinder);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(top_TR_2));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, cylinder);

	//축 그리기
	glm::mat4 bassline = glm::mat4(1.0f);
	glm::mat4 bassline_S1 = glm::mat4(1.0f);
	bassline_S1 = glm::scale(bassline_S1, glm::vec3(10.0, 10.0, 10.0));
	bassline = bassline_S1;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(bassline));
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 3, 2);
	glDrawArrays(GL_LINES, 6, 2);
}

GLvoid TimerFunction(int value) { //두,세번째 행성의 공전 타이머

	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}
GLvoid TimerFunction_crainpos_x_p(int value) { //두,세번째 행성의 공전 타이머
	if (crainpos_xmove_p) {
		crainPos.x += 0.5f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_crainpos_x_p, 1);
	}
}
GLvoid TimerFunction_crainpos_x_m(int value) { //두,세번째 행성의 공전 타이머
	if (crainpos_xmove_m) {
		crainPos.x -= 0.5f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_crainpos_x_m, 1);
	}
}
GLvoid TimerFunction_midrotate_p(int value) {
	if (mid_rotate_p) {
		mid_cube_radian += 11.0f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_midrotate_p, 1);
	}

}
GLvoid TimerFunction_midrotate_m(int value) {
	if (mid_rotate_m) {
		mid_cube_radian -= 11.0f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_midrotate_m, 1);
	}
}
GLvoid TimerFunction_toprotate_p(int value) {
	if (top_rotate_p) {
		if (top_cylinder_radian >= 90) {
			top_rotate_m = true;
			top_rotate_p = false;
			glutTimerFunc(100, TimerFunction_toprotate_m, 1);
		}

		top_cylinder_radian += 9.0f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_toprotate_p, 1);
	}
}
GLvoid TimerFunction_toprotate_m(int value) {
	if (top_rotate_m) {
		if (top_cylinder_radian <= -90) {
			top_rotate_p = true;
			top_rotate_m = false;
			glutTimerFunc(100, TimerFunction_toprotate_p, 1);
		}

		top_cylinder_radian -= 9.0f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_toprotate_m, 1);
	}
}
GLvoid TimerFunction_camerapos_rotate_p(int value) {
	if (cameraPos_rotate_p) {
		cameraPos_rotate_radian += 7.0f;
		
		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_camerapos_rotate_p, 1);
	}
}
GLvoid TimerFunction_camerapos_rotate_m(int value) {
	if (cameraPos_rotate_m) {
		cameraPos_rotate_radian -= 7.0f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_camerapos_rotate_m, 1);
	}
}
GLvoid TimerFunction_cameraview_rotate_p(int value) {
	if (cameraView_rotate_p) {
		cameraView_rotate_radian += 7.0f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_cameraview_rotate_p, 1);
	}
}
GLvoid TimerFunction_cameraview_rotate_m(int value) {
	if (cameraView_rotate_m) {
		cameraView_rotate_radian -= 7.0f;

		glutPostRedisplay();
		glutTimerFunc(100, TimerFunction_cameraview_rotate_m, 1);
	}
}
GLvoid Keyboard(unsigned char key, int x, int y) {
	convert_camera();
	glm::vec3 front_vector = glm::normalize(cameraDirection_now - cameraPos_now);

	glm::mat4 right_rotate = glm::mat4(1.0f);
	right_rotate = glm::rotate(right_rotate, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 l_T1 = glm::mat4(1.0f);
	glm::mat4 l_T2 = glm::mat4(1.0f);
	l_T1 = glm::translate(l_T1, glm::vec3(-cameraPos_now));
	l_T2 = glm::translate(l_T2, glm::vec3(cameraPos_now));

	glm::vec3 rotate_Direction = l_T2 * right_rotate * l_T1 * glm::vec4(cameraDirection_now, 1.0f);
	glm::vec3 right_vector = glm::normalize(rotate_Direction - cameraPos_now);


	switch (key) {
	case 'h': //깊이 검사 은면제거
		if (depth)
			depth = false;
		else depth = true;
		break;
	case 'w': //솔리드, 와이어
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
	case 'b'://크레인 x방향 움직이기
		if (crainpos_xmove_p) {
			crainpos_xmove_p = false;
		}
		else {
			crainpos_xmove_p = true;
			crainpos_xmove_m = false;
			glutTimerFunc(100, TimerFunction_crainpos_x_p, 1);
		}
		break;
	case 'B':
		if (crainpos_xmove_m) {
			crainpos_xmove_m = false;
		}
		else {
			crainpos_xmove_m = true;
			crainpos_xmove_p = false;
			glutTimerFunc(100, TimerFunction_crainpos_x_m, 1);
		}
		break;
	case 'm'://크레인 중앙 몸체 회전
		if (mid_rotate_p) {
			mid_rotate_p = false;
		}
		else {
			mid_rotate_p = true;
			mid_rotate_m = false;
			glutTimerFunc(100, TimerFunction_midrotate_p, 1);
		}
		break;
	case 'M':
		if (mid_rotate_m) {
			mid_rotate_m = false;
		}
		else {
			mid_rotate_m = true;
			mid_rotate_p = false;
			glutTimerFunc(100, TimerFunction_midrotate_m, 1);
		}
		break;
	case 't'://크레인 상단 몸체 휘적휘적
		if (top_rotate_p) {
			top_rotate_p = false;
		}
		else {
			top_rotate_p = true;
			top_rotate_m = false;
			glutTimerFunc(100, TimerFunction_toprotate_p, 1);
		}
		break;
	case 'T':
		if (top_rotate_m) {
			top_rotate_m = false;
		}
		else {
			top_rotate_m = true;
			top_rotate_p = false;
			glutTimerFunc(100, TimerFunction_toprotate_m, 1);
		}
		break;
	case 'y':
		if (cameraView_rotate_p) {
			cameraView_rotate_p = false;
		}
		else {
			cameraView_rotate_p = true;
			cameraView_rotate_m = false;
			glutTimerFunc(100, TimerFunction_cameraview_rotate_p, 1);
		}
		break;
	case 'Y':
		if (cameraView_rotate_m) {
			cameraView_rotate_m = false;
		}
		else {
			cameraView_rotate_m = true;
			cameraView_rotate_p = false;
			glutTimerFunc(100, TimerFunction_cameraview_rotate_m, 1);
		}
		break;
	case 'r':
		cameraPos_rotate_radian += 9.0f;
		break;
	case 'R':
		cameraPos_rotate_radian -= 9.0f;
		break;
	case 'a':
		if (cameraPos_rotate_p) {
			cameraPos_rotate_p = false;
		}
		else {
			cameraPos_rotate_p = true;
			cameraPos_rotate_m = false;
			glutTimerFunc(100, TimerFunction_camerapos_rotate_p, 1);
		}
		break;
	case 'A':
		if (cameraPos_rotate_m) {
			cameraPos_rotate_m = false;
		}
		else {
			cameraPos_rotate_m = true;
			cameraPos_rotate_p = false;
			glutTimerFunc(100, TimerFunction_camerapos_rotate_m, 1);
		}
		break;
	case 'z':
		cameraPos.z += 0.5f;
		cameraDirection.z += 0.5f;
		break;
	case 'Z':
		cameraPos.z -= 0.5f;
		cameraDirection.z -= 0.5f;
		break;
	case 'x':
		cameraPos.x += 0.5f;
		cameraDirection.x += 0.5f;
		break;
	case 'X':
		cameraPos.x -= 0.5f;
		cameraDirection.x -= 0.5f;
		break;
	case 'v':
		cameraPos.y += 0.5f;
		cameraDirection.y += 0.5f;
		break;
	case 'V':
		cameraPos.y -= 0.5f;
		cameraDirection.y -= 0.5f;
		break;
		//방향벡터 곱하기 속도
		//방향벡터
	case 'j'://fps시점 왼쪽 움직임
		cameraPos += glm::vec3(right_vector.x, 0.0, right_vector.z) * 0.5f;
		cameraDirection += glm::vec3(right_vector.x, 0.0, right_vector.z) * 0.5f;
		break;
	case 'l'://fps시점 오른쪽 움직임
		cameraPos -= glm::vec3(right_vector.x, 0.0, right_vector.z) * 0.5f;
		cameraDirection -= glm::vec3(right_vector.x, 0.0, right_vector.z) * 0.5f;
		break;
	case 'i'://fps시점 앞쪽 움직임
		cameraPos += glm::vec3(front_vector.x, 0.0, front_vector.z) * 0.5f;
		cameraDirection += glm::vec3(front_vector.x, 0.0, front_vector.z) * 0.5f;
		break;
	case 'k'://fps시점 뒷쪽 움직임
		cameraPos -= glm::vec3(front_vector.x, 0.0, front_vector.z) * 0.5f;
		cameraDirection -= glm::vec3(front_vector.x, 0.0, front_vector.z) * 0.5f;
		break;
	case 's'://stop
		crainpos_xmove_p = false;
		crainpos_xmove_m = false;
		mid_rotate_p = false;
		mid_rotate_m = false;
		top_rotate_p = false;
		top_rotate_m = false;
		cameraView_rotate_p = false;
		cameraView_rotate_m = false;
		cameraPos_rotate_p = false;
		cameraPos_rotate_m = false;
		break;
	case 'c'://clear
		cameraPos = glm::vec3(0.0f, 0.5f, 10.0f);
		cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraUp = glm::vec3(0.0, 1.0f, 0.0f);
		crainPos = glm::vec3(0.0f, 0.0f, 0.0f);
		mid_cube_radian = 0.0f;
		top_cylinder_radian = 0.0f;
		crainpos_xmove_p = false;
		crainpos_xmove_m = false;
		mid_rotate_p = false;
		mid_rotate_m = false;
		top_rotate_p = false;
		top_rotate_m = false;
		cameraView_rotate_p = false;
		cameraView_rotate_m = false;
		cameraPos_rotate_p = false;
		cameraPos_rotate_m = false;
		cameraView_rotate_radian = 0.0f;
		cameraPos_rotate_radian = 0.0f;
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
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo_position);
	glGenBuffers(3, vbo_normal);
	glGenBuffers(3, vbo_color);

	//s_program[0] 설정
	glUseProgram(s_program);
	//line
	GLint pAttribute = glGetAttribLocation(s_program, "in_Position");
	GLint cAttribute = glGetAttribLocation(s_program, "in_Color");
	GLint nAttribute = glGetAttribLocation(s_program, "in_Normal");

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position[0]);
	glBufferData(GL_ARRAY_BUFFER, objLine.outvertex.size() * sizeof(glm::vec3), &objLine.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[0]);
	glBufferData(GL_ARRAY_BUFFER, objLine.outuv.size() * sizeof(glm::vec3), &objLine.outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[0]);
	glBufferData(GL_ARRAY_BUFFER, objLine.outnormal.size() * sizeof(glm::vec3), &objLine.outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//cube
	//vertex
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position[1]);
	glBufferData(GL_ARRAY_BUFFER, objCube.outvertex.size() * sizeof(glm::vec3), &objCube.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);
	//texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[1]);
	glBufferData(GL_ARRAY_BUFFER, objCube.outuv.size() * sizeof(glm::vec3), &objCube.outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[1]);
	glBufferData(GL_ARRAY_BUFFER, objCube.outnormal.size() * sizeof(glm::vec3), &objCube.outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	//cylinder
	//vertex
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position[2]);
	glBufferData(GL_ARRAY_BUFFER, objCylinder.outvertex.size() * sizeof(glm::vec3), &objCylinder.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);
	//texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color[2]);
	glBufferData(GL_ARRAY_BUFFER, objCylinder.outuv.size() * sizeof(glm::vec3), &objCylinder.outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal[2]);
	glBufferData(GL_ARRAY_BUFFER, objCylinder.outnormal.size() * sizeof(glm::vec3), &objCylinder.outnormal[0], GL_STATIC_DRAW);
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

void convert_camera() {//회전 이동 변환
	glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 view = glm::vec3(0.0, 0.0, 0.0);
	glm::mat4 pos_R1 = glm::mat4(1.0f);
	glm::mat4 pos_T1 = glm::mat4(1.0f);
	glm::mat4 pos_T2 = glm::mat4(1.0f);
	pos_R1 = glm::rotate(pos_R1, glm::radians(cameraPos_rotate_radian), glm::vec3(0.0, 1.0, 0.0));
	pos_T1 = glm::translate(pos_T1, glm::vec3(-cameraDirection.x, -cameraDirection.y, -cameraDirection.z));
	pos_T2 = glm::translate(pos_T2, glm::vec3(cameraDirection.x, cameraDirection.y, cameraDirection.z));
	pos = pos_T2 * pos_R1 * pos_T1 * glm::vec4(cameraPos, 1.0f);

	glm::mat4 view_R1 = glm::mat4(1.0f);
	glm::mat4 view_T1 = glm::mat4(1.0f);
	glm::mat4 view_T2 = glm::mat4(1.0f);
	view_R1 = glm::rotate(view_R1, glm::radians(cameraView_rotate_radian), glm::vec3(0.0, 1.0, 0.0));
	view_T1 = glm::translate(view_T1, glm::vec3(-pos.x, -pos.y, -pos.z));
	view_T2 = glm::translate(view_T2, glm::vec3(pos.x, pos.y, pos.z));
	view = view_T2 * view_R1 * view_T1 * glm::vec4(cameraDirection, 1.0f);

	cameraPos_now = pos;
	cameraDirection_now = view;
}