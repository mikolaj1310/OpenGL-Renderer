#ifndef _SHAPE_H
#define _SHAPE_H

#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>

class Shape
{
public:
	void drawCube();
	void drawSkybox();
	void drawFloor();
	void drawMirror();
};
#endif 
