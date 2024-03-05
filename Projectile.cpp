#include "Projectile.h"
#include "lab_m1/lab5/lab5.h"

#include "TankCamera.h"
#include "Tema2.h"

#include <vector>
#include <chrono>
#include <string>
#include <iostream>

namespace tema {
	Projectile::Projectile(std::shared_ptr<Mesh> m) {
		mesh = m;
		spawn_time = std::chrono::high_resolution_clock::now();
	}
	bool Projectile::shouldStillExist() {
		auto rn = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> millis = rn - spawn_time;
		std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds>(millis);
		if (d.count() > 5000)
			return false;
		return true;
	}
	glm::mat4 Projectile::getTransformMatrix() {
		auto matrix = glm::mat4(1);
		matrix = glm::translate(matrix, position);
		return matrix;
	}
	void Projectile::update() {
		position += direction * 3.0f;
	}
}