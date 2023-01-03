#include "Map2D.h"
#include <memory>


/**
@brief		Constructor
@param		
@return
*/
CMap2D::CMap2D()
	: m_nWidth(0), m_nHeight(0), m_nTileSize(0)
	//, m_pMapWallData(nullptr), m_pMapBotData(nullptr), m_pMapCeilData(nullptr)
	, m_pMapDatas{ nullptr, }
	, m_dMinimapScale(0.2), m_bMinimapVisible(true)
	, m_nPowN(0)
{
}

/**
@brief		Destructor
@param
@return
*/
CMap2D::~CMap2D()
{
	DeleteMap();	
}

/**
@brief		Delete map
@param
@return
*/
void CMap2D::DeleteMap()
{
	for (int i = 0; i < eMapIdx::MapIdxMax; i++)
	{
		if (m_pMapDatas[i] != nullptr)
		{
			for (int j = 0; j < m_nHeight; j++)
			{
				delete[] m_pMapDatas[i][j];
				m_pMapDatas[i][j] = nullptr;
			}

			delete[] m_pMapDatas[i];
			m_pMapDatas[i] = nullptr;
		}
	}

	m_nWidth = 0;
	m_nHeight = 0;
	m_nTileSize = 0;
}

/**
@brief		Set map
@param		nWidth			: Map width
@param		nHeight			: Map height
@param		nMapWallDatas	: Map wall data (2D array)
@param		nMapBotDatas	: Map wall data (2D array)
@param		nMapCeilDatas	: Map wall data (2D array)
@param		nTileSize		: Map tile size
@return
*/
void CMap2D::SetMap(int nWidth, int nHeight, int** nMapWallDatas, int** nMapBotDatas, int** nMapCeilDatas, int nTileSize)
{
	if (nWidth < 0 ||
		nHeight < 0 ||
		nMapWallDatas == nullptr ||
		nMapBotDatas == nullptr ||
		nMapCeilDatas == nullptr ||
		nTileSize <= 0)
	{
		return;
	}

	DeleteMap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nTileSize = nTileSize;

	m_pMapDatas[eMapIdx::MapWall] = new int* [nHeight];
	m_pMapDatas[eMapIdx::MapBottom] = new int* [nHeight];
	m_pMapDatas[eMapIdx::MapCeiling] = new int* [nHeight];

	for (int nY = 0; nY < nHeight; nY++)
	{
		m_pMapDatas[eMapIdx::MapWall][nY] = new int[nWidth];
		m_pMapDatas[eMapIdx::MapBottom][nY] = new int[nWidth];
		m_pMapDatas[eMapIdx::MapCeiling][nY] = new int[nWidth];

		for (int nX = 0; nX < nWidth; nX++)
		{
			m_pMapDatas[eMapIdx::MapWall][nY][nX] = nMapWallDatas[nY][nX];
			m_pMapDatas[eMapIdx::MapBottom][nY][nX] = nMapBotDatas[nY][nX];
			m_pMapDatas[eMapIdx::MapCeiling][nY][nX] = nMapCeilDatas[nY][nX];
		}
	}

	m_nPowN = 0;

	while (nTileSize > 0)
	{
		if (nTileSize / 2 > 0)
			m_nPowN++;
		else
			break;

		nTileSize /= 2;
	}
}

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
void CMap2D::SetMap(int nWidth, int nHeight, int* nMapWallDatas, int* nMapBotDatas, int* nMapCeilDatas, int nTileSize)
{
	if (nWidth < 0 ||
		nHeight < 0 ||
		nMapWallDatas == nullptr ||
		nMapBotDatas == nullptr ||
		nMapCeilDatas == nullptr ||
		nTileSize <= 0)
	{
		return;
	}

	DeleteMap();

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nTileSize = nTileSize;

	m_pMapDatas[eMapIdx::MapWall] = new int* [nHeight];
	m_pMapDatas[eMapIdx::MapBottom] = new int* [nHeight];
	m_pMapDatas[eMapIdx::MapCeiling] = new int* [nHeight];

	for (int nY = 0; nY < nHeight; nY++)
	{
		m_pMapDatas[eMapIdx::MapWall][nY] = new int[nWidth];
		m_pMapDatas[eMapIdx::MapBottom][nY] = new int[nWidth];
		m_pMapDatas[eMapIdx::MapCeiling][nY] = new int[nWidth];

		for (int nX = 0; nX < nWidth; nX++)
		{
			m_pMapDatas[eMapIdx::MapWall][nY][nX] = nMapWallDatas[nY * nHeight + nX];
			m_pMapDatas[eMapIdx::MapBottom][nY][nX] = nMapBotDatas[nY * nHeight + nX];
			m_pMapDatas[eMapIdx::MapCeiling][nY][nX] = nMapCeilDatas[nY * nHeight + nX];
		}
	}

	m_nPowN = 0;

	while (nTileSize > 0)
	{
		if (nTileSize / 2 > 0)
			m_nPowN++;
		else
			break;

		nTileSize /= 2;
	}
}

