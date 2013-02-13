#ifndef CGRENDER_H
#define CGRENDER_H

#include <GL/glut.h>
#include <stdlib.h>
#include <vector>

using namespace std;

#include <iostream>

//Struct to hold vertex information
struct point {
	//Coordinates
	float x;
	float y;
	float z;

	//Vertex Normal
	float xNormal;
	float yNormal;
	float zNormal;

	//2D texture coordinates
	float xTexture;
	float yTexture;

	//List of associated polygons
	vector <int> polygonlist;
};

//Struct to hold polygon information
struct polygon {
	//List of points polygon is made from
	vector <int> pointlist;

	//Polygon normal
	float xNormal;
	float yNormal;
	float zNormal;
};

#endif
