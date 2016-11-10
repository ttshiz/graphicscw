
#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "mat.h"
#include <limits>
#include <fstream>
#include <string>
#include <vector>
// Number of points in polyline
const int NumPoints = 3;

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void myDisplay(void);
void changeColor(void);
void drawPolylineFile(char * filename);
void reshape(double W, double H);

typedef vec2 point2;

using namespace std;

// Array for polyline
point2 points[NumPoints];
GLuint program;

void generateGeometry(void)
{
	// Specifiy the vertices for a triangle
	points[0] = point2(-0.5, -0.5);
	points[1] = point2(0.0, 0.5);
	points[2] = point2(0.5, -0.5);
}

void initGPUBuffers(void)
{
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

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

void display(void)
{
	// All drawing happens in display function
	glClear(GL_COLOR_BUFFER_BIT);                // clear window
glDrawArrays(GL_LINE_LOOP, 0, NumPoints);    // draw the points
glFlush();										// force output to graphics hardware
}

void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler
	cout << key << std::endl;
	cout.flush();
	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case 0160:			// 0160 is p key octal value
		cout << "Do p action" << std::endl;
		break;
	case 0164:			// 0164 is t key octal value
		cout << "Do t action" << std::endl;
		break;
	case 0145:			// 0145 is e key octal value
		cout << "Do e action" << std::endl;
		break;
	case 0147:			// 0147 is g key octal value
		cout << "Do g action" << std::endl;
		break;
	case 0146:			// 0146 is f key octal value
		cout << "Do f action" << std::endl;
		break;
	case 0143:			// 0143 is c key octal value
		cout << "Do c action" << std::endl;
		changeColor();
		break;
	}
}

void changeColor(void) {
	GLfloat current_color[4];
	glGetFloatv(GL_CURRENT_COLOR, current_color);
	int color = 0;
	for (color = 0; color < 3; color++) {
		//cout << color << std::endl;
		if (current_color[color] != 0) {
			break;
		}
	}
	if (color == 0) {
		glColor3f(0.0f, 1.0f, 0.0f);
		//cout << "red to green" << std::endl;
	}
	else if (color == 1) {
		glColor3f(0.0f, 0.0f, 1.0f);
	}
	else {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	myDisplay();
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
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::vector<std::vector<point2>> polys;
	int numpolys;
	file >> numpolys;
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	int pos = 0;
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
		glViewport(0, 0, (GLsizei)((float)H / (float)R), H);
	}
	else {
		glViewport(0, 0, W, H);
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
		glViewport(k*w, h * 5, w, h);
		drawPolylineFile(images[k]);  // need to fix
	}
	//mat4 ortho = Ortho2D(0.0, 1.0, 0.0, 1.0);
	glViewport(0, 0, w, h);
	drawPolylineFile("vinci.dat");
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

	generateGeometry();                           // Call function that generates points to draw
	initGPUBuffers();							   // Create GPU buffers
	shaderSetup();                                // Connect this .cpp file to shader file

	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	mat4 ortho = Ortho2D(0, width, 0, height);
	GLint ProjLoc = glGetUniformLocation(program, "Proj");
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

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