/**
@brief		Get map tile data
@param		eIdx	: Tile's map index
@param		nX		: Tile's x position
@param		nY		: Tile's y position
@return		Map tile data (-1: failed)
*/
int CMap2D::GetMapTileIndex(eMapIdx eIdx, int nX, int nY) const
{
	if (nX < 0 ||
		nX >= m_nWidth ||
		nY < 0 ||
		nY >= m_nHeight ||
		eIdx < 0 ||
		eIdx >= eMapIdx::MapIdxMax)
	{
		return -1;
	}

	return m_pMapDatas[eIdx][nY][nX];
}


/**
@brief		Initialize Tiles (Load Tile Datas)
@param		nTileDatas	: Tile Datas
@return
*/
bool CMap2D::InitTiles(int nTileDatas[], int nSize)
{
	if (nTileDatas == nullptr ||
		nSize <= 0)
		return false;

	int nTileCount = nSize / 3 / TILE_DATA_SIZE / TILE_DATA_SIZE;

	// Tile Data must be divided by 32 and the remainder must be zero.
	if (nSize != nTileCount * 3 * TILE_DATA_SIZE * TILE_DATA_SIZE)
		return false;

	if (m_vTiles.size() > 0)
		m_vTiles.clear();

	int nCurIdx = -1;
	for (int nTile = 0; nTile < nTileCount; nTile++)
	{
		CColors colors;

		for (int nPixelY = 0; nPixelY < TILE_DATA_SIZE; nPixelY++)
		{
			for (int nPixelX = 0; nPixelX < TILE_DATA_SIZE; nPixelX++)
			{
				nCurIdx = (nTile * TILE_DATA_SIZE * TILE_DATA_SIZE * 3) + (nPixelY * TILE_DATA_SIZE * 3) + (nPixelX * 3);

				colors.clrs[nPixelY][nPixelX].clrRed = nTileDatas[nCurIdx];
				colors.clrs[nPixelY][nPixelX].clrGreen = nTileDatas[nCurIdx + 1];
				colors.clrs[nPixelY][nPixelX].clrBlue = nTileDatas[nCurIdx + 2];
			}
		}

		m_vTiles.push_back(colors);
	}

	return true;
}

/**
@brief		Get Tile Colors
@param		eIdx	: Tile's map index
@param		nX		: Tile x position
@param		nY		: Tile y position
@param		color	: Tile Color (out value)
@return		true: success, false: failed
*/
bool CMap2D::GetTileColor(eMapIdx eIdx, int nX, int nY, CColors& color)
{
	if (nX < 0 ||
		nX > m_nWidth ||
		nY < 0 ||
		nY > m_nHeight)
		return false;

	color = m_vTiles.at(GetMapTileIndex(eIdx, nX, nY));

	return true;
}

/**
@brief		Get Tile Color (in Tile)
@param		nTileIndex	: Tile Index
@param		nTileColorX	: Tile color x position
@param		nTileColorY	: Tile color y position
@param		color		: Tile Color (out value)
@return		true: success, false: failed
*/
bool CMap2D::GetTileColor(int nTileIndex, int nTileColorX, int nTileColorY, CColor& color)
{
	if (nTileIndex < 0 ||
		nTileIndex > eTileType::TileTypeMax ||
		nTileColorX < 0 ||
		nTileColorX > TILE_DATA_SIZE ||
		nTileColorY < 0 ||
		nTileColorY > TILE_DATA_SIZE)
		return false;

	color = m_vTiles.at(nTileIndex).clrs[nTileColorY][nTileColorX];

	return true;
}