/*
    This file is part of W3ZMapEditor (c).

    W3ZMapEditor is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    W3ZMapEditor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with W3ZMapEditor; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "stdafx.h"
#include "glcamera.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
// M_PI may not defined in some "math.h"!
#ifndef M_PI
	#define M_PI	3.1415926535897932384626433832795
#endif

/*
class CGLCamera
***************
Implementation
*/
/*
CGLCamera
*********
Standard constructor.

In:
 nothing
*/
CGLCamera::CGLCamera(void)
{
	// coordinates
	m_dCameraX = 1.0;
	m_dCameraY = 1.0;
	m_dCameraZ = 1.0;
	// target
	m_dTargetX = 0.0;
	m_dTargetY = 0.0;
	m_dTargetZ = 0.0;
	// target vector
	m_dTargetVecX = -1.0/sqrt(3.0);
	m_dTargetVecY = -1.0/sqrt(3.0);
	m_dTargetVecZ = -1.0/sqrt(3.0);
	// up vector
	m_dUpX = 0.0;
	m_dUpY = 1.0;
	m_dUpZ = 0.0;
	// save current position
	SavePosition();
}


/*
~CGLCamera
**********
Standard destructor.
*/
CGLCamera::~CGLCamera(void)
{
}


/*
Draw
****
Draws the camera origin and the camera target as two crosses with different
colors. Can be usefull to see from another point of view what a camera does.

In:
 nothing

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::Draw(void)
{
	GLboolean glbLightIsEnabled;
	// gets lighting status
	glbLightIsEnabled = glIsEnabled(GL_LIGHTING);
	// creates the 3D object
	glDisable(GL_LIGHTING); // object not affected by lights
	glBegin(GL_LINES);
		// camera origin
		// X vector
		glColor4fv(CAM_ORIGIN_COLOR);
		glVertex3d(m_dCameraX - CAM_MODEL_WIDTH/2.0, m_dCameraY, m_dCameraZ);
		glVertex3d(m_dCameraX + CAM_MODEL_WIDTH/2.0, m_dCameraY, m_dCameraZ);
		// Y vector
		glVertex3d(m_dCameraX, m_dCameraY - CAM_MODEL_WIDTH/2.0, m_dCameraZ);
		glVertex3d(m_dCameraX, m_dCameraY + CAM_MODEL_WIDTH/2.0, m_dCameraZ);
		// Z vector
		glVertex3d(m_dCameraX, m_dCameraY, m_dCameraZ - CAM_MODEL_WIDTH/2.0);
		glVertex3d(m_dCameraX, m_dCameraY, m_dCameraZ + CAM_MODEL_WIDTH/2.0);

		// camera target
		glColor4fv(CAM_TARGET_COLOR);
		// X vector
		glVertex3d(m_dTargetX - CAM_MODEL_WIDTH/2.0, m_dTargetY, m_dTargetZ);
		glVertex3d(m_dTargetX + CAM_MODEL_WIDTH/2.0, m_dTargetY, m_dTargetZ);
		// Y vector
		glVertex3d(m_dTargetX, m_dTargetY - CAM_MODEL_WIDTH/2.0, m_dTargetZ);
		glVertex3d(m_dTargetX, m_dTargetY + CAM_MODEL_WIDTH/2.0, m_dTargetZ);
		// Z vector
		glVertex3d(m_dTargetX, m_dTargetY, m_dTargetZ - CAM_MODEL_WIDTH/2.0);
		glVertex3d(m_dTargetX, m_dTargetY, m_dTargetZ + CAM_MODEL_WIDTH/2.0);
	glEnd();
	// checks if lighting needs to be re-enabled
	if (glbLightIsEnabled)
	{
		// re-enable lights effects
		glEnable(GL_LIGHTING);
	}
}


/*
Normalize
*********
Normalizes the camera->target vector == makes the distance between the camera
and its target equal to 1.

In:
 nothing

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::Normalize(void)
{
	GLdouble gldDeltaX, gldDeltaY, gldDeltaZ;
	GLdouble gldLength;
	// gets vector
	gldDeltaX = m_dTargetX - m_dCameraX;
	gldDeltaY = m_dTargetY - m_dCameraY;
	gldDeltaZ = m_dTargetZ - m_dCameraZ;
	// gets vector length
	gldLength = sqrt(gldDeltaX*gldDeltaX + gldDeltaY*gldDeltaY + gldDeltaZ*gldDeltaZ);
	// check for a null vector
	if (gldLength == 0)
	{
		gldLength = 1.0;
	}
	// normalize the target vector
	m_dTargetVecX = gldDeltaX/gldLength;
	m_dTargetVecY = gldDeltaY/gldLength;
	m_dTargetVecZ = gldDeltaZ/gldLength;
}


/*
Watch
*****
Sets OpenGL view to this camera view.

In:
 nothing

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::Watch(void)
{
	if (cos(m_gldTheta) < 0)
	{
		gluLookAt(m_dCameraX, m_dCameraY, m_dCameraZ,
				  m_dTargetX, m_dTargetY, m_dTargetZ,
				  m_dUpX, -1.0*m_dUpY, m_dUpZ);
	}
	else
	{
		gluLookAt(m_dCameraX, m_dCameraY, m_dCameraZ,
				  m_dTargetX, m_dTargetY, m_dTargetZ,
				  m_dUpX, m_dUpY, m_dUpZ);
	}
}


/*
MoveForward
***********
Moves the camera forward following the X-Z plane.
If theta is equal to M_PI/2.0 or -M_PI/2.0, it uses the last phi known.

In:
 GLdouble gldDist: forward distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::MoveForward(GLdouble gldDist)
{
	// moves the camera
	m_dCameraX += gldDist*cos(m_gldPhi);
	m_dCameraZ += gldDist*sin(m_gldPhi);
	// moves the target
	m_dTargetX += gldDist*cos(m_gldPhi);
	m_dTargetZ += gldDist*sin(m_gldPhi);
}


/*
MoveBack
********
Moves the camera backward following the X-Z plane.
If theta is equal to M_PI/2.0 or -M_PI/2.0, it uses the last phi known.

In:
 GLdouble gldDist: back distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::MoveBack(GLdouble gldDist)
{
	// moves the camera
	m_dCameraX -= gldDist*cos(m_gldPhi);
	m_dCameraZ -= gldDist*sin(m_gldPhi);
	// moves the target
	m_dTargetX -= gldDist*cos(m_gldPhi);
	m_dTargetZ -= gldDist*sin(m_gldPhi);
}


/*
MoveUp
******
Moves up the camera.

In:
 GLdouble gldDist: up distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::MoveUp(GLdouble gldDist)
{
	// moves the camera
	m_dCameraY += gldDist;
	// moves the target
	m_dTargetY += gldDist;
}


/*
MoveDown
********
Moves down the camera.

In:
 GLdouble gldDist: down distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::MoveDown(GLdouble gldDist)
{
	// moves the camera
	m_dCameraY -= gldDist;
	// moves the target
	m_dTargetY -= gldDist;
}


/*
StrafeLeft
**********
Moves the camera on its left (X-Z plane) without changing its direction.

In:
 GLdouble gldDist: distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::StrafeLeft(GLdouble gldDist)
{
	GLdouble gldPhi2;
	// computes the left direction
	gldPhi2 = m_gldPhi - M_PI/2.0;

	// moves the camera
	m_dCameraX += gldDist*cos(gldPhi2);
	m_dCameraZ += gldDist*sin(gldPhi2);
	// moves the target
	m_dTargetX += gldDist*cos(gldPhi2);
	m_dTargetZ += gldDist*sin(gldPhi2);
}


/*
StrafeRight
***********
Moves the camera on its right (X-Z plane) without changing its direction.

In:
 GLdouble gldDist: distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::StrafeRight(GLdouble gldDist)
{
	GLdouble gldPhi2;
	// computes the right direction
	gldPhi2 = m_gldPhi + M_PI/2.0;

	// moves the camera
	m_dCameraX += gldDist*cos(gldPhi2);
	m_dCameraZ += gldDist*sin(gldPhi2);
	// moves the target
	m_dTargetX += gldDist*cos(gldPhi2);
	m_dTargetZ += gldDist*sin(gldPhi2);
}


/*
ZoomIn
******
Moves the camera forward following its direction.

In:
 GLdouble gldDist: zoom distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::ZoomIn(GLdouble gldDist)
{
	// moves the camera
	m_dCameraX += gldDist*cos(m_gldTheta)*cos(m_gldPhi);
	m_dCameraY += gldDist*sin(m_gldTheta);
	m_dCameraZ += gldDist*cos(m_gldTheta)*sin(m_gldPhi);
	// moves the target
	m_dTargetX += gldDist*cos(m_gldTheta)*cos(m_gldPhi);
	m_dTargetY += gldDist*sin(m_gldTheta);
	m_dTargetZ += gldDist*cos(m_gldTheta)*sin(m_gldPhi);
}


/*
ZoomOut
*******
Moves the camera backward following its direction.

In:
 GLdouble gldDist: zoom distance

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::ZoomOut(GLdouble gldDist)
{
	// moves the camera
	m_dCameraX -= gldDist*cos(m_gldTheta)*cos(m_gldPhi);
	m_dCameraY -= gldDist*sin(m_gldTheta);
	m_dCameraZ -= gldDist*cos(m_gldTheta)*sin(m_gldPhi);
	// moves the target
	m_dTargetX -= gldDist*cos(m_gldTheta)*cos(m_gldPhi);
	m_dTargetY -= gldDist*sin(m_gldTheta);
	m_dTargetZ -= gldDist*cos(m_gldTheta)*sin(m_gldPhi);
}


/*
TurnLeft
********
Turns camera view to the left.

In:
 GLdouble gldAngle: rotation angle (rad)

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::TurnLeft(GLdouble gldAngle)
{
	SetCameraAngles(m_gldTheta, m_gldPhi - gldAngle);
}


/*
TurnRight
*********
Turns camera view to the right.

In:
 GLdouble gldAngle: rotation angle (rad)

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::TurnRight(GLdouble gldAngle)
{
	SetCameraAngles(m_gldTheta, m_gldPhi + gldAngle);
}


/*
TurnUp
******
Makes the camera look up.

In:
 GLdouble gldAngle: rotation angle (rad)

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::TurnUp(GLdouble gldAngle)
{
	SetCameraAngles(m_gldTheta + gldAngle, m_gldPhi);
}


/*
TurnDown
********
Makes the camera look down.

In:
 GLdouble gldAngle: rotation angle (rad)

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::TurnDown(GLdouble gldAngle)
{
	SetCameraAngles(m_gldTheta - gldAngle, m_gldPhi);
}


/*
SetPosition
***********
Sets camera position.

In:
 GLdouble const agldCoords[]: new coordinates

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::SetPosition(GLdouble const agldCoords[])
{
	m_dCameraX = agldCoords[0];
	m_dCameraY = agldCoords[1];
	m_dCameraZ = agldCoords[2];

	// updates angles
	UpdateAngles();
}


/*
GetPosition
***********
Returns Camera position.

In:
 nothing

Out:
 GLdouble agldCoords[]: current camera position

Return:
 nothing
*/
void CGLCamera::GetPosition(GLdouble agldCoords[])
{
	agldCoords[0] = m_dCameraX;
	agldCoords[1] = m_dCameraY;
	agldCoords[2] = m_dCameraZ;
}


