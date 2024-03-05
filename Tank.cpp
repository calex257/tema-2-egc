#include "Tank.h"

namespace tema {
    Tank::Tank() {
    }
    Tank::~Tank() {
    }
    void Tank::MoveForward(float distance) {
        glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
        for (auto& item : items) {
            item.get()->translateBy(dir * distance);
        }
    }
    void Tank::RotateFPOY(float angle) {
        glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
        glm::vec3 oldForward = glm::normalize(forward);
        // extract the vec3 vector and then normalize it
        forward = glm::normalize(glm::vec3(newVector));
        float new_angle = glm::acos(glm::dot(forward, oldForward));
        glm::mat4 aux = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
        glm::vec4 r = glm::vec4(right, 1);
        auto g = glm::normalize(aux * r);
        right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
        auto cr = glm::cross(right, forward);
        up = glm::normalize(glm::cross(right, forward));

        for (auto& item : items) {
            item.get()->RotateFPOY(angle);
        }
    }
}