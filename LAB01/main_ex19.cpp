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
glm::mat4 processInput(GLFWwindow* window);

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
	//exceptionally ignore some errors this time 
	//(when some variables are not used they are optimized out of the programme so you can not set them anymore)
	//usualy there no point setting something that we don't use
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


glm::vec3 cameraPos;
glm::vec3 viewDirection;

int main(int argc, char* argv[])
{
	std::cout << "Welcome to the first exercise session for VR" << std::endl;
	std::cout << "Let's play with the fragment shader again but this time, we will do it on a cube in place of a plane. \n"
		"Open the file LAB01_ex19.frag\n"
		"Read the example and run the code to see what it does. \n"
		"Modify LAB01_ex19.frag to solve the exercise. \n"
		"Here you can move using: \n the arrows of the keyboard (left,right,up, down), \n a and e to go forward/backward, \n r and t to rotate the camera"
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
	// Request a debug context with glfw 
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif


	//Create the window
	GLFWwindow* window = glfwCreateWindow(width, height, "Labo 1", nullptr, nullptr);
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
	//Tell OpenGL to enable debug output and to call the glDebugOutput function(defined above in this code)\n
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

	glEnable(GL_DEPTH_TEST);

	//The PATH_TO_LAB1_SHADERS is a macro that was defined in the cmake to have the path to the shaders more easely
	char fileVert[128] = PATH_TO_LAB1_SHADERS"/LAB01_ex19.vert";
	//char fileFrag[128] = PATH_TO_LAB1_SHADERS"/LAB01_ex19.frag";
	char fileFrag[128] = PATH_TO_LAB1_SHADERS"/LAB01_ex19.frag";
	Shader shader(fileVert, fileFrag);

	
	//  For the example the coordinate uv are put that way
	//   1^
	//	  |
	//    |
	//	  |_______
	//    |       |
	//    |   c   |
	//    |_______|_______________________  
	//   v|       |       |       |      |
	//    |   a   |   d   |   e   |   f  |
	//    |_______|_______|_______|______|
	//    |       |
	//    |   b   |
	//   0|_______|_______________________>
	//					u                1
	//
	//In 3D space it gives something like that
	//
	//                
	//               y   
	//               |      z  
	//               |     / 
	//               |    /
	//            _______
	//			 /  c   /| 
	//			/______/ | ---------> x
	//			|      |d|
	//			|   a  | /
	//			|______|/
	// 
	// 


	const float positionsData[120] = {
		// vertices	xyz	    //textCoord
		//face a
		-1.0, -1.0, 1.0,	0, 0.25 ,
		-1.0, 1.0, 1.0,     0, 0.5 ,
		1.0, -1.0, 1.0,		0.25, 0.25 ,
		1.0, 1.0, 1.0,     0.25, 0.5 ,
		//face b
		-1.0, -1.0, -1.0,   0,0,
		- 1.0, -1.0, 1.0,	0,0.25,
		1.0, -1.0, -1.0,    0.25,0,
		1.0, -1.0, 1.0,     0.25,0.25,
		// face c
		-1.0, 1.0, 1.0,     0,0.5,  
		1.0, 1.0, 1.0,		0.25,0.5,
		-1.0, 1.0, -1.0,	0,0.75,
		1.0, 1.0, -1.0,		0.25,0.75,
		//face d
		1.0, -1.0, 1.0,	0.25, 0.25 ,
		1.0, 1.0, 1.0,     0.25, 0.5 ,
		1.0, -1.0, -1.0,	0.5, 0.25 ,
		1.0, 1.0, -1.0,     0.5, 0.5 ,
		//face e
		-1.0, -1.0, -1.0,	0.5, 0.25 ,
		-1.0, 1.0, -1.0,     0.5, 0.5 ,
		1.0, -1.0, -1.0,	0.75, 0.25 ,
		1.0, 1.0, -1.0,     0.75, 0.5 ,
		//face f
		-1.0, -1.0, -1.0,	0.75, 0.25 ,
		-1.0, 1.0, -1.0,    0.75, 0.5 ,
		-1.0, -1.0, 1.0,	1, 0.25 ,
		-1.0, 1.0, 1.0,     1, 0.5 ,
	};

	//cut into triangles
	const unsigned int indices[36]{
		0,1,2,1,2,3,4,5,6,5,6,7,8,9,10,9,10,11,12,13,14,13,14,15,16,17,18,17,18,19,20,21,22,21,22,23
	};

	//Create the vertex buffer object and the vertex array object
	GLuint VBO, VAO, EBO;
	//generate the buffer and the vertex array
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//define VBO and VAO as active buffer and active vertex array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionsData), positionsData, GL_STATIC_DRAW);

	//EBO for convenience
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	auto attribute = glGetAttribLocation(shader.ID, "position");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);

	auto attribute2 = glGetAttribLocation(shader.ID, "texcoord");
	glEnableVertexAttribArray(attribute2);
	glVertexAttribPointer(attribute2, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//desactive the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int framebuffer_width, framebuffer_height;
	int window_width, window_height;
	double mouse_x, mouse_y;
	float scale_x, scale_y;


	glm::mat4 model = glm::mat4(1.0);
	
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0,1.0,0.0));
	model = glm::translate(model, glm::vec3(0.8, -0.8, -1.0));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

	cameraPos = glm::vec3(0.0, 0.0, 1.0);
	viewDirection = glm::vec3(0.0, 0.0, -1.0);

	glm::mat4 view = glm::lookAt(cameraPos,
		cameraPos + viewDirection,
		glm::vec3(0.0, 1.0, 0.0));

	

	//sync with the screen refresh rate
	glfwSwapInterval(1);
	//Rendering
	while (!glfwWindowShouldClose(window)) {

		view = processInput(window);
		
		glfwPollEvents();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
		shader.setVector2f("iResolution", framebuffer_width, framebuffer_height);

		double ratio = framebuffer_width/ framebuffer_height;
		glm::mat4 proj = glm::perspective(45.0, ratio, 0.01, 100.0);


		shader.setMatrix4("M", model);
		shader.setMatrix4("V", view);
		shader.setMatrix4("P", proj);

		float now = (float)glfwGetTime();
		shader.setFloat("iTime", now);

		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		glfwGetWindowSize(window, &window_width, &window_height);
		scale_x = framebuffer_width / window_width;
		scale_y = framebuffer_height / window_height;
		shader.setVector2f("iMouse", (int)(mouse_x*scale_x), (int)(mouse_y*scale_y));

		glBindVertexArray(VAO);

		shader.use();

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


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

glm::mat4 processInput(GLFWwindow* window) {
	//to move the camera more easely
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraPos += glm::vec3(0.0, 0.1, 0.0);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraPos += glm::vec3(0.0, -0.1, 0.0);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cameraPos += glm::vec3(-0.1, 0.0, 0.0);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cameraPos += glm::vec3(0.1, 0.0, 0.0);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += glm::vec3(0.0, 0.0, -0.1);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += glm::vec3(0.0, 0.0, +0.1);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		viewDirection += glm::vec3(-0.05, 0.0, 0.0);
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		viewDirection += glm::vec3(0.05, 0.0, 0.0);

	glm::mat4 camera = glm::lookAt(cameraPos,
		cameraPos + viewDirection,
		glm::vec3(0.0, 1.0, 0.0));
	return camera;

}
