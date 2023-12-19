#include<iostream>
#include<string>
#include <filesystem>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "stb_image.h"	
#include "texture.h"
#include "camera.h"
#include "engine.h"
#include "sphere.h"
#include "line.h"


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







	std::vector<GLfloat> lineVertices = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};




	//so Specify the viewport of OpenGL in the window
	glViewport(0, 0, 800, 800);
	glEnable(GL_DEPTH_TEST);

	Shader shaderProgram("vertex.shader", "fragment.shader");
	Shader lightShaderProgram("vertex.shader", "light.shader");



	//Creating a Sphere for the light object
	Sphere lightSphere = Sphere(1, 36, 18);
	//creating a sphere for the particles
	Sphere particleSphere = Sphere(1, 36, 18);
	//create a line
	Line line = Line(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));


	

	//Texture texture1("container.jpg");
	//Texture texture2("awesomeface.png");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //enable and disable wireframe mode


	//shaderProgram.setInt("texture1", 0); // don't forget to activate the shader before setting uniforms!  
	//shaderProgram.setInt("texture2", 1);



	//Initialied functions

	/*Camera matrix*/
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


	glm::vec3 direction;


	

	/*
	Particle Simulation right here. 
	*/

	Engine engine = Engine(particleSphere.vertices, particleSphere.indices, 1);
	//we know make the particles


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

		//drawing the light
		



		/* model, view and projection matrices */


	//	defines the matrix of the models position and rotation

		//make it orbit around 0, 0
		float radius = 7.0;
		glm::vec3 lightPos = glm::vec3(radius * sin(glfwGetTime()), 0, radius * cos(glfwGetTime()));

		float changeVar = sin(glfwGetTime());
		//glm::vec3 lightPos = glm::vec3(-1.0 * changeVar, 0.0, -6.0 );
		glm::mat4 modelMatrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)), lightPos);
		
		//model = glm::rotate(model, (float) glfwgettime()* glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/

		
		//defines the world location/view of the models.
		glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		view = camera.GetViewMatrix();


		//we put the world into perspective view.
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);


		
		lightShaderProgram.Activate(); //activating a light shader

		lightShaderProgram.setMat4("model", modelMatrix);
		lightShaderProgram.setMat4("view", view);
		lightShaderProgram.setMat4("projection", projection);


		shaderProgram.setVec3("light.position", lightPos);


		//we use the light sphere.
		lightSphere.DrawSphere();




		//we want to bind the VAO for the lines.
		

		//we change 
		
		modelMatrix = glm::mat4(1.0f);
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f));


		shaderProgram.Activate(); //activating a light shader

		shaderProgram.setMat4("model", modelMatrix);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);



		shaderProgram.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
		
		line.DrawLine();

		/*End it around here*/
		////activate texture unit
		//glActiveTexture(GL_TEXTURE0);
		//texture1.Bind();
		//glActiveTexture(GL_TEXTURE1);
		//texture2.Bind();
		

		shaderProgram.Activate();
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setVec3("viewPos", camera.Position);

		//Light settings
		shaderProgram.setVec3("light.position", lightPos);
		shaderProgram.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//update the physics engine
		//engine.update(deltaTime);

		for (int i = 0; i < engine.numParticles; i++) {

			glm::mat4 modelMatrix = engine.ModelMatrices[i];
			Particle particle = engine.Particles[i];

			//glm::vec3 particlePos = glm::vec3(0.5f, 0.0f, -5.0f);
			glm::vec3 particlePos = glm::vec3(0.0);

			modelMatrix = glm::translate(glm::scale(modelMatrix, glm::vec3(0.2f)), particlePos);
			//calculate the normal matrix
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

			shaderProgram.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
			shaderProgram.setMat4("model", modelMatrix);
			shaderProgram.setMat3("normalMatrix", normalMatrix);

			//object material settings.
			shaderProgram.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			shaderProgram.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			shaderProgram.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			shaderProgram.setFloat("material.shininess", 32.0f);
			


			particleSphere.DrawSphere();
		}


		glfwSwapBuffers(window);



		glfwPollEvents();
	}

	
	////opengl deletes
	//VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();
	//we want
	shaderProgram.Delete();
	lightShaderProgram.Delete();
	//Delete the windows and destroy them
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
