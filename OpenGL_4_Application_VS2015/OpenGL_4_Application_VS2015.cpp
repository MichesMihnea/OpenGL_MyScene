//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC

#include <iostream>
#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"
#define TINYOBJLOADER_IMPLEMENTATION

#include "Model3D.hpp"
#include "Mesh.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														DIMENSIUNI
//											La umbra nu pare sa afecteze modificarile foarte tare
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int glWindowWidth = 1920;
int glWindowHeight = 1080;
int retina_width, retina_height;
const GLuint SHADOW_WIDTH = 15000, SHADOW_HEIGHT = 15000;
GLFWwindow* glWindow = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														Shadereala
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

glm::mat4 modelMatrix;
GLuint modelMatrixUniformLocation;
glm::mat4 viewMatrix;
GLuint viewMatrixUniformLocation;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
GLfloat sunRotationAngle;
GLuint fogUniformLocation;
glm::vec3 lightDirectionMatrix;
GLuint lightDirectionMatrixLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;
glm::vec3 lightPosition;
GLuint lightPositionUniformLocation;
glm::vec3 lightPosition2;
GLuint lightPositionUniformLocation2;
GLuint darkLoc;
glm::mat3 lightDirectionMatrixMatrix;
GLuint lightDirectionMatrixUniformLocation;
GLuint depthMapTexture;
GLuint shadowMapFBO;
float lightX = 0.0f;
float lightY = 200.0f;
float lightZ = 0.0f;
float lightX2 = -3.0f;
float lightY2 = 14.0f;
float lightZ2 = 14.0f;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//												Ce efecte mai dam din taste
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int animOn;
bool visibleFog = false;
int dark = 0;
int animCnt = 0;
int isTour = 0;
int startTour = 0;
int phase = 0;
int count = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														Programele
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

gps::SkyBox mySkyBox;
gps::Shader skyboxShader;
gps::Shader depthMapShader;
gps::Shader myCustomShader;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														Camera
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////




gps::Camera myCamera(glm::vec3(0.0f, 5.0f, 2.5f), glm::vec3(0.0f, 0.0f, -10.0f));
float cameraSpeed = 1.0f;

bool pressedKeys[1024];
float sceneRotationAngle = 0.0f;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														MODELE DE INCARCAT
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

