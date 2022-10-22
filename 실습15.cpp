#include "objRead.h"
#define VERTEXCOUNT 258
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
GLuint vao[6], vbo[6];
objRead cube;
objRead cone;
objRead pyra;
objRead sphere;
GLint Object = cube.loadObj_normalize_center("cube.obj");
GLint Object2 = cone.loadObj_normalize_center("cone.obj");
GLint Object3 = pyra.loadObj_normalize_center("pyra.obj");
GLint Object4 = sphere.loadObj_normalize_center("sphere.obj");
float line[6][3] = {
	{0.0, 0.8, 0.0},
	{0.0, -0.8, 0.0},
	{0.8, -0.2, 0.0},
	{-0.8, 0.2, 0.0},
	{0.6, 0.6, 0.0},
	{-0.6, -0.6, 0.0}
};
float to[VERTEXCOUNT][3] = {};
float angle = 0.1, r = 0.01;
// ��ü ���� ����
bool is_cube = true, is_cone = true, is_pyra = false, is_sphere = false, is_to = false;

// ���� ���� ����
bool is_hid = false;
bool le = false;

// �ִϸ��̼� ����
bool d = false, f = false, g = false, dr = true;

// ��ǥ��ȯ�� ���� ���
glm::mat4 l = glm::mat4(1.0);
glm::mat4 cp = glm::translate(glm::mat4(1.0), glm::vec3(-0.5, 0.25, 0.0)); // cube or pyra transform
glm::mat4 cs = glm::translate(glm::mat4(1.0), glm::vec3(0.5, -0.25, 0.0)); // cone or sphere transform

glm::mat4 sl = glm::mat4(1.0); // line scale
glm::mat4 scp = glm::mat4(1.0); // cube or pryra scale
glm::mat4 scs = glm::mat4(1.0); // cone or sphere scale

glm::mat4 rcp = glm::mat4(1.0); // cube or pyra rotate
glm::mat4 rcs = glm::mat4(1.0); // cone or sphere rotate
glm::mat4 ry = glm::mat4(1.0); // y rotate
glm::mat4 rt = glm::rotate(glm::mat4(1.0), glm::radians(60.0f), glm::vec3(1.0, 0.0, 0.0));
// ȸ�� ����
int ro_y = 0, cp_rx = 0, cp_ry = 0, cs_rx = 0, cs_ry = 0;

// ���� ������
float cube_x = cube.temp_vertices[0].x - 0.5, cube_y = cube.temp_vertices[0].y + 0.25;
float cone_x = cone.temp_vertices[9].x + 0.5, cone_y = cone.temp_vertices[9].y - 0.25;

