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
void Mouse(int button, int state, int x, int y);
void changeOpenGL(int x, int y, float* ox, float* oy);
void timer(int value);
GLfloat triShape[12][3] = { //--- �ﰢ�� ��ġ ��
	
};
const GLfloat colors[12][3] = { //--- �ﰢ�� ������ ����
	{ 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } ,
	{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 },
	{ 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 }, { 0.5, 0.5, 0.5 }
};
GLuint vao, vbo[2];
int cnt = 0;
int dir[4][2] = {
	{1, 1},
	{-1, 1},
	{-1, -1},
	{1, -1}
};
int wh[12] = { 0 };
bool first = true;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example7");
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
	glutMouseFunc(Mouse);
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
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), triShape, GL_DYNAMIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), colors, GL_DYNAMIC_DRAW);
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
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	}
	glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y) {
	float ox, oy;
	if (cnt == 12)
		return;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		changeOpenGL(x, y, &ox, &oy);
		triShape[cnt][0] = ox - 0.1f; triShape[cnt][1] = oy - 0.1f; triShape[cnt][2] = 0.0;
		triShape[cnt + 1][0] = ox + 0.1f; triShape[cnt + 1][1] = oy - 0.1f; triShape[cnt + 1][2] = 0.0;
		triShape[cnt + 2][0] = ox; triShape[cnt + 2][1] = oy + 0.2f; triShape[cnt + 2][2] = 0.0;
		cnt += 3;
	}
}
void timer(int value) {
	for (int i = 0; i < 10; i+=3) {
		if (first) {
			int n = rand() % 4;
			wh[i] = n;
			switch (n) {
			case 0:
				triShape[i][0] += dir[n][0] * 0.1f; triShape[i][1] += dir[n][1] * 0.1f;
				triShape[i+1][0] += dir[n][0] * 0.1f; triShape[i+1][1] += dir[n][1] * 0.1f;
				triShape[i+2][0] += dir[n][0] * 0.1f; triShape[i+2][1] += dir[n][1] * 0.1f;
				break;
			case 1:
				triShape[i][0] += dir[n][0] * 0.1f; triShape[i][1] += dir[n][1] * 0.1f;
				triShape[i + 1][0] += dir[n][0] * 0.1f; triShape[i + 1][1] += dir[n][1] * 0.1f;
				triShape[i + 2][0] += dir[n][0] * 0.1f; triShape[i + 2][1] += dir[n][1] * 0.1f;
				break;
			case 2:
				triShape[i][0] += dir[n][0] * 0.1f; triShape[i][1] += dir[n][1] * 0.1f;
				triShape[i + 1][0] += dir[n][0] * 0.1f; triShape[i + 1][1] += dir[n][1] * 0.1f;
				triShape[i + 2][0] += dir[n][0] * 0.1f; triShape[i + 2][1] += dir[n][1] * 0.1f;
				break;
			default:
				triShape[i][0] += dir[n][0] * 0.1f; triShape[i][1] += dir[n][1] * 0.1f;
				triShape[i + 1][0] += dir[n][0] * 0.1f; triShape[i + 1][1] += dir[n][1] * 0.1f;
				triShape[i + 2][0] += dir[n][0] * 0.1f; triShape[i + 2][1] += dir[n][1] * 0.1f;
				break;
			}
		}
		else {
			triShape[i][0] += dir[wh[i]][0] * 0.1f; triShape[i][1] += dir[wh[i]][0] * 0.1f;
			triShape[i + 1][0] += dir[wh[i]][0] * 0.1f; triShape[i + 1][1] += dir[wh[i]][0] * 0.1f;
			triShape[i + 2][0] += dir[wh[i]][0] * 0.1f; triShape[i + 2][1] += dir[wh[i]][0] * 0.1f;
			if (triShape[i][0] <= -1.0f || triShape[i + 1][0] <= -1.0f || triShape[i + 2][0] <= -1.0f) {//���� ���� ������
				triShape[i][0] += 0.05f; triShape[i][1] += 0.05f;
				triShape[i+1][0] += 0.05f; triShape[i+1][1] += 0.05f;
				triShape[i+2][0] += 0.05f; triShape[i+2][1] += 0.05f;
				wh[i] = rand() % 4;
			}
			else if (triShape[i][0] >= 1.0f || triShape[i + 1][0] >= 1.0f || triShape[i + 2][0] >= 1.0f) {//������ ���� ������
				triShape[i][0] -= 0.05f; triShape[i][1] -= 0.05f;
				triShape[i + 1][0] -= 0.05f; triShape[i + 1][1] -= 0.05f;
				triShape[i + 2][0] -= 0.05f; triShape[i + 2][1] -= 0.05f;
				wh[i] = rand() % 4;
			}
			else if (triShape[i][1] <= -1.0f || triShape[i + 1][1] <= -1.0f || triShape[i + 2][1] <= -1.0f) {//���� ���� ������
				triShape[i][0] += 0.05f; triShape[i][1] += 0.05f;
				triShape[i + 1][0] += 0.05f; triShape[i + 1][1] += 0.05f;
				triShape[i + 2][0] += 0.05f; triShape[i + 2][1] += 0.05f;
				wh[i] = rand() % 4;
			}
			else if (triShape[i][1] >= 1.0f || triShape[i + 1][1] >= 1.0f || triShape[i + 2][1] >= 1.0f) {//�Ʒ��� ���� ������
				triShape[i][0] -= 0.05f; triShape[i][1] -= 0.05f;
				triShape[i + 1][0] -= 0.05f; triShape[i + 1][1] -= 0.05f;
				triShape[i + 2][0] -= 0.05f; triShape[i + 2][1] -= 0.05f;
				wh[i] = rand() % 4;
			}
				
		}
	}
	first = false;
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