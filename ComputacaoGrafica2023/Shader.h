#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
class Shader
{
public:
	Shader();
	~Shader();
	//compilar o c�digo que passar 
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	//passa a localiz��o do arquivo
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	//fun��o que usa no origin.cpp
	void UseProgram();

	//pega a informa��o do projection, faz os calculos e passa para o shader CALCULA DISTANCIA
	GLint GetUniformProjection() { return uniformProjection; }
	//pega a informa��o do model, faz os calculos e passa para o shader CALCULA NO FINAL
	GLint GetUniformModel() { return uniformModel; }

private: 
	//definindo shaderid
	GLuint shaderId;
	//definindo projection e model
	GLint uniformProjection, uniformModel;

	//l� o arquivo e retorna a string que t� la dentro
	std::string ReadFile(const char* fileLocation);
	//compila as strings, � s� ele que pode mandar compilar 
	void Compile(const char* vertexCode, const char* fragmentCode);


};

