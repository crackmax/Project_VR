#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


const int width = 500;
const int height = 500;


GLuint compileShader(std::string shaderCode, GLenum shaderType);
GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);

int main(int argc, char* argv[])
{
	std::cout << "Welcome to exercice one" << std::endl;
	std::cout << "Create a Window and init OpenGL\n"
		"In this exercise you are going to make a window your first rendering pipeline!\n"
		"You need to : \n"
		"\t 1. Initialize the opengl context and create a window using glfw library\n"
		"\t Create 2 functions: \n"
		"\t \t 2. compile_shader : to compile text into a shader \n"
		"\t \t 3. create_program : takes a vertex and a fragment shader and return a pipeline\n"
		"\t 4. Write the code for the fragment and an empty vertex shader\n"
		"\t 5. Create a loop that exit when the window is close and render the pipeline inside\n";

	//Boilerplate
	//1.a Create the OpenGL context 
	

	//1.b Create the window
	GLFWwindow* window; // = ...

	
	//load openGL function
	


	const std::string sourceV = "#version 330 core\n"
		"void main(){ \n"
		"gl_Position = vec4(1.0,1.0,1.0,1.0);\n"
		"}\n"; 
	const std::string sourceF = "#version 330 core\n"
		"precision mediump float; \n"
		"out vec4 fragColor;\n"
		"void main() { \n"
		"// 4. Your code: write the minimal code to make the shader compile \n"
		"fragColor = //... \n"
		"} \n";

	GLuint shaderV = compileShader(sourceV, GL_VERTEX_SHADER);
	GLuint shaderF = compileShader(sourceF, GL_FRAGMENT_SHADER);
	GLuint program = compileProgram(shaderV, shaderF);

	
	// 5. Your code: Rendering
	//5.a change the condition of the while by something relevant
	while (1) {
		//5.b choose the background color

	}

	//clean up ressources

	glfwTerminate();

	return 0;
}

GLuint compileShader(std::string shaderCode, GLenum shaderType)
{
	//2. Your code to compile the shader
	GLuint shader = 0; 
	
	return shader;
}

GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// 3. Your code to compile the program
	GLuint programID = 0;

	
	
	return programID;
}
