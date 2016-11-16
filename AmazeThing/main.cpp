#include <iostream>
#include <cmath>
#include <vector>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Shapes.h"
#include "Map.h"
#include "Collision.h"


void shader(Map &map, Shader & lightingShader, Shader & lampShader, GLuint & containerVAO, GLuint & lightVAO, std::map<int, std::map<int, glm::mat4>> &mModel);

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement(GLboolean collision);

// Window dimensions
const GLuint WIDTH = 640, HEIGHT = 480;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 4.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
GLfloat pathPri = 0.0f;
GLboolean reversed = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

							// The MAIN function, from here we start the application and run the game loop
int main()
{
	Shapes shapes;
	Map map;
	std::vector<GLfloat> vertices;
	GLuint VBO, containerVAO, lightVAO;
	Collision col;

	// Store models for collision detection
	std::vector<glm::mat4> vModel;
	std::vector<std::vector<int>> vModelXZ;
	std::map<int, std::map<int, glm::mat4>> mModel;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 1000, 60);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);


	// Build and compile our shader program
	Shader lightingShader("lighting.vs", "lighting.frag");
	Shader lampShader("lamp.vs", "lamp.frag");

	
	vertices = shapes.getShape("cube");

	// First, set the container's VAO (and VBO)

	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// call shader
		shader(map, lightingShader, lampShader, containerVAO, lightVAO, mModel);

		// collision detection
		GLboolean collision = col.checkCollision(mModel, camera.Position);
		do_movement(collision);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void shader(Map &map, Shader &lightingShader, Shader &lampShader, GLuint &containerVAO, GLuint &lightVAO, std::map<int, std::map<int,glm::mat4>> &mModel) {

	// Use cooresponding shader when setting uniforms/drawing objects
	lightingShader.Use();
	GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");

	GLfloat radius = 2.0f;
	GLfloat camX = sin(glfwGetTime()) * radius;
	GLfloat camZ = cos(glfwGetTime()) * radius;


	// I AM RETARDED
	if (reversed) {
		pathPri -= 0.005f;
	}
	else {
		pathPri += 0.005f;
	}
	if (pathPri < -9.5f) {
		reversed = false;
	}
	else if (pathPri > 2.0f) {
		reversed = true;
	}

	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
	glUniform3f(lightColorLoc, camX, 0.5f, camZ);
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z + pathPri);
	//glUniform3f(lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
	//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

	// Create camera transformations
	glm::mat4 view;
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
	GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
	GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	// Draw the container (using container's vertex attributes)
	glBindVertexArray(containerVAO);
	glm::mat4 model;
	//model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::translate(model, glm::vec3(5, 0, 0));
	//model = glm::rotate(model, 90.0f, glm::vec3(0.1f, 0.0f, 0.0f));

	for (auto line : map.getMap()) {
		for (auto column : line) {
			if (column == 1) {
				model = glm::translate(model, glm::vec3(-1, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
				//std::cout << "x: " << model[0].x << " y: " << model[0].y << " z: " << model[0].z << std::endl;
				mModel[(int)model[3].x][(int)model[3].z] = model;
				//vModel.push_back(model);
				

			}
			else {
				model = glm::translate(model, glm::vec3(-1, 0, 0));
			}
		}
		model = glm::translate(model, glm::vec3(line.size(), 0, -1));
	}


	// Also draw the lamp object, again binding the appropriate shader
	lampShader.Use();
	// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
	modelLoc = glGetUniformLocation(lampShader.Program, "model");
	viewLoc = glGetUniformLocation(lampShader.Program, "view");
	projLoc = glGetUniformLocation(lampShader.Program, "projection");



	// Set matrices
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	model = glm::mat4();

	model = glm::translate(model, glm::vec3(lightPos.x, lightPos.y, lightPos.z + pathPri));

	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	// Draw the light object (using light's vertex attributes)
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

GLchar keyChar;
void do_movement(GLboolean collision)
{
	// Make walls inpassable, do not just hinder camera movement
	// Camera controls
	if (keys[GLFW_KEY_W] && !collision) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
		keyChar = 'W';
	}
	if (keys[GLFW_KEY_S]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		keyChar = 'S';
	}
	if (keys[GLFW_KEY_A]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
		keyChar = 'A';
	}
	if (keys[GLFW_KEY_D]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
		keyChar = 'D';
	}
	if (keys[GLFW_KEY_Q]) {
		camera.ProcessKeyboard(DOWN, deltaTime);

	}
	if (keys[GLFW_KEY_E]) {
		camera.ProcessKeyboard(UP, deltaTime);

	}
	if (keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(ROTATELEFT, deltaTime);
	}
	if (keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(ROTATERIGHT, deltaTime);

	}
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}