gps::Model3D ground;
gps::Model3D boat;
gps::Model3D cottage;
gps::Model3D lake;
gps::Model3D lake1;
gps::Model3D lake2;
gps::Model3D lake3;
gps::Model3D lake4;
gps::Model3D lake5;
gps::Model3D lake6;
gps::Model3D lake7;
gps::Model3D lake8;
gps::Model3D lake9;
gps::Model3D lake10;
gps::Model3D lake11;
gps::Model3D lake12;
gps::Model3D lake13;
gps::Model3D lake14;
gps::Model3D lake15;
gps::Model3D lake16;
gps::Model3D lake17;
gps::Model3D ice;
gps::Model3D sun;
gps::Model3D lamp;
gps::Model3D well;
gps::Model3D trees;
gps::Model3D tower;
gps::Model3D myaxe;
gps::Model3D mypickaxe;
gps::Model3D penguin;
gps::Model3D penguin1;
gps::Model3D penguin2;
gps::Model3D penguin3;
gps::Model3D penguin4;
gps::Model3D penguin5;
gps::Model3D penguin6;

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	//set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set Viewport transform
	glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	myCamera.mouse_callback(xpos, ypos);
}
int isWireFrame = 0;
void processMovement()
{

	if (pressedKeys[GLFW_KEY_Q]) {
		sceneRotationAngle += 1.0f;
		if (sceneRotationAngle > 360.0f)
			sceneRotationAngle -= 360.0f;
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sceneRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(glm::vec3(-lightX2, lightY2, -lightZ2), 1.0f));
		lightPosition2 = lightDirectionMatrixTr;
		lightPositionUniformLocation2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosition2");
		myCustomShader.useShaderProgram();
		glUniform3fv(lightPositionUniformLocation2, 1, glm::value_ptr(lightPosition2));
	}

	if (pressedKeys[GLFW_KEY_E]) {
		sceneRotationAngle -= 1.0f;
		if (sceneRotationAngle < 0.0f)
			sceneRotationAngle += 360.0f;
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sceneRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(glm::vec3(-lightX2, lightY2, -lightZ2), 1.0f));
		lightPosition2 = lightDirectionMatrixTr;
		lightPositionUniformLocation2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosition2");
		myCustomShader.useShaderProgram();
		glUniform3fv(lightPositionUniformLocation2, 1, glm::value_ptr(lightPosition2));
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_N]) {
		sunRotationAngle += 0.3f;
		if (sunRotationAngle > 360.0f)
			sunRotationAngle -= 360.0f;
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_M]) {
		sunRotationAngle -= 0.3f;
		if (sunRotationAngle < 0.0f)
			sunRotationAngle += 360.0f;
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_X]) {
		isWireFrame += 1;
		if (isWireFrame > 50) {
			if (isWireFrame > 100) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			}
			else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		if (isWireFrame > 150) {
			isWireFrame = 0;
		}
	}

	if (pressedKeys[GLFW_KEY_T]) {
		isTour += 1;
		if (isTour > 50) {
			if (startTour == 0) {
				startTour = 1;
				phase = 0;
				count = 0;
				myCamera = gps::Camera(glm::vec3(200.0f, 200.0f, 2.5f), glm::vec3(0.0f, 0.0f, -10.0f));
			}
			else startTour = 0;
			isTour = 0;
		}
	}

	if (pressedKeys[GLFW_KEY_F]) {
		visibleFog = true;
	}

	if (pressedKeys[GLFW_KEY_G]) {
		visibleFog = false;
	}

	if (pressedKeys[GLFW_KEY_C]) {
		dark = 1;
	}

	if (pressedKeys[GLFW_KEY_V]) {
		dark = 0;
	}

	if (pressedKeys[GLFW_KEY_LEFT]) {
		lightX -= 2.0f;
		lightDirectionMatrix = glm::vec3(lightX, lightY, lightZ);
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_RIGHT]) {
		lightX += 2.0f;
		lightDirectionMatrix = glm::vec3(lightX, lightY, lightZ);
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_UP]) {
		lightY += 2.0f;
		lightDirectionMatrix = glm::vec3(lightX, lightY, lightZ);
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_DOWN]) {
		lightY -= 2.0f;
		lightDirectionMatrix = glm::vec3(lightX, lightY, lightZ);
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_PAGE_UP]) {
		lightZ += 2.0f;
		lightDirectionMatrix = glm::vec3(lightX, lightY, lightZ);
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_PAGE_DOWN]) {
		lightZ -= 2.0f;
		lightDirectionMatrix = glm::vec3(lightX, lightY, lightZ);
		glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDirectionMatrix, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrixTr));
	}

	if (pressedKeys[GLFW_KEY_B]) {
		animCnt += 1;
		if (animCnt > 50) {
			animOn = 1;
		}
		else {
			animOn = 0;
		}
		if (animCnt > 100) {
			animCnt = 0;
		}
	}
}

