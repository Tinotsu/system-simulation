#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float4.hpp"
#include <cmath>
#include <vector>

class Object {
  public:
    std::string name;
    glm::vec2 position;     // km/s^2
    glm::vec2 velocity;     // km/s
    glm::vec2 acceleration; // km
    float mass;             // 10^24 kg
    float radius;           // km
    glm::vec4 color;
};

void RunPhysic(float dt, std::vector<Object> &objects);
