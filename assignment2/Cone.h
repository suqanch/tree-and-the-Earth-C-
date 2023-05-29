
#ifndef H_CONE
#define H_CONE

#include <glm/glm.hpp>
#include "SceneObject.h"

class Cone : public SceneObject
{
private:
    glm::vec3 center;
    float radius;
    float height;

public:

    Cone(glm::vec3 c, float r, float h)
        : center(c), radius(r), height(h) {}

    float intersect(glm::vec3 posn, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

};

#endif //!H_Cone
