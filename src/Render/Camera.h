#pragma once

#include "..\Math\Rotation.h"
#include "..\Math\Transform.h"
#include "..\Render\CameraLayout.h"
#include "PixelGroup.h"
#include "Scene.h"
#include "Triangle2D.h"

class Camera {
private:
    Transform* transform;
    CameraLayout* cameraLayout;
    PixelGroup* pixelGroup;
    Quaternion lookDirection;
    bool customDirection = false;

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
            Vector3D rotateRay = transform->GetRotation().RotateVector(Vector3D(pixelRay.X, pixelRay.Y, 0).Multiply(transform->GetScale()).Add(transform->GetPosition()));
            
            color = triangles[triangle]->GetMaterial()->GetRGB(rotateRay, triangles[triangle]->normal);
        }
        
        return color;
    }

public:
    Camera(Transform* transform, CameraLayout* cameraLayout, PixelGroup* pixelGroup){
        this->transform = transform;
        this->pixelGroup = pixelGroup;
        this->cameraLayout = cameraLayout;

        transform->SetBaseRotation(cameraLayout->GetRotation());
    }

    Transform* GetTransform(){
        return transform;
    }

    void SetLookDirection(Quaternion lookDirection){
        customDirection = true;
        this->lookDirection = lookDirection;
    }
    
    void DisableCustomLookDirection(){
        customDirection = false;
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
        
        if(!customDirection){
            lookDirection = transform->GetRotation().Conjugate();
        }
        
        //for each object in the scene, get the triangles
        for(int i = 0; i < scene->numObjects; i++){
            if(scene->objects[i]->IsEnabled()){
                //for each triangle in object, project onto 2d surface, but pass material
                for (int j = 0; j < scene->objects[i]->GetTriangleAmount(); j++) {
                    triangles[triangleCounter] = new Triangle2D(lookDirection, transform, &scene->objects[i]->GetTriangles()[j], scene->objects[i]->GetMaterial());
                    
                    bool triangleInView = pixelGroup->ContainsVector2D(lookDirection.RotateVector(triangles[triangleCounter]->p1)) ||
                                          pixelGroup->ContainsVector2D(lookDirection.RotateVector(triangles[triangleCounter]->p2)) ||
                                          pixelGroup->ContainsVector2D(lookDirection.RotateVector(triangles[triangleCounter]->p3));

                    //triangleInView = 1;// = (triangleInView && !triangles[triangleCounter]->behindCamera);//culling behind camera
                    
                    if(triangleInView) triangleCounter++;
                    else delete triangles[triangleCounter];//out of view space remove from array
                }
            }
        }

        for (unsigned int i = 0; i < pixelGroup->GetPixelCount(); i++) {
            Vector2D pixelRay = Vector2D(lookDirection.RotateVector(pixelGroup->GetPixel(i)->GetPosition() * transform->GetScale()));//scale pixel location prior to rotating and moving
            pixelGroup->GetPixel(i)->Color = CheckRasterPixel(scene, triangles, triangleCounter, pixelRay);
        }

        for (int i = 0; i < triangleCounter; i++){
            delete triangles[i];
        }
        
        delete[] triangles;
    }
    
};
