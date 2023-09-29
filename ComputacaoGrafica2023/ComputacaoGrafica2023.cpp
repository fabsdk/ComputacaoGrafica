#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


void error_glfw(int error, const char* desc)
{
	printf("%d: %s", error, desc);
}

GLuint shaderProgram, VAO, VBO;

static const char* vShader = "                \n\
#version 330                                  \n\
                                              \n\
layout(location=0) in vec3 pos;               \n\
uniform mat4 model;                           \n\
                                              \n\
void main(){                                  \n\
  gl_Position =  model * vec4(pos, 1.0);      \n\
}                                             \n\
";

static const char* fShader = "                \n\
#version 330                                  \n\
                                              \n\
uniform vec3 triColor;                        \n\
out vec4 color;                                \n\
                                              \n\
void main(){                                  \n\
  color = vec4(triColor, 1.0);                \n\
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
		printf("O programa n�o foi iniciado :(");
		return;
	}

	AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

	glLinkProgram(shaderProgram);
}

void CreateTriangle() {
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

//VAO  -> varios VBOS, esta na memoria e por isso precisa do buffer dele
//Vector Buffer Object -> info do vetor

int main()
{
	// caso de erro a fun��o vai ser executada
	glfwSetErrorCallback(error_glfw);

	if (!glfwInit()) {
		printf("GLFW n�o foi iniciado \n");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "Aula 02", NULL, NULL);

	if (!mainWindow) {
		printf("GLFW n�o conseguiu criar a janela\n");
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

	glViewport(0, 0, bufferWidth, bufferHeight);

	//criando o triangulo
	CreateTriangle();
	//compilando o shader
	CompileShader();
	bool direction = true, angleDirection = true, scaleDirection = true;
	float triOffset = 0.0f, maxOffset = 1.0f, minOffset = -1.0f, incOffset = 0.01f;
	float angle = 0.0f, maxAngle = 360.0f, minAngle = -0.1f, incAngle = 0.5f;
	float scale = 0.f, maxScale = 1.0f, minScale = -1.0f, incScale = 0.01f;



	while (!glfwWindowShouldClose(mainWindow)) {
		glClearColor(0.8f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		//Definindo as cores do tringulo
		//Retorna o endere�o de memoria da placa de video
		GLuint uniColor = glGetUniformLocation(shaderProgram, "triColor");
		glUniform3f(uniColor, 0.3f, 0.8f, 0.7f);

		//matriz 4x4
		glm::mat4 model(1.0f);

		//Movendo o triangulo
		if ((triOffset >= maxOffset) || (triOffset <= minOffset))
			direction = !direction;
		triOffset += direction ? incOffset : incOffset * -1;

		//Movimentacao de eixos
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));

		//Calculo de angulo
		if(angle >= maxAngle || angle <= minAngle)
			angleDirection = !angleDirection;
		angle += angleDirection ? incAngle : incAngle * -1;

		//Calculo de scale
		if (scale >= maxScale || scale <= minScale)
			scaleDirection = !scaleDirection;
		scale += scaleDirection ? incScale : incScale * -1;


		//movimentção de angulo
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		//movimentação de scale
		model = glm::scale(model, glm::vec3(scale, scale, 0.0f));


		//Buscando o endere�o de memoria onde esta alterando esse valor
		GLuint unixModel = glGetUniformLocation(shaderProgram, "model");
		//1 ponto flutuante
		glUniformMatrix4fv(unixModel, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
