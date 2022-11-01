#include "objRead.h"
#define VAO_SIZE 13
#define VBO_SIZE 16

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* vertexsource, * fragmentsource; //--- 소스코드 저장 변수
GLuint vertexshader, fragmentshader; //--- 세이더 객체
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
GLuint vao[VAO_SIZE], vbo[VBO_SIZE];
objRead OriginSphere;
objRead subSphere;
objRead subSphere2;
objRead subSphere3;
objRead subsubSphere;
objRead subsubSphere2;
objRead subsubSphere3;
objRead line;
objRead line2;
objRead line3;
objRead subline;
objRead subline2;
objRead subline3;
int object = OriginSphere.loadObj_normalize_center("OriginSphere.obj");
int object2 = subSphere.loadObj_normalize_center("subSphere.obj");
int object3 = subSphere2.loadObj_normalize_center("subSphere2.obj");
int object4 = subSphere3.loadObj_normalize_center("subSphere3.obj");
int object5 = subsubSphere.loadObj_normalize_center("subsubSphere.obj");
int object6 = subsubSphere2.loadObj_normalize_center("subsubSphere2.obj");
int object7 = subsubSphere3.loadObj_normalize_center("subsubSphere3.obj");
int object8 = line.loadObj_normalize_center("line.obj");
int object9 = line2.loadObj_normalize_center("line2.obj");
int object10 = line3.loadObj_normalize_center("line3.obj");
int object11 = subline.loadObj_normalize_center("subline.obj");
int object12 = subline2.loadObj_normalize_center("subline2.obj");
int object13 = subline3.loadObj_normalize_center("subline3.obj");
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
unsigned int projectionLocation;
unsigned int viewLocation;

