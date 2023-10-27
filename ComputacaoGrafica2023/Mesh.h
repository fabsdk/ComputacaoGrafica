#include "GL\glew.h"
class Mesh
{
public:
	Mesh();
	~Mesh();
	void CreateMesh(GLfloat vertices[], unsigned int numOfVertices,
				GLuint* indices, unsigned numOfIndices);
	void RenderMesh();

private:
	GLuint VAO, VBO, IBO;
	unsigned int numOfIndices;


};