#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh.h"


void error_glfw(int error, const char* desc) {
	printf("%d: %s", error, desc);
}

GLuint shaderProgram;
std::vector<Mesh*> meshList;

static const char* vShader = "               \n\
#version 330                                  \n\
                                              \n\
layout(location=0) in vec3 pos;               \n\
uniform mat4 model;							  \n\
out vec4 vColor;							  \n\
                                              \n\
void main(){                                  \n\
  gl_Position = model * vec4(pos, 1.0);		  \n\
  vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);\n\
}                                             \n\
";

static const char* fShader = "                \n\
#version 330                                  \n\
                                              \n\
uniform vec3 triColor;                        \n\
in vec4 vColor;                               \n\
out vec4 color;								  \n\
                                              \n\
void main(){                                  \n\
  color = vColor;                             \n\
}                                             \n\
";

void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType) {
	GLuint _shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	glShaderSource(_shader, 1, code, NULL);
	glCompileShader(_shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(_shader, 1024, NULL, eLog);
		printf("Erro ao compilar %d: %s", shaderType, eLog);
		return;
	}
	glAttachShader(shaderProgram, _shader);
}



void CompileShader() {
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		printf("O programa não foi iniciado \n");
		return;
	}

	AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

	glLinkProgram(shaderProgram);
}

void CreateTriangle() {
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,  //Vertice 0: (Preto)
		0.0f, 1.0f, 0.0f,    //Vertice 1: (Verde)
		1.0f, -1.0f, 0.0f,   //Vertice 2: (Vermelho)
		0.0f, -1.0f, 1.0f     //Vertice 3: (Azul)
	};

	GLuint indices[] = {
		0,1,2,
		1,2,3,
		0,1,3,
		0,2,3
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));
	meshList.push_back(obj1);
}



int main() {
	//Caso o GLWF de erro, execute essa função
	glfwSetErrorCallback(error_glfw);

	if (!glfwInit()) {
		printf("GLFW Não foi iniciado \n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "Aula 02", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW Não consegiu criar a janela\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mainWindow);

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		printf("Erro ao iniciar o Glew");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShader();


	bool direction = true, angleDirection = true, scaleDirection = true;
	float triOffset = 0.0f, maxOffset = 1.0f, minOffset = -1.0f, incOffset = 0.01f;
	float angle = 0.0f, maxAngle = 360.0f, minAngle = -0.1f, incAngle = 0.5f;
	float scale = 0.0f, maxScale = 1.0f, minScale = -1.0f, incScale = 0.01f;

	while (!glfwWindowShouldClose(mainWindow)) {
		glClearColor(0.8f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		//Definindo as cores do tringulo
		//Retorna o endere�o de memoria da placa de video
		GLuint uniColor = glGetUniformLocation(shaderProgram, "triColor");
		glUniform3f(uniColor, 0.3f, 0.8f, 0.7f);

		//Matriz 4x4 (1.0f)
		glm::mat4 model(1.0f);

		//Calculo de movimentação
		if (triOffset >= maxOffset || triOffset <= minOffset)
			direction = !direction;
		triOffset += direction ? incOffset : incOffset * -1;

		//Calculo de angulo
		if (angle >= maxAngle || angle <= minAngle)
			angleDirection = !angleDirection;
		angle += angleDirection ? incAngle : incAngle * -1;

		//Calculo de tamanho
		if (scale >= maxScale || scale <= minScale)
			scaleDirection = !scaleDirection;
		scale += scaleDirection ? incScale : incScale * -1;

		//Movimentação nos eixos
		//model = glm::translate(model, glm::vec3(-triOffset, triOffset, 0.0f));

		//Movimentação de angulo
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

		GLuint uniModel = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh();
		
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}