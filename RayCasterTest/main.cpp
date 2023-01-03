/**
@brief			Main
@author			Eskeptor
@date			2023-01-03
@version		0.0.2
*/

#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include "Player.h"
#include "Map2D.h"

// Radian
constexpr double RAD = 0.0174533;
// PI
constexpr double PI = 3.1415926535;
// OpenGL Window Width
constexpr int WIN_WIDTH = 1024;
// OpenGL Window Height
constexpr int WIN_HEIGHT = 640;
// OpenGL Window -> 3D Width
constexpr int WIN3D_WIDTH = 640;
// OpenGL Window -> 3D Start Position
constexpr int WIN3D_START = 0;
// OpenGL Ray Res
constexpr double RAY_RES = 4.0;
// OpenGL Speedrate
constexpr double SPEED_RATE = 0.1;

// Player class
CPlayer player;
// 2D Map class
CMap2D map2D;

/**
@brief		Degree To Radian Converting Function
@param		dDeg	: Degree
@return		Radian
*/
inline double DegreeToRadian(double dDeg)
{
	return dDeg * PI / 180.0;
}

/**
@brief		Angle Check (Over 360 or Under 0)
@param		dAngle	: Angle
@return		Angle
*/
inline double CheckAngle(double dAngle)
{
	if (dAngle > 359.9)
		dAngle -= 360.0;
	if (dAngle < 0)
		dAngle += 360.0;

	return dAngle;
}

/**
@brief		Measures the distance between two coordinates function
@param		dX1		: 1st x position
@param		dY1		: 1st y position
@param		dX2		: 2nd x position
@param		dY2		: 2nd y position
@param		dAngle	: Angle between 1st and 2nd
@return		Distance
*/
inline double GetDistance(double dX1, double dY1, double dX2, double dY2, double dAngle)
{
	return (cos(DegreeToRadian(dAngle)) * (dX2 - dX1) - sin(DegreeToRadian(dAngle)) * (dY2 - dY1));
}

/**
@brief		Measures the distance between two coordinates function
@param		dX1		: 1st x position
@param		dY1		: 1st y position
@param		dX2		: 2nd x position
@param		dY2		: 2nd y position
@return		Distance
*/
inline double GetDistance(double  dX1, double dY1, double dX2, double dY2)
{
	return sqrt(pow(dX1 - dX2, 2) + pow(dY1 - dY2, 2));
}

/**
@brief		Player draw function
@param
@return
*/
void DrawPlayer()
{
	double dPosX = player.GetPosX() * map2D.GetMinimapScale();
	double dPosY = player.GetPosY() * map2D.GetMinimapScale();
	double dDeltaX = player.GetDeltaX() * map2D.GetMinimapScale() * 5.0;
	double dDeltaY = player.GetDeltaY() * map2D.GetMinimapScale() * 5.0;

	glColor3f(1.0f, 1.0f, 0.0f);
	glPointSize((GLfloat)(player.GetSize() * map2D.GetMinimapScale()));
	glBegin(GL_POINTS);
	{
		glVertex2d(dPosX, dPosY);
	}
	glEnd();

	glLineWidth((GLfloat)((player.GetSize() - 2.0) * map2D.GetMinimapScale()));
	glBegin(GL_LINES);
	{
		glVertex2d(dPosX, dPosY);
		glVertex2d(dPosX + (dDeltaX * 4.0), dPosY + (dDeltaY * 4.0));
	}
	glEnd();
}

/**
@brief		2D Map draw function
@param
@return
*/
void Draw2DMap()
{
	int nWidth = map2D.GetWidth();
	int nHeight = map2D.GetHeight();
	double dTileSize = map2D.GetMinimapTileSize();

	// Background
	//glBegin(GL_QUADS);
	//{
	//	glColor3i(0, 0, 0);
	//	glVertex2d(0.0, 0.0);
	//	glVertex2d(nWidth * dTileSize, 0.0);
	//	glVertex2d(nWidth * dTileSize, nHeight * dTileSize);
	//	glVertex2d(0.0, nHeight * dTileSize);
	//}
	//glEnd();

	for (int nY = 0; nY < nHeight; nY++)
	{
		for (int nX = 0; nX < nWidth; nX++)
		{
			if (map2D.GetMapTileIndex(CMap2D::eMapIdx::MapWall, nX, nY) == 1)
			{
				glColor3f(1, 1, 1);
			}
			else 
			{
				glColor3f(0, 0, 0);
			}

			glBegin(GL_QUADS);
			{
				glVertex2d(nX * dTileSize + 1, nY * dTileSize + 1);
				glVertex2d((nX + 1) * dTileSize - 1, nY * dTileSize + 1);
				glVertex2d((nX + 1) * dTileSize - 1, (nY + 1) * dTileSize - 1);
				glVertex2d(nX * dTileSize + 1, (nY + 1) * dTileSize - 1);
			}
			glEnd();
		}
	}
}



