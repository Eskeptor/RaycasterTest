#pragma once

/**
@brief			Player Information Class
@author			Eskeptor
@date			2023-01-03
@version		0.0.2
*/

class CPlayer
{
// Enums and Class ===========================================================================================
public:
	/**
	@brief		Player Control Key Code
	*/
	enum class eKeyCode
	{
		KeyUp = 0,
		KeyDown,
		KeyLeft,
		KeyRight,
		CodeMax,
	};
// ===========================================================================================================

// Variables =================================================================================================
private:
	// Current X Position (More than 0 : System Position Start 0)
	double m_dPosX;
	// Current Y Position (More than 0 : System Position Start 0)
	double m_dPosY;
	// Current X Delta
	double m_dDeltaX;
	// Current Y Delta
	double m_dDeltaY;
	// Current Angle (ex: 180.0, 360.0)
	double m_dAngle;
	// Current Vertical Distance from wall (for Raycasting)
	double m_dVerDistance;
	// Current Horizontal Distance from wall (fro Raycasting)
	double m_dHorDistance;
	// Collision Width
	double m_dColWidth;
	// FOV Angle
	double m_dFOVAngle;
	// Player Size
	double m_dSize;

	// Previous X Position
	double m_dPrevPosX;
	// Previous Y Position
	double m_dPrevPosY;

	// Key Pressed Toggle
	bool m_bArrKeyPressed[static_cast<int>(eKeyCode::CodeMax)];

	// Keys
	char m_chArrKey[static_cast<int>(eKeyCode::CodeMax)];
// ===========================================================================================================

// Functions =================================================================================================
public:
	/**
	@brief		Constructor
	@param		dPosX		: Current X Position
	@param		dPosY		: Current Y Position
	@param		dDeltaX		: Current X Delta
	@param		dDeltaY		: Current Y Delta
	@param		dAngle		: Current Angle
	@param		dColWidth	: Collision Width
	@return
	*/
	CPlayer(double dSize = 8.0, double dPosX = 0.0, double dPosY = 0.0, double dDeltaX = 0.0, double dDeltaY = 0.0, double dAngle = 0.0, double dColWidth = 6.0)
		: m_dPosX(dPosX), m_dPosY(dPosY), m_dDeltaX(dDeltaX), m_dDeltaY(dDeltaY), m_dAngle(dAngle), m_dColWidth(dColWidth)
		, m_dVerDistance(0.0), m_dHorDistance(0.0)
		, m_dPrevPosX(0.0), m_dPrevPosY(0.0), m_dFOVAngle(30.0), m_dSize(dSize)
		, m_bArrKeyPressed {false, }
		, m_chArrKey {'w', 's', 'a', 'd'}
	{}

	/**
	@brief		Set player size
	@param		dSize	: Player Size
	@return
	*/
	void SetSize(double dSize)
	{
		m_dSize = dSize;
	}
	/**
	@brief		Set current X position
	@param		dPosX	: Set X Position
	@return
	*/
	void SetPosX(double dPosX)
	{
		m_dPrevPosX = m_dPosX;
		m_dPosX = dPosX;
	}
	/**
	@brief		Set current Y position
	@param		dPosY	: Set Y Position
	@return
	*/
	void SetPosY(double dPosY)
	{
		m_dPrevPosY = m_dPosY;
		m_dPosY = dPosY;
	}
	/**
	@brief		Set current position
	@param		dPosX	: Set X Position
	@param		dPosY	: Set Y Position
	*/
	void SetPos(double dPosX, double dPosY)
	{
		m_dPrevPosX = m_dPosX;
		m_dPosX = dPosX;
		m_dPrevPosY = m_dPosY;
		m_dPosY = dPosY;
	}
	/**
	@brief		Increase the current X position
	@param		dAdder	: Increase position
	@return
	*/
	void SetPosXAdd(double dAdder)
	{
		m_dPrevPosX = m_dPosX;
		m_dPosX += dAdder;
	}
	/**
	@brief		Increase the current Y position
	@param		dAdder	: Increase position
	@return
	*/
	void SetPosYAdd(double dAdder)
	{
		m_dPrevPosY = m_dPosY;
		m_dPosY += dAdder;
	}
	/**
	@brief		Set current X delta
	@param		dDeltaX	: Set X Delta
	@return
	*/
	void SetDeltaX(double dDeltaX)
	{
		m_dDeltaX = dDeltaX;
	}
	/**
	@brief		Set current Y delta
	@param		dDeltaY	: Set Y Delta
	@return
	*/
	void SetDeltaY(double dDeltaY)
	{
		m_dDeltaY = dDeltaY;
	}
	/**
	@brief		Set current delta
	@param		dDeltaX	: Set X Delta
	@param		dDeltaY	: Set Y Delta
	@return
	*/
	void SetDelta(double dDeltaX, double dDeltaY)
	{
		m_dDeltaX = dDeltaX;
		m_dDeltaY = dDeltaY;
	}
	/**
	@brief		Set current angle
	@param		dAngle	: Set current Angle
	@return
	*/
	void SetAngle(double dAngle)
	{
		m_dAngle = dAngle;
	}
	/**
	@brief		Increase the current X delta
	@param		dAdder	: Increase delta
	@return
	*/
	void SetDeltaXAdd(double dAdder)
	{
		m_dDeltaX += dAdder;
	}
	/**
	@brief		Increase the current Y delta
	@param		dAdder	: Increase delta
	@return
	*/
	void SetDeltaYAdd(double dAdder)
	{
		m_dDeltaY += dAdder;
	}
	/**
	@brief		Increase the current angle
	@param		dAdder	: Increase angle
	@return
	*/
	void SetAngleAdd(double dAdder)
	{
		m_dAngle += dAdder;
	}
	/**
	@brief		Set current vertical distance
	@param		dVerticalDistance	: Set current vertical distance
	@return
	*/
	void SetVerticalDistance(double dVerticalDistance)
	{
		m_dVerDistance = dVerticalDistance;
	}
	/**
	@brief		Set current horizontal distance
	@param		dHorizontalDistance	: Set current horizontal distance
	@return
	*/
	void SetHorizontalDistance(double dHorizontalDistance)
	{
		m_dHorDistance = dHorizontalDistance;
	}
	/**
	@brief		Set current distance
	@param		dVerticalDistance	: Set current vertical distance
	@param		dHorizontalDistance	: Set current horizontal distance
	@return
	*/
	void SetDistance(double dVerticalDistance, double dHorizontalDistance)
	{
		m_dVerDistance = dVerticalDistance;
		m_dHorDistance = dHorizontalDistance;
	}
	/**
	@brief		Set collision width
	@param		dColWidth	: Set collision width
	@return
	*/
	void SetCollisionWidth(double dColWidth)
	{
		m_dColWidth = dColWidth;
	}

