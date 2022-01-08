#define GLFW_DLL
#define GLEW32_DLL



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "stb_image.h"

#include "OBJ_Loader.h"
#include"Texture.h"
#include"shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"




using namespace std;

GLFWwindow* window = NULL;

const unsigned int width = 1024;
const unsigned int height = 720;




void InitializeOpenGL(int x, int y) {
	if (!glfwInit())
		throw std::runtime_error("glfwInit failed");

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(x, y, "test window", NULL, NULL);
	if (!window)
		throw std::runtime_error("glfwOpenWindow failed.");
	
	// GLFW settings
	glfwMakeContextCurrent(window);

	glewInit();
	
	int nx, ny;
	glfwGetFramebufferSize(window, &nx, &ny);
	glViewport(0, 0, nx, ny);
	//glewExperimental = GL_TRUE;
	

	// ispisi neke informacije
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("renderer: %s\n", renderer);
	printf("OpenGL version supported: %s\n", version);

	// definiraj z-spremnik i test dubine
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);  // manja z-vrijednost jest blizi predmet
}


GLfloat vert1[396];
GLfloat vert2[396];
GLfloat vert3[198];
GLfloat vert4[198];
GLuint ind1[36];
GLuint ind2[36];
GLuint ind3[18];
GLuint ind4[18];

void createObjects() {
	vector<float> vec;
	objl::Loader loader1;
	loader1.LoadFile("cube1.obj");
	

	for (auto ver : loader1.LoadedVertices) {
		vec.push_back(ver.Position.X);
		vec.push_back(ver.Position.Y);
		vec.push_back(ver.Position.Z);
		vec.push_back(0.83f);
		vec.push_back(0.70f);
		vec.push_back(0.44f);
		vec.push_back(ver.TextureCoordinate.X);
		vec.push_back(ver.TextureCoordinate.Y);
		vec.push_back(ver.Normal.X);
		vec.push_back(ver.Normal.Y);
		vec.push_back(ver.Normal.Z);
	}
	int i = 0;
	for (auto ind : loader1.LoadedIndices) {
		ind1[i] = ind;
		i++;
	}
	i = 0;
	for (auto vert : vec) {
		vert1[i] = vert;
		i++;
	}
	vec.clear();

	objl::Loader loader2;
	loader1.LoadFile("cube2.obj");


	for (auto ver : loader2.LoadedVertices) {
		vec.push_back(ver.Position.X);
		vec.push_back(ver.Position.Y);
		vec.push_back(ver.Position.Z);
		vec.push_back(0.83f);
		vec.push_back(0.70f);
		vec.push_back(0.44f);
		vec.push_back(ver.TextureCoordinate.X);
		vec.push_back(ver.TextureCoordinate.Y);
		vec.push_back(ver.Normal.X);
		vec.push_back(ver.Normal.Y);
		vec.push_back(ver.Normal.Z);
	}
	i = 0;
	for (auto ind : loader2.LoadedIndices) {
		ind2[i] = ind;
		i++;
	}
	i = 0;
	for (auto vert : vec) {
		vert2[i] = vert;
		i++;
	}
	vec.clear();

	objl::Loader loader3;
	loader1.LoadFile("pyramid1.obj");


	for (auto ver : loader3.LoadedVertices) {
		vec.push_back(ver.Position.X);
		vec.push_back(ver.Position.Y);
		vec.push_back(ver.Position.Z);
		vec.push_back(0.83f);
		vec.push_back(0.70f);
		vec.push_back(0.44f);
		vec.push_back(ver.TextureCoordinate.X);
		vec.push_back(ver.TextureCoordinate.Y);
		vec.push_back(ver.Normal.X);
		vec.push_back(ver.Normal.Y);
		vec.push_back(ver.Normal.Z);
	}
	i = 0;
	for (auto ind : loader3.LoadedIndices) {
		ind3[i] = ind;
		i++;
	}
	i = 0;
	for (auto vert : vec) {
		vert3[i] = vert;
		i++;
	}
	vec.clear();

	objl::Loader loader4;
	loader1.LoadFile("pyramid2.obj");


	for (auto ver : loader4.LoadedVertices) {
		vec.push_back(ver.Position.X);
		vec.push_back(ver.Position.Y);
		vec.push_back(ver.Position.Z);
		vec.push_back(0.83f);
		vec.push_back(0.70f);
		vec.push_back(0.44f);
		vec.push_back(ver.TextureCoordinate.X);
		vec.push_back(ver.TextureCoordinate.Y);
		vec.push_back(ver.Normal.X);
		vec.push_back(ver.Normal.Y);
		vec.push_back(ver.Normal.Z);
	}
	i = 0;
	for (auto ind : loader4.LoadedIndices) {
		ind4[i] = ind;
		i++;
	}
	i = 0;
	for (auto vert : vec) {
		vert4[i] = vert;
		i++;
	}
	vec.clear();

}


