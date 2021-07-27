#pragma once

#include "..\Math\Rotation.h"
#include "..\Math\Transform.h"
#include "PixelGroup.h"
#include "Scene.h"
#include "Triangle2D.h"

class Camera {
private:
    Transform* transform;
    PixelGroup* pixelGroup;

    RGBColor CheckRasterPixel(Scene* scene, Triangle2D** triangles, int numTriangles, Vector2D pixelRay){
        float zBuffer = 3.402823466e+38f;
        int triangle = 0;
        bool didIntersect = false;
        Vector3D intersect;
        RGBColor color;
        
        for (int t = 0; t < numTriangles; t++) {
            float u = 0.0f, v = 0.0f, w = 0.0f;
            
            if (triangles[t]->DidIntersect(pixelRay, u, v, w)){

                intersect = *triangles[t]->t3p1 + (*triangles[t]->t3e2 * u) + (*triangles[t]->t3e1 * v);
                float rayDistanceToTriangle = transform->GetPosition().Add(Vector3D(pixelRay.X, pixelRay.Y, 0)).CalculateEuclideanDistance(intersect);
                
                if(rayDistanceToTriangle < zBuffer){
                    zBuffer = rayDistanceToTriangle;
                    triangle = t;
                }

                didIntersect = true;
            }
        }

        if(didIntersect){
            Vector3D rotateRay = transform->GetRotation().RotateVector(Vector3D(pixelRay.X, pixelRay.Y, 0).Subtract(transform->GetPosition()));
            
            color = triangles[triangle]->GetMaterial()->GetRGB(rotateRay, triangles[triangle]->normal);
        }
        
        return color;
    }

public:
    Camera(Transform* transform, PixelGroup* pixelGroup){
        this->transform = transform;
        this->pixelGroup = pixelGroup;
    }

    Transform* GetTransform(){
        return transform;
    }

    void Rasterize(Scene* scene) {
        int numTriangles = 0;
    
        //for each object in the scene, get the triangles
        for(int i = 0; i < scene->numObjects; i++){
            if(scene->objects[i]->IsEnabled()){
                numTriangles += scene->objects[i]->GetTriangleAmount();
            }
        }
        
        Triangle2D** triangles = new Triangle2D*[numTriangles];
        int triangleCounter = 0;
        int possible = 0;

        //for each object in the scene, get the triangles
        for(int i = 0; i < scene->numObjects; i++){
            if(scene->objects[i]->IsEnabled()){
                //for each triangle in object, project onto 2d surface, but pass material
                for (int j = 0; j < scene->objects[i]->GetTriangleAmount(); j++) {
                    triangles[triangleCounter] = new Triangle2D(transform, &scene->objects[i]->GetTriangles()[j], scene->objects[i]->GetMaterial());
                    possible++;
                    /*
                    bool triangleInView = pixelGroup->ContainsVector2D(triangles[triangleCounter]->p1) ||
                                          pixelGroup->ContainsVector2D(triangles[triangleCounter]->p2) ||
                                          pixelGroup->ContainsVector2D(triangles[triangleCounter]->p3);
                    */
                    bool triangleInView = 1;
                    
                    if(triangleInView) triangleCounter++;
                    else delete triangles[triangleCounter];//out of view space remove from array
                }
            }
        }
        
        Serial.print(possible);
        Serial.print("\t");
        Serial.println(triangleCounter);
 
        for (unsigned int i = 0; i < pixelGroup->GetPixelCount(); i++) {
            pixelGroup->GetPixel(i)->Color = CheckRasterPixel(scene, triangles, triangleCounter, pixelGroup->GetPixel(i)->GetPosition());
        }

        for (int i = 0; i < triangleCounter; i++){
            delete triangles[i];
        }
        
        delete[] triangles;
    }
    
};
