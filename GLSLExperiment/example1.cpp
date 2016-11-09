// Starter program for HW 1. 
// Program draws a triangle. Study the program first
// The function generateGeometry can be modified to draw many 2D drawings (e.g. 2D Sierpinski Gasket)
// Generated using randomly selected vertices and bisection

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "mat.h"
#include <fstream>
#include <string>
#include <vector>
// Number of points in polyline
const int NumPoints = 3;
const int NumBird = 53*82;

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void myDisplay(void);


typedef vec2 point2;

using namespace std;

// Array for polyline
point2 points[NumPoints];
point2 birdhd[NumBird];
GLuint program;

std::vector<std::vector<unsigned int>> bindices;

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(birdhd), birdhd, GL_STATIC_DRAW);
	
	GLuint birdelements;
	glGenBuffers(1, &birdelements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, birdelements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bindices.size() * sizeof(unsigned int), &bindices[0], GL_STATIC_DRAW);
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

	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	}
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
		std::vector<unsigned int> thisline;
		for (int i = 0; i < numPoints; i++) {
			float x, y;
			file >> x >> y;
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//thispoly.push_back(point2(x, y));
			thisline.push_back(pos);
			birdhd[i] = point2(x, y);
			pos++;
		}
		//glDrawElements(GL_LINE_STRIP, bindices.size(), GL_UNSIGNED_INT, (void*) 0);
		//polys.push_back(thispoly);
		bindices.push_back(thisline);
		glBufferData(GL_ARRAY_BUFFER, sizeof(birdhd), birdhd, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bindices.size() * sizeof(unsigned int), &bindices[0], GL_STATIC_DRAW);
		//glDrawArrays(GL_LINE_STRIP, 0, NumBird);
		glDrawElements(GL_LINE_STRIP, numpolys, GL_UNSIGNED_INT, &bindices[0]);
	}
}
void myDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	float w, h;
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);

	w = width / 10;
	h = height / 6;
	for (int k = 0; k < 10; k++) {
		glViewport(k*w, h * 5, w, h);
		drawPolylineFile("birdhead.dat");  // need to fix
		//DrawArrays(GL_LINE_STRIP, 0, NumPoints);
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

	generateGeometry();                           // Call function that generates points to draw
	initGPUBuffers();							   // Create GPU buffers
	shaderSetup();                                // Connect this .cpp file to shader file

	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	mat4 ortho = Ortho2D(0, width, 0, height);
	GLint ProjLoc = glGetUniformLocation(program, "Proj");
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

	glutDisplayFunc(myDisplay);
	//glutDisplayFunc( display );                    // Register display callback function
	glutKeyboardFunc(keyboard);                  // Register keyboard callback function

												 // Can add minimalist menus here
												 // add mouse handler
												 // add resize window functionality (should probably try to preserve aspect ratio)

												 // enter the drawing loop
	glutMainLoop();
	return 0;
}
