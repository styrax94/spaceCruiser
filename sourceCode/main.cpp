//Windows includes - DO NOT EDIT AS LOTS OF INTERDEPENDENCE

#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include "GLee.h"
#include <gl/freeglut.h>
#include <iostream>
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#include <vector>
#include "SpaceObjects.h"
#include "CollisionManager.h"
#include <time.h>
using namespace std;
//note that this needs gltools.h and gltools.cpp to be included in the shared directory
void resetPerspectiveProjection();
void drawAngleWithScaledText(GLfloat scale);
void setOrthographicProjection();
void ChangeSize(int w, int h);
void RenderScene(void);
void TimerFunc(int value);

int Wwidth;
int Wheight;
//used for the framerate
int frame = 0, timeTot = 0, timebase = 0;
char frameRate[12];
char angleShipRotation[12];
char cameraPositionX[12];
char cameraPositionY[12];
char cameraPositionZ[12];
Vector3D *playerRotation = new Vector3D(0.0f, 0.0f, 0.0f);
Vector3D positionCamera;
bool collisionDetection = false;
bool hudIsActive = false;
Vector3D cameraPosition;

vector<SpaceObjects*>hoops;
vector<SpaceObjects*>crystals;
vector<SpaceObjects*>allObjectsInSpace;
GLboolean populated = false;
const GLint MAXTARGETS = 10;
GLint numOfTargets;
GLboolean isTarget[MAXTARGETS];
Vector3D* camera = new Vector3D(0.0f, 0.0f, -100.0f);
SpaceObjects* planet = new Planet(Vector3D(0.0f, 0.0f, -600.0f), 1, 170.0f, 1.0f);
SpaceObjects * moon = new Planet(Vector3D(-300.0f, 0.0f, 0.0f), 1, 35.0f, 5.0f);
SpaceObjects * space = new Planet(Vector3D(0.0f, 0.0f, 0.0f), 9, 2000.0f, 0.0f);
Vector3D playerPosition;

GLfloat rotateMoon = 0.0;

//for lighting or materials if you want to experiment with these

#pragma region ColorLight 



GLfloat mKa[4] = { 0.11f,0.11f,0.11f,1.0f }; //ambient
GLfloat mKd[4] = { 0.43f,0.43f,0.43f,1.0f }; //diffuse
GLfloat mKs[4] = { 0.8f,0.8f,0.8f,1.0f }; //specular
GLfloat mKe[4] = { 0.5f,0.5f,0.0f,1.0f }; //emission



//////////experementing lights  just neeed them for creating glass effect for crystals
//still not sure which colors i mixed in other project to obtain the effect

GLfloat no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat no_mat2[] = { 0.0f, 0.0f, 0.0f, 0.5f };
GLfloat  mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat  mat_ambient_color[] = { 0.8f, 0.8f, 0.2f, 1.0f };
GLfloat  mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
GLfloat  mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  no_shininess[] = { 0.0f };
GLfloat  low_shininess[] = { 5.0f };
GLfloat  high_shininess[] = { 100.0f };
GLfloat  mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
GLfloat  mat_emission2[] = { 0.5f, 0.2f, 0.2f, 0.0f };
GLfloat  black_ambient[] = { 0.0,0.0,0.0,0.0 };


//spot position and direction
GLfloat	 lightPos[] = { 0.0f, 100.0f, 300.0f, 0.0f };
GLfloat  spotDir[] = { 50.0, 25.0, 0.0 };

