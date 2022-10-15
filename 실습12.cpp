#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objRead.h"

using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* vertexsource, * fragmentsource; //--- �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; //--- ���̴� ��ü
GLuint s_program;
void InitBuffer();
void InitShader();
char* filetobuf(const char* file);
void changeOpenGL(int x, int y, float* ox, float* oy);
void Keyboard(unsigned char key, int x, int y);
float x_init, y_init, ox, oy;
GLuint vao[3], vbo[3];
objRead objReader;
objRead objReader2;
GLint Object = objReader.loadObj_normalize_center("cube.obj");
GLint Object2 = objReader2.loadObj_normalize_center("tet.obj");
float line[4][3] = {
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
};
float line_col[4][3] = {
	{0.8, 0.0, 0.0},
	{0.8, 0.0, 0.0},
	{0.8, 0.0, 0.0},
	{0.8, 0.0, 0.0}
};
int each_cube[6] = {-1, -1, -1, -1, -1, -1 };
int each_tet[4] = { -1, -1, -1, -1};
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example12");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();
	InitShader();
	InitBuffer();
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}
void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return;
	}
}
void InitBuffer()
{
	glGenVertexArrays(3, vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(3, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	
	// x, y ��
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// ����ü 
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, objReader.outvertex.size() * sizeof(glm::vec3), &objReader.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// ���ü
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, objReader2.outvertex.size() * sizeof(glm::vec3), &objReader2.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	
}
void InitShader()
{
	make_vertexShader(); //--- ���ؽ� ���̴� �����
	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}
GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(s_program);
	//--- ����� VAO �ҷ�����

	//x, y�� �׸���
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 4);

	// ����ü �׸���
	glBindVertexArray(vao[1]);
	for (int i = 0; i < 6; i++)
		if(each_cube[i] != -1)
			glDrawArrays(GL_TRIANGLES, each_cube[i], 6);

	// ���ü �׸���
	glBindVertexArray(vao[2]);
	for (int i = 0; i < 4; i++) 
		if (each_tet[i] != -1) 
			glDrawArrays(GL_TRIANGLES, each_tet[i], 3);
	
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
//opengl ��ǥ�� ��ȯ
void Keyboard(unsigned char key, int x, int y)
{
	for (int i = 0; i < 6; i++) {
		each_cube[i] = -1;
		if (i > 3)
			continue;
		else each_tet[i] = -1;
	}
	switch (key) {
		// ����ü �� ���� �׸���
	case'1': each_cube[0] = 0; //��
		break;
	case '2': each_cube[1] = 6; //��
		break;
	case'3': each_cube[2] = 12; //�Ʒ�
		break;
	case '4': each_cube[3] = 18; //��
		break;
	case'5': each_cube[4] = 24; //��
		break;
	case '6': each_cube[5] = 30; //��
		break;
		// ���ü �� ���� �׸���
	case'7': each_tet[0] = 0; //��
		break;
	case '8': each_tet[1] = 3; //��
		break;
	case'9': each_tet[2] = 6; //��
		break;
	case '0': each_tet[3] = 9; //��
		break;
		// ����ü ���ֺ��� ���� �׸���
	case'a': each_cube[0] = 0; each_cube[1] = 6;
		break;
	case'b': each_cube[2] = 12; each_cube[4] = 24;
		break;
	case'c': each_cube[3] = 18; each_cube[5] = 30;
		break;
		// ���ü �Ʒ���� �� ���� ������ �׸���
	case'e': each_tet[0] = 0; each_tet[1] = 3;
		break;
	case'f': each_tet[0] = 0; each_tet[2] = 6;
		break;
	case'g': each_tet[0] = 0; each_tet[3] = 9;
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