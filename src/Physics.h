#include "glm/ext/vector_float2.hpp"
#include <cmath>

class Object {
  public:
    glm::vec2 position;
    glm::vec2 velocity;
    float mass;
};

void RunPhysic(float dt, Object &obj1, Object &obj2);