GLfloat	 lightPos2[] = { 50.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir2[] = { 50.0, 15.0, 0.0 };

GLfloat	 lightPos3[] = { -50.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir3[] = { 50.0, 15.0, 0.0 };


// Useful lighting colour values
GLfloat  whiteLightBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  greenLight[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat  blueLight[] = { 0.0f, 1.0f, 1.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };

#pragma endregion


GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;

GLboolean upKeyPressed = false;
GLboolean downKeyPressed = false;
GLboolean leftKeyPressed = false;
GLboolean rightKeyPressed = false;
GLboolean wKeyPressed = false;
GLboolean sKeyPressed = false;
GLboolean aKeyPressed = false;
GLboolean dKeyPressed = false;
//end of intialisation 

//--- ---------------------------------//
void userInput()
{
	if(upKeyPressed)
	{
		camera->y += -1.0;
		
	}
	else if(downKeyPressed)
	{
		camera->y += 1.0;
		
	}
	else if (leftKeyPressed)
	{
		 playerRotation->z -= 1.0;
		if (playerRotation->z < 0)
		{
			playerRotation->z = 360 + playerRotation->z;
		}
		if (playerRotation->z >= 361)
		{
			playerRotation->z = 0;
		}
		glutPostRedisplay();
	}
	else if (rightKeyPressed)
	{
		playerRotation->z += 1.0;
		if (playerRotation->z >= 360)
		{
			playerRotation->z = 0;
		}
		glutPostRedisplay();
	}
	else if (wKeyPressed)
	{
		camera->z += 1.0;
		
	}
	else if (sKeyPressed)
	{
		camera->z += -1.0;
		
	}
	else if (aKeyPressed)
	{
		camera->x += 1.0;
		
	}
	else if (dKeyPressed)
	{
		camera->x += -1.0;
	}

}
void updatePosition()
{
	planet->setPositionZ(playerPosition.z);

}
void createTargets()
{
	Vector3D position;
	GLfloat x, y, z;
	GLboolean viablePos = false;
	GLfloat randomNum = rand() % 100 - 50;
	while(!viablePos)
	{
		
	  x = rand() % 100 - 50;
	  y = rand() % 150 -75;
	  z = rand() % 160 - 80;
	  
	  position = Vector3D(x, y, z);

	  for(int i =0; i < allObjectsInSpace.size(); i++)
	  {
		 if(!CollisionManager::collisionDetection(position, allObjectsInSpace[i]->getPosition(), 10, allObjectsInSpace[i]->getRadius()))
		 {
			 viablePos = true;
		 
		 }
		
		
	  }
	 
	  if(allObjectsInSpace.size() <= 0)
	  {
		  cout << "three" << endl;
		  viablePos = true;
	  }
	  
	  
	} 
	  SpaceObjects *hoop = new Hoop(position, 7,5.0f,1.0f);
	  SpaceObjects *crystal = new Crystal(position, 1,2.0f,2.0f);
	  hoops.push_back(hoop);
	  crystals.push_back(crystal);
	  allObjectsInSpace.push_back(hoop);
	  allObjectsInSpace.push_back(crystal);
	 

}
//this function pops back to the last projection
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void displayText(float x, float y, int r, int g, int b, const char *string, bool stroke) {
	int j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (int i = 0; i < j; i++) {
		if (stroke) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
		}
		else {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
		}

	}
}

void drawAngleWithScaledText(GLfloat scale) {

	glPushMatrix();
	//draw the text offset from the box
	glTranslatef(Wwidth/2 - Wwidth/20, Wheight/5, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(angleShipRotation, "Angle:%3.0f", playerRotation->z);
	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText
	displayText(500, 100, 0, 1, 0, angleShipRotation, true);
	glPopMatrix();


	glPushMatrix();
	//draw the text offset from the box
	glTranslatef(Wwidth / 2 - Wwidth / 20, Wheight / 4, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(cameraPositionX, "X_POS:%3.0f", playerPosition.x);
	//flip
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText
	displayText(500, 100, 0, 1, 0, cameraPositionX, true);
	glPopMatrix();

	glPushMatrix();
	//draw the text offset from the box
	glTranslatef(Wwidth / 2 - Wwidth / 20, Wheight / 3.5, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(cameraPositionY, "Y_POS:%3.0f", playerPosition.y);
	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText
	displayText(500, 100, 0, 1, 0, cameraPositionY, true);
	glPopMatrix();

	glPushMatrix();
	//draw the text offset from the box
	glTranslatef(Wwidth / 2 - Wwidth / 20, Wheight / 3, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(cameraPositionZ, "Z_POS:%3.0f", playerPosition.z);
	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText
	displayText(500, 100, 0, 1, 0, cameraPositionZ, true);
	glPopMatrix();

}

void drawFrameRate() {

	//this draws the framerate on the screen - useful for testing animation
	frame++;
	timeTot = glutGet(GLUT_ELAPSED_TIME);
	if (timeTot - timebase > 1000) {
		sprintf_s(frameRate, "FPS: %4.2f",
			frame*1000.0 / (timeTot - timebase));
		timebase = timeTot;
		frame = 0;
	}
	//set the text to white
	glColor3f(1.0f, 1.0f, 1.0f);
	//this stop the text being affected by the current tranformation by reseting it
	glLoadIdentity();
	displayText(30, 30, 1, 0, 0, frameRate, false);


}

//sets up the orthographics projection for 2D overlay
void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	Wwidth = w;
	Wheight = h;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;
	//need this for changing projection
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 625
	gluPerspective(60.0f, fAspect, 1.0, 3200.0);
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);

}


GLUquadricObj *space2;      
void drawSpace(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {

	glPushMatrix();
	glTranslatef(x, y, z);
	// Create and texture the ball
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[SPACE]);
	// glDisable(GL_LIGHTING);
	glColor3f(0.5, 0.5, 0.5);
	space2 = gluNewQuadric();
	gluQuadricDrawStyle(space2, GLU_FILL);
	gluQuadricNormals(space2, GLU_SMOOTH);
	gluQuadricOrientation(space2, GLU_INSIDE);
	gluQuadricTexture(space2, GL_TRUE);
	gluSphere(space2, r, 50, 35);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void setOrthographicProjection() {
	// switch to projection mode

	glMatrixMode(GL_PROJECTION);
	// save the previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection

	gluOrtho2D(0, Wwidth, 0, Wheight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -Wheight, 0);
	//set for drawing again
	glMatrixMode(GL_MODELVIEW);
}

void drawHUD(int image)
{
	glEnable(GL_BLEND);
	glColor4f(0.8, 0.8, 0.8, 0.4);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	//bind the texture
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	
	glRotatef(playerRotation->z,0.0f,0.0f,1.0f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);//repeated texture
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(Wwidth, 0.0, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(Wwidth, Wheight, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(0.0, Wheight, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
// Called to draw scene

void RenderScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	
	//movement
	glTranslatef(camera->x, camera->y,camera->z);
	glRotatef(playerRotation->z, 0.0f, 0.0f, 1.0f);

	glPushMatrix();   
	glDisable(GL_LIGHTING);
	glTranslatef(playerPosition.x, playerPosition.y, playerPosition.z);
	space->Render();
	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	if (!populated)
		for (int i = 0; i < MAXTARGETS; i++)
		{
			
			createTargets();
			populated = true;
			numOfTargets = MAXTARGETS;
			
			if(i == 0)
			{
				isTarget[i] = true;
			}
			else
			{
				isTarget[i] = false;
			}
		}
	//check if all targets has been hit, then erases them
	if(numOfTargets <=0)
	{
		populated = false;

		for(int i = 0; i < crystals.size(); i++)
		{
		
			for(int b = 0; b < allObjectsInSpace.size(); b++)
			{
			   if(crystals[i] == allObjectsInSpace[b])
			   {
				   crystals.erase(crystals.begin() + i);
				   hoops.erase(hoops.begin() + i);
				   allObjectsInSpace.erase(allObjectsInSpace.begin() + b);
			   }
				
			}
		
		}
	
	}

	//draws hoops and crystals
	for(int size = 0; size < hoops.size(); size++)
	{
		glPushMatrix();
		if (!crystals[size]->getCollidedWith() && !isTarget[size])
		{
			glColor4f(1.0, 0.0, 0.0, 0.3);
		}
		else if(isTarget[size] && !crystals[size]->getCollidedWith())
		{
			glColor4f(0.0, 0.0, 1.0, 0.3);
		}
		else { glColor4f(0.0, 1.0, 0.0, 0.8); }
		crystals[size]->Render();
		glPopMatrix();

		glPushMatrix();
		hoops[size]->Render();
		glPopMatrix();
	
	}
	
	rotateMoon += 2;
	if(rotateMoon >360)
	{
		rotateMoon = 0;
	}
	
		glPushMatrix();
		if (playerPosition.z - planet->getPosition().z >= 700.0f)
		{
			if (planet->getPosition().z >= -600.0f)
			planet->setPositionZ(1.0f);
		}
		else
		{ 
			
			planet->setPositionZ(-1.0f); 
		
		}
		cout << planet->getPosition().z;
		cout <<playerPosition.z - planet->getPosition().z << endl;
		planet->Render();
		glRotatef(rotateMoon, 0.0f, 0.0f, 1.0f);
		glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		moon->Render();
		glPopMatrix();
		glPopMatrix();
		
		

		//2d
		glPushMatrix();
		glLoadIdentity();
		setOrthographicProjection();

		if (hudIsActive == true)
		{
			drawAngleWithScaledText(0.2f);

			
			glBegin(GL_LINES);
			glColor3f(1.0, 1.0, 1.0);
			glVertex2f(20.0, 40.0);
			glVertex2f(200.0, 40.0);
			glEnd();
			drawFrameRate();
			drawHUD(HUD);
		}
		
		glPopMatrix();
		
	resetPerspectiveProjection();
	glutSwapBuffers();
}
// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
	//textures
	GLuint texture;
	
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//make sure any TGA has no alpha channel - photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is from the OpenGL SuperBible book
	//there are quite a few ways of loading images
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//below are good if we were tiling a repeated pattern

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//try these too
		 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		 glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		//modulate the texture and vertex colours
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		free(pBytes0);
	}

	//enable textures
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal   
	glFrontFace(GL_CCW);// Counter clock-wise polygons face out
	

    glCullFace(GL_FRONT_AND_BACK);

								// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
	//glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0f);
	//glEnable(GL_LIGHT2);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	// All materials hereafter have full specular reflectivity
	// with a high shine

	glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Black blue background clear colour
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}

void TimerFunc(int value)
{
	userInput();
	
	playerPosition = Vector3D(-camera->x, -camera->y, -camera->z);
	GLfloat x, y;
	x = playerPosition.x;
	y = playerPosition.y;
 playerPosition.x = x * cosf(-playerRotation->z*0.017453292519943296) - y*sinf(-playerRotation->z*0.017453292519943296);
 playerPosition.y = x*sinf(-playerRotation->z*0.017453292519943296) + y* cosf(-playerRotation->z*0.017453292519943296);
	
	for(int size = 0; size < allObjectsInSpace.size(); size ++)
	{
		allObjectsInSpace[size]->Update();
	
	}
	

	for (int i = 0; i < crystals.size(); i++)
	{
		if (CollisionManager::collisionDetection(playerPosition, crystals[i]->getPosition(), 1.0f, crystals[i]->getRadius()))
		{
			if(!crystals[i]->getCollidedWith() && isTarget[i])
			{
			  crystals[i]->setCollidedWith();
			  numOfTargets--;
			  isTarget[i] = false;
			  int b = i;
			  b =  b + 1;
			  b=  b % MAXTARGETS;
			  isTarget[b] = true;
			}
			
			
		}

	}
	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);
}  

void init() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	allObjectsInSpace.push_back(planet);
	allObjectsInSpace.push_back(moon);
}

void arrowKeyDown(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		leftKeyPressed = true;
		
	}
	if (key == GLUT_KEY_RIGHT)
	{
		rightKeyPressed = true;
		
	}

	if (key == GLUT_KEY_UP)
	{
		
		upKeyPressed = true;
	}

	if (key == GLUT_KEY_DOWN)
	{
		downKeyPressed = true;
	}


}
void arrowKeyReleased(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
		{
			upKeyPressed = false;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			downKeyPressed = false;
			break;
		}
		case GLUT_KEY_LEFT:
		{
			leftKeyPressed = false;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			rightKeyPressed = false;
			break;
		}

	}
}
void KeyDown(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'w':
		wKeyPressed = true;
		break;
	case'a':
		aKeyPressed = true;
		break;
	case 's':
		sKeyPressed = true;
		break;
	case'd':
		dKeyPressed = true;
		break;
	case 27:		//ESC
		PostQuitMessage(0);
		break;
	case 'h':
		if (hudIsActive)
		{
			hudIsActive = false;
		}
		else
		{
			hudIsActive = true;
		}
		break;
	}
}
void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		wKeyPressed = false;
		break;
	case'a':
		aKeyPressed = false;
		break;
	case 's':
		sKeyPressed = false;
		break;
	case'd':
		dKeyPressed = false;
		break;
	}

}
int main(int argc, char* argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);// a 4:3 ratio
	glutCreateWindow("Space Hoops");
	init();
	SetupRC();
	glutSpecialFunc(arrowKeyDown);
	glutSpecialUpFunc(arrowKeyReleased);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(keyUp);
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, TimerFunc, 1);
	
	
	
	glutMainLoop();
	return 0;
}










