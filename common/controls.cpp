// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(-5.078834, 0.406995, 8.517701);
// Initial horizontal angle : toward -Z
float horizontalAngle = 2.18f;
// Initial vertical angle : none
float verticalAngle = -0.21f;
// Initial Field of View
float initialFoV = 55.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

int updateCamera = 0;

void computeMatricesFromInputs(){

	if (!jogo) {
		speed = 0.0f; 
		mouseSpeed = 0.00f;
	}
	else {
		speed = 3.0f; 
		mouseSpeed = 0.005f;
	}

	if (!glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS ||
		!glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS ||
		!glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		if (updateCamera == 1) {
			position = glm::vec3(-5.078834, 0.406995, 8.517701);
			horizontalAngle = 2.18f;
			verticalAngle = -0.21f;
			initialFoV = 55.0f;
			updateCamera++;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		position = glm::vec3(-3.416483, -2.345027, 2.608511);
		horizontalAngle = 2.64f;
		verticalAngle = 0.10f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		position = glm::vec3(1.588993, -2.024116, 0.457905);
		horizontalAngle = 2.81f;
		verticalAngle = 0.04f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		position = glm::vec3(2.809351, -2.024116, 0.883819);
		horizontalAngle = 2.81f;
		verticalAngle = 0.04f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		position = glm::vec3(4.395720, -2.014686, 1.181107);
		horizontalAngle = 2.87f;
		verticalAngle = 0.02f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		position = glm::vec3(7.199567, -1.995224, 0.950753);
		horizontalAngle = 2.96f;
		verticalAngle = 0.0f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		position = glm::vec3(12.366167, -1.995244, 2.674694);
		horizontalAngle = 2.81f;
		verticalAngle = -0.01f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		position = glm::vec3(16.912003, -2.010592, 2.860157);
		horizontalAngle = 2.81f;
		verticalAngle = -0.02f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		position = glm::vec3(21.121223, -2.051227, 2.451538);
		horizontalAngle = 2.91f;
		verticalAngle = 0.01f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		position = glm::vec3(25.558681, -2.014556, 2.122228);
		horizontalAngle = 2.75f;
		verticalAngle = -0.01f;
		speed = 0.0f;
		mouseSpeed = 0.00f;
		updateCamera = 1;
	}

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	//Condição que faz com que a câmera se mova mais rápido
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 3.0f * 15;
	}
	else {
		speed = 3.0f;
	}

	// Move forward
	if ((glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && jogo == true){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if ((glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && jogo == true){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if ((glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && jogo == true){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if ((glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && jogo == true){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV; //Field of view will determine how big is out peripheral vision, if it´s to small it gives the impression we are really close to the object

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 1500.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}