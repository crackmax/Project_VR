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
	//1. Create the OpenGL context 
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialise GLFW \n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//1.b Create the window
	GLFWwindow * window = glfwCreateWindow(width, height, "Solution 01", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window\n");
	}

	glfwMakeContextCurrent(window);
	//load openGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}


	const std::string sourceV = "#version 330 core\n"
		"void main(){ \n"
		"gl_Position = vec4(1.0,1.0,1.0,1.0);\n"
		"}\n"; 
	const std::string sourceF = "#version 330 core\n"
		"precision mediump float; \n"
		"out vec4 fragColor;\n"
		"void main() { \n"
		" fragColor = vec4(0.2,0.2,0.2,1.0); \n"	// 4. Your code: write the minimal code to make the shader compile"
		"} \n";

	GLuint shaderV = compileShader(sourceV, GL_VERTEX_SHADER);
	GLuint shaderF = compileShader(sourceF, GL_FRAGMENT_SHADER);
	GLuint program = compileProgram(shaderV, shaderF);

	//sync with the screen refresh rate
	glfwSwapInterval(1);
	// 5. Your code: Rendering
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glfwSwapBuffers(window);
	}

	//clean up ressources
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

GLuint compileShader(std::string shaderCode, GLenum shaderType)
{
	//2. Your code to compile the shader
	GLuint shader = glCreateShader(shaderType);
	const char* code = shaderCode.c_str();
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

	GLchar infoLog[1024];
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::string t = "undetermined";
		if (shaderType == GL_VERTEX_SHADER) {
			t = "vertex shader";
		}
		else if (shaderType == GL_FRAGMENT_SHADER) {
			t = "fragment shader";
		}
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of the " << t << ": " << shaderType <<  infoLog << std::endl;
	}
	return shader;
}

GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// 3. Your code to compile the program
	GLuint programID = glCreateProgram();

	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);


	GLchar infoLog[1024];
	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR:  " << infoLog << std::endl;
	}
	return programID;
}
