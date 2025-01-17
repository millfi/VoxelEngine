#pragma once
#include "common.hpp"
#include <vector>
class Mesh {
protected:
	glm::mat4 modelMatrix;
	GLuint VAO;
	GLuint VBO;
	// コピー禁止
	Mesh(const Mesh& o);
	Mesh &operator=(const Mesh& o);
public:
	std::vector<glm::vec3>& position;
	Mesh() : position(position), modelMatrix(modelMatrix) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, position.size(), position.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
	}
	virtual ~Mesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	void draw(GLuint shaderMatLocation) const {
		glBindVertexArray(VAO);
		glUniformMatrix4fv(shaderMatLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, position.size());
	}
};