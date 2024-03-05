#pragma once


#include "components/camera.h"

#include "core/world.h"
#include "core/engine.h"
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/gpu/texture2D.h"
#include "core/managers/resource_path.h"
#include "core/managers/texture_manager.h"
#include <map>

#include "Item.h"

#include "utils/text_utils.h"
#include "utils/memory_utils.h"
#include "utils/gl_utils.h"
#include "utils/math_utils.h"

namespace tema {
	class Tank {
	public:
		const glm::vec3 cannonOffset = glm::vec3(-1.07f, 0.48f, 0);
		glm::mat4 transformMatrix;
		glm::vec3 forward = glm::vec3(-1, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = glm::vec3(0, 0,-1);
		std::vector<std::shared_ptr<Item>> items;
		std::shared_ptr<Item> cannon;
		std::shared_ptr<Item> turret;
		std::shared_ptr<Item> rotation_center;
		int hp = 1;
		Tank();
		~Tank();
		void MoveForward(float distance);
		void RotateFPOY(float angle);
	};
}