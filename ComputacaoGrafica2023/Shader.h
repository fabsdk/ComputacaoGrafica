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
	//compilar o código que passar 
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	//passa a localizção do arquivo
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	//função que usa no origin.cpp
	void UseProgram();

	//pega a informação do projection, faz os calculos e passa para o shader CALCULA DISTANCIA
	GLint GetUniformProjection() { return uniformProjection; }
	//pega a informação do model, faz os calculos e passa para o shader CALCULA NO FINAL
	GLint GetUniformModel() { return uniformModel; }

private: 
	//definindo shaderid
	GLuint shaderId;
	//definindo projection e model
	GLint uniformProjection, uniformModel;

	//lê o arquivo e retorna a string que tá la dentro
	std::string ReadFile(const char* fileLocation);
	//compila as strings, é só ele que pode mandar compilar 
	void Compile(const char* vertexCode, const char* fragmentCode);


};