/*
SetTarget
*********
Changes camera target.

In:
 GLdouble const agldCoords[]: target coordinates

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::SetTarget(GLdouble const agldCoords[])
{
	m_dTargetX = agldCoords[0];
	m_dTargetY = agldCoords[1];
	m_dTargetZ = agldCoords[2];

	// updates angles
	UpdateAngles();
}


/*
GetTarget
*********
Returns camera target coordinates.

In:
 nothing

Out:
 GLdouble agldCoords[]: current camera target coordinates

Return:
 nothing
*/
void CGLCamera::GetTarget(GLdouble agldCoords[])
{
	agldCoords[0] = m_dTargetX;
	agldCoords[1] = m_dTargetY;
	agldCoords[2] = m_dTargetZ;
}


/*
SetUpVector
***********
Changes camera up vector.

In:
 GLdouble const agldCoords[]: new up vector

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::SetUpVector(GLdouble const agldVect[])
{
	m_dUpX = agldVect[0];
	m_dUpY = agldVect[1];
	m_dUpZ = agldVect[2];
}


/*
GetUpVector
***********
Returns camera up vector.

In:
 nothing

Out:
 GLdouble agldCoords[]: current camera target coordinates

Return:
 nothing
*/
void CGLCamera::GetUpVector(GLdouble agldVect[])
{
	agldVect[0] = m_dUpX;
	agldVect[1] = m_dUpY;
	agldVect[2] = m_dUpZ;

}


