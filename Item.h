#pragma once

#include "core/engine.h"
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/gpu/texture2D.h"
#include "core/managers/resource_path.h"
#include "core/managers/texture_manager.h"
#include <map>

#include "utils/text_utils.h"
#include "utils/memory_utils.h"
#include "utils/gl_utils.h"
#include "utils/math_utils.h"

namespace tema {
	class Item {
	public:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Shader> shader;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec3 color;
		std::vector<std::shared_ptr<Mesh>> children;
		glm::vec3 forward = glm::vec3(-1, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = glm::vec3(0, 0, -1);
		Item();
		Item(std::shared_ptr<Mesh> mesh, glm::vec3 color);
		~Item();
		int hp;
		void setPosition(glm::vec3 position);
		void translateBy(glm::vec3 offset);
		void setRotation(glm::vec3 degree);
		void rotateBy(glm::vec3 degree);
		void setScale(glm::vec3 scale);
		void scaleBy(glm::vec3 scale);
		void RotateFPOY(float angle);
		void RotateFPOZ(float angle);
		glm::mat4 getTransformMatrix();
	};
}