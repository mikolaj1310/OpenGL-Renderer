#include "Scene.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
		
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glEnable(GL_LIGHTING);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	crateTexture = SOIL_load_OGL_texture
	(
		"gfx/crate.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	floorTexture = SOIL_load_OGL_texture
	(
		"gfx/floor.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	skyboxTexture = SOIL_load_OGL_texture
	(
		"gfx/skybox.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	
	//load models from file
	deskModel.load("models/desk.obj", "models/desk.png");
	bedModel.load("models/bed.obj", "models/bed.jpg");
	laptopModel.load("models/laptop.obj", "models/laptop.png");
	chairModel.load("models/chair.obj", "models/chair.png");
	claptrapModel.load("models/claptrap.obj", "");
	spaceshipModel.load("models/spaceship.obj", "models/spaceship.jpg");
	
	glutSetCursor(GLUT_CURSOR_NONE);
	// Initialise scene variables
}

void Scene::handleInput(float dt)
{
	// Handle user input
	
	
}

void Scene::update(float dt)
{

	
	// update scene related variables.
	rotation += 5 * dt;

	if (input->isKeyDown('1'))
	{
		glPolygonMode(GL_FRONT, GL_LINE);
		input->SetKeyUp('1');
	}
	if (input->isKeyDown('2'))
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		input->SetKeyUp('2');
	}

	if (input->isKeyDown('a'))
	{
		camera.moveLeft(dt);
	}
	if (input->isKeyDown('d'))
	{
		camera.moveRight(dt);
	}
	if (input->isKeyDown('w'))
	{
		camera.moveForward(dt);
	}
	if (input->isKeyDown('s'))
	{
		camera.moveBackward(dt);
	}

	if (input->isKeyDown(32))
	{
		camera.moveUp(dt);
	}
	if (input->isKeyDown('q'))
	{
		camera.moveDown(dt);
	}
	if (input->isKeyDown('r'))
	{
		camera.resetCamera();
	}

	glutWarpPointer(width / 2, height / 2);
	if (input->getMouseX() < width / 2) {
		camera.rotate((input->getMouseX() - width / 2) * mouseSensitivity);
	}
	if (input->getMouseX() > width / 2) {
		camera.rotate((input->getMouseX() - width / 2) * mouseSensitivity);
	}
	if (input->getMouseY() < height / 2 && camera.getPitch() < 90) {
		camera.tilt(((height - input->getMouseY()) - height / 2) * mouseSensitivity);
	}
	if (input->getMouseY() > height / 2 && camera.getPitch() > -90) {
		camera.tilt((input->getMouseY() - height / 2) * (-1) * mouseSensitivity);
	}
	camera.update(dt);
	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {
	//set lighting ambient, diffuse and position
	GLfloat Light_Ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat Light_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat Light_Position[] = { 30.0f, 20.0f, -30.0f, 1.0f };
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z,
			camera.getLookAt().x, camera.getLookAt().y, camera.getLookAt().z,
			camera.getUp().x, camera.getUp().y, camera.getUp().z);

	
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0);
	glEnable(GL_LIGHT0);
	


	glEnable(GL_TEXTURE_2D);
	// Render geometry/scene here -------------------------------------

	//Draw skybox
	glPushMatrix();
		//move to camera position and draw skybox
		glTranslatef(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z); 
		glBindTexture(GL_TEXTURE_2D, skyboxTexture);
		shape.drawSkybox();
	glPopMatrix();

	//CAMERA ----------------------------------------
	glPushMatrix();
		//draw box around camera for debugging
		glTranslatef(Light_Position[0], Light_Position[1], Light_Position[2]);
		glBindTexture(GL_TEXTURE_2D, NULL);
		shape.drawCube();
	glPopMatrix();

	//render mirror ------------------------------------
	glPushMatrix();

		//enable stencil
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glDisable(GL_DEPTH_TEST);

		//draw mirror
		shape.drawMirror();
		glEnable(GL_DEPTH_TEST);
		
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		//failed attempt at shadows
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_DEPTH_TEST);
			glColor3f(0.1f, 0.1f, 0.1f);
			
			//transform and draw chair shahdow
			glPushMatrix();
				glScalef(1, 0, -1);
				glTranslatef(3, 0, 3.25);
				glRotatef(180, 0, 1, 0);
				glScalef(0.3, 0.3, 0.3);
				glMultMatrixf((GLfloat *)shadowMatrix);
				chairModel.render();
			glPopMatrix();
			
			//set colour back to white and ends the stencil test
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
		glPopMatrix();

			
		//render chair reflect ----------------------------
		glPushMatrix();
			glTranslatef(3, 0, 3.25);
			glRotatef(180, 0, 1, 0);
			glScalef(0.3, 0.3, 0.3);
			chairModel.render();
		glPopMatrix();

		//render desk reflect -----------------------------
		glPushMatrix();
		glTranslatef(2.25, 0, 1.5);
		glRotatef(5, 0, 1, 0);
		deskModel.render();
		glPopMatrix();

		//FLOOR reflect -----------------------------------------
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shape.drawFloor();
		glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();

		//render bed reflect ------------------------------------
		glPushMatrix();
		glTranslatef(7, 0, 0.25);
		glScalef(5, 5, 5);
		glRotatef(90, 0, 1, 0);
		bedModel.render();
		glPopMatrix();

		//render laptop reflect ------------------------------------
		glPushMatrix();
			glTranslatef(3, 1.78, 1);
			glRotatef(183, 0, 1, 0);
			glScalef(3.5, 3.5, 3.5);

			laptopModel.render();
		glPopMatrix();

			
		//render claptrap reflect ---------------------------------
		//draw 2 cubes, claptrap orbits the bottom cube, spaceship orbits the top cube
		glPushMatrix();
			glTranslatef(10, 0, 10);
			glPushMatrix();
				//draw first cube
				glTranslatef(0, 1, 0);
				glBindTexture(GL_TEXTURE_2D, crateTexture);
				shape.drawCube();
				
				glPushMatrix();
					//draw second cube
					glTranslatef(0, 1.5, 0);
					glScalef(0.5, 0.5, 0.5);
						
					glBindTexture(GL_TEXTURE_2D, crateTexture);
					shape.drawCube();

					//position and draw spaceship
					glPushMatrix();
						glRotatef(rotation * 3, 0, 1, 1);
						glTranslatef(4, 0, 0);
						glRotatef(45, 1, 0, 0);
						spaceshipModel.render();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, NULL);
			//position and draw claptrap
			glRotatef(-rotation, 0, 1, 0);
			glTranslatef(3, 0, 0);
			glScalef(40, 40, 40);			
			claptrapModel.render();
		glPopMatrix();

			

		//-------------------------
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);

		glColor4f(0.8f, 0.8f, 1.0f, 0.8f);
		shape.drawMirror();
		glEnable(GL_LIGHTING);
		glDisable(GL_BLEND);
			
		//render floor ------------------------------------
		glPushMatrix();
			glTranslatef(0, 0, -15);
			glBindTexture(GL_TEXTURE_2D, floorTexture);
			shape.drawFloor();
			glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();

		//render chair -------------------------------------
		glPushMatrix();
			glTranslatef(3, 0, -3.25);
			glScalef(-0.3, 0.3, 0.3);
				
			chairModel.render();
		glPopMatrix();

		//render desk ------------------------------------
		glPushMatrix();
			glTranslatef(2.25, 0, -1.5);
			glRotatef(-185, 0, 1, 0);
			glScalef(-1, 1, 1);
			deskModel.render();
		glPopMatrix();

		//render bed ------------------------------------
		glPushMatrix();
			glTranslatef(11.03f, 0, -0.25);
			glScalef(5, 5, 5);
			glRotatef(-90, 0, 1, 0);

			bedModel.render();
		glPopMatrix();

		//render laptop ------------------------------------
		glPushMatrix();
			glTranslatef(3, 1.78, -1);
			glRotatef(3, 0, 1, 0);
			glScalef(3.5, 3.5, 3.5);

			laptopModel.render();
		glPopMatrix();

			
		//render claptrap ---------------------------------
		//draw 2 cubes, claptrap orbits the bottom cube, spaceship orbits the top cube
		glPushMatrix();
			glTranslatef(10, 0, -10);

			glPushMatrix();
				//draw first cube
				glTranslatef(0, 1, 0);
				glBindTexture(GL_TEXTURE_2D, crateTexture);
				shape.drawCube();
				glPushMatrix();
					//draw second cube
					glTranslatef(0, 1.5, 0);
					glScalef(0.5, 0.5, 0.5);
						
					glBindTexture(GL_TEXTURE_2D, crateTexture);
					shape.drawCube();

					//position and draw spaceship
					glPushMatrix();
						glRotatef(rotation * 3, 0, 1, 1);
						glTranslatef(4, 0, 0);
						glRotatef(45, 1, 0, 0);
						spaceshipModel.render();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, NULL);
			//position and draw claptrap
			glRotatef(-rotation, 0, 1, 0);
			glTranslatef(3, 0, 0);
			glScalef(40, 40, 40);			
			claptrapModel.render();
		glPopMatrix();
			
		//more failed shadow attempts
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glColor3f(0.1f, 0.1f, 0.1f);
		glPushMatrix();
		glMultMatrixf((GLfloat *)shadowMatrix);

		glTranslatef(11.03f, 0, 0.25);
		glScalef(5, 5, 5);
		glRotatef(-90, 0, 1, 0);

		bedModel.render();

		glColor3f(1, 1, 1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

		
	glPopMatrix();


	//Draw spinning cubes outside of mirror
	glTranslatef(0, 0, -30);
	glPushMatrix();
		
		glRotatef(rotation, 1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, crateTexture);
		shape.drawCube();

		glTranslatef(5, 0, 0);
		shape.drawCube();

		glTranslatef(5, 0, 0);
		shape.drawCube();
		glBindTexture(GL_TEXTURE_2D, NULL);
		
		
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	
}

// Calculates FPS
void Scene::calculateFPS()
{

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan() {

	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
}