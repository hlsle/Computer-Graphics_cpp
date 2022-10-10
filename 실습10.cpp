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
GLfloat line[15][3] = {
	//��
	{-0.5,  0.5, 0.0},
	//��
	{-0.7,  0.5, 0.0},
	{-0.3, 0.5, 0.0},
	//�ﰢ��
	{-0.5, 0.7, 0.0},
	{-0.75, 0.15, 0.0},
	{-0.25, 0.15, 0.0},
	//�簢��
	{-0.25, 0.65, 0.0},
	{-0.75, 0.65, 0.0},
	{-0.75, 0.25, 0.0},
	{-0.25, 0.25, 0.0},
	//������
	{-0.5, 0.8, 0.0},
	{0.3 * cos(3.14 * 0.9) - 0.5, 0.3 * sin(3.14 * 0.9) + 0.5, 0.0},
	{0.3 * cos(3.14 * -0.7) - 0.5, 0.3 * sin(3.14 * -0.7) + 0.5, 0.0},
	{0.3 * cos(3.14 * -0.3) - 0.5, 0.3 * sin(3.14 * -0.3) + 0.5, 0.0},
	{0.3 * cos(3.14 * 0.1) - 0.5, 0.3 * sin(3.14 * 0.1) + 0.5, 0.0}
};
GLfloat pol_color[15][3] = {
	//�� ����
	{0.0, 0.0, 0.0},
	//�� ����
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	//�ﰢ�� ����
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	//�簢�� ����
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	//������ ����
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
};
GLfloat tri[15][3] = {
	//��
	{0.5,  0.5, 0.0},
	//��
	{0.7,  0.5, 0.0},
	{0.3, 0.5, 0.0},
	//�ﰢ��
	{0.5, 0.7, 0.0},
	{0.75, 0.15, 0.0},
	{0.25, 0.15, 0.0},
	//�簢��
	{0.25, 0.65, 0.0},
	{0.75, 0.65, 0.0},
	{0.75, 0.25, 0.0},
	{0.25, 0.25, 0.0},
	//������
	{0.5, 0.8, 0.0},
	{0.3 * cos(3.14 * 0.9) + 0.5, 0.3 * sin(3.14 * 0.9) + 0.5, 0.0},
	{0.3 * cos(3.14 * -0.7) + 0.5, 0.3 * sin(3.14 * -0.7) + 0.5, 0.0},
	{0.3 * cos(3.14 * -0.3) + 0.5, 0.3 * sin(3.14 * -0.3) + 0.5, 0.0},
	{0.3 * cos(3.14 * 0.1) + 0.5, 0.3 * sin(3.14 * 0.1) + 0.5, 0.0}
};
GLfloat rect[15][3] = {
	//��
	{-0.5,  -0.5, 0.0},
	//��
	{-0.7,  -0.5, 0.0},
	{-0.3, -0.5, 0.0},
	//�ﰢ��
	{-0.5, -0.15, 0.0},
	{-0.75, -0.7, 0.0},
	{-0.25, -0.7, 0.0},
	//�簢��
	{-0.25, -0.65, 0.0},
	{-0.75, -0.65, 0.0},
	{-0.75, -0.25, 0.0},
	{-0.25, -0.25, 0.0},
	//������
	{-0.5, -0.2, 0.0},
	{0.3 * cos(3.14 * 0.9) - 0.5, 0.3 * sin(3.14 * 0.9) - 0.5, 0.0},
	{0.3 * cos(3.14 * -0.7) - 0.5, 0.3 * sin(3.14 * -0.7) - 0.5, 0.0},
	{0.3 * cos(3.14 * -0.3) - 0.5, 0.3 * sin(3.14 * -0.3) - 0.5, 0.0},
	{0.3 * cos(3.14 * 0.1) - 0.5, 0.3 * sin(3.14 * 0.1) - 0.5, 0.0}
};
GLfloat pen[15][3] = {
	//��
	{0.5, -0.5, 0.0},
	//��
	{0.7, -0.5, 0.0},
	{0.3, -0.5, 0.0},
	//�ﰢ��
	{0.5, -0.15, 0.0},
	{0.75, -0.7, 0.0},
	{0.25, -0.7, 0.0},
	//�簢��
	{0.25, -0.65, 0.0},
	{0.75, -0.65, 0.0},
	{0.75, -0.25, 0.0},
	{0.25, -0.25, 0.0},
	//������
	{0.5, -0.2, 0.0},
	{0.3 * cos(3.14 * 0.9) + 0.5, 0.3 * sin(3.14 * 0.9) - 0.5, 0.0},
	{0.3 * cos(3.14 * -0.7) + 0.5, 0.3 * sin(3.14 * -0.7) - 0.5, 0.0},
	{0.3 * cos(3.14 * -0.3) + 0.5, 0.3 * sin(3.14 * -0.3) - 0.5, 0.0},
	{0.3 * cos(3.14 * 0.1) + 0.5, 0.3 * sin(3.14 * 0.1) - 0.5, 0.0}
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
int l = 1, t = 3, r = 6, p = 10;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(pol_color), pol_color, GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(pol_color), pol_color, GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(pol_color), pol_color, GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(pol_color), pol_color, GL_STATIC_DRAW);
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
	if (l < 1) {
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, 1);
		glPointSize(1.0);
	}
	else if (l < 3) {
		glLineWidth(3.0);
		glDrawArrays(GL_LINES, 1, 2);
		glLineWidth(1.0);
	}
	else if (l < 6)
		glDrawArrays(GL_TRIANGLES, 3, 3);
	else if (l < 10)
		glDrawArrays(GL_TRIANGLE_FAN, 6, 4);
	else
		glDrawArrays(GL_TRIANGLE_FAN, 10, 5);
	
	glBindVertexArray(vao[2]);
	if (t < 1) {
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, 1);
		glPointSize(1.0);
	}
	else if (t < 3) {
		glLineWidth(3.0);
		glDrawArrays(GL_LINES, 1, 2);
		glLineWidth(1.0);
	}
	else if (t < 6)
		glDrawArrays(GL_TRIANGLES, 3, 3);
	else if (t < 10)
		glDrawArrays(GL_TRIANGLE_FAN, 6, 4);
	else
		glDrawArrays(GL_TRIANGLE_FAN, 10, 5);
	
	glBindVertexArray(vao[3]);
	if (r < 1) {
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, 1);
		glPointSize(1.0);
	}
	else if (r < 3) {
		glLineWidth(3.0);
		glDrawArrays(GL_LINES, 1, 2);
		glLineWidth(1.0);
	}
	else if (r < 6)
		glDrawArrays(GL_TRIANGLES, 3, 3);
	else if (r < 10)
		glDrawArrays(GL_TRIANGLE_FAN, 6, 4);
	else
		glDrawArrays(GL_TRIANGLE_FAN, 10, 5);
	
	glBindVertexArray(vao[4]);
	if (p < 1) {
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, 1);
		glPointSize(1.0);
	}
	else if (p < 3) {
		glLineWidth(3.0);
		glDrawArrays(GL_LINES, 1, 2);
		glLineWidth(1.0);
	}
	else if (p < 6)
		glDrawArrays(GL_TRIANGLES, 3, 3);
	else if (p < 10)
		glDrawArrays(GL_TRIANGLE_FAN, 6, 4);
	else
		glDrawArrays(GL_TRIANGLE_FAN, 10, 5);
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void timer(int value) {
	l++; t++; r++; p++;
	if (l == 15)
		l = 0;
	if (t == 15)
		t = 0;
	if (r == 15)
		r = 0;
	if (p == 15)
		p = 0;
	glutPostRedisplay();
	glutTimerFunc(500, timer, 1);
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