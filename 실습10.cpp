#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <math.h>
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
void timer(int value);
GLfloat line[5][3] = {
	{-0.7, 0.5, 0.0},
	{-0.3, 0.5, 0.0},
	{-0.5, 0.5, 0.0},
	{-0.5, 0.5, 0.0},
	{-0.5, 0.5, 0.0}
};
GLfloat line_color[5][3] = {
	//������ ����
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
};
GLfloat tri[5][3] = {
	{0.3, 0.5, 0.0},
	{0.7, 0.5, 0.0},
	{0.5, 0.74, 0.0},
	{0.5, 0.74, 0.0},
	{0.5, 0.74, 0.0}
};
GLfloat tri_color[5][3] = {
	//������ ����
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0}
};
GLfloat rect[5][3] = {
	{-0.7, -0.5, 0.0},
	{-0.3, -0.5, 0.0},
	{-0.7, -0.26, 0.0},
	{-0.3, -0.26, 0.0},
	{-0.5, -0.26, 0.0}
};
GLfloat rect_color[5][3] = {
	//������ ����
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0}
};
GLfloat pen[5][3] = {
	{0.35, -0.6, 0.0},
	{0.65, -0.6, 0.0},
	{0.3, -0.3, 0.0},
	{0.7, -0.3, 0.0},
	{0.5, -0.2, 0.0}
};
GLfloat pen_color[5][3] = {
	//������ ����
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}
};
GLfloat slice[4][3] = {
	{-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0}, {0.0, -1.0, 0.0}
};
GLfloat slice_color[4][3] = {
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 0.0 }
};
GLuint vao[5], vbo[2], ebo;
int l = 1, t = 1, r = 1, p = 1;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example10");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();
	InitShader();
	InitBuffer();
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glutTimerFunc(500, timer, 1);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
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
	glGenVertexArrays(1, &vao[0]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao[0]); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(slice), slice, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(slice_color), slice_color, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);

	//��
	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_color), line_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//�ﰢ
	glGenVertexArrays(1, &vao[2]);
	glBindVertexArray(vao[2]);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_color), tri_color, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);

	//�簢
	glGenVertexArrays(1, &vao[3]);
	glBindVertexArray(vao[3]);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_color), rect_color, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);

	//����
	glGenVertexArrays(1, &vao[4]);
	glBindVertexArray(vao[4]);
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(pen), pen, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(pen_color), pen_color, GL_STATIC_DRAW);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	InitShader();
	InitBuffer();

	glUseProgram(s_program);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);

	glBindVertexArray(vao[1]);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);
	glDrawArrays(GL_TRIANGLES, 2, 3);

	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);
	glDrawArrays(GL_TRIANGLES, 2, 3);

	glBindVertexArray(vao[3]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);
	glDrawArrays(GL_TRIANGLES, 2, 3);

	glBindVertexArray(vao[4]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);
	glDrawArrays(GL_TRIANGLES, 2, 3);

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void timer(int value) {
	l++; t++; r++; p++;
	if (l < 10) {
		line[2][1] += 0.03f;
		line[3][1] += 0.03f;
		line[4][1] += 0.03f;
	}
	else if (l < 20) {
		line[2][0] -= 0.02f;
		line[3][0] += 0.02f;
	}
	else if (l < 30) {
		line[0][0] += 0.011f;
		line[1][0] -= 0.011f;
		line[0][1] -= 0.008f;
		line[1][1] -= 0.008f;
		line[2][1] -= 0.011f;
		line[3][1] -= 0.011f;
		line[2][0] += 0.005f;
		line[3][0] -= 0.005f;
	}
	else if (l < 40) {
		line[4][1] -= 0.023f;
		line[2][1] -= 0.011f;
		line[3][1] -= 0.011f;
		line[0][1] += 0.008f;
		line[1][1] += 0.008f;
		line[0][0] += 0.008f;
		line[1][0] -= 0.008f;
		line[2][0] += 0.014f;
		line[3][0] -= 0.014f;
	}

	if (t < 12) {
		tri[2][0] -= 0.02f;
		tri[3][0] += 0.02f;
	}
	else if (t < 21) {
		tri[0][0] += 0.011f;
		tri[1][0] -= 0.011f;
		tri[0][1] -= 0.008f;
		tri[1][1] -= 0.008f;
		tri[2][1] -= 0.011f;
		tri[3][1] -= 0.011f;
		tri[2][0] += 0.005f;
		tri[3][0] -= 0.005f;
	}
	else if (t < 30) {
		tri[4][1] -= 0.025f;
		tri[2][1] -= 0.012f;
		tri[3][1] -= 0.012f;
		tri[0][1] += 0.009f;
		tri[1][1] += 0.009f;
		tri[0][0] += 0.01f;
		tri[1][0] -= 0.01f;
		tri[2][0] += 0.016f;
		tri[3][0] -= 0.016f;
	}

	if (r < 10) {
		rect[0][0] += 0.011f;
		rect[1][0] -= 0.011f;
		rect[0][1] -= 0.008f;
		rect[1][1] -= 0.008f;
		rect[2][1] -= 0.011f;
		rect[3][1] -= 0.011f;
		rect[2][0] += 0.005f;
		rect[3][0] -= 0.005f;
	}
	else if (r < 19) {
		rect[4][1] -= 0.023f;
		rect[2][1] -= 0.012f;
		rect[3][1] -= 0.012f;
		rect[0][1] += 0.009f;
		rect[1][1] += 0.009f;
		rect[0][0] += 0.0114f;
		rect[1][0] -= 0.0114f;
		rect[2][0] += 0.0166;
		rect[3][0] -= 0.0166f;
	}

	if (p < 12) {
		pen[4][1] -= 0.0252f;
		pen[2][1] -= 0.0155f;
		pen[3][1] -= 0.0155f;
		pen[0][1] += 0.0125f;
		pen[1][1] += 0.0125f;
		pen[0][0] += 0.014f;
		pen[1][0] -= 0.014f;
		pen[2][0] += 0.019f;
		pen[3][0] -= 0.019f;
	}
	glutPostRedisplay();
	glutTimerFunc(400, timer, 1);
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