/*
SetCameraAngles
***************
Changes camera angle.

In:
 GLdouble gldTheta: theta angle (see class description for angles details)
 GLdouble gldPhi: phi angle (see class description for angles details)

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::SetCameraAngles(GLdouble gldTheta, GLdouble gldPhi)
{
	m_gldTheta = gldTheta;
	m_gldPhi = gldPhi;
	m_dTargetX = cos(gldTheta)*cos(gldPhi) + m_dCameraX;
	m_dTargetY = sin(gldTheta) + m_dCameraY;
	m_dTargetZ = cos(gldTheta)*sin(gldPhi) + m_dCameraZ;
}


/*
GetCameraAngles
***************
Returns camera angles.

In:
 nothing

Out:
 GLdouble &gldTheta: current theta angle (Rad)
 GLdouble &gldPhi: current phi angle (Rad)

Return:
 nothing
*/
void CGLCamera::GetCameraAngles(GLdouble &gldTheta, GLdouble &gldPhi)
{
	gldTheta = m_gldTheta;
	gldPhi = m_gldPhi;
}


/*
UpdateAngles
************
Updates camera angles.
Call this function when the direction vector has changed (camera moved or target moved).

In:
 nothing

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::UpdateAngles(void)
{
		// updates angles
	Normalize();
	if ((0 == m_dTargetVecX) && (0 == m_dTargetVecZ))
	{
		if (m_dTargetVecY < 0)
		{
			m_gldPhi = -1.0*M_PI/2.0;
		}
		else
		{
			m_gldPhi = M_PI/2.0;
		}
	}
	else
	{
		m_gldPhi = atan2(m_dTargetVecZ, m_dTargetVecX);
	}

	m_gldTheta = asin(m_dTargetVecY);
}


/*
SavePosition
************
Saves camera current position and target.

In:
 nothing

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::SavePosition(void)
{
	// saved coordinates
	m_dSavedPosX = m_dCameraX;
	m_dSavedPosY = m_dCameraY;
	m_dSavedPosZ = m_dCameraZ;
	// saved target coordinates
	m_dSavedTargetX = m_dTargetX;
	m_dSavedTargetY = m_dTargetY;
	m_dSavedTargetZ = m_dTargetZ;
}


/*
LoadPosition
************
Loads camera saved position and target.

In:
 nothing

Out:
 nothing

Return:
 nothing
*/
void CGLCamera::LoadPosition(void)
{
	// saved coordinates
	m_dCameraX = m_dSavedPosX;
	m_dCameraY = m_dSavedPosY;
	m_dCameraZ = m_dSavedPosZ;
	// saved target coordinates
	m_dTargetX = m_dSavedTargetX;
	m_dTargetY = m_dSavedTargetY;
	m_dTargetZ = m_dSavedTargetZ;
	// updates angles
	UpdateAngles();
}