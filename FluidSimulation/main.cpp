#include<iostream>
#include<string>
#include <filesystem>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>
#include<imgui.h>	
#include <sstream>

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
#include "box.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

float lastX = 400;
float lastY = 300;

float window_height = 1080;
float window_width = 1920;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float keyTime = 0.0f;

float fov = 45.0f;

bool firstMouse = true;
bool showMouse = false;

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

	if(!showMouse)
		camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{


	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !showMouse)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !showMouse)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !showMouse)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !showMouse)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !showMouse)
		camera.ProcessKeyboard(Camera_Movement::UP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !showMouse)
		camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && !showMouse)
		camera.DoubleMovementSpeed = true;
	else
		camera.DoubleMovementSpeed = false;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		//mouse

		float curKeyTime = glfwGetTime();
		float deltaKeyTime = curKeyTime - keyTime;
		keyTime = curKeyTime;
		if (deltaKeyTime > 0.1) {
			showMouse = !showMouse;
			if (showMouse)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
			
}
// Function to convert std::list<int> to string
std::string listToString(const std::list<int>& myList) {
	// Create a stringstream to build the string
	std::stringstream ss;

	// Use an iterator to traverse the list
	for (auto it = myList.begin(); it != myList.end(); ++it) {
		// Append the current element to the stringstream
		ss << *it;

		// Add a comma and space unless it's the last element
		if (std::next(it) != myList.end()) {
			ss << ", ";
		}
	}

	// Convert the stringstream to a string
	return ss.str();
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
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "YoutubeOpenGL", NULL, NULL);

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

	keyTime = glfwGetTime();

	//Load GLAD so it configures OpenGL
	gladLoadGL();


	/*
	Loading IMGUI
	*/

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	bool p_open = true;




	std::vector<GLfloat> lineVertices = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};




	//so Specify the viewport of OpenGL in the window
	glViewport(0, 0, window_width, window_height);
	glEnable(GL_DEPTH_TEST);

	/*  Initializing the Shader Programs      */
	Shader shaderProgram("vertex.shader", "fragment.shader");
	Shader lightShaderProgram("vertex.shader", "light.shader");



	/*Creating the primitives*/

	//Creating the bounding box
	Box boundingBox = Box(20, 30, 30);

	//Creating a Sphere for the light object
	Sphere lightSphere = Sphere(1, 18, 18);
	//creating a sphere for the particles
	Sphere particleSphere = Sphere(1, 36, 18);
	//create a line
	Line lineX = Line(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	Line lineY = Line(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	Line lineZ = Line(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	

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

	float maxBoxLength = boundingBox.length;
	float maxBoxWidth = boundingBox.width;
	float maxBoxHeight = boundingBox.height;
	

	/*
	Particle Simulation right here. 
	*/
	//21y6 is nice
	Engine engine = Engine(particleSphere.vertices, particleSphere.indices, 343, 0.5f, glm::vec3(boundingBox.width,boundingBox.height,boundingBox.length));
	//we know make the particles
	camera.Position = glm::vec3(0.0, 0.0, 20.0);

	bool playSim = false;
	bool simStart = true;
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		//ImGui setup
		ImGui:ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Simulation Settings", &p_open, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::InputFloat("Particle Size", &engine.particleSize, 0.01f, 1.0f, "%.4f");
		ImGui::InputFloat("Kernel size", &engine.kernelRadius, engine.particleSize, 5*engine.particleSize, "%.4f");
		ImGui::SliderFloat("Pressure Multipler", &engine.pressureMultiplier, 1.0f, 100.0f, "%.4f");
		ImGui::SliderFloat("Gravitational Force", &engine.gravitationalAcceleration, 0.0f, 10.0, "%.4f");
		std::string text = "Max Velocity: " + std::to_string(engine.maxSpeed);
		std::string text2 = "Max PressureForce: " + std::to_string(engine.maxPressureForce);
		std::string text3 = "Rest Desnity: " + std::to_string(engine.restDensity);

		ImGui::Text(text.c_str());
		ImGui::Text(text2.c_str());
		ImGui::Text(text3.c_str());

		std::string fps_text = "FPS : " + std::to_string(1.0f/(deltaTime));
		ImGui::Text(fps_text.c_str());
		std::string particleInfo = "Particle Info \nParticle Pressure Force : " + std::to_string(glm::length(engine.Particles[0].pressureForce))
			+ "\nParticle Viscosity Force: " + std::to_string(glm::length(engine.Particles[0].viscosityForce))
			+ "\nParticle density: " + std::to_string(engine.Particles[0].density)
			+ "\nParticle Acceleration: " + std::to_string(glm::length(engine.Particles[0].Acceleration))
			+ "\nParticle Velocity: " + std::to_string(glm::length(engine.Particles[0].Velocity));
		ImGui::Text(particleInfo.c_str());

		if (ImGui::Button("Play/Pause")) {
			playSim = !playSim;
			simStart = false;
		}
		if (simStart && ImGui::SliderInt("numParticles", &engine.numParticles, 1, 100)) {
			engine.arrangeParticles();
		}
		if (simStart && ImGui::SliderFloat("particleSpacing", &engine.particleSpacing, 2*engine.particleSize, 1.0)) {
			engine.arrangeParticles();
		}
		ImGui::Text("Box Dimensions");
		if (ImGui::SliderFloat("Box Length", &boundingBox.length, 1.0, 5* maxBoxLength)
			|| ImGui::SliderFloat("Box Width", &boundingBox.width, 1.0, 5 * maxBoxWidth)
			|| ImGui::SliderFloat("Box Height", &boundingBox.height, 1.0, 5 * maxBoxHeight))
		{
			//we got to update the bounding box 
			boundingBox.updateBox();
			engine.updateBoundingBox(boundingBox.length, boundingBox.width, boundingBox.height);
		}

		ImGui::End();



		//Time calculations
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/* Processing Input from window*/
		processInput(window);
		glClearColor(0.133, 0.247, 0.322, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

		


		/* model, view and projection matrices */


		//Defines the world location/view of the models.
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();


		//we put the world into perspective view.
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), window_height / 600.0f, 0.1f, 100.0f);




		/* Light calculations around here  */
		//make it orbit around 0, 0
		glm::vec3 lightPos = glm::vec3(0, 0, 10);

		// Model Matrix for lighting.
		glm::mat4 modelMatrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)), lightPos);
		

	

		
		lightShaderProgram.Activate(); //activating a light shader
		lightShaderProgram.setMat4("model", modelMatrix);
		lightShaderProgram.setMat4("view", view);
		lightShaderProgram.setMat4("projection", projection);




		//we use the light sphere.
		lightSphere.DrawSphere();



		/*  Drawing a Line  */
		modelMatrix = glm::mat4(1.0f);


		shaderProgram.Activate(); //activating a light shader

		shaderProgram.setMat4("model", modelMatrix);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);



		shaderProgram.setVec3("light.ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		shaderProgram.setVec3("light.diffuse", glm::vec3(1.0f, 0.0f, 0.0f)); // darken diffuse light a bit
		shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
		
		lineX.DrawLine();



		shaderProgram.setVec3("light.ambient", glm::vec3(0.0f, 1.0f, 0.0f));
		shaderProgram.setVec3("light.diffuse", glm::vec3(0.0f, 1.0f, 0.0f)); // darken diffuse light a bit
		shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f));

		lineY.DrawLine();


		shaderProgram.setVec3("light.ambient", glm::vec3(0.0f, 0.0f, 1.0f));
		shaderProgram.setVec3("light.diffuse", glm::vec3(0.0f, 0.0f, 1.0f)); // darken diffuse light a bit
		shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f));
		lineZ.DrawLine();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		boundingBox.DrawBox();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		shaderProgram.Activate();
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setVec3("viewPos", camera.Position);

		//Light settings
		shaderProgram.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shaderProgram.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		shaderProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//update the physics engine
		if(playSim)
			engine.update(deltaTime);

		for (int i = 0; i < engine.numParticles; i++) {

			glm::mat4 modelMatrix = engine.ModelMatrices[i];
			Particle particle = engine.Particles[i];

			//glm::vec3 particlePos = glm::vec3(0.5f, 0.0f, -5.0f);

			//modelMatrix = glm::translate(glm::scale(modelMatrix, glm::vec3(0.2f)), particlePos);
			//calculate the normal matrix
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));


			//got to fix the lighting issue. 
			shaderProgram.setVec3("light.position", lightPos - particle.Position);
			shaderProgram.setVec3("objectColor", engine.getColor2(glm::length(particle.Velocity)));
			shaderProgram.setMat4("model", modelMatrix);
			shaderProgram.setMat3("normalMatrix", normalMatrix);

			//object material settings.
			shaderProgram.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			shaderProgram.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			shaderProgram.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			shaderProgram.setFloat("material.shininess", 32.0f);
			


			particleSphere.DrawSphere();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
