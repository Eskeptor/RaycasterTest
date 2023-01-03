#pragma once
#include <vector>

constexpr int TILE_DATA_SIZE = 32;

/**
@brief			2D Map Information Class
@author			Eskeptor
@date			2023-01-03
@version		0.0.2
*/
class CMap2D
{
// Enums and Class ===========================================================================================
public:
	/**
	@brief		Map's tile type
	*/
	enum eTileType
	{
		Bottom = 0,
		Wall = 1,
		Door = 2,
		TileTypeMax,
	};
	/**
	@brief		Map Index (for Map data array)
	*/
	enum eMapIdx
	{
		MapWall = 0,
		MapBottom,
		MapCeiling,
		MapIdxMax
	};

	/**
	@brief		Color structure
	*/
	struct CColor
	{
	public:
		// Red (0 ~ 255)
		int clrRed;
		// Green (0 ~ 255)
		int clrGreen;
		// Blue (0 ~ 255)
		int clrBlue;
	};
	/**
	@brief		Color structure array struct
	*/
	struct CColors
	{
	public:
		// Color datas
		CColor clrs[TILE_DATA_SIZE][TILE_DATA_SIZE];
	};
// ===========================================================================================================

// Variables =================================================================================================
private:
	// Map Datas
	int** m_pMapDatas[eMapIdx::MapIdxMax];
	// Map Width
	int m_nWidth;
	// Map Height
	int m_nHeight;
	// Map Tile Size
	int m_nTileSize;
	// Minimap Scale Size
	double m_dMinimapScale;
	// Minimap Visible
	bool m_bMinimapVisible;
	// Power of 2
	int m_nPowN;
	// Tiles
	std::vector<CColors> m_vTiles;
// ===========================================================================================================

// Functions =================================================================================================
public:
	/**
	@brief		Constructor
	@param
	@return
	*/
	CMap2D();
	/**
	@brief		Destructor
	@param
	@return
	*/
	~CMap2D();

	/**
	@brief		Delete map
	@param
	@return
	*/
	void DeleteMap();

	/**
	@brief		Set map
	@param		nWidth			: Map width
	@param		nHeight			: Map height
	@param		nMapWallDatas	: Map wall data (2D array)
	@param		nMapBotDatas	: Map bottom data (2D array)
	@param		nMapCeilDatas	: Map ceiling data (2D array)
	@param		nTileSize		: Map tile size
	@return
	*/
	void SetMap(int nWidth, int nHeight, int** nMapWallDatas, int** nMapBotDatas, int** nMapCeilDatas, int nTileSize);
	/**
	@brief		Set map
	@param		nWidth			: Map width
	@param		nHeight			: Map height
	@param		nMapWallDatas	: Map wall data (1D array)
	@param		nMapBotDatas	: Map bottom data (1D array)
	@param		nMapCeilDatas	: Map ceiling data (1D array)
	@param		nTileSize		: Map tile size
	@return
	*/
	void SetMap(int nWidth, int nHeight, int* nMapWallDatas, int* nMapBotDatas, int* nMapCeilDatas, int nTileSize);

	/**
	@brief		Set minimap setting
	@param		bVisible		: Minimap visible
	@param		dMinimapScale	: Minimap scale size
	@return
	*/
	void SetMinimap(bool bVisible, double dMinimapScale = 0.0)
	{
		m_bMinimapVisible = bVisible;
		m_dMinimapScale = dMinimapScale >= 0.0 ? dMinimapScale : 0.0;
	}
	
	/**
	@brief		Initialize Tiles (Load Tile Datas)
	@param		nTileDatas	: Tile Datas
	@return		true: Success, false: failed
	*/
	bool InitTiles(int nTileDatas[], int nSize);

	/**
	@brief		Get Tile Colors
	@param		eIdx	: Tile's map index
	@param		nX		: Tile x position
	@param		nY		: Tile y position
	@param		colors	: Tile Colors (out value)
	@return		true: success, false: failed
	*/
	bool GetTileColor(eMapIdx eIdx, int nX, int nY, CColors& colors);

	/**
	@brief		Get Tile Color (in Tile)
	@param		nTileIndex	: Tile Index
	@param		nTileColorX	: Tile color x position
	@param		nTileColorY	: Tile color y position
	@param		color		: Tile Color (out value)
	@return		true: success, false: failed
	*/
	bool GetTileColor(int nTileIndex, int nTileColorX, int nTileColorY, CColor& color);
	
	/**
	@brief		Get map tile data
	@param		eIdx	: Tile's map index
	@param		nX		: Tile's x position
	@param		nY		: Tile's y position
	@return		Map tile data (-1: failed)
	*/
	int GetMapTileIndex(eMapIdx eIdx, int nX, int nY) const;

	/**
	@brief		Get map width
	@param
	@return		Map width
	*/
	int GetWidth() const
	{
		return m_nWidth;
	}
	/**
	@brief		Get map height
	@param
	@return		Map height
	*/
	int GetHeight() const
	{
		return m_nHeight;
	}
	/**
	@brief		Get map tile size
	@param
	@return		Map tile size
	*/
	int GetTileSize() const
	{
		return m_nTileSize;
	}
	/**
	@brief		Get map total tile count
	@param
	@return		Map Total tile count
	*/
	int GetTotalTileCount() const 
	{
		return m_nWidth * m_nHeight;
	}
	/**
	@brief		Get minimap scale size
	@param
	@return		Minimap scale size
	*/
	double GetMinimapScale() const
	{
		return m_dMinimapScale;
	}
	/**
	@brief		Get minimap tile size
	@param
	@return		Minimap tile size
	*/
	double GetMinimapTileSize() const
	{
		return (double)m_nTileSize * m_dMinimapScale;
	}
	/**
	@brief		Get tile size's pow of 2
	@param
	@return		Tile size's pow of 2
	*/
	int GetPowN() const
	{
		return m_nPowN;
	}
// ===========================================================================================================
};

