#include<iostream>
#include<string>
#include <filesystem>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "stb_image.h"	
#include "texture.h"
#include "camera.h"

float lastX = 400;
float lastY = 300;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

float fov = 45.0f;

bool firstMouse = true;

Camera camera;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	/*
	Mostly can be moved to the mouse class
	*/
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback	(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

int main() {

	glfwInit();


	// Telling GLFW (the window manager) what version of OPENGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLfloat vertices[] =
	{
		//Vertex positions		Colors						Textures
		//front face
		 0.5f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,// top right    0
		 0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f, 	1.0f, 0.0f,// bottom right 1
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f, 	0.0f, 0.0f,// bottom left  2
		-0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 0.0f, 	0.0f, 1.0f,// top left     3

		//backface
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f , 	1.0f, 1.0f,// top right    4
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,// bottom right 5
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f, 	0.0f, 0.0f,// bottom left  6
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 0.0f, 	0.0f, 1.0f// top left      7


	};


	GLuint indices[] =
	{
		//front face
		0, 1, 3,
		1, 2, 3,

		//backface
		4,5,7,
		5,6,7,

		//top face
		0, 3,4,
		3, 4, 7,

		//bottom face
		1, 2, 5,
		2, 5, 6,

		//right face
		4, 5, 1,
		4, 1, 0,

		//left face
		2,  3, 6,
		3, 6, 7
		
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};



	// Creating a window object called YOUTUBE OpenGL of a size 800x800 pixells	.
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);

	// Incase the window creation fails.
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	//Load GLAD so it configures OpenGL
	gladLoadGL();

	//so Specify the viewport of OpenGL in the window
	glViewport(0, 0, 800, 800);
	glEnable(GL_DEPTH_TEST);

	Shader shaderProgram("vertex.shader", "fragment.shader");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	

	//Texture coordinates
	//Link texture array via VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void *)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float))); 

	//Texture texture("container.jpg");


	//

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Texture texture1("container.jpg");
	Texture texture2("awesomeface.png");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //enable and disable wireframe mode

	shaderProgram.Activate();
	shaderProgram.setInt("texture1", 0); // don't forget to activate the shader before setting uniforms!  
	shaderProgram.setInt("texture2", 1);


	//Initialied functions

	/*Camera matrix*/
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


	glm::vec3 direction;


	






	// Main while loop
	while (!glfwWindowShouldClose(window))
	{


		//time calculations
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//we create the colors of the tiangle
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram.id, "ourColor");

		/* Include all mathematics over here*/

		//const float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;




		//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);


		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //up vector in world space
		//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		//glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

		


		//direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		//direction.y = sin(glm::radians(pitch));
		//direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		//cameraFront = glm::normalize(direction);

		/* model, view and projection matrices */


		//defines the matrix of the models position and rotation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float) glfwGetTime()* glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		
		//defines the world location/view of the models.
		glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = camera.GetViewMatrix();


		//we put the world into perspective view.
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		
		//float timeValue = glfwGetTime();
		//float angle = sin(timeValue) * 90.0f;
		//
		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0, 1.0, 1.0));

		unsigned int modelLoc = glGetUniformLocation(shaderProgram.id, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		unsigned int viewLoc = glGetUniformLocation(shaderProgram.id, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int projectionLoc = glGetUniformLocation(shaderProgram.id, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		/*End it around here*/
		//activate texture unit
		glActiveTexture(GL_TEXTURE0);
		texture1.Bind();
		glActiveTexture(GL_TEXTURE1);
		texture2.Bind();


		VAO1.Bind();
		shaderProgram.Activate();
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setMat4("model", model);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}


		
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		//here we draw the triangles
		// 
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		
		
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);



		glfwPollEvents();
	}

	
	//opengl deletes
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	//Delete the windows and destroy them
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