/**
@brief		2D Ray draw function
@param
@return
*/
void DrawRay2D()
{
	int nDepthOfField = 0;
	double dTan = 0.0;
	double dRayX = 0.0;
	double dRayY = 0.0;
	double dOffsetX = 0.0;
	double dOffsetY = 0.0;
	double dHorX = 0.0;
	double dHorY = 0.0;
	double dVerX = 0.0;
	double dVerY = 0.0;
	double dDistanceT = 0.0;
	int nFOVAngle = (int)player.GetFOVAngle();
	double dAngle = CheckAngle(player.GetAngle() + (nFOVAngle / 2.0));
	double dLineH = 0.0;
	double dLineOffset = 0.0;
	double dFishAngle = 0.0;
	double dCosAngle = 0.0;
	double dSinAngle = 0.0;
	double dShade = 0.0;
	int nLine3DWidth = WIN3D_WIDTH / 2 / 20;

	int nMapX = 0;
	int nMapY = 0;

	int nHorMapTileIdx = 0;
	int nVerMapTileIdx = 0;

	int nCurMapTileIdx = 0;

	for (int nR = 0; nR < nFOVAngle; nR++)
	{
		nHorMapTileIdx = 0;
		nVerMapTileIdx = 0;

		dCosAngle = cos(DegreeToRadian(dAngle));
		dSinAngle = sin(DegreeToRadian(dAngle));

		// Horizontal Raycasting ==============================================================
		{
			nDepthOfField = 0;
			player.SetHorizontalDistance(1000000.0);
			dHorX = player.GetPosX();
			dHorY = player.GetPosY();
			dTan = 1.0 / tan(DegreeToRadian(dAngle));

			// Up View
			if (dSinAngle > 0.001)
			{
				dRayY = (((int)player.GetPosY() | (map2D.GetTileSize() - 1)) & ~(map2D.GetTileSize() - 1)) - 0.0001;
				dRayX = (player.GetPosY() - dRayY) * dTan + player.GetPosX();
				dOffsetY = -map2D.GetTileSize();
				dOffsetX = -dOffsetY * dTan;
			}
			// Down View
			else if (dSinAngle < -0.001)
			{
				dRayY = (((int)player.GetPosY() | (map2D.GetTileSize() - 1)) & ~(map2D.GetTileSize() - 1)) + map2D.GetTileSize();
				dRayX = (player.GetPosY() - dRayY) * dTan + player.GetPosX();
				dOffsetY = map2D.GetTileSize();
				dOffsetX = -dOffsetY * dTan;
			}
			// Left or Right View (Straight)
			else
			{
				dRayX = player.GetPosX();
				dRayY = player.GetPosY();
				nDepthOfField = nLine3DWidth;
			}

			while (nDepthOfField < nLine3DWidth)
			{
				nMapX = (int)dRayX >> map2D.GetPowN();
				nMapY = (int)dRayY >> map2D.GetPowN();

				// Wall hit
				if (nMapX >= 0 &&
					nMapX < map2D.GetWidth() &&
					nMapY >= 0 &&
					nMapY < map2D.GetHeight())
				{
					nCurMapTileIdx = map2D.GetMapTileIndex(CMap2D::eMapIdx::MapWall, nMapX, nMapY);

					if (nCurMapTileIdx > CMap2D::eTileType::Bottom)
					{
						player.SetHorizontalDistance(GetDistance(player.GetPosX(), player.GetPosY(), dRayX, dRayY, dAngle));
						nDepthOfField = nLine3DWidth;
						nHorMapTileIdx = nCurMapTileIdx;
					}
					// Check next line
					else
					{
						dRayX += dOffsetX;
						dRayY += dOffsetY;
						nDepthOfField += 1;
					}
				}
				// Check next line
				else
				{
					dRayX += dOffsetX;
					dRayY += dOffsetY;
					nDepthOfField += 1;
				}
			}

			dHorX = dRayX;
			dHorY = dRayY;
		}
		// ====================================================================================

		// Vertical Raycasting ================================================================
		{
			nDepthOfField = 0;
			player.SetVerticalDistance(1000000.0);
			dVerX = player.GetPosX();
			dVerY = player.GetPosY();
			dTan = tan(DegreeToRadian(dAngle));

			// Left View
			if (dCosAngle > 0.001)
			{
				dRayX = (((int)player.GetPosX() | (map2D.GetTileSize() - 1)) & ~(map2D.GetTileSize() - 1)) + map2D.GetTileSize();
				dRayY = (player.GetPosX() - dRayX) * dTan + player.GetPosY();
				dOffsetX = map2D.GetTileSize();
				dOffsetY = -dOffsetX * dTan;
			}
			// Right View
			else if (dCosAngle < -0.001)
			{
				dRayX = (((int)player.GetPosX() | (map2D.GetTileSize() - 1)) & ~(map2D.GetTileSize() - 1)) - 0.0001;
				dRayY = (player.GetPosX() - dRayX) * dTan + player.GetPosY();
				dOffsetX = -map2D.GetTileSize();
				dOffsetY = -dOffsetX * dTan;
			}
			// Left or Right View
			else
			{
				dRayX = player.GetPosX();
				dRayY = player.GetPosY();
				nDepthOfField = nLine3DWidth;
			}

			while (nDepthOfField < nLine3DWidth)
			{
				nMapX = (int)dRayX >> map2D.GetPowN();
				nMapY = (int)dRayY >> map2D.GetPowN();

				// Wall hit
				if (nMapX >= 0 &&
					nMapX < map2D.GetWidth() &&
					nMapY >= 0 &&
					nMapY < map2D.GetHeight())
				{
					nCurMapTileIdx = map2D.GetMapTileIndex(CMap2D::eMapIdx::MapWall, nMapX, nMapY);

					if (nCurMapTileIdx > CMap2D::eTileType::Bottom)
					{
						player.SetVerticalDistance(GetDistance(player.GetPosX(), player.GetPosY(), dRayX, dRayY, dAngle));
						nDepthOfField = nLine3DWidth;
						nVerMapTileIdx = nCurMapTileIdx;
					}
					// Check next line
					else
					{
						dRayX += dOffsetX;
						dRayY += dOffsetY;
						nDepthOfField += 1;
					}
				}
				// Check next line
				else
				{
					dRayX += dOffsetX;
					dRayY += dOffsetY;
					nDepthOfField += 1;
				}
			}
			dVerX = dRayX;
			dVerY = dRayY;
		}
		// ====================================================================================

		// Draw 3D Map ========================================================================
		{
			if (player.GetVerticalDistance() > player.GetHorizontalDistance())
			{
				dShade = 1.0;
				dRayX = dHorX;
				dRayY = dHorY;
				dDistanceT = player.GetHorizontalDistance();
				glColor3f(0.7f, 0.0f, 0.0f);
				nCurMapTileIdx = nHorMapTileIdx;
			}
			else
			{
				dShade = 0.5;
				dRayX = dVerX;
				dRayY = dVerY;
				dDistanceT = player.GetVerticalDistance();
				glColor3f(0.9f, 0.0f, 0.0f);
				nCurMapTileIdx = nVerMapTileIdx;
			}

			dFishAngle = CheckAngle(player.GetAngle() - dAngle);
			dDistanceT *= cos(DegreeToRadian(dFishAngle));

			double dTyOffset = 0.0;
			dLineH = (double)(map2D.GetTotalTileCount() * WIN3D_WIDTH) / dDistanceT;
			double dTyStep = (double)TILE_DATA_SIZE / dLineH;
			if (dLineH > WIN3D_WIDTH)
			{
				dTyOffset = (dLineH - WIN3D_WIDTH) / 2.0;
				dLineH = WIN3D_WIDTH;
			}
			dLineOffset = (WIN3D_WIDTH / 2.0) - ((int)dLineH >> 1);

			double dTy = (int)(dTyOffset * dTyStep + nCurMapTileIdx);
			double dTx = 0;
			if (dShade == 1.0)
			{
				dTx = (double)((int)(dRayX / 2.0) % TILE_DATA_SIZE);

				if (dAngle > 180.0)
					dTx = (TILE_DATA_SIZE - 1) - dTx;
			}
			else
			{
				dTx = (double)((int)(dRayY / 2.0) % TILE_DATA_SIZE);

				if (dAngle > 90.0 &&
					dAngle < 270.0)
					dTx = (TILE_DATA_SIZE - 1) - dTx;
			}

			// Draw Walls
			{
				glPointSize((GLfloat)nLine3DWidth);
				glBegin(GL_POINTS);
				{
					CMap2D::CColor color{ 0, };

					for (int i = 0; i < (int)dLineH; i++)
					{
						map2D.GetTileColor(nCurMapTileIdx, (int)dTx, (int)dTy, color);
						glColor3ub((GLubyte)(color.clrRed * dShade), (GLubyte)(color.clrGreen * dShade), (GLubyte)(color.clrBlue * dShade));
						glVertex2d(nR * nLine3DWidth + WIN3D_START, i + dLineOffset);

						dTy += dTyStep;
					}
				}
				glEnd();
			}

			// TODO Draw Floors
			{
				glLineWidth((GLfloat)nLine3DWidth);
				glColor3f(0.28f, 0.55f, 0.28f);
				glBegin(GL_LINES);
				{
					glVertex2d(nR * nLine3DWidth + WIN3D_START, dLineH + dLineOffset);
					glVertex2d(nR * nLine3DWidth + WIN3D_START, WIN3D_WIDTH);
				}
				glEnd();
			}

			// TODO Draw Ceilings
			{

			}
		}
		// ====================================================================================

		// Draw Ray Beam ======================================================================
		{
			glLineWidth(3);
			glBegin(GL_LINES);
			{
				glVertex2d(player.GetPosX() * map2D.GetMinimapScale(), player.GetPosY() * map2D.GetMinimapScale());
				glVertex2d(dRayX * map2D.GetMinimapScale(), dRayY * map2D.GetMinimapScale());
			}
			glEnd();
		}
		// ====================================================================================

		// Next angle =========================================================================
		{
			dAngle = CheckAngle(dAngle - 1);
		}
		// ====================================================================================
	}
}

