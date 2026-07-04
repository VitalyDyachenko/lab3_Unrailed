// Terrain.cpp:
#include "Terrain.h"
#include "Random.h"

namespace unrailed_game
{
	void Terrain::ShiftMap()
	{
		for (int x = 0; x < fieldX; x++)
		{
			for (int y = 0; y < fieldY; y++)
			{
				terrain[x][y] = terrain[x + locationX - fieldX][y];
			}
		}
		LocationPosition = 0;
		for (int x = fieldX; x < locationX; x++)
		{
			for (int y = 0; y < fieldY; y++)
			{
				terrain[x][y].type = TerrainType::Empty;
				terrain[x][y].count = 0;
			}
		}
		ReGenerateMap();
	}

	void Terrain::GenerateStartMap()
	{
		GenerateClusters(spawnX, TerrainType::Forest, 20, 40, 5);
		GenerateClusters(spawnX, TerrainType::Moutains, 20, 40, 4);
		GenerateClusters(spawnX, TerrainType::Water, 20, 30, 3);
		GenerateRivers(spawnX, 3);

		SetStation();
	}
	void Terrain::ReGenerateMap()
	{
		GenerateClusters(fieldX, TerrainType::Forest, 20, 40, 5);
		GenerateClusters(fieldX, TerrainType::Moutains, 20, 40, 4);
		GenerateClusters(fieldX, TerrainType::Water, 20, 30, 3);
		GenerateRivers(fieldX, 3);

		SetStation();
		/*
		for (int x = 3; x < locationX; x++)
		{
			if (terrain[x][fieldY / 2 - 1].type != TerrainType::Station)
				terrain[x][fieldY / 2 - 1] = TerrainTile(TerrainType::Rails);
		}*/
	}

	void Terrain::SetStation()
	{
		int Y = randomInt(1, fieldY - 2);
		int X = locationX + trainX - fieldX - 1;
		terrain[X + 1][Y] = TerrainTile(TerrainType::Station);
		terrain[X + 2][Y] = TerrainTile(TerrainType::Rails);
		terrain[X][Y - 1] = TerrainTile(TerrainType::Barrier);
		terrain[X + 1][Y - 1] = TerrainTile(TerrainType::Barrier);
		terrain[X + 2][Y - 1] = TerrainTile(TerrainType::Barrier);
	}

	void Terrain::GenerateClusters(int old_part, TerrainType clusterType, int minSize, int maxSize, int count)
	{
		int n = locationX - old_part;
		int m = fieldY;

		for (int cluster = 0; cluster < count; cluster++)
		{
			// Выбираем случайную точку для центра кластера
			int startX = randomInt(0, n - 1);
			int startY = randomInt(0, m - 1);

			int clusterSize = randomInt(minSize, maxSize);

			// BFS для заполнения кластера
			queue<pair<int, int>> q;
			vector<vector<bool>> visited(n, vector<bool>(m, false));

			q.push({ startX, startY });
			visited[startX][startY] = true;
			int filled = 0;

			while (!q.empty() && filled < clusterSize)
			{
				auto current = q.front();
				int x = current.first;
				int y = current.second;
				q.pop();

				terrain[x + old_part][y] = clusterType;
				filled++;

				// Добавляем соседей
				vector<pair<int, int>> directions = { {-1,0}, {1,0}, {0,-1}, {0,1} };
				for (auto dir : directions)
				{
					int dx = dir.first;
					int dy = dir.second;
					int nx = x + dx;
					int ny = y + dy;

					if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny] && randomDouble() > 0.3)
					{
						visited[nx][ny] = true;
						q.push({ nx, ny });
					}
				}
			}
		}
	}

	void Terrain::GenerateRivers(int old_part, int count)
	{
		for (int i = 0; i < count; i++)
		{
			int n = locationX - old_part;
			int m = fieldY;

			// Выбираем начальную точку на границе
			int startX, startY;
			int border = randomInt(0, 3); // 0: верх, 1: низ, 2: лево, 3: право

			switch (border)
			{
			case 0: // Верхняя граница
				startX = 0;
				startY = randomInt(0, m - 1);
				break;
			case 1: // Нижняя граница
				startX = n - 1;
				startY = randomInt(0, m - 1);
				break;
			case 2: // Левая граница
				startX = randomInt(0, n - 1);
				startY = 0;
				break;
			case 3: // Правая граница
				startX = randomInt(0, n - 1);
				startY = m - 1;
				break;
			}

			int x = startX, y = startY;
			int maxSteps = n * m / 2; // Максимальное количество шагов
			int steps = 0;

			// Определяем целевое направление (противоположная граница)
			int targetX, targetY;
			if (startX == 0) targetX = n - 1;      // Сверху вниз
			else if (startX == n - 1) targetX = 0; // Снизу вверх
			else targetX = startX;

			if (startY == 0) targetY = m - 1;      // Слева направо
			else if (startY == m - 1) targetY = 0; // Справа налево
			else targetY = startY;

			while (steps < maxSteps) {
				// Проверяем, достигли ли мы противоположной границы
				if ((startX == 0 && x == n - 1) || (startX == n - 1 && x == 0) ||
					(startY == 0 && y == m - 1) || (startY == m - 1 && y == 0))
				{
					break;
				}

				// Устанавливаем реку в текущей позиции
				terrain[x + old_part][y] = TerrainType::Water;

				// Определяем предпочтительные направления движения
				vector<pair<int, int>> directions;

				// Основное направление к цели
				if (startX == 0 || startX == n - 1)
				{
					// Вертикальные реки
					if (targetX > x) directions.push_back({ 1, 0 });  // Вниз
					else directions.push_back({ -1, 0 });             // Вверх
				}
				else
				{
					// Горизонтальные реки
					if (targetY > y) directions.push_back({ 0, 1 });  // Вправо
					else directions.push_back({ 0, -1 });             // Влево
				}

				// Добавляем боковые направления для естественности
				if (startX == 0 || startX == n - 1)
				{
					directions.push_back({ 0, 1 });   // Вправо
					directions.push_back({ 0, -1 });  // Влево
				}
				else
				{
					directions.push_back({ 1, 0 });   // Вниз
					directions.push_back({ -1, 0 });  // Вверх
				}

				// Выбираем направление с учетом весов
				double randVal = randomDouble();
				pair<int, int> move;

				if (randVal < 0.6)
				{
					// 60% - основное направление
					move = directions[0];
				}
				else if (randVal < 0.8)
				{
					// 20% - первое боковое направление
					move = directions[1];
				}
				else
				{
					// 20% - второе боковое направление
					move = directions[2];
				}

				int newX = x + move.first;
				int newY = y + move.second;

				// Проверяем, что не вышли за границы
				if (newX >= 0 && newX < n && newY >= 0 && newY < m)
				{
					x = newX;
					y = newY;
				}
				else
				{
					// Если уперлись в границу, заканчиваем
					break;
				}

				steps++;

				// Небольшая вероятность закончить реку раньше
				if (randomDouble() < 0.05) break;
			}

			// Убедимся, что конечная точка тоже река
			if (x >= 0 && x < n && y >= 0 && y < m)
			{
				terrain[x + old_part][y] = TerrainType::Water;
			}
		}
	}
}