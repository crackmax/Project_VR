#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include "shader.h"

#include <chrono>
#include <thread>


const int width = 700;
const int height = 500;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#ifndef NDEBUG
//source: https://learnopengl.com/In-Practice/Debugging
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
	if (id == 1282) return;

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

int main(int argc, char* argv[])
{
	std::cout << "Welcome to the first exercise session for VR" << std::endl;
	std::cout << "Let's play with the fragment shader\n"
		"Open the file LAB01.frag\n"
		"Read the example and run the code to see what it does. \n"
		"Modify LAB01.frag to solve the exercise.\n"
		;

	//Boilerplate
	//Create the OpenGL context 
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialise GLFW \n");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
	//A1. Request a debug context with glfw 
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif


	//Create the window
	GLFWwindow* window = glfwCreateWindow(width, height, "Labo 01", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window\n");
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//load openGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

#ifndef NDEBUG
	//2. Tell OpenGL to enable debug output and to call the glDebugOutput function(defined above in this code)\n
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

	char fileVert[128] = PATH_TO_LAB1_SHADERS"/LAB01.vert";
	char fileFrag[128] = PATH_TO_LAB1_SHADERS"/LAB01.frag";
	Shader shader(fileVert, fileFrag);

	

	// First object!
	const float positionsData[18] = {
		// vertices
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		-1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		-1.0, 1.0, 0.0,
		1.0, -1.0, 0.0
	};

	//Create the vertex buffer object and the vertex array object
	GLuint VBO, VAO;
	//generate the buffer and the vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//define VBO and VAO as active buffer and active vertex array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionsData), positionsData, GL_STATIC_DRAW);

	auto attribute = glGetAttribLocation(shader.ID, "position");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, false, 0, 0);

	//desactive the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int framebuffer_width, framebuffer_height;
	int window_width, window_height;
	double mouse_x, mouse_y;
	float scale_x, scale_y;


	//sync with the screen refresh rate
	glfwSwapInterval(1);
	//Rendering
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
		shader.setVector2f("iResolution", framebuffer_width, framebuffer_height);

		float now = (float)glfwGetTime();
		shader.setFloat("iTime", now);

		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		glfwGetWindowSize(window, &window_width, &window_height);
		scale_x = framebuffer_width / window_width;
		scale_y = framebuffer_height / window_height;

		shader.setVector2f("iMouse", (int)(mouse_x*scale_x), (int)(mouse_y*scale_y));
		

		glBindVertexArray(VAO);

		//glUseProgram(program);
		shader.use();

		glDrawArrays(GL_TRIANGLES, 0, 6);
	


		glfwSwapBuffers(window);
	}

	//clean up ressource
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