/**
@brief		Input Key Check
@param
@return
*/
void CheckKeyInput()
{
	static double dFrame1 = 0.0;
	static double dFrame2 = 0.0;
	double dFPS = 0.0;

	dFrame2 = glutGet(GLUT_ELAPSED_TIME);
	dFPS = dFrame2 - dFrame1;
	dFrame1 = glutGet(GLUT_ELAPSED_TIME);

	if (player.GetKeyPressed(CPlayer::eKeyCode::KeyUp))
	{
		if (player.IsCollision() == false)
		{
			player.SetPosXAdd(player.GetDeltaX() * SPEED_RATE * dFPS);
			player.SetPosYAdd(player.GetDeltaY() * SPEED_RATE * dFPS);
		}
	}
	if (player.GetKeyPressed(CPlayer::eKeyCode::KeyDown))
	{
		int nPlayerMapX = player.GetPosX() / map2D.GetTileSize();
		int nPlayerMapY = player.GetPosY() / map2D.GetTileSize();
		int nOffsetX = (nPlayerMapX - (player.GetDeltaX() > 0 ? -5 : 5)) / map2D.GetTileSize();
		int nOffsetY = (nPlayerMapY - (player.GetDeltaY() > 0 ? -5 : 5)) / map2D.GetTileSize();
		
		if (map2D.GetMapTileIndex(CMap2D::eMapIdx::MapWall, nPlayerMapX + nOffsetX, nPlayerMapY) == 0)
		{
			player.SetPosXAdd(-player.GetDeltaX() * SPEED_RATE * dFPS);
		}
		if (map2D.GetMapTileIndex(CMap2D::eMapIdx::MapWall, nPlayerMapX, nPlayerMapY + nOffsetY) == 0)
		{
			player.SetPosYAdd(-player.GetDeltaY() * SPEED_RATE * dFPS);
		}
	}
	if (player.GetKeyPressed(CPlayer::eKeyCode::KeyLeft))
	{
		player.SetAngleAdd(SPEED_RATE * dFPS);
		player.SetAngle(CheckAngle(player.GetAngle()));
		player.SetDeltaX(cos(player.GetAngleRad()));
		player.SetDeltaY(-sin(player.GetAngleRad()));
	}
	if (player.GetKeyPressed(CPlayer::eKeyCode::KeyRight))
	{
		player.SetAngleAdd(-SPEED_RATE * dFPS);
		player.SetAngle(CheckAngle(player.GetAngle()));
		player.SetDeltaX(cos(player.GetAngleRad()));
		player.SetDeltaY(-sin(player.GetAngleRad()));
	}
	glutPostRedisplay();
}

