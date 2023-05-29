#include <math.h>
#include "Cone.h"

float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
    float a = pow(dir.x, 2.0)+pow(dir.z,2.0)-pow((radius/height)*dir.y, 2.0);
    float b1 = pow(radius/height,2.0)*dir.y*(p0.y-center.y-height);
    float b2 = 2*(dir.x*(p0.x-center.x)+dir.z*(p0.z-center.z)-b1);
    float c1 = (pow(p0.y-center.y,2.0)+height*(height-2*p0.y+2*center.y));
    float c2 = pow(p0.x-center.x,2.0)+pow(p0.z-center.z,2)-pow(radius/height,2.0)*c1;

    float d = pow(b2,2)-4.0*a*c2;
    float t1= (-b2-sqrt(d))/(2*a);
    float t2= (-b2+sqrt(d))/(2*a);

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


glm::vec3 Cone::normal(glm::vec3 pt)
{
    float theta = radius/height;
    float alpha = glm::atan((pt.x-center.x)/(pt.z-center.z));
    glm::vec3 v1 = glm::vec3(glm::cos(theta)*glm::sin(alpha), glm::sin(theta),glm::cos(theta)*glm::cos(alpha));
    glm::vec3 n = glm::normalize(v1);

    return n;
}



