/* Written by Jeffrey Chastine, 2012 */
/*Shamelessly stolen & edited by Mark Elsinger for academic purposes*/
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
GLuint shaderProgramID;
GLuint vao = 0;
GLuint vbo;
GLuint positionID, colorID;
int offset = 0;
int size = 4;

#pragma region SHADER_FUNCTIONS
static char* readFile(const char* filename) {
	// Open the file
	FILE* fp = fopen (filename, "r");
	// Move the file pointer to the end of the file and determing the length
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* contents = new char[file_length+1];
	// zero out memory
	for (int i = 0; i < file_length+1; i++) {
		contents[i] = 0;
	}
	// Here's the actual read
	fread (contents, 1, file_length, fp);
	// This is how you denote the end of a string in C
	//contents[file_length+1] = '\0'; //not really, this is out of bounds!
	contents[file_length] = '\0'; //this is the correct end location, zero-indexed!
	fclose(fp);
	return contents;
}

bool compiledStatus(GLint shaderID){
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled) {
		return true;
	}
	else {
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = new char[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
		printf ("%s\n", msgBuffer);
		delete (msgBuffer);
		return false;
	}
}

GLuint makeVertexShader(const char* shaderSource) {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource (vertexShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(vertexShaderID);
	bool compiledCorrectly = compiledStatus(vertexShaderID);
	if (compiledCorrectly) {
		return vertexShaderID;
	}
	return -1;
}

GLuint makeFragmentShader(const char* shaderSource) {
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(fragmentShaderID);
	bool compiledCorrectly = compiledStatus(fragmentShaderID);
	if (compiledCorrectly) {
		return fragmentShaderID;
	}
	return -1;
}

GLuint makeShaderProgram (GLuint vertexShaderID, GLuint fragmentShaderID) {
	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShaderID);
	glAttachShader(shaderID, fragmentShaderID);
	glLinkProgram(shaderID);
	return shaderID;
}
#pragma endregion SHADER_FUNCTIONS

// Any time the window is resized, this function gets called.  It's setup to the
// "glutReshapeFunc" in main.
void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}

// Here is the function that gets called each time the window needs to be redrawn.
// It is the "paint" method for our program, and is set up from the glutDisplayFunc in main
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glDrawArrays(GL_TRIANGLE_FAN, offset, size);			
	glutSwapBuffers();
}

void mousefunc(int button, int state, int x, int y) {
	if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
		exit(0);
	}
	else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		if(offset == 0)
		{
			offset = 4;
			size = 8;
		}
		else if(offset == 4)
		{
			offset = 12;
			size = 8;
		}
		else
		{
			offset = 0;
			size = 4;
		}
  	}
  	printf("Offset is %d!\n", offset);
  	printf("Size is %d!\n", size);
  	render();
}

int main (int argc, char** argv) {
	// Standard stuff...
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Lab 1");
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutMouseFunc(mousefunc);
	glewInit();

	// Vertices and colors of a triangle
	// notice, position values are between -1.0f and +1.0f
	GLfloat vertices[] = {
				///////////Square///////////
				-0.5f, -0.5f, 0.0f,				// Lower-left
				0.5f, -0.5f, 0.0f,				// Lower-right
				0.5f, 0.5f, 0.0f,					// Top-right
				-0.5f, 0.5f, 0.0f,				// Top-left
				
				////////////Octo////////////
				0.5f, 0.25f, 0.0f,				//CCW
				0.25f, 0.5f, 0.0f,
				-0.25f, 0.5f, 0.0f,
				-0.5f, 0.25f, 0.0f,
				-0.5f, -0.25f, 0.0f,
				-0.25f, -0.5f, 0.0f,
				0.25f, -0.5f, 0.0f,
				0.5f, -0.25f, 0.0f,
				
				////////////Star////////////
				0.1f, 0.1f, 0.0f,				//CCW
				0.0f, 0.5f, 0.0f,
				-0.1f, 0.1f, 0.0f,
				-0.5f, 0.0f, 0.0f,
				-0.1f, -0.1f, 0.0f,
				0.0f, -0.5f, 0.0f,
				0.1f, -0.1f, 0.0f,
				0.5f, 0.0f, 0.0f 
				};			
					
	GLfloat colors[] = {
				///////////Square//////////
				0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
				
				///////////Octo////////////
				0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				
				///////////Star/////////////
				0.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				};

	// Make a shader
	char* vertexShaderSourceCode = readFile("vertexshader.glsl");
	char* fragmentShaderSourceCode = readFile("fragmentshader.glsl");
	GLuint vertShaderID = makeVertexShader(vertexShaderSourceCode);
	GLuint fragShaderID = makeFragmentShader(fragmentShaderSourceCode);
	shaderProgramID = makeShaderProgram(vertShaderID, fragShaderID);

	printf ("vertShaderID is %d\n", vertShaderID);
	printf ("fragShaderID is %d\n", fragShaderID);
	printf ("shaderProgramID is %d\n", shaderProgramID);


	// Create the "remember all"
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, 7*20*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3*20*sizeof(GLfloat), vertices);
	// Load the colors right after that
    glBufferSubData(GL_ARRAY_BUFFER, 3*20*sizeof(GLfloat),4*20*sizeof(GLfloat), colors);

	// Find the position of the variables in the shader
	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramID, "s_vColor");
	printf ("s_vPosition's ID is %d\n", positionID);
	printf ("s_vColor's ID is %d\n", colorID);

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(3*20*sizeof(GLfloat)));
    //glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);

	glutMainLoop();

	return 0;
}
