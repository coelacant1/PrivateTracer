#pragma once

#include "..\Render\Object3D.h"
#include "..\Materials\SimpleMaterial.h"

class Cube{
private:
	Vector3D basisVertices[33] = {Vector3D(-100.0000f,-100.0000f,-100.0000f),Vector3D(-100.0000f,100.0000f,-100.0000f),Vector3D(100.0000f,-100.0000f,-100.0000f),Vector3D(100.0000f,100.0000f,-100.0000f),Vector3D(100.0000f,-100.0000f,100.0000f),Vector3D(100.0000f,100.0000f,100.0000f),Vector3D(-100.0000f,-100.0000f,100.0000f),Vector3D(-100.0000f,100.0000f,100.0000f),Vector3D(75.0000f,-100.0000f,-75.0000f),Vector3D(75.0000f,75.0000f,-100.0000f),Vector3D(-100.0000f,-75.0000f,75.0000f),Vector3D(-75.0000f,-75.0000f,-100.0000f),Vector3D(100.0000f,75.0000f,75.0000f),Vector3D(-100.0000f,75.0000f,-75.0000f),Vector3D(100.0000f,-75.0000f,-75.0000f),Vector3D(75.0000f,100.0000f,75.0000f),Vector3D(-75.0000f,75.0000f,100.0000f),Vector3D(75.0000f,-75.0000f,100.0000f),Vector3D(-75.0000f,100.0000f,-75.0000f),Vector3D(-75.0000f,-100.0000f,75.0000f),Vector3D(-75.0000f,75.0000f,-100.0000f),Vector3D(100.0000f,45.0000f,-75.0000f),Vector3D(-100.0000f,75.0000f,75.0000f),Vector3D(100.0000f,75.0000f,-75.0000f),Vector3D(75.0000f,-75.0000f,-100.0000f),Vector3D(75.0000f,75.0000f,100.0000f),Vector3D(-100.0000f,-75.0000f,-75.0000f),Vector3D(75.0000f,100.0000f,-75.0000f),Vector3D(100.0000f,-75.0000f,75.0000f),Vector3D(-75.0000f,-75.0000f,100.0000f),Vector3D(-75.0000f,100.0000f,75.0000f),Vector3D(75.0000f,-100.0000f,75.0000f),Vector3D(-75.0000f,-100.0000f,-75.0000f)};
	IndexGroup basisIndexes[49] = {IndexGroup(21,12,1),IndexGroup(20,32,5),IndexGroup(19,28,4),IndexGroup(27,14,2),IndexGroup(6,5,29),IndexGroup(30,17,8),IndexGroup(3,4,22),IndexGroup(19,4,2),IndexGroup(18,30,7),IndexGroup(31,2,8),IndexGroup(20,5,7),IndexGroup(23,7,8),IndexGroup(3,22,15),IndexGroup(25,4,3),IndexGroup(10,21,2),IndexGroup(14,23,8),IndexGroup(9,33,1),IndexGroup(26,18,5),IndexGroup(30,8,7),IndexGroup(33,7,1),IndexGroup(18,7,5),IndexGroup(33,20,7),IndexGroup(14,8,2),IndexGroup(17,6,8),IndexGroup(16,8,6),IndexGroup(10,2,4),IndexGroup(27,2,1),IndexGroup(31,19,2),IndexGroup(28,6,4),IndexGroup(23,11,7),IndexGroup(16,31,8),IndexGroup(24,22,4),IndexGroup(32,3,5),IndexGroup(4,6,13),IndexGroup(11,1,7),IndexGroup(4,13,24),IndexGroup(9,1,3),IndexGroup(25,10,4),IndexGroup(3,15,29),IndexGroup(32,9,3),IndexGroup(17,26,6),IndexGroup(6,29,13),IndexGroup(12,3,1),IndexGroup(26,5,6),IndexGroup(12,25,3),IndexGroup(3,29,5),IndexGroup(21,1,2),IndexGroup(11,27,1),IndexGroup(28,16,6)};
	TriangleGroup triangleGroup = TriangleGroup(&basisVertices[0], &basisIndexes[0], 33, 49);
	SimpleMaterial simpleMaterial = SimpleMaterial(RGBColor(128, 128, 128));
	Object3D basisObj = Object3D(&triangleGroup, &simpleMaterial);

public:
	Cube(){}

	Object3D* GetObject(){
		return &basisObj;
	}
};