/**
@brief		Display Function - glutDisplayFunc
@param
@return
*/
void GLFuncDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CheckKeyInput();
	DrawRay2D();
	Draw2DMap();
	DrawPlayer();
	glutSwapBuffers();
}

/**
@brief		Input Key Down Function - glutKeyboardFunc
@param		chKey	: Input key
@param		nX		: Mouse x position (when key is downed)
@param		nY		: Mouse y position (when key is downed)
@return
*/
void GLFuncKeyDown(unsigned char chKey, int nX, int nY)
{
	if (player.GetKey(CPlayer::eKeyCode::KeyUp) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyUp, true);
	}
	if (player.GetKey(CPlayer::eKeyCode::KeyDown) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyDown, true);
	}
	if (player.GetKey(CPlayer::eKeyCode::KeyLeft) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyLeft, true);
	}
	if (player.GetKey(CPlayer::eKeyCode::KeyRight) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyRight, true);
	}

	glutPostRedisplay();
}

/**
@brief		Input Key Up Function - glutKeyboardUpFunc
@param		chKey	: Input key
@param		nX		: Mouse x position (when key is downed)
@param		nY		: Mouse y position (when key is downed)
@return
*/
void GLFuncKeyUp(unsigned char chKey, int nX, int nY)
{
	if (player.GetKey(CPlayer::eKeyCode::KeyUp) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyUp, false);
	}
	if (player.GetKey(CPlayer::eKeyCode::KeyDown) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyDown, false);
	}
	if (player.GetKey(CPlayer::eKeyCode::KeyLeft) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyLeft, false);
	}
	if (player.GetKey(CPlayer::eKeyCode::KeyRight) == chKey)
	{
		player.SetKeyPressed(CPlayer::eKeyCode::KeyRight, false);
	}

	glutPostRedisplay();
}