vector<glm::vec3> origin_col(2880, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> sub_col(2880, glm::vec3(0.0, 1.0, 0.0));
vector<glm::vec3> line_col(100, glm::vec3(0.0, 0.0, 0.0));

//all translate
glm::mat4 T = glm::mat4(1.0); 
//sub sphere translate
glm::mat4 T_sub;
glm::mat4 T_sub2;
glm::mat4 T_sub3;
glm::mat4 T_subs;

//y rotate
glm::mat4 RY = glm::mat4(1.0); 
//origin line rotate
glm::mat4 R_line_x = glm::rotate(glm::mat4(1.0), glm::radians(5.0f), glm::vec3(1.0, 0.0, 0.0));
glm::mat4 R_line_y = glm::rotate(glm::mat4(1.0), glm::radians(5.0f), glm::vec3(0.0, 1.0, 0.0));
glm::mat4 R_line_y2 = glm::rotate(glm::mat4(1.0), glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
glm::mat4 R_line_z = glm::rotate(glm::mat4(1.0), glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
glm::mat4 R_line_z2 = glm::rotate(glm::mat4(1.0), glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));


bool ry = false, rY = false;
float r = 1.8, angle = 0.1, subr = 1.1, subangle = 0.1;
int s1 = 0, s2 = 100, s3 = 200, ss = 0;
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example17");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	
	glewInit();

	InitShader();
	InitBuffer();
	projection = glm::ortho(-2.5f, 2.5f, -2.5f, 2.5f, -2.5f, 2.5f);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timer, 1);
	glutTimerFunc(100, subTimer, 1);
	glutTimerFunc(100, subTimer2, 1);
	glutTimerFunc(100, subTimer3, 1);
	glutMainLoop();
}
void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void InitBuffer()
{
	glGenVertexArrays(VAO_SIZE, vao);
	glGenBuffers(VBO_SIZE, vbo);

	//origin
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, OriginSphere.outvertex.size() * sizeof(glm::vec3), &OriginSphere.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, OriginSphere.outvertex.size() * sizeof(glm::vec3), &origin_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//sub
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, subSphere.outvertex.size() * sizeof(glm::vec3), &subSphere.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, subSphere.outvertex.size() * sizeof(glm::vec3), &sub_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, subSphere2.outvertex.size() * sizeof(glm::vec3), &subSphere2.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, subSphere.outvertex.size() * sizeof(glm::vec3), &sub_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, subSphere3.outvertex.size() * sizeof(glm::vec3), &subSphere3.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, subSphere.outvertex.size() * sizeof(glm::vec3), &sub_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//subsub
	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, subsubSphere.outvertex.size() * sizeof(glm::vec3), &subsubSphere.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, subsubSphere2.outvertex.size() * sizeof(glm::vec3), &subsubSphere2.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, subsubSphere3.outvertex.size() * sizeof(glm::vec3), &subsubSphere3.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	//line
	glBindVertexArray(vao[7]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, line.outvertex.size() * sizeof(glm::vec3), &line.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, line_col.size() * sizeof(glm::vec3), &line_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[8]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, line2.outvertex.size() * sizeof(glm::vec3), &line2.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, line_col.size() * sizeof(glm::vec3), &line_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[9]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, line3.outvertex.size() * sizeof(glm::vec3), &line3.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, line_col.size() * sizeof(glm::vec3), &line_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//subsub line
	glBindVertexArray(vao[10]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, subline.outvertex.size() * sizeof(glm::vec3), &subline.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, line_col.size() * sizeof(glm::vec3), &line_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[11]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, subline2.outvertex.size() * sizeof(glm::vec3), &subline2.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, line_col.size() * sizeof(glm::vec3), &line_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao[12]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	glBufferData(GL_ARRAY_BUFFER, subline3.outvertex.size() * sizeof(glm::vec3), &subline3.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, line_col.size() * sizeof(glm::vec3), &line_col[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}
void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}
GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);
	glEnable(GL_DEPTH_TEST);

	viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	
	projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));

	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T));
	//make Origin Sphere
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, object);
	
	//make subSphere
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY* T * T_sub));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, object2);
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_sub2));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, object3);
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_sub3));
	glBindVertexArray(vao[3]);
	glDrawArrays(GL_TRIANGLES, 0, object3);
	
	//make subsubSphere
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_subs * T_sub));
	glBindVertexArray(vao[4]);
	glDrawArrays(GL_TRIANGLES, 0, object4);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_subs * T_sub2));
	glBindVertexArray(vao[5]);
	glDrawArrays(GL_TRIANGLES, 0, object5);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_subs * T_sub3));
	glBindVertexArray(vao[6]);
	glDrawArrays(GL_TRIANGLES, 0, object6);
	
	//make origin Line
	glBindVertexArray(vao[7]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T));
	glDrawArrays(GL_LINES, 0, object8);
	
	glBindVertexArray(vao[8]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T));
	glDrawArrays(GL_LINES, 0, object8);
	
	glBindVertexArray(vao[9]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T));
	glDrawArrays(GL_LINES, 0, object8);

	//make sub Line
	glBindVertexArray(vao[10]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_sub));
	glDrawArrays(GL_LINES, 0, object8);

	glBindVertexArray(vao[11]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_sub2));
	glDrawArrays(GL_LINES, 0, object8);

	glBindVertexArray(vao[12]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(RY * T * T_sub3));
	glDrawArrays(GL_LINES, 0, object8);

	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		// 직교/원근 투영
	case 'p':
		projection = glm::ortho(-2.5f, 2.5f, -2.5f, 2.5f, -2.5f, 2.5f);
		break;
	case 'P':
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -7.0));
		break;

		// 와이어/솔리드 모델
	case 'm':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'M':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

		//좌우상하 이동
	case'w': T = glm::translate(T, glm::vec3(0.0, 0.1, 0.0));
		break;
	case'a': T = glm::translate(T, glm::vec3(-0.1, 0.0, 0.0));
		break;
	case's': T = glm::translate(T, glm::vec3(0.0, -0.1, 0.0));
		break;
	case'd': T = glm::translate(T, glm::vec3(0.1, 0.0, 0.0));
		break;

		//앞뒤이동
	case 'z': T = glm::translate(T, glm::vec3(0.0, 0.0, 0.15));
		break;
	case 'x':T = glm::translate(T, glm::vec3(0.0, 0.0, -0.15));
		break;

		//y축 양/음회전
	case 'y':
		if (!ry) {
			ry = true;
			rY = false;
		}
		else ry = false;
		break;
	case 'Y':
		if (!rY) {
			rY = true;
			ry = false;
		}
		else rY = false;
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {
	T_subs = glm::translate(glm::mat4(1.0),
		glm::vec3(subline.temp_vertices[ss].x, subline.temp_vertices[ss].y, subline.temp_vertices[ss].z));
	ss++;
	if (ss == subline.temp_vertices.size())
		ss = 0;
	if (ry)
		RY = glm::rotate(RY, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));
	if(rY)
		RY = glm::rotate(RY, glm::radians(1.0f), glm::vec3(0.0, 1.0, 0.0));

	glutTimerFunc(10, Timer, 1);
	glutPostRedisplay();
}
void subTimer(int value) {
	T_sub = glm::translate(glm::mat4(1.0), 
		glm::vec3(line.temp_vertices[s1].x, line.temp_vertices[s1].y, line.temp_vertices[s1].z));
	s1++;
	if (s1 == line.temp_vertices.size())
		s1 = 0;
	glutTimerFunc(50, subTimer, 1);
	glutPostRedisplay();
}
void subTimer2(int value) {
	T_sub2 = glm::translate(glm::mat4(1.0),
		glm::vec3(line2.temp_vertices[s1].x, line2.temp_vertices[s1].y, line2.temp_vertices[s1].z));
	s2++;
	if (s2 == line2.temp_vertices.size())
		s2 = 0;
	glutTimerFunc(100, subTimer2, 1);
	glutPostRedisplay();
}
void subTimer3(int value) {
	T_sub3 = glm::translate(glm::mat4(1.0),
		glm::vec3(line3.temp_vertices[s1].x, line3.temp_vertices[s1].y, line3.temp_vertices[s1].z));
	s3++;
	if (s3 == line.temp_vertices.size())
		s3 = 0;
	glutTimerFunc(150, subTimer3, 1);
	glutPostRedisplay();
}
//opengl 좌표로 변환
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