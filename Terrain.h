// Terrain.h:
#pragma once

#include <queue>

#include "Constants.h"

namespace unrailed_game
{
	enum class TerrainType
	{
		Empty,
		Barrier,
		Forest,
		Moutains,
		Water,
		Rails,
		Station
	};
	class TerrainTile
	{
	public:
		TerrainType type;
		int count;
		TerrainTile() : type(TerrainType::Empty), count(4) {};
		TerrainTile(TerrainType t) : type(t), count(4) {};
		bool Break()
		{
			count--;
			if (count == 0) type = TerrainType::Empty;
			return (count == 0);
		}
	};
	typedef std::array<std::array<TerrainTile, fieldY>, locationX> terrain_t;

	class Terrain
	{
	private:
		terrain_t terrain;
		int LocationPosition; // Положение локации относительно экрана
	public:
		Terrain()
		{
			for (int y = 0; y < fieldY; y++)
			{
				for (int x = 0; x < fieldX; x++)
				{
					terrain[x][y] = TerrainTile(TerrainType::Empty);
				}
			}

			GenerateStartMap();

			for (int y = fieldY / 2 - 1; y >= 0; y--)
			{
				terrain[2][y] = TerrainTile(TerrainType::Rails);
			}
			/*
			for (int x = 3; x < locationX; x++)
			{
				if (terrain[x][fieldY / 2 - 1].type != TerrainType::Station)
					terrain[x][fieldY / 2 - 1] = TerrainTile(TerrainType::Rails);
			}*/

			// Установка начальной генерации:
			terrain[0][fieldY - 1] = TerrainTile(TerrainType::Forest);
			terrain[0][fieldY - 2] = TerrainTile(TerrainType::Forest);
			terrain[0][fieldY - 3] = TerrainTile(TerrainType::Forest);
			terrain[0][fieldY - 4] = TerrainTile(TerrainType::Forest);
			terrain[1][fieldY - 1] = TerrainTile(TerrainType::Forest);
			terrain[1][fieldY - 2] = TerrainTile(TerrainType::Forest);
			terrain[1][fieldY - 3] = TerrainTile(TerrainType::Forest);
			terrain[2][fieldY - 1] = TerrainTile(TerrainType::Moutains);
			terrain[2][fieldY - 2] = TerrainTile(TerrainType::Moutains);
			terrain[2][fieldY - 3] = TerrainTile(TerrainType::Moutains);
			terrain[3][fieldY - 1] = TerrainTile(TerrainType::Moutains);
			terrain[3][fieldY - 2] = TerrainTile(TerrainType::Moutains);
			terrain[3][fieldY - 3] = TerrainTile(TerrainType::Moutains);
			terrain[4][fieldY - 1] = TerrainTile(TerrainType::Moutains);
			terrain[4][fieldY - 2] = TerrainTile(TerrainType::Moutains);
		}
		const terrain_t& Get() const { return terrain; }
		int Position() const { return LocationPosition; }
		
		bool Break(int x, int y) { return terrain[x][y].Break(); }
		void SetRail(int x, int y) { terrain[x][y] = TerrainType::Rails; }
		void SetBridge(int x, int y) { terrain[x][y] = TerrainType::Empty; }
		void Move() { LocationPosition++; }
		
		void ShiftMap();
		void GenerateStartMap();
		void ReGenerateMap();
		void SetStation();
		void GenerateClusters(int old_part, TerrainType clusterType, int minSize, int maxSize, int count = 1);
		void GenerateRivers(int old_part, int count = 1);
	};
}