void initShadowMap()
{
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels()
{
	ground = gps::Model3D("objects/mygnd/mygnd.obj", "objects/mygnd/");
	lake = gps::Model3D("objects/water/water.obj", "objects/water/");
	lake1 = gps::Model3D("objects/water/water1.obj", "objects/water/");
	lake2 = gps::Model3D("objects/water/water2.obj", "objects/water/");
	lake3 = gps::Model3D("objects/water/water3.obj", "objects/water/");
	lake4 = gps::Model3D("objects/water/water4.obj", "objects/water/");
	lake5 = gps::Model3D("objects/water/water5.obj", "objects/water/");
	lake6 = gps::Model3D("objects/water/water6.obj", "objects/water/");
	lake7 = gps::Model3D("objects/water/water7.obj", "objects/water/");
	lake8 = gps::Model3D("objects/water/water8.obj", "objects/water/");
	lake9 = gps::Model3D("objects/water/water9.obj", "objects/water/");
	lake10 = gps::Model3D("objects/water/water10.obj", "objects/water/");
	lake11 = gps::Model3D("objects/water/water11.obj", "objects/water/");
	lake12 = gps::Model3D("objects/water/water12.obj", "objects/water/");
	lake13 = gps::Model3D("objects/water/water13.obj", "objects/water/");
	lake14 = gps::Model3D("objects/water/water14.obj", "objects/water/");
	lake15 = gps::Model3D("objects/water/water15.obj", "objects/water/");
	lake16 = gps::Model3D("objects/water/water16.obj", "objects/water/");
	lake17 = gps::Model3D("objects/water/water17.obj", "objects/water/");
	sun = gps::Model3D("objects/sun/sun.obj", "objects/sun/");
	ice = gps::Model3D("objects/ice/ice.obj", "objects/ice/");
	cottage = gps::Model3D("objects/cottage/cottage.obj", "objects/cottage/");
	boat = gps::Model3D("objects/boat/boat.obj", "objects/boat/");
	penguin = gps::Model3D("objects/penguins/penguin.obj", "objects/penguins/");
	penguin1 = gps::Model3D("objects/penguins/penguin1.obj", "objects/penguins/");
	penguin2 = gps::Model3D("objects/penguins/penguin2.obj", "objects/penguins/");
	penguin3 = gps::Model3D("objects/penguins/penguin3.obj", "objects/penguins/");
	penguin4 = gps::Model3D("objects/penguins/penguin4.obj", "objects/penguins/");
	penguin5 = gps::Model3D("objects/penguins/penguin5.obj", "objects/penguins/");
	penguin6 = gps::Model3D("objects/penguins/penguin6.obj", "objects/penguins/");
	lamp = gps::Model3D("objects/mylamp/lamp.obj", "objects/mylamp/");
	well = gps::Model3D("objects/barrel/barrel.obj", "objects/barrel/");
	trees = gps::Model3D("objects/tree/tree.obj", "objects/tree/");
	tower = gps::Model3D("objects/tower/tower.obj", "objects/tower/");
	myaxe = gps::Model3D("objects/axe and pickaxe/myaxe.obj", "objects/axe and pickaxe/");
	mypickaxe = gps::Model3D("objects/axe and pickaxe/mypickaxe.obj", "objects/axe and pickaxe/");
}

void initShaders()
{
	myCustomShader.loadShader("shaders/myCustomShader.vert", "shaders/myCustomShader.frag");
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	depthMapShader.loadShader("shaders/depthMapShader.vert", "shaders/depthMapShader.frag");
	myCustomShader.useShaderProgram();
}

void initUniforms()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrixUniformLocation = glGetUniformLocation(myCustomShader.shaderProgram, "modelMatrix");
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	lightDirectionMatrixUniformLocation = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirectionMatrixMatrix");

	viewMatrix = myCamera.getViewMatrix();
	viewMatrixUniformLocation = glGetUniformLocation(myCustomShader.shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	
	normalMatrix = glm::mat3(glm::inverseTranspose(viewMatrix * modelMatrix));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	lightDirectionMatrix = glm::vec3(lightX, lightY, lightZ);
	lightDirectionMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirectionMatrix");
	glUniform3fv(lightDirectionMatrixLoc, 1, glm::value_ptr(lightDirectionMatrix));

	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); 
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightPosition = lightDirectionMatrix;
	lightPositionUniformLocation = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosition");
	glUniform3fv(lightPositionUniformLocation, 1, glm::value_ptr(lightPosition));

	lightPosition2 = glm::vec3(-lightX2, lightY2, -lightZ2);
	lightPositionUniformLocation2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosition2");
	glUniform3fv(lightPositionUniformLocation2, 1, glm::value_ptr(lightPosition2));

	fogUniformLocation = glGetUniformLocation(myCustomShader.shaderProgram, "visibleFog");
}

void initSkyBox() {

	std::vector<const GLchar*> faces;

	faces.push_back("textures/ely_peaks/peaks_ft.tga");
	faces.push_back("textures/ely_peaks/peaks_bk.tga");
	faces.push_back("textures/ely_peaks/peaks_up.tga");//a3a = up
	faces.push_back("textures/ely_peaks/peaks_dn.tga");
	faces.push_back("textures/ely_peaks/peaks_rt.tga");
	faces.push_back("textures/ely_peaks/peaks_lf.tga");

	mySkyBox.Load(faces);
	skyboxShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "viewMatrix"), 1, GL_FALSE,
		glm::value_ptr(viewMatrix));
	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));
}

