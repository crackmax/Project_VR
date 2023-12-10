#ifndef OBJECT_H
#define OBJECT_H

#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*Principe :
* On donne le path du fichier -> on lit le fichier
* 2 �tape
* 1)load le model -> lit le fichier ligne par ligne
* liste de position de normal de texture
* suivant la premi�re lettre : lit les valeur suivant et les met dans un vec puis push dans la bonne liste
* en gros sotck les data dans une frome de tableau
*/

struct Vertex {
	glm::vec3 Position;
	glm::vec2 Texture;
	glm::vec3 Normal;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};


class Object
{
public:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<Vertex> vertices;

	int numVertices;

	GLuint textureID;

	GLuint VBO, VAO;

	glm::mat4 model = glm::mat4(1.0);


	Object(const char* path, const char* texture = nullptr) {

		std::ifstream infile(path);
		//TODO Error management
		std::string line;
		while (std::getline(infile, line))
		{
			std::istringstream iss(line);
			std::string indice;
			iss >> indice;
			//std::cout << "indice : " << indice << std::endl;
			if (indice == "v") {
				float x, y, z;
				iss >> x >> y >> z;
				positions.push_back(glm::vec3(x, y, z));

			}
			else if (indice == "vn") {
				float x, y, z;
				iss >> x >> y >> z;
				normals.push_back(glm::vec3(x, y, z));
			}
			else if (indice == "vt") {
				float u, v;
				iss >> u >> v;
				textures.push_back(glm::vec2(u, v));
			}
			else if (indice == "f") {
				std::string f1, f2, f3;
				iss >> f1 >> f2 >> f3;

				std::string p, t, n;

				//for face 1
				Vertex v1;

				p = f1.substr(0, f1.find("/"));
				f1.erase(0, f1.find("/") + 1);

				t = f1.substr(0, f1.find("/"));
				f1.erase(0, f1.find("/") + 1);

				n = f1.substr(0, f1.find("/"));


				v1.Position = positions.at(std::stof(p) - 1);
				v1.Normal = normals.at(std::stof(n) - 1);
				v1.Texture = textures.at(std::stof(t) - 1);
				vertices.push_back(v1);

				//for face 12
				Vertex v2;

				p = f2.substr(0, f2.find("/"));
				f2.erase(0, f2.find("/") + 1);

				t = f2.substr(0, f2.find("/"));
				f2.erase(0, f2.find("/") + 1);

				n = f2.substr(0, f2.find("/"));


				v2.Position = positions.at(std::stof(p) - 1);
				v2.Normal = normals.at(std::stof(n) - 1);
				v2.Texture = textures.at(std::stof(t) - 1);
				vertices.push_back(v2);

				//for face 3
				Vertex v3;

				p = f3.substr(0, f3.find("/"));
				f3.erase(0, f3.find("/") + 1);

				t = f3.substr(0, f3.find("/"));
				f3.erase(0, f3.find("/") + 1);

				n = f3.substr(0, f3.find("/"));


				v3.Position = positions.at(std::stof(p) - 1);
				v3.Normal = normals.at(std::stof(n) - 1);
				v3.Texture = textures.at(std::stof(t) - 1);
				vertices.push_back(v3);
			}
		}
		//std::cout << positions.size() << std::endl;
		//std::cout << normals.size() << std::endl;
		//std::cout << textures.size() << std::endl;
		std::cout << "Load model with " << vertices.size() << " vertices" << std::endl;

		infile.close();

		numVertices = vertices.size();
	}
	void loadTexture(const char* imagePath, GLenum textureUnit) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* image = stbi_load(imagePath, &width, &height, &channels, STBI_rgb);

