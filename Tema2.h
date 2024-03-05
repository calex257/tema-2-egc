#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "TankCamera.h"
#include "Tank.h"
#include "Projectile.h"

namespace tema {
    class Tema2 : public gfxc::SimpleScene {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderTank(Tank& t);
        bool checkGameShouldEnd();

    protected:
        implemented::TankCamera* camera;
        std::chrono::high_resolution_clock::time_point game_begin;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float orthoxmic = -10.0f;
        float orthoxmare = 10.0f;
        float orthoymic = -10.0f;
        float orthoymare = 10.0f;
        float fov = 60;
        float rot = 0.0f;
        float ox = 0.0f;
        float near = 0.01f;
        float count = 0;
        std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
        Tank my_tank;
        std::vector<std::shared_ptr<Projectile>> projectiles;
        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