int n = 0, m = 0, cp_i, cs_i;
float cen_cp[2] = {
	-0.5, 0.25
};
float cen_cs[2] = {
	0.5, -0.25
};
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example15");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	srand((unsigned int)time(NULL));
	glewInit();
	
	// ȸ���� ��� ���ؽ��� �Է�
	for (int i = 0; i < VERTEXCOUNT; i++) {
		to[i][0] = r * cos(angle);
		to[i][1] = r * sin(angle);
		to[i][2] = 0.0f;
		r += 0.004f;
		angle += 0.256f;
	}
	InitShader();
	InitBuffer();
	glClearColor(0.0, 0.0, 0.0, 1.0f);
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
	glGenVertexArrays(6, vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(6, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	// x, y ��
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// ����ü 
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, cube.outvertex.size() * sizeof(glm::vec3), &cube.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// ����
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, cone.outvertex.size() * sizeof(glm::vec3), &cone.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// �簢��
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, pyra.outvertex.size() * sizeof(glm::vec3), &pyra.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// ��
	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sphere.outvertex.size() * sizeof(glm::vec3), &sphere.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(to), to, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(s_program);
	//--- ����� VAO �ҷ�����

	//x, y�� �׸���
	glBindVertexArray(vao[0]); 
	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(l * ry * sl));
	glDrawArrays(GL_LINES, 0, 6);

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sl *  ry *  cp * rcp  * scp));

	// ����ü �׸���
	if (is_cube) {
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, Object);
	}

	// �簢�� �׸���
	if (is_pyra) {
		glBindVertexArray(vao[3]);
		glDrawArrays(GL_TRIANGLES, 0, Object3);
	}

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(sl * ry *  cs * rcs * scs));
	// ���� �׸���
	if (is_cone) {
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, Object2);
	}

	// �� �׸���
	if (is_sphere) {
		glBindVertexArray(vao[4]);
		glDrawArrays(GL_TRIANGLES, 0, Object4);
	}
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(rt));
	
	if (is_to) {
		glBindVertexArray(vao[5]);
		glPointSize(5.0);
		glDrawArrays(GL_LINE_STRIP, 0, VERTEXCOUNT-1);
	}
	glPointSize(1.0);
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		// �ٸ� �������� �ٲٱ�
	case 'c':
		if (is_cube) {
			is_cube = false;
			is_cone = false;
			is_pyra = true;
			is_sphere = true;
		}
		else if (is_pyra) {
			is_cube = true;
			is_cone = true;
			is_pyra = false;
			is_sphere = false;
		}
		break;

		// ���� ��ü Ȯ��, ���
	case 'q': scp = glm::scale(scp, glm::vec3(1.1, 1.1, 1.1));
		break;
	case 'Q': scp = glm::scale(scp, glm::vec3(0.8, 0.8, 0.8));
		break;

		// ������ ��ü Ȯ��, ���
	case 'w': scs = glm::scale(scs, glm::vec3(1.1, 1.1, 1.1));
		break;
	case 'W': scs = glm::scale(scs, glm::vec3(0.8, 0.8, 0.8));
		break;

		// ���� ��ü �߽��� �������� x��  ��/���� �������� ȸ��
	case 'x':
		if (cs_rx == 1) {
			cs_rx = 0;
			break;
		}
		cs_rx = 1;
		break;
	case 'X':
		if (cs_rx == -1) {
			cs_rx = 0;
			break;
		}
		cs_rx = -1;
		break;

		// ���� ��ü x, y, z�� �̵�
	case 'i': 
		cp = glm::translate(cp, glm::vec3(0.1, 0.0, 0.0));
		cube_x += 0.1;
		break;
	case 'I': 
		cp = glm::translate(cp, glm::vec3(-0.1, 0.0, 0.0));
		cube_x -= 0.1;
		break;

	case 'o': 
		cp = glm::translate(cp, glm::vec3(0.0, 0.1, 0.0));
		cube_y += 0.1;
		break;
	case 'O':
		cp = glm::translate(cp, glm::vec3(0.0, -0.1, 0.0));
		cube_y -= 0.1;
		break;

	case 'p': cp = glm::translate(cp, glm::vec3(0.0, 0.0, 0.2));
		break;
	case 'P': cp = glm::translate(cp, glm::vec3(0.0, 0.0, -0.2));
		break;

		// ������ ��ü x, y, z �̵�
	case 'j': 
		cs = glm::translate(cs, glm::vec3(0.1, 0.0, 0.0));
		cone_x += 0.1;
		break;
	case 'J': 
		cs = glm::translate(cs, glm::vec3(-0.1, 0.0, 0.0));
		cone_x -= 0.1;
		break;

	case 'k': 
		cs = glm::translate(cs, glm::vec3(0.0, 0.1, 0.0));
		cone_y += 0.1;
		break;
	case 'K': 
		cs = glm::translate(cs, glm::vec3(0.0, -0.1, 0.0));
		cone_y -= 0.1;
		break;

	case 'l': cs = glm::translate(cs, glm::vec3(0.0, 0.0, 0.2));
		break;
	case 'L': cs = glm::translate(cs, glm::vec3(0.0, 0.0, -0.2));
		break;

		// ���� ����
	case 'e': 
		sl = glm::scale(sl, glm::vec3(1.1, 1.1, 1.1));
		break;
	case 'E': 
		sl = glm::scale(sl, glm::vec3(0.8, 0.8, 0.8));
		break;

		// ���� ��ü �߽��� �������� y�� �� / ���� �������� ȸ��
	case 'y':
		if (cs_ry == 1) {
			cs_ry = 0;
			break;
		}
		cs_ry = 1;
		break;
	case 'Y':
		if (cs_ry == -1) {
			cs_ry = 0;
			break;
		}
		cs_ry = -1;
		break;

		// ���� ��ü �߽��� �������� x��  ��/���� �������� ȸ��
	case 'a':
		if (cp_rx == 1) {
			cp_rx = 0;
			break;
		}
		cp_rx = 1;
		break;
	case 'A':
		if (cp_rx == -1) {
			cp_rx = 0;
			break;
		}
		cp_rx = -1;
		break;

		// ���� ��ü �߽��� �������� y�� �� / ���� �������� ȸ��
	case 'b':
		if (cp_ry == 1) {
			cp_ry = 0;
			break;
		}
		cp_ry = 1;
		break;
	case 'B':
		if (cp_ry == -1) {
			cp_ry = 0;
			break;
		}
		cp_ry = -1;
		break;

		// �� ��ü�� y�� �������� ��/�� �������� ȸ��
	case 'r':
		if (ro_y == 1) {
			ro_y = 0;
			break;
		}
		ro_y = 1;
		break;
	case 'R':
		if (ro_y == -1) {
			ro_y = 0;
			break;
		}
		ro_y = -1;
		break;
		
		// �� ��ü x, z�̵�
	case 'n':
		cp = glm::translate(cp, glm::vec3(0.1, 0.0, 0.0));
		cs = glm::translate(cs, glm::vec3(0.1, 0.0, 0.0));
		cube_x += 0.1;
		cone_x += 0.1;
		break;
	case 'N':
		cp = glm::translate(cp, glm::vec3(-0.1, 0.0, 0.0));
		cs = glm::translate(cs, glm::vec3(-0.1, 0.0, 0.0));
		cube_x -= 0.1;
		cone_x -= 0.1;
		break;
	case 'm':
		cp = glm::translate(cp, glm::vec3(0.0, 0.0, 0.2));
		cs = glm::translate(cs, glm::vec3(0.0, 0.0, 0.2));
		break;
	case 'M':
		cp = glm::translate(cp, glm::vec3(0.0, 0.0, -0.2));
		cs = glm::translate(cs, glm::vec3(0.0, 0.0, -0.2));
		break;

		// ȸ����
	case 'd':
		if (is_to)
			is_to = false;
		else {
			float min_cp = 2.0, min_cs = 2.0, min_x, min_y, min_x2, min_y2;
			for (int i = 0; i < VERTEXCOUNT; i++) {
				if (min_cp >= pow(to[i][0] - cen_cp[0], 2) + pow(to[i][1] - cen_cp[1], 2)) {
					min_cp = pow(to[i][0] - cen_cp[0], 2) + pow(to[i][1] - cen_cp[1], 2);
					min_x = to[i][0];
					min_y = to[i][1];
					cp_i = i;
				}	
				if (min_cs >= pow(to[i][0] - cen_cs[0], 2) + pow(to[i][1] - cen_cs[1], 2)) {
					min_cs = pow(to[i][0] - cen_cs[0], 2) + pow(to[i][1] - cen_cs[1], 2);
					min_x2 = to[i][0];
					min_y2 = to[i][1];
					cs_i = i;
				}
			}

			cp = glm::translate(glm::mat4(1.0), glm::vec3(min_x, min_y * 1.5 / 3, 0.0));
			cs = glm::translate(glm::mat4(1.0), glm::vec3(min_x2, min_y2 * 1.5 / 3, 0.0));

			is_to = true;
		}
		glutTimerFunc(100, Timer, 1);
		break;

		// ȸ���� ȸ�� ����
	case '1':
		dr = true;
		break;
	case '2': dr = false;
		break;

		// ���� �Դٰ���
	case 'f':
		if (!f)
			f = true;
		else f = false;
		glutTimerFunc(100, Timer, 1);
		break;

		// ��� �ڸ��� �̵�
	case 'g':
		if (!g)
			g = true;
		else g = false;
		glutTimerFunc(100, Timer, 1);
		break;

		// ����ġ
	case 's':
		ro_y = 0;
		cp_rx = 0;
		cp_ry = 0;
		cs_rx = 0;
		cs_ry = 0;
		cp = glm::translate(glm::mat4(1.0), glm::vec3(-0.5, 0.25, 0.0));
		cs = glm::translate(glm::mat4(1.0), glm::vec3(0.5, -0.25, 0.0));
		rcp = glm::mat4(1.0);
		rcs = glm::mat4(1.0);
		ry = glm::mat4(1.0);
		sl = glm::mat4(1.0);
		scp = glm::mat4(1.0);
		scs = glm::mat4(1.0);
		n = 0; m = 0;
		cube_x = cube.temp_vertices[0].x - 0.5, cube_y = cube.temp_vertices[0].y + 0.25;
		cone_x = cone.temp_vertices[9].x + 0.5, cone_y = cone.temp_vertices[9].y - 0.25;
		is_to = false;
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {

	if (ro_y == 1) {
		ry = glm::rotate(ry, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (ro_y == -1) {
		ry = glm::rotate(ry, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
	}

	if (cp_rx == 1) {
		rcp = glm::rotate(rcp, glm::radians(5.0f), glm::vec3(1.0, 0.0, 0.0));
	}
	else if (cp_rx == -1) {
		rcp = glm::rotate(rcp, glm::radians(-5.0f), glm::vec3(1.0, 0.0, 0.0));
	}

	if (cp_ry == 1) {
		rcp = glm::rotate(rcp, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (cp_ry == -1) {
		rcp = glm::rotate(rcp, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
	}

	if (cs_rx == 1) {
		rcs = glm::rotate(rcs, glm::radians(5.0f), glm::vec3(1.0, 0.0, 0.0));
	}
	else if (cs_rx == -1) {
		rcs = glm::rotate(rcs, glm::radians(-5.0f), glm::vec3(1.0, 0.0, 0.0));
	}

	if (cs_ry == 1) {
		rcs = glm::rotate(rcs, glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	else if (cs_ry == -1) {
		rcs = glm::rotate(rcs, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
	}

	if (is_to) {
		float min_x, min_y, min_x2, min_y2;
		min_x = to[cp_i][0];
		min_y = to[cp_i][1];

		min_x2 = to[cs_i][0];
		min_y2 = to[cs_i][1];

		cp = glm::translate(glm::mat4(1.0), glm::vec3(min_x, min_y * 1.5 / 3, 0.0));
		cs = glm::translate(glm::mat4(1.0), glm::vec3(min_x2, min_y2 * 1.5 / 3, 0.0));

		if (dr) {
			cp_i--; cs_i--;
		}
		else {
			cp_i++; cs_i++;
		}
	}
	if(f){
		if (n < 101) {
			cp = glm::translate(cp, glm::vec3((cube.temp_vertices[0].x - cube_x) / 100,
				(cube.temp_vertices[0].y - cube_y) / 100, 0.0));
			cs = glm::translate(cs, glm::vec3((cone.temp_vertices[9].x - cone_x) / 100,
				(cone.temp_vertices[9].y - cone_y) / 100, 0.0));
		}
		else if (n < 201) {
			cp = glm::translate(cp, glm::vec3(-(cube.temp_vertices[0].x - cube_x) / 100,
				-(cube.temp_vertices[0].y - cube_y) / 100, 0.0));
			cs = glm::translate(cs, glm::vec3(-(cone.temp_vertices[9].x - cone_x) / 100,
				-(cone.temp_vertices[9].y - cone_y) / 100, 0.0));
		}
		else {
			n = -1;
			f = false;
		}
		n++;
	}
	
	if (g) {
		if (m < 101) {
			cp = glm::translate(cp, glm::vec3((cone_x - cube_x) / 100,
				(cone_y - cube_y) / 100, 0.0));
			cs = glm::translate(cs, glm::vec3(-(cone_x - cube_x) / 100,
				-(cone_y - cube_y) / 100, 0.0));
		}
		else if (m < 201) {
			cp = glm::translate(cp, glm::vec3(-(cone_x - cube_x) / 100,
				-(cone_y - cube_y) / 100, 0.0));
			cs = glm::translate(cs, glm::vec3((cone_x - cube_x) / 100,
				(cone_y - cube_y) / 100, 0.0));
		}
		else {
			m = -1;
			g = false;
		}
		m++;
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