		if (image) {
			glActiveTexture(textureUnit); // Activate the specified texture unit
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(image);
		}
		else {
			std::cerr << "Failed to load texture: " << stbi_failure_reason() << std::endl;
		}
	}

	void calculateTangentBitangent(const glm::vec3& edge1, const glm::vec3& edge2, const glm::vec2& deltaUV1, const glm::vec2& deltaUV2, glm::vec3& tangent, glm::vec3& bitangent)
	{
		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);
	}

	// Function to calculate tangent and bitangent vectors for the entire mesh
	void calculateTangentBitangent()
	{
		for (int i = 0; i < numVertices; i += 3)
		{
			glm::vec3 edge1 = vertices[i + 1].Position - vertices[i].Position;
			glm::vec3 edge2 = vertices[i + 2].Position - vertices[i].Position;

			glm::vec2 deltaUV1 = vertices[i + 1].Texture - vertices[i].Texture;
			glm::vec2 deltaUV2 = vertices[i + 2].Texture - vertices[i].Texture;

			glm::vec3 tangent(0.0f), bitangent(0.0f);
			calculateTangentBitangent(edge1, edge2, deltaUV1, deltaUV2, tangent, bitangent);

			// Accumulate tangent and bitangent vectors for each vertex
			vertices[i].Tangent += tangent;
			vertices[i + 1].Tangent += tangent;
			vertices[i + 2].Tangent += tangent;

			vertices[i].Bitangent += bitangent;
			vertices[i + 1].Bitangent += bitangent;
			vertices[i + 2].Bitangent += bitangent;
		}

		// Normalize tangent and bitangent vectors for each vertex
		for (int i = 0; i < numVertices; ++i)
		{
			vertices[i].Tangent = glm::normalize(vertices[i].Tangent);
			vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
		}
	}



	void makeObject(Shader shader, bool texture = true) {
		/* This is a working but not perfect solution, you can improve it if you need/want
		* What happens if you call this function twice on an Model ?
		* What happens when a shader doesn't have a position, tex_coord or normal attribute ?
		*/
		calculateTangentBitangent(); // Assuming this function populates the Tangent and Bitangent vectors in each vertex

		// Adjust the size of the data array to accommodate the new vertex structure
		float* data = new float[14 * numVertices];

		for (int i = 0; i < numVertices; i++) {
			Vertex v = vertices.at(i);

			// Position
			data[i * 14] = v.Position.x;
			data[i * 14 + 1] = v.Position.y;
			data[i * 14 + 2] = v.Position.z;

			// Texture Coordinates
			data[i * 14 + 3] = v.Texture.x;
			data[i * 14 + 4] = v.Texture.y;

			// Normal
			data[i * 14 + 5] = v.Normal.x;
			data[i * 14 + 6] = v.Normal.y;
			data[i * 14 + 7] = v.Normal.z;

			// Tangent
			data[i * 14 + 8] = v.Tangent.x;
			data[i * 14 + 9] = v.Tangent.y;
			data[i * 14 + 10] = v.Tangent.z;

			// Bitangent
			data[i * 14 + 11] = v.Bitangent.x;
			data[i * 14 + 12] = v.Bitangent.y;
			data[i * 14 + 13] = v.Bitangent.z;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		//define VBO and VAO as active buffer and active vertex array
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, data, GL_STATIC_DRAW);


		auto att_pos = glGetAttribLocation(shader.ID, "position");
		if (att_pos != -1) {
			glEnableVertexAttribArray(att_pos);
			glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 14 * sizeof(float), (void*)0);
		}


		if (texture) {
			auto att_tex = glGetAttribLocation(shader.ID, "tex_coord");
			if (att_tex != -1) {
				glEnableVertexAttribArray(att_tex);
				glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 14 * sizeof(float), (void*)(3 * sizeof(float)));
			}

		}
		auto att_col = glGetAttribLocation(shader.ID, "normal");
		if (att_col != -1) {
			glEnableVertexAttribArray(att_col);
			glVertexAttribPointer(att_col, 3, GL_FLOAT, false, 14 * sizeof(float), (void*)(5 * sizeof(float)));
		}

		auto att_tan = glGetAttribLocation(shader.ID, "tangent");
		if (att_tan != -1) {
			glEnableVertexAttribArray(att_tan);
			glVertexAttribPointer(att_tan, 3, GL_FLOAT, false, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		}
		auto att_bitan = glGetAttribLocation(shader.ID, "bitangent");
		if (att_bitan != -1) {
			glEnableVertexAttribArray(att_bitan);
			glVertexAttribPointer(att_bitan, 3, GL_FLOAT, false, 14 * sizeof(float), (void*)(11 * sizeof(float)));
		}
		//desactive the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		delete[] data;

	}

	void draw() {
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}


};
#endif