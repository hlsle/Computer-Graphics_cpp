#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
using namespace std;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* vertexsource, * fragmentsource; //--- �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; //--- ���̴� ��ü
GLuint s_program;
void InitBuffer();
void InitShader();
char* filetobuf(const char* file);
void Keyboard(unsigned char key, int x, int y);
void changeOpenGL(int x, int y, float* ox, float* oy);
void timer(int value);
GLfloat triShape[16][3] = { //--- �ﰢ�� ��ġ ��
	{ 0.6, 0.5, 0.0 }, { 0.8, 0.5, 0.0 }, { 0.7, 0.8, 0.0},
	{ -0.5, -0.8, 0.0 }, { -0.7, -0.8, 0.0 }, { -0.6, -0.5, 0.0},
	{ 0.5, 0.4, 0.0 }, { 0.3, 0.4, 0.0 }, { 0.4, 0.1, 0.0},
	{ -0.5, -0.4, 0.0 }, { -0.3, -0.4, 0.0 }, { -0.4, -0.1, 0.0},
	{ 0.5, 0.4, 0.0 }, { 0.5, -0.4, 0.0 }, { -0.5, -0.4, 0.0}, { -0.5, 0.4, 0.0}
};
GLfloat colors[12][3] = { //--- �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } ,
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }
};
GLuint vao, vbo[2];
int dir[4][2] = {
	{1, 1},
	{-1, 1},
	{-1, -1},
	{1, -1}
};
int wh[6] = { -1, 0, 0, -1, 0, 0};
int n = 0, d = -1, stop = 0;
bool col = false;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Example8");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();
	InitShader();
	InitBuffer();
	glutTimerFunc(100, timer, 1);
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
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, sizeof(triShape), triShape, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
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
	//--- ����� ���� ����
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	InitShader();
	InitBuffer();
	
	glUseProgram(s_program);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	//--- �ﰢ�� �׸���
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glDrawArrays(GL_LINE_LOOP, 12, 4);
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'o':
		if (wh[0] != -2) {
			wh[0] = -2; wh[3] = -2;
		}
		else if(wh[0] == -2) {
			wh[0] = -1; wh[3] = -1;
		}
		break;
	case 'i':
		if (stop == 0) {
			stop = d;
			d = 0;
		}
		else {
			d = stop;
			stop = 0;
		}
		break;
	}
	glutPostRedisplay();
}
void timer(int value) {
	if(wh[0]!=-2)
		for (int i = 0; i < 5; i += 3) {
		if (wh[i] == -1) {
			n = rand() % 4;
			wh[i] = n;
		}
		switch (i) {
		case 0:
			triShape[i][0] += dir[wh[i]][0] * 0.05f; triShape[i][1] += dir[wh[i]][1] * 0.05f;
			triShape[i + 1][0] += dir[wh[i]][0] * 0.05f; triShape[i + 1][1] += dir[wh[i]][1] * 0.05f;
			triShape[i + 2][0] += dir[wh[i]][0] * 0.05f; triShape[i + 2][1] += dir[wh[i]][1] * 0.05f;
			break;
		case 3:
			triShape[i][0] += dir[wh[i]][0] * 0.05f; triShape[i][1] += dir[wh[i]][1] * 0.05f;
			triShape[i + 1][0] += dir[wh[i]][0] * 0.05f; triShape[i + 1][1] += dir[wh[i]][1] * 0.05f;
			triShape[i + 2][0] += dir[wh[i]][0] * 0.05f; triShape[i + 2][1] += dir[wh[i]][1] * 0.05f;
			break;
		}

		//����
		if (triShape[i][0] <= -1.0f) { // �ٱ� �浹
			triShape[i][0] = -1.0f; triShape[i][1] += 0.1f;
			triShape[i + 1][0] = -1.0f; triShape[i + 1][1] = triShape[i][1] - 0.2f;
			triShape[i + 2][0] = triShape[i][0] + 0.3f; triShape[i + 2][1] = triShape[i][1] - 0.1f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i][0] >= 1.0f) {// �ٱ� �浹
			triShape[i][0] = 1.0f; triShape[i][1] = triShape[i][1] - 0.1f;
			triShape[i + 1][0] = triShape[i][0]; triShape[i + 1][1] = triShape[i][1] + 0.2f;
			triShape[i + 2][0] = triShape[i][0] + 0.3f; triShape[i + 2][1] = triShape[i][1] + 0.1f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i][1] >= 1.0f) {// �ٱ� �浹
			triShape[i][0] += 0.1f; triShape[i][1] = 1.0f;
			triShape[i + 1][0] = triShape[i][0] - 0.2f; triShape[i + 1][1] = 1.0f;
			triShape[i + 2][0] = triShape[i][0] - 0.1f; triShape[i + 2][1] = 0.7f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i][1] <= -1.0f) {// �ٱ� �浹
			triShape[i][0] -= 0.1f; triShape[i][1] = -1.0f;
			triShape[i + 1][0] = triShape[i][0] + 0.2f; triShape[i + 1][1] = -1.0f;
			triShape[i + 2][0] = triShape[i][0] + 0.1f; triShape[i + 2][1] = -0.7f;
			wh[i] = -1; col = true;
		}
		//���� �簢�� �浹
		else if (triShape[i][0] >= -0.5f && triShape[i][0] <= -0.4f &&
			triShape[i][1] <= 0.4f && triShape[i][1] >= -0.4f) {
			triShape[i][0] = -0.5f; triShape[i][1] -= 0.1f;
			triShape[i + 1][0] = -0.5f; triShape[i + 1][1] = triShape[i][1] + 0.2f;
			triShape[i + 2][0] = -0.8f; triShape[i + 2][1] = triShape[i][1] + 0.1f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i][0] <= 0.5f && triShape[i][0] >= 0.4f &&
			triShape[i][1] <= 0.4f && triShape[i][1] >= -0.4f) {
			triShape[i][0] = 0.5f; triShape[i][1] += 0.1f;
			triShape[i + 1][0] = 0.5f; triShape[i + 1][1] = triShape[i][1] - 0.2f;
			triShape[i + 2][0] = 0.8f; triShape[i + 2][1] = triShape[i][1] - 0.1f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i][1] <= 0.4f && triShape[i][1] >= 0.3f &&
			triShape[i][0] < 0.5f && triShape[i][0] > -0.5f) {
			triShape[i][0] -= 0.1f; triShape[i][1] = 0.4f;
			triShape[i + 1][0] = triShape[i][0] + 0.2f; triShape[i + 1][1] = 0.4f;
			triShape[i + 2][0] = triShape[i][0] + 0.1f; triShape[i + 2][1] = 0.7f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i][1] >= -0.4f && triShape[i][1] <= -0.3f &&
			triShape[i][0] < 0.5f && triShape[i][0] > -0.5f) {
			triShape[i][0] += 0.1f; triShape[i][1] = -0.4f;
			triShape[i + 1][0] = triShape[i][0] - 0.2f; triShape[i + 1][1] = -0.4f;
			triShape[i + 2][0] = triShape[i][0] - 0.1f; triShape[i + 2][1] = -0.7f;
			wh[i] = -1; col = true;
		}

		//������
		if (triShape[i + 1][0] <= -1.0f) {
			triShape[i + 1][0] = -1.0f; triShape[i + 1][1] = triShape[i + 1][1] - 0.1f;
			triShape[i + 2][0] = -0.7f; triShape[i + 2][1] = triShape[i + 1][1] + 0.1f;
			triShape[i][0] = -1.0f; triShape[i][1] = triShape[i + 1][1] + 0.2f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 1][0] >= 1.0f) {
			triShape[i + 1][0] = 1.0f; triShape[i + 1][1] = triShape[i + 1][1] + 0.1f;
			triShape[i + 2][0] = 0.7f; triShape[i + 2][1] = triShape[i + 1][1] - 0.1f;
			triShape[i][0] = 1.0f; triShape[i][1] = triShape[i + 1][1] - 0.2f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 1][1] >= 1.0f) {
			triShape[i + 1][0] = triShape[i + 1][0] - 0.1f; triShape[i + 1][1] = 1.0f;
			triShape[i + 2][0] = triShape[i + 1][0] + 0.1f; triShape[i + 2][1] = 0.7f;
			triShape[i][0] = triShape[i + 1][0] + 0.2f; triShape[i][1] = 1.0f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 1][1] <= -1.0f) {
			triShape[i + 1][0] = triShape[i + 1][0] + 0.1f; triShape[i + 1][1] = -1.0f;
			triShape[i + 2][0] = triShape[i + 1][0] - 0.1f; triShape[i + 2][1] = -0.7f;
			triShape[i][0] = triShape[i + 1][0] - 0.2f; triShape[i][1] = -1.0f;
			wh[i] = -1; col = true;
		}
		//���� �簢�� �浹
		else if (triShape[i + 1][0] >= -0.5f && triShape[i + 1][0] <= -0.4f &&
			triShape[i + 1][1] <= 0.4f && triShape[i + 1][1] >= -0.4f) {
			triShape[i + 1][0] = -0.5f; triShape[i + 1][1] = triShape[i + 1][1] + 0.1f;
			triShape[i + 2][0] = -0.8f; triShape[i + 2][1] = triShape[i + 1][1] - 0.1f;
			triShape[i][0] = -0.5f; triShape[i][1] = triShape[i + 1][1] - 0.2f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 1][1] <= 0.4f && triShape[i + 1][1] >= 0.3f &&
			triShape[i + 1][0] <= 0.5f && triShape[i + 1][0] >= -0.5f) {
			triShape[i + 1][0] += 0.1f; triShape[i + 1][1] = 0.4f;
			triShape[i + 2][0] = triShape[i + 1][0] - 0.1f; triShape[i + 2][1] = 0.7f;
			triShape[i][0] = triShape[i + 1][0] - 0.2f; triShape[i][1] = 0.4f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 1][0] <= 0.5f && triShape[i + 1][0] >= 0.4f &&
			triShape[i + 1][1] < 0.4f && triShape[i + 1][1] > -0.4f) {
			triShape[i + 1][0] = 0.5f; triShape[i + 1][1] = triShape[i + 1][1] - 0.1f;
			triShape[i + 2][0] = 0.7f; triShape[i + 2][1] = triShape[i + 1][1] + 0.1f;
			triShape[i][0] = 0.5f; triShape[i][1] = triShape[i + 1][1] + 0.2f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 1][1] >= -0.4f && triShape[i + 1][1] <= -0.3f && 
			triShape[i + 1][0] < 0.5f && triShape[i + 1][0] > -0.5f) {
			triShape[i + 1][0] -= 0.1f; triShape[i + 1][1] = -0.4f;
			triShape[i + 2][0] = triShape[i + 1][0] + 0.1f; triShape[i + 2][1] = -0.7f;
			triShape[i][0] = triShape[i + 1][0] + 0.2f; triShape[i][1] = -0.4f;
			wh[i] = -1; col = true;
		}

		//��
		if (triShape[i + 2][1] >= 1.0f) {
			triShape[i + 2][1] = 0.7f;
			triShape[i + 1][0] = triShape[i + 2][0] - 0.1f; triShape[i + 1][1] = 1.0f;
			triShape[i][0] = triShape[i + 2][0] + 0.1f; triShape[i][1] = 1.0f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 2][1] <= -1.0f) {
			triShape[i + 2][1] = -0.7f;
			triShape[i + 1][0] = triShape[i + 2][0] + 0.1f; triShape[i + 1][1] = -1.0f;
			triShape[i][0] = triShape[i + 2][0] - 0.1f; triShape[i][1] = -1.0f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 2][0] <= -1.0f) {
			triShape[i + 2][0] = -0.7f;
			triShape[i + 1][0] = -1.0f; triShape[i + 1][1] = triShape[i + 2][1] - 0.1f;
			triShape[i][0] = -1.0f; triShape[i][1] = triShape[i + 2][1] + 0.1f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 2][0] >= 1.0f) {
			triShape[i + 2][0] = 0.7f;
			triShape[i + 1][0] = 1.0f; triShape[i + 1][1] = triShape[i + 2][1] + 0.1f;
			triShape[i][0] = 1.0f; triShape[i][1] = triShape[i + 2][1] - 0.1f;
			wh[i] = -1; col = true;
		}

		//���� �簢�� �浹
		else if (triShape[i + 2][1] >= -0.4f && triShape[i + 2][1] <= -0.3f && 
			triShape[i + 2][0] <= 0.5f && triShape[i + 2][0] >= -0.5f) {
			triShape[i + 2][1] = -0.7f;
			triShape[i + 1][0] = triShape[i + 2][0] - 0.1f; triShape[i + 1][1] = -0.4f;
			triShape[i][0] = triShape[i + 2][0] + 0.1f; triShape[i][1] = -0.4f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 2][1] <= 0.4f && triShape[i + 2][1] >= 0.3f && 
			triShape[i + 2][0] <= 0.5f && triShape[i + 2][0] >= -0.5f) {
			triShape[i + 2][1] = 0.7f;
			triShape[i + 1][0] = triShape[i + 2][0] + 0.1f; triShape[i + 1][1] = 0.4f;
			triShape[i][0] = triShape[i + 2][0] - 0.1f; triShape[i][1] = 0.4f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 2][0] <= 0.5f && triShape[i + 2][0] >= 0.4f && 
			triShape[i + 2][1] <= 0.4f && triShape[i + 2][1] >= -0.4f) {
			triShape[i + 2][0] = 0.8f;
			triShape[i + 1][0] = 0.5f;
			triShape[i][0] = 0.5f;
			wh[i] = -1; col = true;
		}
		else if (triShape[i + 2][0] >= -0.5f && triShape[i + 2][0] <= -0.4f && 
			triShape[i + 2][1] <= 0.4f && triShape[i + 2][1] >= -0.4f) {
			triShape[i + 2][0] = -0.8f;
			triShape[i + 1][0] = -0.5f;
			triShape[i][0] = -0.5f;
			wh[i] = -1; col = true;
		}

		if (col) {
			if (i == 3) {
				for (int j = 3; j < 6; j++) {
					colors[j][0] = (float)rand() / (float)RAND_MAX;
					colors[j][1] = (float)rand() / (float)RAND_MAX;
					colors[j][2] = (float)rand() / (float)RAND_MAX;
				}
			}
			else {
				for (int j = 0; j < 3; j++) {
					colors[j][0] = (float)rand() / (float)RAND_MAX;
					colors[j][1] = (float)rand() / (float)RAND_MAX;
					colors[j][2] = (float)rand() / (float)RAND_MAX;
				}
			}
			col = false;
		}
		}

	triShape[6][0] += d * 0.05f;
	triShape[7][0] += d * 0.05f;
	triShape[8][0] += d * 0.05f;

	triShape[9][0] -= d * 0.05f;
	triShape[10][0] -= d * 0.05f;
	triShape[11][0] -= d * 0.05f;

	if (triShape[10][0] == 0.5 || triShape[9][0] == -0.5) {
		d *= -1;
		for (int j = 6; j < 12; j++) {
			colors[j][0] = (float)rand() / (float)RAND_MAX;
			colors[j][1] = (float)rand() / (float)RAND_MAX;
			colors[j][2] = (float)rand() / (float)RAND_MAX;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(100, timer, 1);
}
//opengl ��ǥ�� ��ȯ
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