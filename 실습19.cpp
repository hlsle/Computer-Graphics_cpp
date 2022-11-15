#include "objRead.h"
#define VAO_SIZE 4
#define VBO_SIZE 8

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
void subTimer(int value);
void subTimer2(int value);
void subTimer3(int value);
float x_init, y_init, ox, oy;
int w, h;
GLuint vao[VAO_SIZE], vbo[VBO_SIZE];
glm::vec3 cameraPos = glm::vec3(0.0f, 0.4f, 1.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 

glm::vec3 cameraPos2 = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDirection2 = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp2 = glm::vec3(0.0f, 0.0f, -1.0f);

glm::vec3 cameraPos3 = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraDirection3 = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp3 = glm::vec3(0.0f, 1.0f, 0.0f);

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
float cam_angle = glm::radians(0.1f), cam_angle2 = glm::radians(0.1f);
float cam_directangle = glm::radians(180.0f), cam_directangle2, cam_directangle3;
float r = 1.0, r2 = 0.0;
void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("Example19");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	w = 1000;
	h = 700;
	glewInit();
	for (int i = 0; i < 8; i++) {
		v.push_back(make_pair(0, false));
	}
	InitShader();
	InitBuffer();
	//���� ����

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
	// first viewport
	glViewport(0, h/4, w/2, h/2);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);
	glEnable(GL_DEPTH_TEST); 
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -4.0));

	viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- ���� ��ȯ �� ����
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
	
	// second viewport
	glViewport(w/2, h/2, w / 2, h / 2);
	view = glm::lookAt(cameraPos2, cameraDirection2, cameraUp2);
	projection = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -10.0f, 10.0f);
	viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));

	modelLocation = glGetUniformLocation(s_program, "trans");
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

	// third viewport
	glViewport(w/2, 0, w / 2, h / 2);
	view = glm::lookAt(cameraPos3, cameraDirection3, cameraUp3);
	projection = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -10.0f, 10.0f);
	viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));
	
	modelLocation = glGetUniformLocation(s_program, "trans");
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

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
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
		// �������� ��ȯ �� scale
		n = glm::normalize(cameraDirection - cameraPos);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos += n;
		cameraDirection += n;

		n = glm::normalize(cameraUp2);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos2 += n;
		cameraDirection2 += n;

		n = glm::normalize(cameraDirection3 - cameraPos3);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos3 += n;
		cameraDirection3 += n;
		break;
	case 'Z':
		n = glm::normalize(cameraDirection - cameraPos);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos -= n;
		cameraDirection -= n;

		n = glm::normalize(cameraUp2);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos2 -= n;
		cameraDirection2 -= n;

		n = glm::normalize(cameraDirection3 - cameraPos3);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos3 -= n;
		cameraDirection3 -= n;
		break;
	case 'x':
		// ī�޶� �ٶ󺸴� ����� ������ �����ϸ� ������ ����
		n = glm::normalize(glm::cross(cameraUp, cameraDirection - cameraPos));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos += n;
		cameraDirection += n;

		n = glm::normalize(glm::cross(cameraUp2, cameraDirection2 - cameraPos2));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos2 += n;
		cameraDirection2 += n;

		n = glm::normalize(glm::cross(cameraUp3, cameraDirection3 - cameraPos3));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos3 += n;
		cameraDirection3 += n;
		break;
	case 'X':
		n = glm::normalize(glm::cross(cameraUp, cameraDirection - cameraPos));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos -= n;
		cameraDirection -= n;

		n = glm::normalize(glm::cross(cameraUp2, cameraDirection2 - cameraPos2));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos2 -= n;
		cameraDirection2 -= n;

		n = glm::normalize(glm::cross(cameraUp3, cameraDirection3 - cameraPos3));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos3 -= n;
		cameraDirection3 -= n;
		break;

		//camera y rotate
	case 'y':
		// ī�޶� ���� ���⿡�� �ݽð� �������� ���ϴ� 360���� ��
		if (cam_directangle > 3.1f)
			cam_directangle = glm::radians(360.0f) -
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));
		// ó�� ī�޶� ��ġ�̴� 360���� �E �ʿ� ����
		if (cam_directangle > 6.13f)
			cam_directangle =
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));

		cam_directangle += 0.1;
		cameraDirection.x = sin(cam_directangle) + cameraPos.x;
		cameraDirection.z = cos(cam_directangle) + cameraPos.z;
		
		cameraUp2.x = sin(cam_directangle);
		cameraUp2.z = cos(cam_directangle);

		cameraDirection3.x = sin(cam_directangle) + cameraPos3.x;
		cameraDirection3.z = cos(cam_directangle) + cameraPos3.z;
		break;
	case 'Y':
		if (cam_directangle > 3.1f)
			cam_directangle = glm::radians(360.0f) -
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));
		if (cam_directangle > 6.13f)
			cam_directangle =
			acos((cameraDirection.z - cameraPos.z) /
				sqrt(pow(cameraDirection.x - cameraPos.x, 2) + pow(cameraDirection.z - cameraPos.z, 2)));
		cam_directangle -= 0.1;
		cameraDirection.x = sin(cam_directangle) + cameraPos.x;
		cameraDirection.z = cos(cam_directangle) + cameraPos.z;

		cameraUp2.x = sin(cam_directangle);
		cameraUp2.z = cos(cam_directangle);

		cameraDirection3.x = sin(cam_directangle) + cameraPos3.x;
		cameraDirection3.z = cos(cam_directangle) + cameraPos3.z;
		break;

		//camera revolution y
	case 'r':
		// ī�޶� ��ġ�� ���� r�� ����
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1) {
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
			r2 = sqrt(pow(cameraPos2.x, 2) + pow(cameraPos2.z, 2));
		}
		// ī�޶� x��ġ�� ���� ���� ����
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);

		if (cameraPos2.x < 0)
			cam_angle2 = glm::radians(360.0f) - acos(cameraPos2.z / r2);
		else if (cameraPos2.x > 0)
			cam_angle2 = acos(cameraPos2.z / r2);

		cam_angle += 0.1f;
		cam_angle2 += 0.1f;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;

		// ó�� ��ġ�̸�
		if (cameraPos2.x == 0.0f && cameraPos2.z == 0.0f) {
			cameraUp2.x = -sin(cam_angle);
			cameraUp2.z = -cos(cam_angle);
		}
		else {
			cameraPos2.x = sin(cam_angle2) * r2;
			cameraPos2.z = cos(cam_angle2) * r2;
			cameraDirection2 = cameraPos2;
			cameraDirection2.y = 0.0;
		}

		cameraPos3.x = sin(cam_angle) * r;
		cameraPos3.z = cos(cam_angle) * r;
		break;
	case 'R':
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1) {
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
			r2 = sqrt(pow(cameraPos2.x, 2) + pow(cameraPos2.z, 2));
		}
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);

		if (cameraPos2.x < 0)
			cam_angle2 = glm::radians(360.0f) - acos(cameraPos2.z / r2);
		else if (cameraPos2.x > 0)
			cam_angle2 = acos(cameraPos2.z / r2);

		cam_angle -= 0.1f;
		cam_angle2 -= 0.1f;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;

		if (cameraPos2.x == 0.0f && cameraPos2.z == 0.0f) {
			cameraUp2.x = -sin(cam_angle);
			cameraUp2.z = -cos(cam_angle);
		}
		else {
			cameraPos2.x = sin(cam_angle2) * r2;
			cameraPos2.z = cos(cam_angle2) * r2;
			cameraDirection2 = cameraPos2;
			cameraDirection2.y = 0.0;
		}
		cameraPos3.x = sin(cam_angle) * r;
		cameraPos3.z = cos(cam_angle) * r;
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
		cameraPos2 = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraDirection2 = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraUp2 = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraPos3 = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraDirection3 = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraUp3 = glm::vec3(0.0f, 1.0f, 0.0f);
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
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1) {
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
			r2 = sqrt(pow(cameraPos2.x, 2) + pow(cameraPos2.z, 2));
		}
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);
		cam_angle += 0.1;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;

		if (cameraPos2.x == 0.0f && cameraPos2.z == 0.0f) {
			cameraUp2.x = -sin(cam_angle);
			cameraUp2.z = -cos(cam_angle);
		}
		else {
			cameraPos2.x = sin(cam_angle) * r2;
			cameraPos2.z = cos(cam_angle) * r2;
		}

		cameraPos3.x = sin(cam_angle) * r;
		cameraPos3.z = cos(cam_angle) * r;
	}
	if (v[7].second) {
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1) {
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
			r2 = sqrt(pow(cameraPos2.x, 2) + pow(cameraPos2.z, 2));
		}
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);
		cam_angle -= 0.1;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;

		if (cameraPos2.x == 0.0f && cameraPos2.z == 0.0f) {
			cameraUp2.x = -sin(cam_angle);
			cameraUp2.z = -cos(cam_angle);
		}
		else {
			cameraPos2.x = sin(cam_angle) * r2;
			cameraPos2.z = cos(cam_angle) * r2;
		}

		cameraPos3.x = sin(cam_angle) * r;
		cameraPos3.z = cos(cam_angle) * r;
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