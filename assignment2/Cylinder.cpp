#include <math.h>
#include "Cylinder.h"



float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
    float a = pow(dir.x, 2) + pow(dir.z, 2);
    float b = 2 * (dir.x * (p0.x-center.x) + dir.z * (p0.z - center.z));
    float c = pow((p0.x - center.x), 2) + pow((p0.z - center.z),2) - pow(radius, 2);


    float d = b*b - 4*a*c;
    float t1 = (-b - sqrt(d)) / (2*a);
    //printf("%f", t1);
    float t2 = (-b + sqrt(d)) / (2*a);
    //printf("%f", t1);



    glm::vec3 p1 = p0 + t1*dir;
    glm::vec3 p2 = p0 + t2*dir;


    if (t1 > 0 and (p1.y < height + center.y and p1.y > center.y)){
        return t1;
    } else {
        if(t2 > 0 and p2.y < height +center.y and p2.y > center.y) {
            return t2;
        } else {
            return -1;
        }
    }




}

glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n = glm::vec3 (p.x-center.x, 0, p.z-center.z);
    n = glm::normalize(n);
    return n;
}
