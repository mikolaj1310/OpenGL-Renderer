#include "shape.h"


//    6-------5
//   /|      /|
//  1-------0 |
//  | |     | |
//  | 7-----|-4
//  |/      |/
//  2------ 3

extern float cubeVerts[] = { 1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,	 // 0-1-2-3 front
							 1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,  // 0-3-4-5 right
							 1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,  // 0-5-6-1 top
						  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1, // 1-6-7-2 left
						  -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1, // 7-4-3-2 bottom
						  1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1	 // 4-7-6-5 back
};

extern float norms[] = { 0,0,1,  0,0,1,  0,0,1,  0,0,1,             // 0-1-2-3
						 1,0,0,  1,0,0,  1,0,0, 1,0,0,              // 0-3-4-5
						 0,1,0,  0,1,0,  0,1,0, 0,1,0,              // 0-5-6-1
						 -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // 1-6-7-2
						 0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // 7-4-3-2
						 0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1		    // 4-7-6-5	
};

extern float texcoords[]= {
							1, 0,	 1, 1,	0, 1,	 0, 0,	//front
							1, 0,	 1, 1,	0, 1,	 0, 0,	//right
							1, 0,	 1, 1,	0, 1,	 0, 0,	//top
							1, 0,	 1, 1,	0, 1,	 0, 0,	//left
							1, 0,	 1, 1,	0, 1,	 0, 0,	//bottom
							1, 0,	 1, 1,	0, 1,	 0, 0,	//back
							};	

extern GLubyte cubeIndices[] ={	0, 1, 2, 3,		//front
								4, 5, 6, 7,		//right
								8, 9, 10, 11,	//top
								12, 13, 14, 15,	//left
								16, 17, 18, 19,	//bottom
								20, 21, 22, 23	//back
};

void Shape::drawCube()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//point to where vertex, normal, index and texture coordinates are
	glVertexPointer(3, GL_FLOAT, 0, cubeVerts);
	glNormalPointer(GL_FLOAT, 0, norms);
	glIndexPointer(GL_FLOAT, 0, cubeIndices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	//draw my cube based on what is contained in the arrays
	glDrawElements(GL_QUADS, sizeof(cubeIndices), GL_UNSIGNED_BYTE, cubeIndices);
	

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void Shape::drawSkybox()
{
	//disable lighting and depth test so it gets rendered behind everything
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//draw cube
	glPushMatrix();
		glBegin(GL_QUADS);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.25f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);	//V0

		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.5f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);	//V3

		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(1.0f, 1.0f, -1.0f);	//V7

		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f(-1.0f, 1.0f, -1.0f);	//V4

		// Front Wall
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.75f, 0.25f);
		glVertex3f(1.0f, 1.0f, 1.0f);	//V3

		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.25f);
		glVertex3f(-1.0f, 1.0f, 1.0f);	//V0

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(-1.0f, -1.0f, 1.0f);//V1

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.75f, 0.5f);
		glVertex3f(1.0f, -1.0f, 1.0f);	//V2

		// Back Wall
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f(-1.0f, 1.0f, -1.0f);	//V4

		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(1.0f, 1.0f, -1.0f);	//V7

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(1.0f, -1.0f, -1.0f);	//V6

		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f(-1.0f, -1.0f, -1.0f);//V5

		// Right Wall
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.5f, 0.25f);
		glVertex3f(1.0f, 1.0f, -1.0f);	//V7

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.75f, 0.25f);
		glVertex3f(1.0f, 1.0f, 1.0f);	//V3

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.75f, 0.5f);
		glVertex3f(1.0f, -1.0f, 1.0f);	//V2

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(1.0f, -1.0f, -1.0f);	//V6

		// Left Wall
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.25f);
		glVertex3f(-1.0f, 1.0f, 1.0f);	//V0

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.25f, 0.25f);
		glVertex3f(-1.0f, 1.0f, -1.0f);	//V4

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f(-1.0f, -1.0f, -1.0f);//V5

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.5f);
		glVertex3f(-1.0f, -1.0f, 1.0f);	//V1

		// Floor
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.25f, 0.5f);
		glVertex3f(-1.0f, -1.0f, -1.0f);//V5

		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.5f, 0.5f);
		glVertex3f(1.0f, -1.0f, -1.0f);	//V6

		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.5f, 0.75f);
		glVertex3f(1.0f, -1.0f, 1.0f);	//V2

		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.25f, 0.75f);
		glVertex3f(-1.0f, -1.0f, 1.0f);	//V1

		glEnd();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

}

void Shape::drawFloor()
{
	//set texture to repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//draw flat plane
	//set max texture coordinates to 3 so the texture repeats 3 times
	glBegin(GL_QUADS);
		glEnable(GL_BLEND);
		glColor3f(0, 0, 0);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 3.0f);
		glVertex3f(0.0f, 0.0f, 15.0f);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(3.0f, 3.0f);
		glVertex3f(15.0f, 0.0f, 15.0f);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(3.0f, 0.0f);
		glVertex3f(15.0f, 0.0f, 0.0f);
		glDisable(GL_BLEND);
	glEnd();
}

void Shape::drawMirror()
{
	//draw flat plane representing the mirror
	glBegin(GL_QUADS);
	
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0, 0, 0);

		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 2.0f);
		glVertex3f(0, 3, 0);

		glNormal3f(0, 0, 1);
		glTexCoord2f(2.0f, 2.0f);
		glVertex3f(15, 3, 0);

		glNormal3f(0, 0, 1);
		glTexCoord2f(2.0f, 0.0f);
		glVertex3f(15, 0, 0);

	glEnd();	
}