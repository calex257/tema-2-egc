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
#include <chrono>

#include "Item.h"

#include "utils/text_utils.h"
#include "utils/memory_utils.h"
#include "utils/gl_utils.h"
#include "utils/math_utils.h"

namespace tema {
	class Projectile {
	public:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Shader> shader;
		std::chrono::high_resolution_clock::time_point spawn_time;
		float speed;
		glm::vec3 position;
		glm::vec3 direction;
		Projectile(std::shared_ptr<Mesh> m);
		bool shouldStillExist();
		glm::mat4 getTransformMatrix();
		void update();
	};
}