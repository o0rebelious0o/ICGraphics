#include "cgFile.h"
#include "cgRender.h"
#include "cgNormals.h"

/* Variables declared in cgRender.cpp *********************/
extern int pointCount;
extern int polygonCount;

extern vector <point> points;
extern vector <polygon> polygons;
/**********************************************************/

void readVTK()
{
	ifstream vtkFile;			//Input vtk file
	string temp;				//String to absorb file contents
	point currentPoint;			//Temporary vertex struct to work with
	polygon currentPolygon;		//Temporary polygon struct to work with
	int polygonPoints;			//Number of vertices a polygon has
	int pointNumber;			//Var to extract vertex number with

	cout << "Fetching geometry" << endl;
	
	//Open vtk file
	vtkFile.open("../data/face.vtk");

	//Absort header
	vtkFile >> temp;
	while (temp != "POINTS"){
		vtkFile >> temp;
	}
	//Number of vertices
	vtkFile >> pointCount;
	//Absorb unwanted information
	vtkFile >> temp;
	//Save all vertices from file
	for (int i=0; i < pointCount; i++)
	{
		vtkFile >> currentPoint.x;
		vtkFile >> currentPoint.y;
		vtkFile >> currentPoint.z;
		points.push_back(currentPoint);
	};
	//Absorb unwanted information
	vtkFile >> temp;
	//Number of polygons
	vtkFile >> polygonCount;
	//Absort unwanted information
	vtkFile >> temp;
	//Save all polygons to file
	for (int i=0; i < polygonCount; i++)
	{
		//Number of vertices polygon has
		vtkFile >> polygonPoints;
		//Clear temporary polygon struct
		currentPolygon.pointlist.clear();
		//Capture all associated vertices and save in vertex list
		for (int j=0; j < polygonPoints; j++)
		{
			vtkFile >> pointNumber;
			points[pointNumber].polygonlist.push_back(i);
			currentPolygon.pointlist.push_back(pointNumber);
		};

	//Calculate polygon normals
	currentPolygon = processNormals(currentPolygon);

	//Update associated vector normals for efficiency
	accumulateVNormals(currentPolygon.pointlist, currentPolygon.xNormal, currentPolygon.yNormal, currentPolygon.zNormal);

	//Push polygon template to polygon list to store
	polygons.push_back(currentPolygon);
	};

	//Absorb unwanted information
	while (temp != "float")
	{
		vtkFile >> temp;
	};

	//Read in x,y texture reference for each point
	for (int k = 0; k < pointCount; k++)
	{
		vtkFile >> points[k].xTexture;
		vtkFile >> points[k].yTexture;
	};

	//Close input file
	vtkFile.close();
}

void readTexture(void)
{
	string type;										//Temp var to read into
	extern int texHeight, texWidth, texMax;				//Global texture details
	extern unsigned char * ppmTex;						//Buffer to hold texture temporarily

	ifstream ppmFile;									//Input ppm file

	cout << "Fetching texture" << endl;

	//Open ppm file in binary mode
	ppmFile.open("../data/face.ppm", ios::binary);

	//Strip header
	ppmFile >> type;
	ppmFile >> texWidth;
	ppmFile >> texHeight;
	ppmFile >> texMax;
	
	//Strip extra pesky whitespace which otherwise causes colour shift in texture
	ppmFile.get();
	
	//Allocate buffer memory based on size of texture
	ppmTex = new unsigned char [(3*texWidth*texHeight*sizeof(unsigned int))];

	//Read block of data into the buffer for texture binding
	ppmFile.read((char *)ppmTex, (3*texWidth*texHeight)*sizeof(unsigned int));

	//Close input file
	ppmFile.close();
}
