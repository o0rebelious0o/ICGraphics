#include "cgNormals.h"
#include "cgRender.h"

//Global variables declared in cgRender.cpp
extern vector <point> points;
extern vector <polygon> polygons;

polygon processNormals(polygon p)
{
	float v1x, v1y, v1z, v2x, v2y, v2z, magnitude, xNormal, yNormal, zNormal;

	//Calculate two vectors for polygon to cross product
	v1x = (points[p.pointlist[1]].x - points[p.pointlist[0]].x);
	v1y = (points[p.pointlist[1]].y - points[p.pointlist[0]].y);
	v1z = (points[p.pointlist[1]].z - points[p.pointlist[0]].z);
	v2x = (points[p.pointlist[0]].x - points[p.pointlist[2]].x);
	v2y = (points[p.pointlist[0]].y - points[p.pointlist[2]].y);
	v2z = (points[p.pointlist[0]].z - points[p.pointlist[2]].z);

	//Calculate cross product to give normal
	xNormal = (v1y*v2z) - (v1z*v2y);
	yNormal = (v1z*v2x) - (v1x*v2z);
	zNormal = (v1x*v2y) - (v1y*v2x);

	//Normalize normal vector
	magnitude = xNormal*xNormal;
	magnitude += yNormal*yNormal;
	magnitude += zNormal*zNormal;
	magnitude = sqrt(magnitude);
	p.xNormal = xNormal/magnitude;
	p.yNormal = yNormal/magnitude;
	p.zNormal = zNormal/magnitude;

	return p;
}

void accumulateVNormals(vector <int> v, float x, float y, float z)
{
	for (int i = 0; i < v.size(); i++)
	{
		points[v[i]].xNormal += x;
		points[v[i]].yNormal += y;
		points[v[i]].zNormal += z;
	};
}

void normaliseVNormals(void)
{
	float magnitude;
	//For each vertex
	for ( int i = 0; i < points.size(); i++)
	{	
		//Calculate magnitude and normalise vector
		magnitude = sqrt(points[i].xNormal*points[i].xNormal + points[i].yNormal*points[i].yNormal + points[i].zNormal*points[i].zNormal);
		points[i].xNormal = points[i].xNormal/magnitude;
		points[i].yNormal = points[i].yNormal/magnitude;
		points[i].zNormal = points[i].zNormal/magnitude;
	};
}
