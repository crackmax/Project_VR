#include<iostream>
#define _USE_MATH_DEFINES
//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<glm/gtc/matrix_inverse.hpp>
#include <cmath>
#include <vector>
#include <map>
#include "camera.h"
#include "shader.h"
#include "object.h"

GLuint textureID;
const int width = 1200;
const int height = 1200;
const float orbit_radius = 8.0;

GLuint compileShader(std::string shaderCode, GLenum shaderType);
GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);
void loadCubemapFace(const char* file, const GLenum& targetCube);
void processInput(GLFWwindow* window);
void updateParticles();
void emitParticles();
void renderParticles();
void loadBumbMap();

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

Camera camera(glm::vec3(-10, 8.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.0F, -30.0F);


int main(int argc, char* argv[])
{
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
	GLFWwindow* window = glfwCreateWindow(width, height, "Project", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window\n");
	}

	glfwMakeContextCurrent(window);
	/* Motivated student can implement the rotation using the mouse
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	*/
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

	glEnable(GL_DEPTH_TEST);
	char fileVert[128] = PATH_TO_SHADERS"/earth.vert";
	char fileFrag[128] = PATH_TO_SHADERS"/earth.frag";
	char sunVert[128] = PATH_TO_SHADERS"/sun.vert";
	char sunFrag[128] = PATH_TO_SHADERS"/sun.frag";
	char reflectiveVert[128] = PATH_TO_SHADERS"/reflective.vert";
	char reflectiveFrag[128] = PATH_TO_SHADERS"/reflective.frag";
	char particleVert[128] = PATH_TO_SHADERS"/particles.vert";
	char particleFrag[128] = PATH_TO_SHADERS"/particles.frag";
	const std::string sourceVCubeMap = "#version 330 core\n"
		"layout (location = 0) in vec3 position; \n"
		"layout (location = 1) in vec2 tex_coords; \n"
		"layout (location = 2) in vec3 normal; \n"

		//only P and V are necessary
		"uniform mat4 V; \n"
		"uniform mat4 P; \n"

		"out vec3 texCoord_v; \n"

		" void main(){ \n"
		"texCoord_v = position;\n"
		//remove translation info from view matrix to only keep rotation
		"mat4 V_no_rot = mat4(mat3(V)) ;\n"
		"vec4 pos = P * V_no_rot * vec4(position, 1.0); \n"
		// the positions xyz are divided by w after the vertex shader
		// the z component is equal to the depth value
		// we want a z always equal to 1.0 here, so we set z = w!
		// Remember: z=1.0 is the MAXIMUM depth value ;)
		"gl_Position = pos.xyww;\n"
		"\n"
		"}\n";

	const std::string sourceFCubeMap =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"precision mediump float; \n"
		"uniform samplerCube cubemapSampler; \n"
		"in vec3 texCoord_v; \n"
		"void main() { \n"
		"FragColor = texture(cubemapSampler,texCoord_v); \n"
		"} \n";


	Shader cubeMapShader = Shader(sourceVCubeMap, sourceFCubeMap);

	Shader shaderEarth(fileVert, fileFrag);
	Shader shaderSun(sunVert, sunFrag);
	Shader reflectiveShader(reflectiveVert, reflectiveFrag);



	char path1[] = PATH_TO_OBJECTS"/sphere_smooth.obj";

	char pathCube[] = PATH_TO_OBJECTS "/cube.obj";
	Object cubeMap(pathCube);
	cubeMap.makeObject(cubeMapShader);

	GLuint cubeMapTexture;
	glGenTextures(1, &cubeMapTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

	// texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(true);

	std::string pathToCubeMap = PATH_TO_IMAGES "/cubemap/";

	std::map<std::string, GLenum> facesToLoad = {
		{pathToCubeMap + "right.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_X},
		{pathToCubeMap + "top.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
		{pathToCubeMap + "front.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
		{pathToCubeMap + "left.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
		{pathToCubeMap + "bottom.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
		{pathToCubeMap + "back.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
	};
	//load the six faces
	for (std::pair<std::string, GLenum> pair : facesToLoad) {
		loadCubemapFace(pair.first.c_str(), pair.second);
	}



	Object earth(path1);
	Object sun(path1);
	Object reflectiveSphere(path1);
	reflectiveSphere.makeObject(reflectiveShader);

	



	earth.loadTexture(PATH_TO_IMAGES"/2k_earth_daymap.jpg",GL_TEXTURE1);

	earth.makeObject(shaderEarth,true);
	earth.model = glm::scale(earth.model, glm::vec3(0.6, 0.6, 0.6));
	earth.model = glm::translate(earth.model, glm::vec3(4.0, 0.6, 0.6));
	loadBumbMap();
	
	sun.loadTexture(PATH_TO_IMAGES"/sun.jpg", GL_TEXTURE2);
	sun.makeObject(shaderSun,true);
	sun.model = glm::scale(sun.model, glm::vec3(2.2, 2.2, 2.2));

	glm::vec3 light_pos = glm::vec3(1.0, 2.0, 1.5);
	reflectiveSphere.model = glm::translate(reflectiveSphere.model, glm::vec3(0.0, 0.0, -2.0));
	reflectiveSphere.model = glm::scale(reflectiveSphere.model, glm::vec3(0.5, 0.5, 0.5));


	const glm::vec3 lightColor = glm::vec3(0.99	, 0.72, 0.7);
	float lightIntensity = 1.0f;
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 perspective = camera.GetProjectionMatrix();

	const glm::vec3 sunPosition = glm::vec3(glm::inverse(sun.model)[3]);
	float degree = 0.0;
	float degreeSun = 0.0;
	float travelDistance = 0.0;
	float travelDistance_reflective = 3.0;

	while (!glfwWindowShouldClose(window)) {

		processInput(window);
		view = camera.GetViewMatrix();
		glfwPollEvents();
		double now = glfwGetTime();
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "OpenGL Error before rendering: " << error << std::endl;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		emitParticles();
		updateParticles();

		glm::vec3 cameraPosition = glm::vec3(glm::inverse(view)[3]);
		shaderEarth.use();
		shaderEarth.setInteger("textureSampler", 1);
		shaderEarth.setInteger("normalMap", 3);
		shaderEarth.setMatrix4("M", earth.model);
		shaderEarth.setMatrix4("V", view);
		shaderEarth.setMatrix4("P", perspective);
		shaderEarth.setVector3f("u_light_pos", sunPosition);
		shaderEarth.setVector3f("u_view_pos", cameraPosition);
		shaderEarth.setVector3f("u_light_color", lightColor);
		shaderEarth.setFloat("u_light_intensity", lightIntensity);
		glm::mat4 itM = glm::inverseTranspose(earth.model);
		shaderEarth.setMatrix4("itM", itM);
		earth.draw();

		

		shaderSun.use();
		shaderSun.setInteger("textureSampler", 2);
		shaderSun.setMatrix4("V", view);
		shaderSun.setMatrix4("P", perspective);
		shaderSun.setMatrix4("M", sun.model);
		shaderSun.setMatrix4("itM", glm::inverseTranspose(sun.model));
		shaderSun.setVector3f("u_light_pos", cameraPosition);
		shaderSun.setVector3f("u_light_color", lightColor);
		shaderSun.setFloat("u_light_intensity", lightIntensity);

		sun.draw();
		renderParticles();
		// render particles here to make them around the sun and use the sun shader to have same color and light intensity as the sun
		

		float rotationSpeed = 0.005f;
		float travel = 0.001f;
		float travel_reflective = 0.001f;
		float rotationSpeedSun = 0.001f;

		degree -= rotationSpeed * 17.0f;
		travelDistance += travel * 17.0f;
		degreeSun += rotationSpeedSun * 15.0f;
		travelDistance_reflective += travel_reflective * 10.0f;

		float new_x = orbit_radius * cos(glm::radians(travelDistance));
		float new_y = orbit_radius * sin(glm::radians(travelDistance));

		earth.model = glm::mat4(1.0);
		sun.model = glm::mat4(1.0);
		earth.model = glm::translate(earth.model, glm::vec3(new_x, 0.0, new_y));
		earth.model = glm::rotate(earth.model, glm::radians(degree), glm::vec3(0.0, 1.0, 0.0));
		earth.model = glm::rotate(earth.model, glm::radians(23.5f), glm::vec3(0.0, 0.0, 1.0));
		sun.model = glm::rotate(sun.model, glm::radians(degreeSun), glm::vec3(0.0, 1.0, 0.0));

		reflectiveShader.use();

		reflectiveShader.setMatrix4("M", reflectiveSphere.model);
		reflectiveShader.setMatrix4("itM", glm::inverseTranspose(reflectiveSphere.model));
		reflectiveShader.setMatrix4("V", view);
		reflectiveShader.setMatrix4("P", perspective);
		reflectiveShader.setVector3f("u_view_pos", camera.Position);

		float reflective_x = (orbit_radius + 5.0) * cos(glm::radians(travelDistance_reflective));
		float reflective_y = (orbit_radius + 5.0) * sin(glm::radians(travelDistance_reflective));
		//ADD BUMB MAPPING TO EARTH
		reflectiveSphere.draw();
		reflectiveSphere.model = glm::mat4(1.0);
		reflectiveSphere.model = glm::translate(reflectiveSphere.model, glm::vec3(reflective_x, 0.0, reflective_y));

		glDepthFunc(GL_LEQUAL);
		cubeMapShader.use();
		cubeMapShader.setMatrix4("V", view);
		cubeMapShader.setMatrix4("P", perspective);
		cubeMapShader.setInteger("cubemapTexture", 0);
		cubeMap.draw();
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
	}


	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
void loadCubemapFace(const char* path, const GLenum& targetFace)
{
	int imWidth, imHeight, imNrChannels;
	unsigned char* data = stbi_load(path, &imWidth, &imHeight, &imNrChannels, 0);
	if (data)
	{
		glTexImage2D(targetFace, 0, GL_RGB, imWidth, imHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	stbi_image_free(data);
}

void loadBumbMap() {
	GLuint normalMapTexture;
	glGenTextures(1, &normalMapTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalMapTexture);

	// Load normal map image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(PATH_TO_IMAGES"/2k_earth_normal_map.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		std::cout << "Normal map loaded successfully." << std::endl;
	}
	else {
		// Handle error (failed to load image)
		std::cout << "Failed to load normal map image" << std::endl;
		const char* stbiError = stbi_failure_reason();
		if (stbiError) {
			std::cout << "stb_image error: " << stbiError << std::endl;
		}
		stbi_image_free(data);
	}
}

void processInput(GLFWwindow* window) {
	float deltaTime = 0.001;
	float deltaTime2 = 0.01;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboardMovement(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(1, 0.0, deltaTime2);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(-1, 0.0, deltaTime2);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, 1.0, deltaTime2);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboardRotation(0.0, -1.0, deltaTime2);


}


struct Particle {
	float x, y, z;
	float vx, vy, vz;
	float lifetime;
	float r, g, b;
};

std::vector<Particle> particles;

const int numParticles = 1000;
const float emissionRate = 0.05f;

void emitParticles() {
	for (int i = 0; i < numParticles; ++i) {
		if ((rand() / (float)RAND_MAX) < emissionRate) {
			Particle particle;

			// Put the particles at a random location inside the sphere
			particle.x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
			particle.y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
			particle.z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;

			float speed = 0.001f;

			//Generates random azimuthal angle and polar angle in order to have random point on a sphere.
			//This ensueres that the particles will move in random directions
			float theta = 2.0f * M_PI * (rand() / (float)RAND_MAX);
			float phi = acos(2.0f * (rand() / (float)RAND_MAX) - 1.0f);

			particle.vx = speed * sin(phi) * cos(theta);
			particle.vy = speed * sin(phi) * sin(theta);
			particle.vz = speed * cos(phi);

			particle.lifetime = 10.0f;
			particle.r = 1.0f;
			particle.g = 0.0f;
			particle.b = 0.0f;

			particles.push_back(particle);
		}
	}
}





void updateParticles() {
	// makes the particles move and reduce their lifetime
	for (auto& particle : particles) {
		particle.x += particle.vx;
		particle.y += particle.vy;
		particle.z += particle.vz;
		particle.lifetime -= 0.01f;
	}

	// Remove dead particles if they died
	particles.erase(std::remove_if(particles.begin(), particles.end(),
		[](const Particle& p) { return p.lifetime <= 0.0f; }),
		particles.end());
}

void renderParticles() {
	GLuint particleVAO, particleVBO;

	glGenVertexArrays(1, &particleVAO);
	glBindVertexArray(particleVAO);

	glGenBuffers(1, &particleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	// gives particles data, total size of data in bytes that is passed: particles.size() * sizeof(Particle)
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, x));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glDrawArrays(GL_POINTS, 0, particles.size());

	// Cleanup because otherwise memory would be full fast
	glDeleteVertexArrays(1, &particleVAO);
	glDeleteBuffers(1, &particleVBO);
}

