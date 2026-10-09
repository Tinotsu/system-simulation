#pragma once

#include "Physics.h"
#include "PhysicsConstant.h"
#include "glm/ext/vector_float2.hpp"
#include <cmath>
#include <vector>

void RunPhysic(float dt, std::vector<Object> &objects) {
    for (int n = 0; n < objects.size(); n++) {
        objects[n].acceleration = {0, 0};
    }

    for (int n = 0; n < objects.size(); n++) {
        for (int i = n + 1; i < objects.size(); i++) {

            if (n != i) {
                float dx = objects[n].position.x - objects[i].position.x;
                float dy = objects[n].position.y - objects[i].position.y;
                float rSquare = dx * dx + dy * dy;

                if (rSquare != 0.0) {
                    glm::vec2 direction = {dx / sqrt(rSquare),
                                           dy / sqrt(rSquare)};
                    float F;
                    F = (G * objects[n].mass * objects[i].mass) / rSquare;
                    objects[n].acceleration -=
                        direction * (F / objects[n].mass);
                    objects[i].acceleration +=
                        direction * (F / objects[i].mass);
                }
            }
        }
    }

    for (int n = 0; n < objects.size(); n++) {
        objects[n].velocity += objects[n].acceleration * dt;
        objects[n].position += objects[n].velocity * dt;
    }
}
