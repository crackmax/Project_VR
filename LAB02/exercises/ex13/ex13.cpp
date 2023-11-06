#include<iostream>
//for debug
#include <fstream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//1. include the stb library, look at their read me


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

glm::vec3 cameraPos;
glm::vec3 viewDirection;

int main(int argc, char* argv[])
{
	std::cout << "Welcome to exercice 13: " << std::endl;
	std::cout << "Using texture\n"
		"Now we will use a texture to colour our triangle.\n"
		"You have at your disposition the texture for a box and the picture of a beautifull horse.\n"
		"First try to apply the texture for the box, then the image with the horse and look what happens.\n"
		"You need to : \n"
		"-	Include the stb_image library"
		"-	Define texture coordinate along the position of the vertices\n"
		"-	Create the texture object and set its parameter\n"
		"-	Load the image of the texture\n"
		"-	Adapt the vertexAttribPointer so that the vertex shader know how to read the relevant attribute\n"
	"-	From the rendering loop, Use a uniform to pass the texture to the fragment shader \n"
	"-	Activate the texture \n"
	"-	In the fragment shader, use the texture coordinate and the texture to define the color\n";

	//Boilerplate
	//Create the OpenGL context 
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


	//Create the window
	GLFWwindow* window = glfwCreateWindow(width, height, "Exercise 13", nullptr, nullptr);
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
		//5. Your code

	
		"uniform mat4 M; \n"
		"uniform mat4 V; \n"
		"uniform mat4 P; \n"
		" void main(){ \n"
		"gl_Position = P*M*V*vec4(position, 1);\n"
		//5. your code
		"}\n"; 
	const std::string sourceF = "#version 330 core\n"
		"out vec4 FragColor;"
		"precision mediump float; \n"
		//6. Get the texture 
		"void main() { \n"
		//8. use the texture and the coordinates
		"FragColor = vec4(1.0,0.5,0.9,1.0); \n"
		"} \n";

	GLuint shaderV = compileShader(sourceV, GL_VERTEX_SHADER);
	GLuint shaderF = compileShader(sourceF, GL_FRAGMENT_SHADER);
	GLuint program = compileProgram(shaderV, shaderF);

	// First object!
	//2. Define the position of the vertices and the coordinates in the texture (in 2D)
	const float positionsData[9] = {
		// vertices			
		-1.0, -1.0, 0.0,	
		1.0, -1.0, 0.0,		
		0.0, 1.0, 0.0,		
	};

	//3. Create, activate and bind the texture

	
	//3. Define the parameters for the texture


	//4. Load the image
	//Carefull depending on where your executable is, the relative path might be different from what you think it is
	//Try to use an absolute path
	//image usually have thei 0.0 at the top of the vertical axis and not the bottom like opengl expects
	
	

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
	glVertexAttribPointer(attribute, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	//5. VertexAttribPointer also read the texture coordinates

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


	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model,glm::vec3(0.5,0.5,-1.0));
	model = glm::scale(model, glm::vec3(0.5,0.5,1.0));

	cameraPos = glm::vec3(0.0, 0.0, 1.0);
	viewDirection = glm::vec3(0.0, 0.0, -1.0);

	glm::mat4 view = glm::lookAt(cameraPos,
		cameraPos + viewDirection,
		glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 perspective = glm::perspective(45.0, 500.0/500.0, 0.01, 100.0);

	//uniform
	auto u_model = glGetUniformLocation(program, "M");
	auto u_view = glGetUniformLocation(program, "V");
	auto u_perspective = glGetUniformLocation(program, "P");

	//6. get the uniform location for the texture



	//sync with the screen refresh rate
	glfwSwapInterval(1);
	//Rendering
	
	while (!glfwWindowShouldClose(window)) {
		view = processInput(window);
		glfwPollEvents();
		double now = glfwGetTime();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		glBindVertexArray(VAO);

		glUseProgram(program);
		

		
		glUniformMatrix4fv(u_model,1,GL_FALSE,glm::value_ptr(model));
		glUniformMatrix4fv(u_view,1,GL_FALSE,glm::value_ptr(view));
		glUniformMatrix4fv(u_perspective,1,GL_FALSE,glm::value_ptr(perspective));
		
		//6. Send the texture
		
		//7. activate the texture
		
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