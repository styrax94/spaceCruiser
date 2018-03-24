#pragma once
#include "shared\freeglut.h"
#include "windows.h"
#include "math.h"
#include <iostream>
#include "Vector3D.h"
#include "images.h"

class SpaceObjects {

	protected:
	Vector3D position;
	GLint type;
	GLboolean collidedWith;
	GLfloat radius;
	GLfloat rotation;

	public:
		virtual void setPositionZ(GLfloat z) = 0;
		virtual Vector3D getPosition() = 0;
		virtual bool getCollidedWith() = 0;
		virtual void setCollidedWith() = 0;
		virtual GLfloat getRadius() = 0;
		virtual void Render() = 0;
		virtual void Update() = 0;
		
		

};

class Planet : public SpaceObjects
{
private:
	GLfloat rotationAmount;
public:
	Planet()
	{
	}
	Planet(Vector3D Pos, GLint Type, GLfloat r,GLfloat rotateAmount) 
	{
		position = Pos;
		type = Type;
		collidedWith = false;
		radius = r;
	    rotation = 0.0f;
		rotationAmount = rotateAmount;

	}
	
	void setPositionZ(GLfloat Z)
	{
		position.z += Z;
	}
	Vector3D getPosition()
	{
		return position;
	};
	bool getCollidedWith()
	{
		return collidedWith;
	}
	void setCollidedWith()
	{
		collidedWith = true;
	}
	GLfloat getRadius()
	{
		return radius;
	}
	void Render()                           
	{
		
		glTranslatef(position.x, position.y, position.z);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(rotation, 0.0f, 0.0f, 1.0f);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[type]);
		GLUquadricObj *quadricJupiter;
		quadricJupiter = gluNewQuadric();
		gluQuadricDrawStyle(quadricJupiter, GLU_FILL);
		gluQuadricNormals(quadricJupiter, GLU_SMOOTH);
		if (type == 1)
		{
			gluQuadricOrientation(quadricJupiter, GLU_OUTSIDE);
		}
		else
		{
			gluQuadricOrientation(quadricJupiter, GLU_INSIDE);
		}
		
		gluQuadricTexture(quadricJupiter, GL_TRUE);
	   
		gluSphere(quadricJupiter, radius, 50, 35);
	
		glDisable(GL_TEXTURE_2D);
	};

	void Update()
	{
		rotation += rotationAmount;
	}


};
class Crystal : public SpaceObjects
{

	GLfloat rotationAmount;
public:

	Crystal(Vector3D Pos, GLint Type, GLfloat r,GLfloat rotateAmount)
	{
		position = Pos;
		type = Type;
		collidedWith = false;
		radius = r;
		rotation = 0.0f;
		rotationAmount = rotateAmount;
	}
	Crystal(){}

	void setPositionZ(GLfloat Z)
	{
		position.z = Z;
	}

	Vector3D getPosition()
	{
		return position;
	};
	bool getCollidedWith()
	{
		return collidedWith;
	}
	void setCollidedWith()
	{
		collidedWith = true;
	}
	GLfloat getRadius()
	{
		return radius;
	}
	void Render()
	{
		glTranslatef(position.x, position.y, position.z);
		glPushMatrix();
		glRotatef(rotation, 0.0, 1.0, 0.0);
		glScalef(radius, radius, radius);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glutSolidIcosahedron();
		glDisable(GL_BLEND);
		glPopMatrix();
	};

	void Update()
	{
		rotation += rotationAmount;
	}


};
class Hoop : public SpaceObjects
{
	GLfloat rotationAmount;
public:
	Hoop(Vector3D Pos, GLint Type, GLfloat r, GLfloat rotateAmount)
	{
		position = Pos;
		type = Type;
		collidedWith = false;
		radius = r;
		rotation = 0.0f;
		rotationAmount = rotateAmount;
	}
	Hoop(){}

	void setPositionZ(GLfloat Z)
	{
		position.z = Z;
	}

	Vector3D getPosition()
	{
		return position;
	};
	bool getCollidedWith()
	{
		return collidedWith;
	}
	void setCollidedWith()
	{
		collidedWith = true;
	}
	GLfloat getRadius()
	{
		return radius;
	}
	void Render()
	{

		glTranslatef(position.x, position.y, position.z);
		//glPushMatrix();
		
		glEnable(GL_BLEND);
		glColor4f(0.8f, 0.8f, 0.8f, 0.4f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[type]);
		glPushMatrix();
		glRotatef(-rotation, 0.0f,1.0f,0.0f);
		glutSolidTorus(1.0f, radius, 30, 30);
		glPopMatrix();
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	//	glPopMatrix();

	};

	void Update()
	{
		rotation += rotationAmount;
	}


};



