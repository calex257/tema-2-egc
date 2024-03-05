#include "lab_m1/lab5/lab5.h"

#include "TankCamera.h"
#include "Tema2.h"
#include "Projectile.h"

#include <math.h>
#include <vector>
#include <string>
#include <iostream>



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


namespace tema {


	Tema2::Tema2() {
	}


	Tema2::~Tema2() {
	}


	void Tema2::Init() {
		renderCameraTarget = false;
		game_begin = std::chrono::high_resolution_clock::now();
		camera = new implemented::TankCamera();
		camera->Set(glm::vec3(3.5f, 1.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		{
			auto mesh = std::make_shared<Mesh>("box");
			//Mesh* mesh = new Mesh("box");
 

			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			//Mesh* mesh = new Mesh("Body");'

			auto mesh = std::make_shared<Mesh>("Body");
			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "corp.obj");
			
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			auto mesh = std::make_shared<Mesh>("senile");
			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "senile.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			auto mesh = std::make_shared<Mesh>("tun");
			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "tun2.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			auto mesh = std::make_shared<Mesh>("turela");
			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), "turela.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			auto mesh = std::make_shared<Mesh>("sphere");
			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		{
			Shader* shader = new Shader("TankShader");
			shader->AddShader(PATH_JOIN(window->props.selfDir, "src\\tema2", "shaders", "VTankShader.glsl"), GL_VERTEX_SHADER);
			shader->AddShader(PATH_JOIN(window->props.selfDir, "src\\tema2", "shaders", "FTankShader.glsl"), GL_FRAGMENT_SHADER);
			shader->CreateAndLink();
			shaders[shader->GetName()] = shader;
		}

		std::shared_ptr<Item> cannon = std::make_shared<Item>(meshes["tun"], glm::vec3(0.31f, 0.31f, 0.31f));
		my_tank.items.push_back(cannon);
		my_tank.cannon = cannon;
		cannon.get()->position = my_tank.cannonOffset;
		std::shared_ptr<Item> turret = std::make_shared<Item>(meshes["turela"], glm::vec3(0.38f, 0.4f, 0.27f));
		my_tank.items.push_back(turret);
		my_tank.turret = turret;

		my_tank.items.push_back(std::make_shared<Item>(meshes["senile"], glm::vec3(0.36f, 0.36f, 0.36f)));
		my_tank.items.push_back(std::make_shared<Item>(meshes["Body"], glm::vec3(0.56f, 0.65f, 0.49f)));
		// TODO(student): After you implement the changing of the projection
		// parameters, remove hardcodings of these parameters
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);

	}


	void Tema2::FrameStart() {
		// Clears the color buffer (using the previously set color) and depth buffer
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glm::ivec2 resolution = window->GetResolution();
		// Sets the screen area where to draw
		glViewport(0, 0, resolution.x, resolution.y);
	}

	void Tema2::RenderTank(Tank& t) {
		glm::mat4 mat = glm::mat4(1);
		mat = glm::translate(mat, t.cannon.get()->position);
		mat = glm::translate(mat, -t.cannonOffset);
		mat = glm::rotate(mat, t.cannon.get()->rotation.y, glm::vec3(0, 1, 0));
		mat = glm::translate(mat, t.cannonOffset);
		mat = glm::rotate(mat, t.cannon.get()->rotation.z, glm::vec3(0, 0, 1));
		shaders["TankShader"]->Use();
		int loc = glGetUniformLocation(shaders["TankShader"]->GetProgramID(), "hp");
		if (loc == -1) {
			std::cout << "futere";
		}
		glUniform1iv(loc,1,&t.hp);
		RenderMesh(t.cannon.get()->mesh.get(), shaders["TankShader"], mat, glm::vec3(0.5f,0.5f,0.5f));
		for (auto& item : t.items) {
			if (item != t.cannon) {
				auto matrix = item.get()->getTransformMatrix();
				RenderMesh(item.get()->mesh.get(), shaders["TankShader"], matrix, item.get()->color);
			}
		}
	}

	bool Tema2::checkGameShouldEnd() {
		auto rn = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> d = rn - game_begin;
		auto seconds = std::chrono::duration_cast<std::chrono::seconds>(d);
		if (seconds.count() > 90) {
			return true;
		}
		return false;
	}

