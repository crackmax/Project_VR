#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Don't forget the glm header
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <chrono>
#include <thread>


const int width = 500;
const int height = 500;


GLuint compileShader(std::string shaderCode, GLenum shaderType);
GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
glm::mat4 processInput(GLFWwindow *window);


#ifndef NDEBUG
void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
#endif

//1.define the parameter of the camera as global variables
glm::vec3 cameraPos;
glm::vec3 viewDirection;

int main(int argc, char* argv[])
{
	std::cout << "Welcome to exercice 12: " << std::endl;
	std::cout << "Playing with matrix - 4\n"
		"Now that we have a proper camera, try to move it\n"
		"Create a function to get the keyboard input\n"
		"Which parameter do you need to change to move the camera ?\n"
		"Implement left, right, up and down with the arrows, forward and backward with a and q, and looking around with r and t \n"
		"Think carefully about your axis system. \n"
		"You need to : \n"
		"-	Create the model, view and perspective matrices\n"
		"-	Implement the function that will process the input\n"
		"-	Use your function to modify the view matrix in each render loop\n"
		"-	Apply the transformation in the vertex shader\n";

	//Boilerplate
	//1. Create the OpenGL context 
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialise GLFW \n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
	//create a debug context to help with Debugging
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif


	// Create the window
	GLFWwindow* window = glfwCreateWindow(width, height, "Solution 12", nullptr, nullptr);
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

#ifndef NDEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif

	const std::string sourceV = "#version 330 core\n"
		"in vec3 position; \n"
		"in vec3 color; \n"
		"out vec4 v_color; \n"
		//4 . Your code 
		"uniform mat4 M; \n"
		"uniform mat4 V; \n"
		"uniform mat4 P; \n"
		" void main(){ \n"
		//4. Your code
		"gl_Position = P*M*V*vec4(position, 1);\n"
		"v_color = vec4(color, 1.0);\n"

		"}\n"; 
	const std::string sourceF = "#version 330 core\n"
		"out vec4 FragColor;"
		"precision mediump float; \n"
		"in vec4 v_color;\n"
		"void main() { \n"
		"FragColor = v_color; \n"
		"} \n";

	GLuint shaderV = compileShader(sourceV, GL_VERTEX_SHADER);
	GLuint shaderF = compileShader(sourceF, GL_FRAGMENT_SHADER);
	GLuint program = compileProgram(shaderV, shaderF);

	// First object!
	const float positionsData[18] = {
		// vertices			// colors
		-1.0, -1.0, 0.0, 1.0, 0.0, 0.0,
		1.0, -1.0, 0.0, 0.0, 1.0, 0.0,
		0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
	};

	//Create the buffer

	
	GLuint VBO, VAO;
	//generate the buffer and the vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//define VBO and VAO as active buffer and active vertex array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionsData), positionsData, GL_STATIC_DRAW);

	auto attribute = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);

	auto att_color = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(att_color);
	glVertexAttribPointer(att_color, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	//desactive the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	double prev = 0;
	int deltaFrame = 0;
	//fps function
	auto fps = [&](double now) {
		double deltaTime = now - prev;
		deltaFrame++;
		if (deltaTime > 0.5) {
			prev = now;
			const double fpsCount = (double) deltaFrame / deltaTime;
			deltaFrame = 0;
			std::cout << "\r FPS: " << fpsCount;
			std::cout.flush();
		}
	};

	//1. Matrix for the model, view and projection
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model,glm::vec3(0.5,0.5,0.0));
	model = glm::scale(model, glm::vec3(0.5,0.5,1.0));

    cameraPos = glm::vec3(0.0,0.0,1.0);
	viewDirection = glm::vec3(0.0, 0.0, -1.0);

	glm::mat4 view = glm::lookAt(cameraPos,
	cameraPos+viewDirection,
	glm::vec3(0.0,1.0,0.0));

	glm::mat4 perspective = glm::perspective(45.0, 500.0/500.0, 0.01, 100.0);

	//get the uniform
	auto u_model = glGetUniformLocation(program, "M");
	auto u_view = glGetUniformLocation(program, "V");
	auto u_perspective = glGetUniformLocation(program, "P");

	
	//sync with the screen refresh rate
	glfwSwapInterval(1);
	//Rendering

	
	while (!glfwWindowShouldClose(window)) {
		//3. Update the view matrix
		view = processInput(window);
		glfwPollEvents();
		double now = glfwGetTime();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		
		glUseProgram(program);

		//4. Use the model, view and projection matrices in the vertex shader
		glUniformMatrix4fv(u_model,1,GL_FALSE,glm::value_ptr(model));
		glUniformMatrix4fv(u_view,1,GL_FALSE,glm::value_ptr(view));
		glUniformMatrix4fv(u_perspective,1,GL_FALSE,glm::value_ptr(perspective));
		

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		fps(now);
		glfwSwapBuffers(window);
	}

	//clean up ressource
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
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of the " << t << ": " << shaderType << infoLog << std::endl;
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


glm::mat4 processInput(GLFWwindow *window){

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//2. Implement what happens when a key is pressed
	//This is a very lazy camera, try to turn it around then move around
	// are you still moving frontward, backarw, left and right according to the camera axis ?
	//You can upgrade it to be better
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
		cameraPos += glm::vec3(0.0, 0.1, 0.0);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos += glm::vec3(0.0, -0.1, 0.0);

	if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
		cameraPos += glm::vec3(-0.1, 0.0, 0.0);
	if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
		cameraPos += glm::vec3(0.1, 0.0, 0.0);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += glm::vec3 (0.0,0.0,-0.1);
	if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += glm::vec3(0.0, 0.0, +0.1);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		viewDirection += glm::vec3(-0.05,0.0,0.0);
	if(glfwGetKey(window,GLFW_KEY_T) == GLFW_PRESS)
		viewDirection += glm::vec3(0.05, 0.0, 0.0);
	
	glm::mat4 camera = glm::lookAt(cameraPos,
		cameraPos + viewDirection,
		glm::vec3(0.0, 1.0, 0.0));
	return camera;
		

	

}