void createLight() {
	objl::Loader loader;
	loader.LoadFile("Light.obj");

	vector<float> vec;
	GLfloat vertLight[240];
	GLuint indLight[240];
	
	for (auto ver : loader.LoadedVertices) {
		vec.push_back(ver.Position.X);
		vec.push_back(ver.Position.Y);
		vec.push_back(ver.Position.Z);
	}
	int i = 0;
	for (auto ind : loader.LoadedIndices) {
		indLight[i] = ind;
		i++;
	}
	i = 0;
	for (auto vert : vec) {
		vertLight[i] = vert;
		i++;
	}

	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(vertLight, sizeof(vertLight));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(indLight, sizeof(indLight));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
}

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};



int main()
{
	// Initialize GLFW
	InitializeOpenGL(width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("shader.vert", "shader.frag");


	createObjects();

	VAO VAO2;
	VAO VAO3;
	VAO VAO4;
	VAO VAO5;

	VAO2.Bind();
	VAO3.Bind();
	VAO4.Bind();
	VAO5.Bind();
	
	VBO VBO2(vert1, sizeof(vert1));
	VBO VBO3(vert2, sizeof(vert2));
	VBO VBO4(vert3, sizeof(vert3));
	VBO VBO5(vert4, sizeof(vert4));

	EBO EBO2(ind1, sizeof(ind1));
	EBO EBO3(ind2, sizeof(ind2));
	EBO EBO4(ind3, sizeof(ind3));
	EBO EBO5(ind4, sizeof(ind4));

	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO2.LinkAttrib(VBO2, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	VAO3.LinkAttrib(VBO3, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO3.LinkAttrib(VBO3, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO3.LinkAttrib(VBO3, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO3.LinkAttrib(VBO3, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	VAO4.LinkAttrib(VBO4, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO4.LinkAttrib(VBO4, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO4.LinkAttrib(VBO4, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO4.LinkAttrib(VBO4, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	VAO5.LinkAttrib(VBO5, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO5.LinkAttrib(VBO5, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO5.LinkAttrib(VBO5, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO5.LinkAttrib(VBO5, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();


	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));


	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	objl::Loader loader;
	loader.LoadFile("Light.obj");

	vector<float> vec;
	GLfloat vertLight[720];
	GLuint indLight[240];

	for (auto ver : loader.LoadedVertices) {
		vec.push_back(ver.Position.X);
		vec.push_back(ver.Position.Y);
		vec.push_back(ver.Position.Z);
	}
	int i = 0;
	for (auto ind : loader.LoadedIndices) {
		indLight[i] = ind;
		i++;
	}
	i = 0;
	for (auto vert : vec) {
		vertLight[i] = vert;
		i++;
	}

	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(vertLight, sizeof(vertLight));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(indLight, sizeof(indLight));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(2.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	
	//glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	//glm::mat4 lightModel = glm::mat4(1.0f);
	glm::vec3 pyramidPos = glm::vec3(-5.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	//std::string texPath = "/Resources/YoutubeOpenGL 6 - Textures/";

	// Texture;

	//Original code from the tutorial
	Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);


	
	Camera camera(1024, 720, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		
		camera.Inputs(window);

		// Updates and exports the camera matrix to the Vertex Shader
		

		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);


		camera.Matrix(shaderProgram, "camMatrix");
		
		

		
		// Binds texture so that is appears in rendering
		popCat.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		/*
		VAO2.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(ind1) / sizeof(int), GL_UNSIGNED_INT, 0);
		
		VAO3.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(ind2) / sizeof(int), GL_UNSIGNED_INT, 0);

		VAO4.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(ind3) / sizeof(int), GL_UNSIGNED_INT, 0);

		VAO5.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(ind4) / sizeof(int), GL_UNSIGNED_INT, 0);
		*/

		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indLight) / sizeof(int), GL_UNSIGNED_INT, 0);

























		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			camera.updateMatrix(45.0f, 0.1f, 100.0f, true);
		}
		else {
			camera.updateMatrix(45.0f, 0.1f, 100.0f, false);
		}

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