	void Tema2::Update(float deltaTimeSeconds) {
		RenderTank(my_tank);
		// TODO(student): Draw more objects with different model matrices.
		// Attention! The `RenderMesh()` function overrides the usual
		// `RenderMesh()` that we've been using up until now. This new
		// function uses the view matrix from the camera that you just
		// implemented, and the local projection matrix.

		// Render the camera target. This is useful for understanding where
		//// the rotation point is, when moving in third-person camera mode.
		if (checkGameShouldEnd()) {
			exit(0);
		}
		std::vector<int> vect;
		for (int i = 0; i < projectiles.size(); i++) {
			if (!projectiles[i].get()->shouldStillExist()) {
				vect.push_back(i);
				continue;
			}
			projectiles[i].get()->update();
			auto mat = projectiles[i].get()->getTransformMatrix();
			RenderMesh(projectiles[i].get()->mesh.get(), shaders["TankShader"], mat, glm::vec3(1, std::min(glm::distance(projectiles[i].get()->position, my_tank.cannon.get()->position) / 50.0f, 0.5f), 0));
		}
		for (auto& i : vect) {
			projectiles.erase(projectiles.begin() + i);
		}
		if (renderCameraTarget) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderMesh(meshes["sphere"].get(), shaders["TankShader"], modelMatrix, glm::vec3(1,1,1));
		}
	}


	void Tema2::FrameEnd() {
		DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
		//projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, near, 200.0f);
	}


	void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color) {
		if (!mesh || !shader || !shader->program)
			return;

		// Render an object using the specified shader and the specified position
		shader->Use();
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		int location = glGetUniformLocation(shader->GetProgramID(), "newColor");
		glUniform3fv(location, 1, glm::value_ptr(color));
		mesh->Render();
	}


	/*
	 *  These are callback functions. To find more about callbacks and
	 *  how they behave, see `input_controller.h`.
	 */


	void Tema2::OnInputUpdate(float deltaTime, int mods) {
		// move the camera only if MOUSE_RIGHT button is pressed
		//if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO(student): Translate the camera forward
			my_tank.MoveForward(cameraSpeed * deltaTime);
			camera->position = my_tank.items[2].get()->position - 4.0f * camera->forward + 0.5f * camera->up;
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->RotateThirdPerson_OY(RADIANS(1) * 0.035f * 10);
			my_tank.RotateFPOY(RADIANS(1) * 0.035f * 10);
			camera->position = my_tank.items[2].get()->position - 4.0f * camera->forward + 0.5f * camera->up;
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO(student): Translate the camera backward
			my_tank.MoveForward(-cameraSpeed * deltaTime);
			camera->position = my_tank.items[2].get()->position - 4.0f * camera->forward + 0.5f * camera->up;
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO(student): Translate the camera to the right

			camera->RotateThirdPerson_OY(-RADIANS(1) * 0.035f * 10);

			my_tank.RotateFPOY(-RADIANS(1) * 0.035f * 10);
			camera->position = my_tank.items[2].get()->position - 4.0f * camera->forward + 0.5f * camera->up;
		}
		// TODO(student): Change projection parameters. Declare any extra
		// variables you might need in the class header. Inspect this file
		// for any hardcoded projection arguments (can you find any?) and
		// replace them with those extra variables.

	}


	void Tema2::OnKeyPress(int key, int mods) {

		if (key == GLFW_KEY_R) {
			my_tank.turret.get()->rotation = my_tank.items[2].get()->rotation;
			my_tank.cannon.get()->rotation.y = my_tank.items[2].get()->rotation.y;
			camera->forward = my_tank.forward;
			camera->right = my_tank.right;
			camera->up = my_tank.up;
			camera->position = my_tank.items[2].get()->position - 4.0f * camera->forward + 0.5f * camera->up;
		}
		// Add key press event
		if (key == GLFW_KEY_H) {
			my_tank.hp++;
		}
		// TODO(student): Switch projections
		if (key == GLFW_KEY_O) {
			projectionMatrix = glm::ortho(orthoxmic, orthoxmare, orthoymic, orthoymare, 0.0f, 100.0f);
		}
		if (key == GLFW_KEY_P) {
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
		}
	}


	void Tema2::OnKeyRelease(int key, int mods) {
		// Add key release event
	}


	void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
		// Add mouse move event
		float sensivityOX = 0.01f;
		float sensivityOY = 0.003f;
		my_tank.turret.get()->RotateFPOY(-RADIANS(deltaX) * sensivityOX * 10);
		my_tank.cannon.get()->RotateFPOY(-RADIANS(deltaX) * sensivityOX * 10);
		my_tank.cannon.get()->RotateFPOZ(-RADIANS(deltaY) * sensivityOY * 10);
		/*my_tank.turret.get()->rotateBy(glm::vec3(0, -RADIANS(deltaX) * sensivityOX * 10, 0));
		my_tank.cannon.get()->rotateBy(glm::vec3(0, -RADIANS(deltaX) * sensivityOX * 10, 0));
		my_tank.cannon.get()->rotation.z = (mouseY - window->GetResolution().y / 2.0f) / (float)window->GetResolution().y;
		*/
		if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

			camera->RotateThirdPerson_OX(-RADIANS(deltaY) * sensivityOX * 10);
			camera->RotateThirdPerson_OY(-RADIANS(deltaX) * sensivityOY * 10);
			return;
		}
	}


	void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
		// Add mouse button press event
		if (button == GLFW_MOUSE_BUTTON_2) {
			std::cout << "2";
			std::shared_ptr<Projectile> p = std::make_shared<Projectile>(meshes["sphere"]);
			p.get()->direction = my_tank.cannon.get()->forward;
			p.get()->position = my_tank.turret.get()->position + p.get()->direction;
			projectiles.emplace_back(p);
		}
		if (button == GLFW_MOUSE_BUTTON_3) {
			std::cout << "3";
		}
	}


	void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
		// Add mouse button release event
	}


	void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
	}


	void Tema2::OnWindowResize(int width, int height) {
	}

}