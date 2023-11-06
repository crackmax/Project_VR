#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>


const int width = 500;
const int height = 500;


GLuint compileShader(std::string shaderCode, GLenum shaderType);
GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);

int main(int argc, char* argv[])
{
	std::cout << "Welcome to exercice two" << std::endl;
	std::cout << "First Triangle\n"
		"Let's make our first Vertex Shader to handle custom data read from a buffer.\n"
		"You need to : \n"
		"\t 1. Create a buffer containing the triangle vertices\n"
		"\t 2. Send this data to the GPU using a OpenGL buffer object\n"
		"\t 3. Retrieve this data in the vertex shader\n";

	//Boilerplate
	//Create the OpenGL context 
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialise GLFW \n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	//Create the window
	GLFWwindow * window = glfwCreateWindow(width, height, "Exercise 2", nullptr, nullptr);
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


	// 3. Your code here: What is the output of a vertex program?
	const std::string sourceV = "#version 330 core\n"
		"//3. Get the input data for the position \n"
		" void main(){ \n"
		//3. use the position data
		"gl_Position;\n"
		"}\n"; 
	const std::string sourceF = "#version 330 core\n"
		"precision mediump float; \n"
		"out vec4 fragColor;\n"
		"void main() { \n"
		" fragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"	
		"} \n";

	GLuint shaderV = compileShader(sourceV, GL_VERTEX_SHADER);
	GLuint shaderF = compileShader(sourceF, GL_FRAGMENT_SHADER);
	GLuint program = compileProgram(shaderV, shaderF);

	// First object!
	// 1. Your code: Triangle vertices
	

	//Create the buffer
	// 2. Your code: make a VBO (buffer) and a VAO (vertex buffer object) and send the data from the positionData to the GPU
	GLuint VBO, VAO;
	//generate the buffer and the vertex array
	

	//define VBO and VAO as active buffer and active vertex array
	
	//Specify the vertex attributes
	
	
	//desactive the buffer
	

	//to be in sync with the screen (by default it will be most of the time be in sync but not on every platform)
	glfwSwapInterval(1);
	//Rendering
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//draw the background and clear previous image
		

		//Bind the buffer, use the program and draw the object
		
		glfwSwapBuffers(window);
	}
	
	//clean up ressources
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

GLuint compileShader(std::string shaderCode, GLenum shaderType)
{
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
