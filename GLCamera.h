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
#pragma once
#include <gl/gl.h>
#include <gl/glu.h>

//CAMERA CONSTANTS
const GLfloat CAM_MODEL_WIDTH			= 2.0;
const GLfloat CAM_ORIGIN_COLOR[]		= {1.0, 0.0, 0.0, 1.0}; // red
const GLfloat CAM_TARGET_COLOR[]		= {0.0, 1.0, 0.0, 1.0}; // green

/*
*******************
* class CGLCamera *
*******************
This class implements a camera object and some usefull camera functions.
It assumes that the horizontal axis is X (right positive),
 the vertical axis is Y (up positive)
 and the depth axis is Z (back of the screen is negative)
The two angles associated with the camera are theta and phi.
Both of these angles are radian angles and share the same top which is the camera
coordinates.

Theta is the angle between the X-Z plane and the target.
Phi is the angle between the X-Y plane and the target.

*/
class CGLCamera
{
protected:
	// coordinates
	GLdouble m_dCameraX;
	GLdouble m_dCameraY;
	GLdouble m_dCameraZ;
	// target
	GLdouble m_dTargetX;
	GLdouble m_dTargetY;
	GLdouble m_dTargetZ;
	// target vector (normalized)
	GLdouble m_dTargetVecX;
	GLdouble m_dTargetVecY;
	GLdouble m_dTargetVecZ;
	// angles
	GLdouble m_gldPhi;
	GLdouble m_gldTheta;
	// up vector
	GLdouble m_dUpX;
	GLdouble m_dUpY;
	GLdouble m_dUpZ;
	// saved coordinates
	GLdouble m_dSavedPosX;
	GLdouble m_dSavedPosY;
	GLdouble m_dSavedPosZ;
	// saved target coordinates
	GLdouble m_dSavedTargetX;
	GLdouble m_dSavedTargetY;
	GLdouble m_dSavedTargetZ;
	// normalize camera-target vector function
	void Normalize();
	void UpdateAngles();
public:
	// constructor
	CGLCamera();
	// destructor
	~CGLCamera();
	void Watch();
	void MoveForward(GLdouble gldDist);
	void MoveBack(GLdouble gldDist);
	void MoveUp(GLdouble gldDist);
	void MoveDown(GLdouble gldDist);
	void StrafeLeft(GLdouble gldDist);
	void StrafeRight(GLdouble gldDist);
	void ZoomIn(GLdouble gldDist);
	void ZoomOut(GLdouble gldDist);
	void TurnLeft(GLdouble gldAngle);
	void TurnRight(GLdouble gldAngle);
	void TurnUp(GLdouble gldAngle);
	void TurnDown(GLdouble gldAngle);
	void SetPosition(GLdouble const agldCoords[3]);
	void GetPosition(GLdouble agldCoords[3]);
	void SetTarget(GLdouble const agldCoords[3]);
	void GetTarget(GLdouble agldCoords[3]);
	void SetUpVector(GLdouble const agldVect[3]);
	void GetUpVector(GLdouble agldVect[3]);
	void SetCameraAngles(GLdouble gldTheta, GLdouble gldPhi);
	void GetCameraAngles(GLdouble &gldTheta, GLdouble &gldPhi);
	void SavePosition();
	void LoadPosition();
	void Draw();
};