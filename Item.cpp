#include "Item.h"

namespace tema {
	Item::Item() {
	}
	Item::Item(std::shared_ptr<Mesh> mesh, glm::vec3 color) {
		position = glm::vec3(0);
		scale = glm::vec3(1);
		rotation = glm::vec3(0);
		this->mesh = mesh;
		this->color = color;
	}
	Item::~Item() {
	}
	void Item::setPosition(glm::vec3 position) {
		this->position = position;
	}
	void Item::translateBy(glm::vec3 offset) {
		this->position += offset;
	}
	void Item::setRotation(glm::vec3 degree) {
		this->rotation = degree;
	}
	void Item::rotateBy(glm::vec3 degree) {
		this->rotation += degree;
	}
	void Item::setScale(glm::vec3 scale) {
		this->scale = scale;
	}
	void Item::scaleBy(glm::vec3 scale) {
		this->scale += scale;
	}
	glm::mat4 Item::getTransformMatrix() {
		auto mat = glm::mat4(1);
		mat = glm::translate(mat, position);
		glm::vec3 axis[] = {
			glm::vec3(1,0,0),
			glm::vec3(0,1,0),
			glm::vec3(0,0,1),
		};
		mat = glm::rotate(mat, rotation.x, axis[0]);
		mat = glm::rotate(mat, rotation.y, axis[1]);
		mat = glm::rotate(mat, rotation.z, axis[2]);
		
		return mat;
	}
	void Item::RotateFPOY(float angle) {
		glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
		// extract the vec3 vector and then normalize it
		forward = glm::normalize(glm::vec3(newVector));
		right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
		up = glm::normalize(glm::cross(right, forward));
		rotateBy(glm::vec3(0, angle, 0));
	}
	void Item::RotateFPOZ(float angle) {
		glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);

		// extract the vec3 vector and then normalize it
		forward = glm::normalize(glm::vec3(newVector));

		up = glm::cross(right, forward);
		rotateBy(glm::vec3(0, 0, -angle));
	}
}