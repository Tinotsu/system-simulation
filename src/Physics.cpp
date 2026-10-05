#pragma once

#include "Physics.h"
#include "PhysicsConstant.h"
#include "glm/ext/vector_float2.hpp"
#include <cmath>

void RunPhysic(float dt, Object &obj1, Object &obj2) {
    float dx = obj1.position.x - obj2.position.x;
    float dy = obj1.position.y - obj2.position.y;
    float distance = sqrt(dx * dx + dy * dy);
    glm::vec2 direction1 = {dx / distance, dy / distance};
    glm::vec2 direction2 = {dx / distance, dy / distance};

    float GForce = (g * obj1.mass * obj2.mass) / (distance * distance);
    float acc = GForce / obj1.mass;
    glm::vec2 accCoor1 = {acc * direction1.x, acc * direction1.y};
    glm::vec2 accCoor2 = {-acc * direction2.x, -acc * direction2.y};

    obj1.velocity -= accCoor1 * dt;
    obj2.velocity -= accCoor2 * dt;

    obj1.position.x += obj1.velocity.x * dt;
    obj1.position.y += obj1.velocity.y * dt;
    obj2.position.x += obj2.velocity.x * dt;
    obj2.position.y += obj2.velocity.y * dt;
}
