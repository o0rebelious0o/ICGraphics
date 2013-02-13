#ifndef CG_NORMALS
#define CG_NORMALS

#include <stdlib.h>
#include <math.h>
#include "cgRender.h"

using namespace std;

polygon processNormals(polygon);

void accumulateVNormals(vector <int>, float, float, float);

void normaliseVNormals(void);

#endif
