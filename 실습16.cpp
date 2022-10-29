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
void Timer(int value);
float x_init, y_init, ox, oy;
GLuint vao[3], vbo[6];
objRead cube;
objRead pyra;
GLint Object = cube.loadObj_normalize_center("cube.obj");
GLint Object2 = pyra.loadObj_normalize_center("pyra.obj");
float line[6][3] = {
	{0.0, 0.8, 0.0},
	{0.0, -0.8, 0.0},
	{0.8, -0.2, 0.0},
	{-0.8, 0.2, 0.0},
	{0.6, 0.6, 0.0},
	{-0.6, -0.6, 0.0}
};
float line_col[12][3] = {
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
};
float cube_col[36][3] = {
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
float pyra_col[21][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 0.0, 0.0},
	{1.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 1.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
};
float cube_front[6][3];
float cube_top[6][3];
float cube_side[12][3];

// ��ǥ��ȯ�� ���� ���
glm::mat4 ry = glm::mat4(1.0); // y rotate
glm::mat4 T_cu = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -0.25, 0.0));
glm::mat4 R_cu = glm::rotate(glm::mat4(1.0), glm::radians(20.0f), glm::vec3(1.0, 1.0, 0.0));

bool is_h = false, is_cube = true, is_pyra = false, is_ry = false;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example16");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();

	InitShader();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timer, 1);
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
	glGenBuffers(6, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	// x, y ��
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_col), line_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// ����ü 
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, cube.outvertex.size() * sizeof(glm::vec3), &cube.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_col), cube_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// �簢��
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, pyra.outvertex.size() * sizeof(glm::vec3), &pyra.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyra_col), pyra_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(s_program);
	//--- ����� VAO �ҷ�����

	//x, y, z�� �׸���
	glBindVertexArray(vao[0]);
	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
	glDrawArrays(GL_LINES, 0, 6);

	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ry * T_cu * R_cu));
	if (is_cube) {
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	if (is_pyra) {
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, Object2);
	}

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		// ���� ����
	case 'c':
		if (is_cube) {
			is_cube = false;
			is_pyra = true;
		}
		else {
			is_cube = true;
			is_pyra = false;
		}
	case 'h':
		if (!is_h)
			glEnable(GL_DEPTH_TEST);
		else  glDisable(GL_DEPTH_TEST);
		break;

		// y�� ����
	case 'y':
		if (!is_ry)
			is_ry = true;
		else is_ry = false;
		break;

		// ����ü ���� �ִϸ��̼�
	case 't':
		break;
	case 'T':
		break;

		// ����ü �ո� ���� �ݱ�
	case 'f':
		break;
	case 'F':
		break;

		// ����ü ���� ���� �ݱ�
	case '1':
		break;
	case '2':
		break;

		// �簢�� �� �� ���� �ݱ�
	case 'o':
		break;
	case 'O':
		break;

		// ����/���� ����
	case 'p':
		break;
	case 'P':
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {
	if (is_ry) {
		ry = glm::rotate(ry, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	glutTimerFunc(100, Timer, 1);
	glutPostRedisplay();
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