/**
@brief		Resize Window Funcition - glutReshapeFunc
@param		nWidth	: Window Width
@param		nHeight	: Window Height
@return
*/
void GLFuncResize(int nWidth, int nHeight)
{
	glutReshapeWindow(WIN_WIDTH, WIN_HEIGHT);
}

int snTile[] =
{
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,
	220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,220,

	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
	128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,
};

/**
@brief		Player and Map Initialize Function
@param
@return
*/
void Init()
{
	int nMapWalls[] =
	{
		1,1,1,1,1,1,1,1,1,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,1,0,0,0,0,0,0,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,0,0,0,0,1,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,1,1,1,1,1,1,1,1,1,
	};
	int nMapBottom[] =
	{
		1,1,1,1,1,1,1,1,1,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,1,0,0,0,0,0,0,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,0,0,0,0,1,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,1,1,1,1,1,1,1,1,1,
	};
	int nMapCeiling[] =
	{
		1,1,1,1,1,1,1,1,1,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,1,0,0,0,0,0,0,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,0,0,0,0,1,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,0,1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,0,1,
		1,1,1,1,1,1,1,1,1,1,
	};

	
	int nTileSize = sizeof(snTile) / sizeof(int);

	map2D.SetMap(10, 10, nMapWalls, nMapBottom, nMapCeiling, 64);
	map2D.InitTiles(snTile, nTileSize);
	player.SetFOVAngle(60.0);
	player.SetPos(300, 300);
	player.SetDelta(cos(player.GetAngle()) * 5, sin(player.GetAngle()) * 5);

	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	gluOrtho2D(0, WIN_WIDTH, WIN_HEIGHT, 0);
}

/**
@brief		Main Function
@param
@return
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("OpenGL");
	Init();
	glutDisplayFunc(GLFuncDisplay);
	glutKeyboardFunc(GLFuncKeyDown);
	glutKeyboardUpFunc(GLFuncKeyUp);
	glutReshapeFunc(GLFuncResize);
	glutMainLoop();

	return 0;
}