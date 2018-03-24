#include "shared\freeglut.h"
#include <windows.h>
#include "Vector3D.h"

class CollisionManager {

	
	public:
		CollisionManager()
		{
			
		}
		  bool static collisionDetection(Vector3D firstVector, Vector3D otherVector, GLfloat radiusPlayer, GLfloat radiusObject)
		{
			GLfloat diffX = firstVector.x- otherVector.x;
			GLfloat diffY = firstVector.y - otherVector.y;
			GLfloat diffZ = firstVector.z - otherVector.z;

			float vecDist = sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
			if (vecDist <= (radiusPlayer + radiusObject))
			{
				return true;
			}
			return false;
		}

};


