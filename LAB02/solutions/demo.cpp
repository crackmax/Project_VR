//small bits of code to screen shot for the slide

//slide 7

GLuint ID;
glCreateTYPE(1, &ID);
glBindTYPE(GL_TYPE, ID);
glDO_SOME_ACTION(...);
glBindTYPE(GL_TYPE, 0);

//slide 8

const float data[9] = {
	// vertices		
	-1.0, -1.0, 0.0,	
	1.0, -1.0, 0.0,		
	0.0, 1.0, 0.0,		
};

GLuint VBO;
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER, 0);

const std::string sourceV = 
	"#version 330 core\n"
	"layout (location=0) in vec3 position; \n"
	"void main(){ \n"
	"gl_Position = vec4(position, 1);\n"
	"}\n";

auto attribute = glGetAttribLocation(program, "position");
glEnableVertexAttribArray(attribute);
glVertexAttribPointer(attribute, 3, GL_FLOAT, false, 0, 0);

glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);


//slide 9

const std::string sourceV = "#version 330 core\n"
		"in vec3 position;\n"
		"out vec4 color;\n"
		" void main(){\n"
		"gl_Position = vec4(position, 1);\n"
		"color = gl_Position * 0.5 + 0.5;\n"
		"}\n";

const std::string sourceF = "#version 330 core\n"
	"precision mediump float;\n"
    "out vec4 FragColor;\n"
	"in vec4 color;\n"
	"void main() {\n"
	"FragColor = color;\n"
	"}\n";

//slide 10

const std::string sourceF = "#version 330 core\n"
		"precision mediump float; \n"
        "out vec4 fragColor;\n"
		"in vec3 color;\n"
		"uniform float time;\n"
		"void main() { \n"
		"fragColor = vec4(color * time, 1.0); \n"
		"} \n";

auto u_time = glGetUniformLocation(program, "time");

glUniform1f(u_time, (float)glfwGetTime());

//slide 13

void glVertexAttribPointer(	GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);

const float objectData[24] = {
		// vertices         //color         //tex coord
		-1.0, -1.0, 0.0,    1.0, 0.0, 0.0,  0.0, 0.0,
		1.0, -1.0, 0.0,     0.0, 1.0, 0.0,  1.0, 0.0,
		0.0, 1.0, 0.0,      0.0, 0.0, 1.0,  0.5, 1.0,
	};


auto attribute = glGetAttribLocation(program, "position");

const std::string sourceV = "#version 330 core\n"
		"layout (location = 0) in vec3 position; \n"
        "layout (location = 1) in vec3 color; \n"
        "layout (location = 2) in vec3 texcoord; \n"
		"void main(){ \n"
		"gl_Position = vec4(position, 1);\n"
		"}\n"; 

(3+2+2) * sizeof(float)

//slide 15

GLuint VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);

glBindVertexArray(VAO);



//slide 20

GLuint texture;
glGenTextures(1, &texture);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

stbi_set_flip_vertically_on_load(true);
int imWidth, imHeight, imNrChannels;
char file[128] = "../../../../LAB02/solutions/ex13/horse.jpg";
unsigned char* data = stbi_load(file, &imWidth, &imHeight, &imNrChannels, 0);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imWidth, imHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
glGenerateMipmap(GL_TEXTURE_2D);

stbi_image_free(data);
glBindTexture(GL_TEXTURE_2D, 0);

//slide 21

const float positionsData[15] = {
	// vertices		  // texture coords
	-1.0, -1.0, 0.0,	0.0, 0.0,
	 1.0, -1.0, 0.0,	1.0, 0.0,
	 0.0,  1.0, 0.0,	0.5, 1.0,
};

glUniform1i(u_texture, 0);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture);

glDrawArrays(GL_TRIANGLES, 0, 3);

const std::string sourceF = "#version 330 core\n"
"out vec4 FragColor;"
"precision mediump float; \n"
"in vec2 v_texcoords; \n"
"uniform sampler2D u_texture; \n"
"void main() { \n"
"FragColor = texture(u_texture, v_texcoords); \n"
"} \n";

//slide 23

while (!glfwWindowShouldClose(window)) {
	glfwPollEvents();
	double now = glfwGetTime();

	//DRAW EVERYTHING

	fps(now);
	glfwSwapBuffers(window);
}

glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);
glUseProgram(program);
glDrawArrays(GL_TRIANGLES, 0, 3);