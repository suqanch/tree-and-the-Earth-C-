/*==================================================================================
* COSC 363  Computer Graphics (2022)  Assignment2
* Zhenyuan He
* ID: 11969443/zhe27
*===================================================================================
*/
#include <math.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "Sphere.cpp"
#include "SceneObject.h"
#include "SceneObject.cpp"
#include "Ray.h"
#include "Ray.cpp"
#include "Cone.h"
#include "Cone.cpp"
#include "Plane.h"
#include "Plane.cpp"
#include "Cylinder.h"
#include "Cylinder.cpp"
#include "TextureBMP.h"
#include "TextureBMP.cpp"
#include <GL/freeglut.h>
using namespace std;

TextureBMP texture1;
TextureBMP texture2;


const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;

vector<SceneObject*> sceneObjects;


//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------

void Cube(float x, float y, float z, float size)
{
    glm::vec3 point_1 = glm::vec3(x, y, z);
    glm::vec3 point_2 = glm::vec3(x+size, y, z);
    glm::vec3 point_3 = glm::vec3(x+size, y+size, z);
    glm::vec3 point_4 = glm::vec3(x, y+size, z);


    glm::vec3 point_5 = glm::vec3(x+size, y, z-size);
    glm::vec3 point_6 = glm::vec3(x+size, y + size, z-size);
    glm::vec3 point_7 = glm::vec3(x, y+size, z-size);
    glm::vec3 point_8 = glm::vec3(x, y, z-size);

    Plane* plane1 = new Plane(point_2, point_5, point_6, point_3);
    Plane* plane2 = new Plane(point_8, point_5, point_2, point_1);
    Plane* plane3 = new Plane(point_4, point_3, point_6, point_7);
    Plane* plane4 = new Plane(point_1, point_2, point_3, point_4);
    Plane* plane5 = new Plane(point_4, point_7, point_8, point_1);
    Plane* plane6 = new Plane(point_5, point_8, point_7, point_6);


    plane1->setColor(glm::vec3(1, 0.7, 0.0));
    plane2->setColor(glm::vec3(1, 0.7, 0.0));
    plane3->setColor(glm::vec3(1, 0.7, 0.0));
    plane4->setColor(glm::vec3(1, 0.7, 0.0));
    plane5->setColor(glm::vec3(1, 0.7, 0.0));
    plane6->setColor(glm::vec3(1, 0.7, 0.0));



    sceneObjects.push_back(plane1);
    sceneObjects.push_back(plane2);
    sceneObjects.push_back(plane3);
    sceneObjects.push_back(plane4);
    sceneObjects.push_back(plane5);
    sceneObjects.push_back(plane6);
}

void back_ground()
{
    Plane *back_ground = new Plane (glm::vec3(-80., -20, -280),//Point A
                            glm::vec3(80., -20, -280),//Point B
                            glm::vec3(80., 80, -280),//Point C
                            glm::vec3(-80., 80, -280));//Point D
    back_ground->setColor(glm::vec3(0.2, 0.2, 0.2));
    back_ground->setSpecularity(false);
    sceneObjects.push_back(back_ground);
}

glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0);                     //Background colour = (0,0,0)
    glm::vec3 lightPos1(20, 40, -3);                 //Light's position
    glm::vec3 lightPos2(-20, 40, -3);                 //Light's position
    glm::vec3 color(0);

    SceneObject* obj;

    ray.closestPt(sceneObjects);                    //Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;       //no intersection
    obj = sceneObjects[ray.index];                  //object on which the closest point of intersection is found


    // normal vector calculater here
    glm::vec3 normalVector = sceneObjects[ray.index]->normal(ray.hit);
    normalVector = glm::normalize(normalVector);

    glm::vec3 lightVec1 = lightPos1 - ray.hit;
    glm::vec3 lightVec2 = lightPos2 - ray.hit;