	/**
	@brief		Set FOV Angle
	@param		dAngle		: FOV Angle (0 ~ 180)
	@return
	*/
	void SetFOVAngle(double dAngle)
	{
		m_dFOVAngle = (dAngle > 0) ? (dAngle <= 180.0 ? dAngle : 180.0) : 1.0;
	}

	/**
	@brief		Set control key
	@param		eCode	: Changed key code
	@param		chKey	: Changed key
	@return
	*/
	void SetKey(eKeyCode eCode, char chKey)
	{
		m_chArrKey[static_cast<int>(eCode)] = chKey;
	}
	/**
	@brief		Set control key pressed
	@param		eCode		: Changed key code
	@param		bPressed	: Changed key pressed status
	@return
	*/
	void SetKeyPressed(eKeyCode eCode, bool bPressed)
	{
		m_bArrKeyPressed[static_cast<int>(eCode)] = bPressed;
	}

	/**
	@brief		Get player size
	@param
	@return		Player size
	*/
	double GetSize() const
	{
		return m_dSize;
	}
	/**
	@brief		Get current X position
	@param
	@return		Current X position
	*/
	double GetPosX() const
	{
		return m_dPosX;
	}
	/**
	@brief		Get current Y position
	@param
	@return		Current Y position
	*/
	double GetPosY() const
	{
		return m_dPosY;
	}
	/**
	@brief		Get current X delta
	@param
	@return		Current X delta
	*/
	double GetDeltaX() const
	{
		return m_dDeltaX;
	}
	/**
	@brief		Get current Y delta
	@param
	@return		Current Y delta
	*/
	double GetDeltaY() const
	{
		return m_dDeltaY;
	}
	/**
	@brief		Get current angle
	@param
	@return		Current angle
	*/
	double GetAngle() const
	{
		return m_dAngle;
	}
	double GetAngleRad() const
	{
		return m_dAngle * 3.1415926535 / 180.0;
	}
	/**
	@brief		Get current vertical distance
	@param
	@return		Current vertical distance
	*/
	double GetVerticalDistance() const
	{
		return m_dVerDistance;
	}
	/**
	@brief		Get current horizontal distance
	@param
	@return		Current horizontal distance
	*/
	double GetHorizontalDistance() const
	{
		return m_dHorDistance;
	}
	/**
	@brief		Get collision width
	@param
	@return		Collision width
	*/
	double GetCollisionWidth() const
	{
		return m_dColWidth;
	}
	/**
	@brief		Get previous X position
	@param
	@return		Previous X position
	*/
	double GetPrevPosX() const 
	{
		return m_dPrevPosX;
	}
	/**
	@brief		Get previous Y position
	@param
	@return		Previous Y position
	*/
	double GetPrevPosY() const
	{
		return m_dPrevPosY;
	}
	/**
	@brief		Get FOV Angle
	@param
	@return		FOV Angle
	*/
	double GetFOVAngle() const
	{
		return m_dFOVAngle;
	}

	/**
	@brief		Check Collision detected with wall
	@param
	@return		true: Collision, false: Not collision
	*/
	bool IsCollision() const
	{
		return (m_dVerDistance <= m_dColWidth || m_dHorDistance <= m_dColWidth);
	}

	/**
	@brief		Get key code
	@param		eCode	: Key code
	@return		Key code (char)
	*/
	char GetKey(eKeyCode eCode) const
	{
		return m_chArrKey[static_cast<int>(eCode)];
	}
	/**
	@brief		Get key pressed status
	@param		eCode	: Key code
	@return		Pressed status
	*/
	bool GetKeyPressed(eKeyCode eCode) const
	{
		return m_bArrKeyPressed[static_cast<int>(eCode)];
	}
// ===========================================================================================================
};