#include "glm/ext/vector_float2.hpp"
#include <cmath>
#include <vector>

class Object {
  public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
    float mass;
};

void RunPhysic(float dt, std::vector<Object> &objects);
