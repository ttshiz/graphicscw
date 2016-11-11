
#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "mat.h"
#include <limits>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib>

// remember to prototype
void generateGinger(void);
void initGPUBuffers(void);
void shaderSetup(void);
void displayGinger(void);
void keyboard(unsigned char key, int x, int y);
void myDisplay(void);
void changeColor(void);
void drawPolylineFile(char * filename);
void reshape(int W, int H);
void reshape2(int m, int n, int W, int H);
void myDisplayT(void);

typedef vec2 point2;

using namespace std;

const int NumGinger = 1000;
const int NumFern = 1000;
GLuint program;
point2 gingerpoints[NumGinger];
point2 fernpoints[NumFern];

void generateGinger(void){
	int X = 115;
	int Y = 121;
	int M = 40;
	int L = 3;
	gingerpoints[0] = point2(X, Y);
	for(int n = 1; n < NumGinger; n++) {
		int newX = M*(1 + 2 * L) - Y + abs(X - L*M);
		int newY = X;
		gingerpoints[n] = point2(newX, newY);
	}
}

void generateFern(void) {
	float X = 0;
	float Y = 0;
	for (int i = 0; i < NumFern; i++) {
		int random_func = (rand() % (int)(3 + 1));
		if (random_func == 0) {
			int X = 0.0*X + 0.0*Y + 0;
			int Y = 0.0*X + 0.16*Y + 0.0;
			fernpoints[i] = point2(X, Y);
		}
		else if (random_func == 1) {
			int X = 0.20*X + -0.26*Y + 0.0;
			int Y = 0.23*X + 0.22*Y + 1.6;
			fernpoints[i] = point2(X, Y);
		}
		else if (random_func == 2) {
			int X = -0.15*X + 0.28*Y + 0.0;
			int Y = 0.26*X + 0.24*Y + 0.44;
			fernpoints[i] = point2(X, Y);
		}
		else if (random_func == 3) {
			int X = 0.85*X + 0.04*Y + 0.0;
			int Y = -0.04*X + 0.85*Y + 1.6;
			fernpoints[i] = point2(X, Y);
		}
	}
}

void displayGinger(void)
{
	// All drawing happens in display function
	glClear(GL_COLOR_BUFFER_BIT);                // clear window
	glBufferData(GL_ARRAY_BUFFER, sizeof(point2)*NumGinger, &gingerpoints[0], GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, NumGinger);    // draw the points
	glFlush();										// force output to graphics hardware
}

void displayFern(void)
{
	// All drawing happens in display function
	glClear(GL_COLOR_BUFFER_BIT);                // clear window
	glBufferData(GL_ARRAY_BUFFER, sizeof(point2)*NumFern, &fernpoints[0], GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, NumGinger);    // draw the points
	glFlush();										// force output to graphics hardware
}

void initGPUBuffers(void)
{
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint birdbuffer;
	glGenBuffers(1, &birdbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, birdbuffer);
}

void shaderSetup(void)
{
	// Load shaders and use the resulting shader program

	program = InitShader("vshader1.glsl", "fshader1.glsl");

	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0);        // sets white as color used to clear screen
}

void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler
	GLint ColorLoc = glGetUniformLocation(program, "newColor");
	vec4 nColor = vec4(1.0, 0.0, 0.0, 1.0);
	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case 0160:			// 0160 is p key octal value
		cout << "Do p action" << std::endl;
		glUniform4fv(ColorLoc, 1, nColor);
		glutDisplayFunc(myDisplay);
		glutPostRedisplay();
		break;
	case 0164:			// 0164 is t key octal value
		cout << "Do t action" << std::endl;
		glutDisplayFunc(myDisplayT);
		glutPostRedisplay();
		break;
	case 0145:			// 0145 is e key octal value
		cout << "Do e action" << std::endl;
		break;
	case 0147:			// 0147 is g key octal value
		cout << "Do g action" << std::endl;
		glutDisplayFunc(displayGinger);
		glutPostRedisplay();
		break;
	case 0146:			// 0146 is f key octal value
		cout << "Do f action" << std::endl;
		glutDisplayFunc(displayFern);
		glutPostRedisplay();
		break;
	case 0143:			// 0143 is c key octal value
		cout << "Do c action" << std::endl;
		changeColor();
		break;
	}
}

void changeColor(void) {
	GLint ColorLoc = glGetUniformLocation(program, "newColor");
	GLfloat * colorParams = new GLfloat[4];
	glGetUniformfv(program, ColorLoc, colorParams);
	GLfloat current_color[4];
	for (int i = 0; i < 4; i++) {
		current_color[i] = colorParams[i];
	}
	int color = 0;
	for (color = 0; color < 3; color++) {
		if (current_color[color] != 0) {
			break;
		}
	}
	if (color == 0) {
		vec4 nColor = vec4(0.0, 1.0, 0.0, 1.0);
		glUniform4fv(ColorLoc, 1, nColor);
	}
	else if (color == 1) {
		vec4 nColor = vec4(0.0, 0.0, 1.0, 1.0);
		glUniform4fv(ColorLoc, 1, nColor);
	}
	else {
		vec4 nColor = vec4(1.0, 0.0, 0.0, 1.0);
		glUniform4fv(ColorLoc, 1, nColor);
	}
	glutPostRedisplay();
}