//~ // texture
    if (ray.index == 1)     //texture ball
    {
        float U = asin(normalVector.x) / M_PI + 0.5;
        float V = asin(normalVector.y) / M_PI + 0.5;
        color = texture1.getColorAt(U, V);
        obj->setColor(color);
    }


    // floor patten
    if (ray.index == 2)
    {
        int stripeWidth = 5;

        int iz = (ray.hit.z - 300) / stripeWidth;
        int k1 = iz%2;

        int ix = (ray.hit.x - 300) / stripeWidth;
        int k2 = ix%2;

        if((k1 !=0 and k2 != 0) or (k1 ==0 and k2 == 0)){
            color = glm::vec3(0.06, 0.15, 0.47);
        } else {
            color = glm::vec3(1,1,1);
        }
        obj->setColor(color);
    }

    if (ray.index >= 12)
    {
        int k = ray.hit.x + ray.hit.y;
        int k1 = k %3;
        if (k1 == 0) {
            color = glm::vec3(0.3, 0.80, 0.45);
        } else if (k1 == 1) {
            color = glm::vec3(1,0,0);
        } else {
            color = glm::vec3(1,1,1);
        }
        obj->setColor(color);
    }

//~ //40., -20, -280
//~ //texture back ground
    if(ray.index == 3)
    {
        //y1 = -20, y2 = 80, x1 = -80, x2 = 80;
        float texcoords = (ray.hit.x + 80)/(80+80);
        float texcoordt = (ray.hit.y +20)/(80+20);
        if(texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
        {
            color=texture2.getColorAt(texcoords, texcoordt);
            obj->setColor(color);
        }
    }





//shadow computing
    Ray shadowRay1(ray.hit, lightVec1);
    shadowRay1.closestPt(sceneObjects);                    //Compare the ray with all objects in the scene
    Ray shadowRay2(ray.hit, lightVec2);
    shadowRay2.closestPt(sceneObjects);

    if(shadowRay1.index > -1 and shadowRay1.dist < glm::length(lightVec1)) {
        if(shadowRay2.index > -1 and shadowRay2.dist < glm::length(lightVec2)) {
            color = 0.2f * obj->getColor();
        } else{
            color = obj->lighting(lightPos2, -ray.dir, ray.hit);
        }
    } else {
        if(shadowRay2.index > -1 and shadowRay2.dist < glm::length(lightVec2)) {
            color = obj->lighting(lightPos1, -ray.dir, ray.hit);
        } else{
            color = (obj->lighting(lightPos1, -ray.dir, ray.hit))
            + (obj->lighting(lightPos2, -ray.dir, ray.hit))
            - (0.2f * obj->getColor());
        }
    }

//Transparency computing
    if (obj->isTransparent() && step < MAX_STEPS) {
        float rho = obj->getReflectionCoeff();
        //glm::vec3 n = obj->normal(ray.hit);
        glm::vec3 g = ray.dir;
        Ray transRay(ray.hit, g);
        transRay.closestPt(sceneObjects);
        glm::vec3 h = ray.dir;
        Ray transRay2(transRay.hit, h);
        //glm::vec3 m obj->normal(ray.hit);
        glm::vec3 transpColor = trace(transRay2, step + 1);
        color = color + (rho * transpColor);
        }


//reflection computing
    if (obj->isReflective() && step < MAX_STEPS) {
        float rho = obj->getReflectionCoeff();
        glm::vec3 normalVec = obj->normal(ray.hit);
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
        Ray reflectedRay(ray.hit, reflectedDir);
        glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
        color = color + (rho * reflectedColor);
        }

    if (obj->isRefractive() && step < MAX_STEPS) {
            float eta = (float)1 / obj->getRefractiveIndex();
            glm::vec3 normalVec = obj->normal(ray.hit);
            glm::vec3 refractDir = glm::refract(ray.dir, normalVec, eta);
            Ray refractRay(ray.hit, refractDir);
            refractRay.closestPt(sceneObjects);
            normalVec = obj->normal(refractRay.hit);
            refractDir = glm::refract(refractRay.dir, -normalVec, 1.0f / eta);
            Ray resultRay(refractRay.hit, refractDir);
            glm::vec3 refractedColor = trace(resultRay, step + 1);
            color = color + (obj->getRefractionCoeff() * refractedColor);
    }
    return color;
    }

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
glm::vec3 anti_aliasing(glm::vec3 eye, float cellX, float cellY, float xp, float yp)
{
    float x1 = cellX * 1/4;
    float x2 = cellX * 3/4;
    float y1 = cellY * 1/4;
    float y2 = cellY * 3/4;


    glm::vec3 color(0);

    glm::vec3 dir1(xp+x1, yp+y1, -EDIST);//left bot
    glm::vec3 dir2(xp+x2, yp+y1, -EDIST);//right bot
    glm::vec3 dir3(xp+x2, yp+y2, -EDIST);//right top
    glm::vec3 dir4(xp+x1, yp+y2, -EDIST);//left top

    Ray ray1 = Ray(eye, dir1);
    Ray ray2 = Ray(eye, dir2);
    Ray ray3 = Ray(eye, dir3);
    Ray ray4 = Ray(eye, dir4);

    color+=(trace(ray1,1)+trace(ray2,1)+trace(ray3,1)+trace(ray4,1));
    return color*0.25f;
}

