#include "objRead.h"
#define VAO_SIZE 7
#define VBO_SIZE 14
#define SPEED 0.01
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
void Timer2(int value);
float x_init, y_init, ox, oy;
GLuint vao[VAO_SIZE], vbo[VBO_SIZE];
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
unsigned int projectionLocation;
unsigned int viewLocation;
objRead stage;
objRead head;
objRead body;
objRead arm;
objRead leg;
objRead nose;
objRead box;
int object = stage.loadObj_normalize_center("stage.obj");
int object2 = head.loadObj_normalize_center("head.obj");
int object3 = body.loadObj_normalize_center("body.obj");
int object4 = arm.loadObj_normalize_center("arm.obj");
int object5 = leg.loadObj_normalize_center("leg.obj");
int object6 = nose.loadObj_normalize_center("nose.obj");
int object7 = box.loadObj_normalize_center("box.obj");

float stage_col[36][3] = {
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},

	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},

	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},

	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},

	{0.0, 1.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 1.0, 1.0},
	{0.0, 1.0, 1.0},

	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0}
};
float head_col[36][3] = {
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5}
};
float body_col[36][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
};
float arm_col[36][3] = {
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0},
	{1.0, 0.0, 1.0}
};
float leg_col[36][3] = {
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0},
	{0.3, 0.4, 1.0}
};
float nose_col[36][3] = {
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0},
	{1.0, 1.0, 0.0}
};
float box_col[3][3];

// 중심을 원점에 맞춤
glm::mat4 T_stage = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -stage.scaleY / 2, 0.0));

// stage 앞면을 원점에 맞추고 회전, 원래 자리
glm::mat4 T_stage_front = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -stage.scaleZ/2));
glm::mat4 T_stage_frontR = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, stage.scaleZ / 2));
glm::mat4 T_robot = glm::mat4(1.0);
glm::mat4 T_head = glm::translate(glm::mat4(1.0), glm::vec3(0.0, body.scaleY / 2  - leg.scaleY / 4, 0.0));
glm::mat4 T_body = glm::translate(glm::mat4(1.0), glm::vec3(0.0, leg.scaleY, 0.0));
glm::mat4 T_arm = glm::translate(glm::mat4(1.0), glm::vec3(-body.scaleX*0.57, -arm.scaleY * 0.71, 0.0));
glm::mat4 T_arm2 = glm::translate(glm::mat4(1.0), glm::vec3(body.scaleX*0.57, -arm.scaleY * 0.71, 0.0));
glm::mat4 T_leg = glm::translate(glm::mat4(1.0), glm::vec3(-body.scaleX / 4, 0.0, 0.0));
glm::mat4 T_leg2 = glm::translate(glm::mat4(1.0), glm::vec3(body.scaleX / 4, 0.0, 0.0));
glm::mat4 T_nose = glm::translate(glm::mat4(1.0), glm::vec3(0.0, body.scaleY / 2 - leg.scaleY / 2, head.scaleZ*0.6));
glm::mat4 T_jump = glm::mat4(1.0);
glm::mat4 T_arm_walk = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -arm.scaleY, 0.0));
glm::mat4 T_arm_walk2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, arm.scaleY, 0.0));
glm::mat4 T_leg_walk = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -leg.scaleY, 0.0));
glm::mat4 T_leg_walk2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, leg.scaleY, 0.0));
glm::mat4 T_box = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -stage.scaleY * 0.475, 0.5));

glm::mat4 R_stage_front = glm::mat4(1.0);
glm::mat4 R_robot = glm::mat4(1.0);
glm::mat4 R_arm = glm::mat4(1.0);
glm::mat4 R_arm2 = glm::mat4(1.0);
glm::mat4 R_leg = glm::mat4(1.0);
glm::mat4 R_leg2 = glm::mat4(1.0);

glm::vec3 n;

