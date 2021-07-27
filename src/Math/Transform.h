#pragma once

#include "Rotation.h"
#include "Vector3D.h"

class Transform{
private:
    Quaternion rotation = Quaternion(1, 0, 0, 0);
    Vector3D position = Vector3D(0, 0, 0);
    Vector3D scale = Vector3D(1, 1, 1);

public:
    Transform(){}

    Transform(Vector3D eulerXYZR, Vector3D position, Vector3D scale){
        this->rotation = Rotation(EulerAngles(eulerXYZR, EulerConstants::EulerOrderXYZR)).GetQuaternion();;
        this->position = position;
        this->scale = scale;
    }

    Transform(Quaternion rotation, Vector3D position, Vector3D scale){
        this->rotation = rotation;
        this->position = position;
        this->scale = scale;
    }

    void SetRotation(Quaternion rotation){
        this->rotation = rotation;
    }

    void SetRotation(Vector3D eulerXYZR){
        this->rotation = Rotation(EulerAngles(eulerXYZR, EulerConstants::EulerOrderXYZR)).GetQuaternion();
    }

    Quaternion GetRotation(){
        return rotation;
    }

    void SetPosition(Vector3D position){
        this->position = position;
    }

    Vector3D GetPosition(){
        return position;
    }

    void SetScale(Vector3D scale){
        this->scale = scale;
    }

    Vector3D GetScale(){
        return scale;
    }

    void Rotate(Vector3D eulerXYZR){
        this->rotation = this->rotation * Rotation(EulerAngles(eulerXYZR, EulerConstants::EulerOrderXYZR)).GetQuaternion();
    }

    void Rotate(Quaternion rotation){
        this->rotation = this->rotation * rotation;
    }

    void Translate(Vector3D offset){
        this->position = this->position + offset;
    }

    void Scale(Vector3D scale){
        this->scale = this->scale * scale;
    }
};