void drawPolylineFile(char * filename) {
	std::ifstream file(filename);
	if (file.fail()) {
		cout << "Failed to open file!" << std::endl;
	}
	std::string str;
	while (true) {
		std::getline(file, str);
		if (str[0] == '*') {
			break;
		}
	}
	float left, top, right, bottom;
	file >> std::skipws >> left >> top >> right >> bottom;
	mat4 ortho = Ortho2D(left, right, bottom, top);
	GLint ProjLoc = glGetUniformLocation(program, "Proj");
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::vector<std::vector<point2>> polys;
	int numpolys;
	file >> numpolys;
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for (int p = 0; p < numpolys; p++) {
		int numPoints;
		file >> numPoints;
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::vector<point2> thispoly;
		int i;
		for (i = 0; i < numPoints; i++) {
			float x, y;
			file >> x >> y;
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			thispoly.push_back(point2(x, y));
		}
		polys.push_back(thispoly);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point2)*i, &thispoly[0], GL_STATIC_DRAW);
		glDrawArrays(GL_LINE_STRIP, 0, i);
	}
}

void reshape(int W, int H) {
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	float R = width / height;
	if (R > ((float)W / (float)H)) {
		glViewport(0, 0, W, (GLsizei)((float)W / (float)R));
	}
	else if (R < ((float)W / (float)H)) {
		glViewport(0, 0, (GLsizei)(H * R), H);
	}
	else {
		glViewport(0, 0, W, H);
	}
}

void reshape2(int m, int n, int W, int H) {
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	float R = width / height;
	if (R > ((float)W / (float)H)) {
		glViewport(m*W, H*n, W, (GLsizei)((float)W / (float)R));
	}
	else if (R < ((float)W / (float)H)) {
		glViewport(m*W, H*n, (GLsizei)(H * R), H);
	}
	else {
		glViewport(m*W, H*n, W, H);
	}
}

void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	float w, h;
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);

	w = width / 10;
	h = height / 6;

	char * images[] = {"dino.dat", "birdhead.dat", "dragon.dat", "house.dat", "knight.dat", "rex.dat"
		, "scene.dat", "usa.dat", "vinci.dat", "dragon.dat"};  // dino.dat
	for (int k = 0; k < 10; k++) {
		reshape2(k, 5, (int)w, (int)h);
		drawPolylineFile(images[k]);  
	}
	mat4 ortho = Ortho2D(0.0, 1.0, 0.0, 1.0);
	GLint ProjLoc = glGetUniformLocation(program, "Proj");
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);
	reshape((int)width, (int)(5*height/6));
	drawPolylineFile("vinci.dat");
	glFlush();
}

void myDisplayT(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	float w, h;
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);

	w = width / 10;
	h = height / 6;

	char * images[] = { "dino.dat", "birdhead.dat", "dragon.dat", "house.dat", "knight.dat", "rex.dat"
		, "scene.dat", "usa.dat", "vinci.dat", "dragon.dat" };  // dino.dat
	for (int k = 0; k < 10; k++) {
		//glViewport(k*w, h * 5, w, h);
		reshape2(k, 5, (int)w, (int)h);
		drawPolylineFile(images[k]);
	}

	float mw = width / 5;
	float mh = (height - h) / 5;
	for (int j = 0; j < 5; j++) {
		for (int l = 0; l < 5; l++) {
			reshape2(j, l, (int) mw, (int)mh);
			int random_file = (rand() % (int)(9 + 1));
			drawPolylineFile(images[random_file]);
		}
	}
	glFlush();
}


int main(int argc, char **argv)
{
	// main function: program starts here
	glutInit(&argc, argv);                       // intialize GLUT  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // single framebuffer, colors in RGB
	glutInitWindowSize(640, 480);                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100, 150);               // Top left corner at (100, 150)
	glutCreateWindow("App");            // Create Window

	glewInit();										// init glew

	generateGinger();                           // Call function that generates points to draw
	initGPUBuffers();							   // Create GPU buffers
	shaderSetup();                                // Connect this .cpp file to shader file

	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	mat4 ortho = Ortho2D(0, width, 0, height);
	GLint ProjLoc = glGetUniformLocation(program, "Proj");
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

	vec4 startColor = vec4(1.0, 0.0, 0.0, 1.0);
	GLint ColorLoc = glGetUniformLocation(program, "newColor");
	glUniform4fv(ColorLoc, 1, startColor);

	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	//glutDisplayFunc( display );                    // Register display callback function
	glutKeyboardFunc(keyboard);                  // Register keyboard callback function

												 // Can add minimalist menus here
												 // add mouse handler
												 // add resize window functionality (should probably try to preserve aspect ratio)

												 // enter the drawing loop
	glutMainLoop();
	return 0;
}