float cam_angle = glm::radians(0.1f);
float r = 1.0;
bool stage_open = false, jump = false;
int stage_state = 0, stage_num = 0, j = 0;
int walk = -1;
float pos_x = body.aveX, pos_y = -stage.scaleY / 2, pos_z = body.aveZ;
float box_posx = box.scaleX / 2, box_posy = box.scaleY - stage.scaleY / 2,
box_posz = -box.scaleZ + 0.5f;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Example20");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;

	glewInit();

	InitShader();
	InitBuffer();
	//원근 투영
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(100, Timer, 1);
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

	//plane
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, stage.outvertex.size() * sizeof(glm::vec3), &stage.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stage_col), stage_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//head
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, head.outvertex.size() * sizeof(glm::vec3), &head.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(head_col), head_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//body
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, body.outvertex.size() * sizeof(glm::vec3), &body.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(body_col), body_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//arm
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, arm.outvertex.size() * sizeof(glm::vec3), &arm.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arm_col), arm_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//leg
	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, leg.outvertex.size() * sizeof(glm::vec3), &leg.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leg_col), leg_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//nose
	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, nose.outvertex.size() * sizeof(glm::vec3), &nose.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nose_col), nose_col, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//box
	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	glBufferData(GL_ARRAY_BUFFER, box.outvertex.size() * sizeof(glm::vec3), &box.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_col), box_col, GL_STATIC_DRAW);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	//cout << pos_y << endl;
	viewLocation = glGetUniformLocation(s_program, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));

	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_stage));
	
	// stage
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glDrawArrays(GL_TRIANGLES, 18, 18);
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_stage * T_stage_frontR * R_stage_front * T_stage_front));
	glDrawArrays(GL_TRIANGLES, 12, 6);

	if ((pos_x > box_posx || pos_x < -box_posx || pos_z < box_posz || pos_z > box_posz + box.scaleZ)
		&& pos_y != -stage.scaleY / 2 && !jump) {
		pos_y = -stage.scaleY / 2;
		T_jump = glm::mat4(1.0);
	}
	// head
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_jump * T_robot * T_head * R_robot));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, object2);
	
	//body
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_jump * T_robot * T_body * R_robot));
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_TRIANGLES, 0, object3);
	
	//arm
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_jump * T_robot * R_robot * T_arm * T_arm_walk2 * R_arm * T_arm_walk));
	glBindVertexArray(vao[3]);
	glDrawArrays(GL_TRIANGLES, 0, object4);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_jump * T_robot * R_robot * T_arm2 * T_arm_walk2 * R_arm2 * T_arm_walk));
	glDrawArrays(GL_TRIANGLES, 0, object4);

	//leg
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_jump * T_robot * R_robot * T_stage * T_leg * T_leg_walk2 * R_leg * T_leg_walk));
	glBindVertexArray(vao[4]);
	glDrawArrays(GL_TRIANGLES, 0, object5);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_jump * T_robot * R_robot * T_stage * T_leg2 * T_leg_walk2 * R_leg2 * T_leg_walk));
	glDrawArrays(GL_TRIANGLES, 0, object5);

	//nose
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_jump * T_robot * R_robot * T_nose));
	glBindVertexArray(vao[5]);
	glDrawArrays(GL_TRIANGLES, 0, object6);

	//box
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(T_box));
	glBindVertexArray(vao[6]);
	glDrawArrays(GL_TRIANGLES, 0, object7);

	glutSwapBuffers(); //--- 화면에 출력하기
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		// stage open/close
	case 'o':
		if (stage_open) {
			stage_open = false;
			stage_state = 0;
			break;
		}
		if (stage_num != 9) {
			stage_open = true;
			stage_state = 1;
		}
		break;
	case 'O':
		if (stage_open) {
			stage_open = false;
			stage_state = 0;
			break;
		}
		if (stage_num != 0) {
			stage_open = true;
			stage_state = 2;
		}
		break;

		// move robot
	case 'w':
		pos_z -= SPEED;
		walk++;
		if (walk % 12 < 3) {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else if (walk % 12 < 9) {
			R_arm = glm::rotate(R_arm, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		if ((pos_x <= box_posx && pos_x >= -box_posx) && pos_y == -stage.scaleY / 2 &&
			(pos_z >= box_posz && pos_z <= box_posz + box.scaleZ)) {
			pos_z += SPEED;
			break;
		}
		T_robot = glm::translate(T_robot, glm::vec3(0.0, 0.0, -SPEED));
		R_robot = glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		
		if (pos_z < -0.64) {
			T_robot = glm::translate(T_robot, glm::vec3(0.0, 0.0, 1.3));
			pos_z = 0.65;
		}
		break;
	case 'a':
		pos_x -= SPEED;
		walk++;
		if (walk % 12 < 3) {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else if (walk % 12 < 9) {
			R_arm = glm::rotate(R_arm, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		if ((pos_x <= box_posx && pos_x >= -box_posx) && pos_y == -stage.scaleY / 2 &&
			(pos_z >= box_posz && pos_z <= box_posz + box.scaleZ)) {
			pos_x += SPEED;
			break;
		}
		T_robot = glm::translate(T_robot, glm::vec3(-SPEED, 0.0, 0.0));
		R_robot = glm::rotate(glm::mat4(1.0), glm::radians(270.0f), glm::vec3(0.0, 1.0, 0.0));
	
		if (pos_x < -0.64) {
			T_robot = glm::translate(T_robot, glm::vec3(1.3, 0.0, 0.0));
			pos_x = 0.65;
		}
		break;
	case 's':
		pos_z += SPEED;
		walk++;
		if (walk % 12 < 3) {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else if (walk % 12 < 9) {
			R_arm = glm::rotate(R_arm, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		if ((pos_x <= box_posx && pos_x >= -box_posx) && pos_y == -stage.scaleY / 2 &&
			(pos_z >= box_posz && pos_z <= box_posz + box.scaleZ)) {
			pos_z -= SPEED;
			break;
		}
		T_robot = glm::translate(T_robot, glm::vec3(0.0, 0.0, SPEED));
		R_robot = glm::mat4(1.0);
	
		if (pos_z > 0.64) {
			T_robot = glm::translate(T_robot, glm::vec3(0.0, 0.0, -1.4));
			pos_z = -0.65;
		}
		break;
	case 'd':
		pos_x += SPEED;
		walk++;
		if (walk % 12 < 3) {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else if (walk % 12 < 9) {
			R_arm = glm::rotate(R_arm, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		else {
			R_arm = glm::rotate(R_arm, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg2 = glm::rotate(R_leg2, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_arm2 = glm::rotate(R_arm2, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
			R_leg = glm::rotate(R_leg, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
		}
		if ((pos_x <= box_posx && pos_x >= -box_posx) && pos_y == -stage.scaleY / 2 &&
			(pos_z >= box_posz && pos_z <= box_posz + box.scaleZ)) {
			pos_x -= SPEED;
			break;
		}
		T_robot = glm::translate(T_robot, glm::vec3(SPEED, 0.0, 0.0));
		R_robot = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));

		if (pos_x > 0.64) {
			T_robot = glm::translate(T_robot, glm::vec3(-1.3, 0.0, 0.0));
			pos_x = -0.65;
		}
		break;

		// jump
	case 'j':
		jump = true;
		break;

		// reset
	case 'i':
		T_stage = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -stage.scaleY / 2, 0.0));
		T_stage_front = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -stage.scaleZ / 2));
		T_stage_frontR = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, stage.scaleZ / 2));
		T_robot = glm::mat4(1.0);
		T_head = glm::translate(glm::mat4(1.0), glm::vec3(0.0, body.scaleY / 2 - leg.scaleY / 4, 0.0));
		T_body = glm::translate(glm::mat4(1.0), glm::vec3(0.0, leg.scaleY, 0.0));
		T_arm = glm::translate(glm::mat4(1.0), glm::vec3(-body.scaleX * 0.57, -arm.scaleY * 0.71, 0.0));
		T_arm2 = glm::translate(glm::mat4(1.0), glm::vec3(body.scaleX * 0.57, -arm.scaleY * 0.71, 0.0));
		T_leg = glm::translate(glm::mat4(1.0), glm::vec3(-body.scaleX / 4, 0.0, 0.0));
		T_leg2 = glm::translate(glm::mat4(1.0), glm::vec3(body.scaleX / 4, 0.0, 0.0));
		T_nose = glm::translate(glm::mat4(1.0), glm::vec3(0.0, body.scaleY / 2 - leg.scaleY / 2, head.scaleZ * 0.6));
		T_jump = glm::mat4(1.0);
		T_arm_walk = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -arm.scaleY, 0.0));
		T_arm_walk2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, arm.scaleY, 0.0));
		T_leg_walk = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -leg.scaleY, 0.0));
		T_leg_walk2 = glm::translate(glm::mat4(1.0), glm::vec3(0.0, leg.scaleY, 0.0));
		R_stage_front = glm::mat4(1.0);
		R_robot = glm::mat4(1.0);
		R_arm = glm::mat4(1.0);
		R_arm2 = glm::mat4(1.0);
		R_leg = glm::mat4(1.0);
		R_leg2 = glm::mat4(1.0);
		cam_angle = glm::radians(0.1f);
		r = 1.0;
		stage_open = false; jump = false;
		stage_state = 0; stage_num = 0; j = 0;
		walk = -1;
		pos_x = body.aveX; pos_z = body.aveZ;
		break;

		// move camera
	case 'z':
		n = glm::normalize(cameraDirection - cameraPos);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos += n;
		cameraDirection += n;
		break;
	case 'Z':
		n = glm::normalize(cameraDirection - cameraPos);
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos -= n;
		cameraDirection -= n;
		break;
	case 'x':
		//카메라 업벡터와 보는방향 벡터의 외적은 오른쪽 벡터
		n = glm::normalize(glm::cross(cameraUp, cameraDirection - cameraPos));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos += n;
		cameraDirection += n;
		break;
	case 'X':
		n = glm::normalize(glm::cross(cameraUp, cameraDirection - cameraPos));
		n.x *= 0.1; n.y *= 0.1; n.z *= 0.1;
		cameraPos -= n;
		cameraDirection -= n;
		break;
	
		// rotate camera y
	case 'y':
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1)
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);

		cam_angle += 0.1;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;
		break;
	case 'Y':
		if (sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2)) != 1)
			r = sqrt(pow(cameraPos.x, 2) + pow(cameraPos.z, 2));
		if (cameraPos.x < 0)
			cam_angle = glm::radians(360.0f) - acos(cameraPos.z / r);
		else if (cameraPos.x > 0)
			cam_angle = acos(cameraPos.z / r);
		cam_angle -= 0.1;
		cameraPos.x = sin(cam_angle) * r;
		cameraPos.z = cos(cam_angle) * r;
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {
	
	if (stage_open && stage_state == 1) {
		R_stage_front = glm::rotate(R_stage_front, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
		stage_num++;
		if (stage_num == 9 || stage_num == 0)
			stage_open = false;
	}
	else if (stage_open && stage_state == 2) {
		R_stage_front = glm::rotate(R_stage_front, glm::radians(-10.0f), glm::vec3(1.0, 0.0, 0.0));
		stage_num--;
		if (stage_num == 0)
			stage_open = false;
	}

	if (jump) {
		j++;
		if (j > 4) {
			T_jump = glm::translate(T_jump, glm::vec3(0.0, -0.04, 0.0));
			pos_y -= 0.04f;
			if ((pos_x <= box_posx && pos_x >= -box_posx) && pos_y <= box_posy &&
				(pos_z >= box_posz && pos_z <= box_posz + box.scaleZ)) {
				jump = false; j = 0;
			}
			if (j == 8) {
				jump = false; j = 0;
			}
		}
		else {
			T_jump = glm::translate(T_jump, glm::vec3(0.0, 0.04, 0.0));
			pos_y += 0.04f;
		}
	}
	glutTimerFunc(100, Timer, 1);
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