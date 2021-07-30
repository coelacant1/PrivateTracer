#pragma once
#pragma once

#include "..\Materials\Material.h"
#include "..\Math\Transform.h"
#include "Triangle3D.h"
#include "..\Math\Vector2D.h"

class Triangle2D {
private:
    float denominator = 0.0f;
  
public:
	Vector2D p1;
	Vector2D p2;
	Vector2D p3;
    Vector2D v0;
    Vector2D v1;
	Vector3D normal;
    Material* material;

    Vector3D* t3p1;
    Vector3D* t3e2;
    Vector3D* t3e1;

    float averageDepth = 0.0f;

	Triangle2D(){}

	Triangle2D(Vector2D p1, Vector2D p2, Vector2D p3) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;

        v0 = p2 - p1;
        v1 = p3 - p1;
        
        denominator = 1.0f / (v0.X * v1.Y - v1.X * v0.Y);
	}

	Triangle2D(Quaternion lookDirection, Transform* camT, Triangle3D* t, Material* material) {
        this->material = material;

        Vector3D p1Normalized = camT->GetRotation().Multiply(lookDirection).UnrotateVector(*t->p1 - camT->GetPosition());
        Vector3D p2Normalized = camT->GetRotation().Multiply(lookDirection).UnrotateVector(*t->p2 - camT->GetPosition());
        Vector3D p3Normalized = camT->GetRotation().Multiply(lookDirection).UnrotateVector(*t->p3 - camT->GetPosition());

        averageDepth = (p1Normalized.Z + p2Normalized.Z + p3Normalized.Z) / 3.0f;

		this->p1 = Vector2D(p1Normalized);
		this->p2 = Vector2D(p2Normalized);
		this->p3 = Vector2D(p3Normalized);

		normal = t->Normal();

        t3p1 = t->p1;
        t3e1 = &t->edge1;
        t3e2 = &t->edge2;
        
        v0 = p2 - p1;
        v1 = p3 - p1;

        denominator = 1.0f / (v0.X * v1.Y - v1.X * v0.Y);
	}

    Material* GetMaterial(){
        return material;
    }

    bool DidIntersect(Vector2D ray, float& u, float& v, float& w) {
        float v2X = ray.X - p1.X;
        float v2Y = ray.Y - p1.Y;
        v = (v2X * v1.Y - v1.X * v2Y) * denominator;
        w = (v0.X * v2Y - v2X * v0.Y) * denominator;
        u = 1.0f - v - w;

        return u > 0 && v > 0 && w > 0;
    }

    String ToString() {
        return p1.ToString() + " " + p2.ToString() + " " + p3.ToString();
    }
};