void display()
{
    float xp, yp;  //grid point
    float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
    float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
    glm::vec3 eye(0., 0., 0.);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a tiny quad.

    for (int i = 0; i < NUMDIV; i++)    //Scan every cell of the image plane
    {
        xp = XMIN + i * cellX;
        for (int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j * cellY;
            /////////////////////////// anti ali start ////////////////////
            glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);  //direction of the primary ray


            //case1: no anti
            //~ Ray ray = Ray(eye, dir);
            //~ glm::vec3 col = trace(ray, 1);

            //case2: anti ali
            glm::vec3 col = anti_aliasing(eye,cellX,cellY,xp,yp);

            glColor3f(col.r, col.g, col.b);
            ////////////////////////// anti end /////////////////////////


            glVertex2f(xp, yp);
            glVertex2f(xp + cellX, yp);
            glVertex2f(xp + cellX, yp + cellY);
            glVertex2f(xp, yp + cellY);
        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

    texture1 = TextureBMP((char*)"sky.bmp");
    texture2 = TextureBMP((char*)"galaxy.bmp");

//0
    Sphere *sphere1 = new Sphere(glm::vec3(-10.0, -10.0, -70.0), 4.0);
    sphere1->setColor(glm::vec3(0, 0, 0.2));   //Set colour to blue
    sceneObjects.push_back(sphere1);         //Add sphere to scene objects
    sphere1->setTransparency(true);
    sphere1->setRefractivity(true, 1, 1.003);
    sphere1->setReflectivity(true, 0.2);

//1
    Sphere *sphere2 = new Sphere(glm::vec3(0.0, -5.0, -115.0), 5.0);
    sphere2->setColor(glm::vec3(1, 0, 0));   //Set colour to red
    sceneObjects.push_back(sphere2);         //Add sphere to scene objects
    sphere2->setSpecularity(true);
    //sphere2->setShininess(2);

//2
    Plane *plane = new Plane (glm::vec3(-40., -20, -60), //Point A
                              glm::vec3(40., -20, -60), //Point B
                              glm::vec3(40., -20, -280), //Point C
                              glm::vec3(-40., -20, -280)); //Point D
    plane->setColor(glm::vec3(0.8, 0.8, 0));  // orange
    sceneObjects.push_back(plane);
    plane->setSpecularity(false);

//3
    back_ground();
//4
    Cylinder *cylinder1 = new Cylinder(glm::vec3(20.0, -20.0, -120.0),2.0,20.0);
    cylinder1->setColor(glm::vec3(0.36, 0.30, 0.17));   //Set colour to qian blue
    sceneObjects.push_back(cylinder1);         //Add sphere to scene objects
//5,6,7,8,9,10
    Cube(-5, -20, -110, 10);

//11
    Sphere *sphere11 = new Sphere(glm::vec3(7.0, -10.0, -70.0), 2.0);
    sphere11->setColor(glm::vec3(0, 0, 0.2));   //Set colour to blue
    sceneObjects.push_back(sphere11);         //Add sphere to scene objects
    sphere11->setTransparency(true);
    sphere11->setReflectivity(true, 0.2);

//12 0.3, 0.80, 0.45
    Cone* cone1 = new Cone(glm::vec3(20, -10, -120), 10, 5);
    cone1->setColor(glm::vec3(0.08, 0.44, 0.0));
    sceneObjects.push_back(cone1);

    Cone* cone2 = new Cone(glm::vec3(20, -7, -120), 8, 5);
    cone2->setColor(glm::vec3(0.08, 0.44, 0.0));
    sceneObjects.push_back(cone2);

    Cone* cone3 = new Cone(glm::vec3(20, -4, -120), 5, 5);
    cone3->setColor(glm::vec3(0.08, 0.34, 0.0));
    sceneObjects.push_back(cone3);


    Cone* cone4 = new Cone(glm::vec3(20, -1, -120), 3, 4);
    cone4->setColor(glm::vec3(0.08, 0.24, 0.0));
    sceneObjects.push_back(cone4);


}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Assignmen2");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