void resetWater(int index) {
	char path1[] = "objects/water/water";
	char path2[] = "objects/water/";
	sprintf(path1, "%d.obj", index);
	lake = gps::Model3D("objects/water/water.obj", "objects/water/");
	
}


glm::mat4 lightSpaceTransform()
{
	const GLfloat near_plane = 1.0f, far_plane = 1000.0f;//am modificat de mi-a sarit toate alea
	glm::mat4 lightProjection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, near_plane, far_plane);

	glm::vec3 lightDirectionMatrixTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(glm::vec3(-lightX, lightY, -lightZ), 1.0f));
	glm::mat4 lightView = glm::lookAt(lightDirectionMatrixTr, myCamera.getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

int index = 0;
int targetX = 1200;
int targetY = 500;
int axePhase = 0;
float axeLoc = 0.0f;
glm::mat4 axeModel;
float delta = 0;
float movementSpeed = 200; // units per second
void updateDelta(double elapsedSeconds) {
	delta = delta + movementSpeed * elapsedSeconds;
}
double lastTimeStamp = glfwGetTime();


void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	processMovement();

	if (startTour == 1) {
		if (phase == 0) {
			if (delta < 100) {
				myCamera.mouse_callback(targetX + 0, targetY + delta);
				double currentTimeStamp = glfwGetTime();
				if(currentTimeStamp - lastTimeStamp < 0.75)
					updateDelta(currentTimeStamp - lastTimeStamp);
				else updateDelta(0);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				phase = 1;
			}
		}
		else if (phase == 1) {
			if (delta > -100) {
				myCamera.mouse_callback(targetX + 0, targetY + delta);
				double currentTimeStamp = glfwGetTime();
				updateDelta(-(currentTimeStamp - lastTimeStamp));
				lastTimeStamp = currentTimeStamp;
			}
			else {
				phase = 2;
			}
		}
		else if (phase == 2) {
			if (delta < 0) {
				myCamera.mouse_callback(targetX + 0, targetY + delta);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 3;
			}
		}
		if (phase == 3) {
			if (delta < 100) {
				myCamera.mouse_callback(targetX + delta, targetY + 0);
				double currentTimeStamp = glfwGetTime();
				if (currentTimeStamp - lastTimeStamp < 0.75)
					updateDelta(currentTimeStamp - lastTimeStamp);
				else updateDelta(0);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				phase = 4;
			}
		}
		else if (phase == 4) {
			if (delta > -100) {
				myCamera.mouse_callback(targetX + delta, targetY + 0);
				double currentTimeStamp = glfwGetTime();
				updateDelta(-(currentTimeStamp - lastTimeStamp));
				lastTimeStamp = currentTimeStamp;
			}
			else {
				phase = 5;
			}
		}
		else if (phase == 5) {
			if (delta < 0) {
				myCamera.mouse_callback(targetX + delta, targetY + 0);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 6;
			}
		}
		else if (phase == 6) {
			if (delta < 200) {
				myCamera.mouse_callback(targetX + 0, targetY - delta);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 7;
			}
		}
		else if (phase == 7) {
			if (delta < 300) {
				myCamera.move(gps::MOVE_FORWARD, cameraSpeed * 2);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 8;
			}
		}
		else if (phase == 8) {
			if (delta < 200) {
				myCamera.mouse_callback(targetX + 0, targetY + delta - 200);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 9;
			}
		}
		else if (phase == 9) {
			if (delta < 1500) {
				myCamera.mouse_callback(targetX + delta, targetY - 0);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 10;
			}
		}
		else if (phase == 10) {
			if (delta < 500) {
				myCamera.mouse_callback(targetX + 1500, targetY + delta);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 11;
			}
		}
		else if (phase == 11) {
			if (delta < 300) {
				myCamera.move(gps::MOVE_FORWARD, cameraSpeed * 1.35);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 12;
			}
		}
		else if (phase == 12) {
			if (delta < 425) {
				myCamera.mouse_callback(targetX + 1500, targetY - delta + 500);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 13;
			}
		}
		else if (phase == 13) {
			if (delta < 300) {
				myCamera.move(gps::MOVE_FORWARD, cameraSpeed * 1.35);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 14;
			}
		}
		else if (phase == 14) {
			if (delta < 600) {
				myCamera.mouse_callback(targetX + 1500 + delta, targetY + 75);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 15;
			}
		}
		else if (phase == 15) {
		if (delta < 300) {
			myCamera.move(gps::MOVE_FORWARD, cameraSpeed * 1.35);
			double currentTimeStamp = glfwGetTime();
			updateDelta(currentTimeStamp - lastTimeStamp);
			lastTimeStamp = currentTimeStamp;
		}
		else {
			delta = 0;
			phase = 16;
		}
		}
		else if (phase == 16) {
			if (delta < 600) {
				myCamera.mouse_callback(targetX + 2100 + delta, targetY + 75);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 17;
			}
		}
		else if (phase == 17) {
			if (delta < 600) {
				myCamera.move(gps::MOVE_FORWARD, cameraSpeed * 1.35);
				double currentTimeStamp = glfwGetTime();
				updateDelta(currentTimeStamp - lastTimeStamp);
				lastTimeStamp = currentTimeStamp;
			}
			else {
				delta = 0;
				phase = 18;
			}
		}
		else if (phase == 18) {
		if (delta < 1200) {
			myCamera.mouse_callback(targetX + 2700 + delta, targetY + 75);
			double currentTimeStamp = glfwGetTime();
			updateDelta(currentTimeStamp - lastTimeStamp);
			lastTimeStamp = currentTimeStamp;
		}
		else {
			delta = 0;
			phase = 19;
		}
		}
	}

	lightPosition = glm::vec3(-lightX, lightY, -lightZ);
	lightPositionUniformLocation = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosition");
	glUniform3fv(lightPositionUniformLocation, 1, glm::value_ptr(lightPosition));

	depthMapShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(lightSpaceTransform()));

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														DEPTH MAP
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	//create model matrix for nanosuit
	modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(sceneRotationAngle), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "modelMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(modelMatrix));

	cottage.Draw(depthMapShader);

	lake.Draw(depthMapShader);

	ice.Draw(depthMapShader);

	ground.Draw(depthMapShader);

	well.Draw(depthMapShader);

	myaxe.Draw(depthMapShader);


	mypickaxe.Draw(depthMapShader);

	trees.Draw(depthMapShader);

	tower.Draw(depthMapShader);

	penguin.Draw(depthMapShader);
	penguin1.Draw(depthMapShader);
	penguin2.Draw(depthMapShader);
	penguin3.Draw(depthMapShader);
	penguin4.Draw(depthMapShader);
	penguin5.Draw(depthMapShader);
	penguin6.Draw(depthMapShader);

	lamp.Draw(depthMapShader);

	
	int turn = 0;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	glViewport(0, 0, retina_width, retina_height);
	myCustomShader.useShaderProgram();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														SHADER PRINCIPAL
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (visibleFog) {
		glUniform3fv(fogUniformLocation, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
	}
	else {
		glUniform3fv(fogUniformLocation, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
	}


	//compute light direction transformation matrix
	lightDirectionMatrixMatrix = glm::mat3(glm::inverseTranspose(viewMatrix));
	//send lightDirectionMatrix matrix data to shader
	glUniformMatrix3fv(lightDirectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(lightDirectionMatrixMatrix));

	//initialize the view matrix
	viewMatrix = myCamera.getViewMatrix();
	//send view matrix data to shader	
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	//initialize the model matrix
	modelMatrix = glm::mat4(1.0f);
	//create model matrix
	modelMatrix = glm::rotate(modelMatrix, glm::radians(sceneRotationAngle), glm::vec3(0, 1, 0));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	
	//send lightSpace matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(lightSpaceTransform()));

	//bind the depth map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

	//create normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(viewMatrix*modelMatrix));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	ground.Draw(myCustomShader);

	well.Draw(myCustomShader);

	if (axePhase > 200)
		axePhase = 0;

	if (axePhase == 0)
		axeLoc = 0.04f;

	else if (axePhase <= 100){
		axeModel = glm::rotate(modelMatrix, glm::radians(axeLoc), glm::vec3(0, 0, 1));
		axeLoc += 0.04f;
	}
	else if (axePhase > 100){
		axeModel = glm::rotate(modelMatrix, glm::radians(axeLoc), glm::vec3(0, 0, 1));
		axeLoc -= 0.04f;
	}

	axePhase++;

	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "modelMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(axeModel));

	myaxe.Draw(myCustomShader);

	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "modelMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(modelMatrix));

	mypickaxe.Draw(myCustomShader);

	trees.Draw(myCustomShader);

	tower.Draw(myCustomShader);
	
	mySkyBox.Draw(skyboxShader, viewMatrix, projection);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//														ANIMATIE APA
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float anim = 1.5f;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//							Un lac static sub cel animat ca se vede urat altfel
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	lake.Draw(myCustomShader);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//									Lacul animat vine peste lacul static
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.05f, 0.0f));
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//create normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(viewMatrix*modelMatrix));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


		if (animOn) {

			if (index < 10)
				lake.Draw(myCustomShader);
			if (index < 20 * anim && index > 10 * anim)
				lake1.Draw(myCustomShader);
			if (index > 20 * anim  && index < 30 * anim)
				lake2.Draw(myCustomShader);
			if (index > 30 * anim  && index < 40 * anim)
				lake3.Draw(myCustomShader);
			if (index > 40 * anim  && index < 50 * anim)
				lake4.Draw(myCustomShader);
			if (index > 50 * anim  && index < 60 * anim)
				lake5.Draw(myCustomShader);
			if (index > 60 * anim  && index < 70 * anim)
				lake6.Draw(myCustomShader);
			if (index > 70 * anim  && index < 80 * anim)
				lake7.Draw(myCustomShader);
			if (index > 80 * anim  && index < 90 * anim)
				lake8.Draw(myCustomShader);
			if (index > 90 * anim  && index < 100 * anim)
				lake9.Draw(myCustomShader);
			if (index > 100 * anim  && index < 110 * anim)
				lake10.Draw(myCustomShader);
			if (index > 110 * anim  && index < 120 * anim)
				lake11.Draw(myCustomShader);
			if (index > 120 * anim  && index < 130 * anim)
				lake12.Draw(myCustomShader);
			if (index > 130 * anim  && index < 140 * anim)
				lake13.Draw(myCustomShader);
			if (index > 140 * anim  && index < 150 * anim)
				lake14.Draw(myCustomShader);
			if (index > 150 * anim  && index < 160 * anim)
				lake15.Draw(myCustomShader);
			if (index > 160 * anim  && index < 170 * anim)
				lake16.Draw(myCustomShader);
			if (index > 170 * anim  && index < 180 * anim)
				lake17.Draw(myCustomShader);


			index++;

			if (index > 180 * anim) {
				index = 10;
			}
		}
		else lake2.Draw(myCustomShader);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//											Modelul inapoi la normal
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.05f, 0.0f));
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	normalMatrix = glm::mat3(glm::inverseTranspose(viewMatrix*modelMatrix));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	ice.Draw(myCustomShader);

	cottage.Draw(myCustomShader);

	boat.Draw(myCustomShader);

	penguin.Draw(myCustomShader);
	penguin1.Draw(myCustomShader);
	penguin2.Draw(myCustomShader);
	penguin3.Draw(myCustomShader);
	penguin4.Draw(myCustomShader);
	penguin5.Draw(myCustomShader);
	penguin6.Draw(myCustomShader);

	lamp.Draw(myCustomShader);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//											SOARELE - SEPARAT DE SCENA
//---------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(sunRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-lightDirectionMatrix.x, lightDirectionMatrix.y, -lightDirectionMatrix.z));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.05f, 1.05f));
	//model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	normalMatrix = glm::mat3(glm::inverseTranspose(viewMatrix * modelMatrix));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	sun.Draw(myCustomShader);

	myCustomShader.useShaderProgram();

	darkLoc = glGetUniformLocation(myCustomShader.shaderProgram, "dark");
	glUniform1i(darkLoc, dark);
	
	
}

int main(int argc, const char * argv[]) {

	initOpenGLWindow();
	initOpenGLState();
	initShadowMap();
	initModels();
	initShaders();
	initUniforms();	
	initSkyBox();

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(glWindow, cursor);
	
	while (!glfwWindowShouldClose(glWindow)) {
		renderScene();
		glfwSetCursorPosCallback(glWindow, mouseCallback);

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	//close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
