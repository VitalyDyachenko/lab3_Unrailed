// GameEngine.h:
#pragma once

#include <iostream>
#include <thread> // Для sleep_for
#include <chrono> // Для времени
#include <array>
#include <list>

#include "Constants.h"
#include "Painter.h"
#include "GameObject.h"
#include "Terrain.h"
#include "KeyReader.h"

using namespace std::chrono_literals;

namespace unrailed_game
{
	class GameEngine
	{
	private:
		const std::chrono::microseconds tick_time = 200ms;

		bool pause;
		bool lose;
		int difficulty; // 0,1,2
		int train_speed = 70; // Раз во сколько тиков двигается поезд

		int location; // Номер пройденной локации
		int distation;
		int score;

		painter::Painter P;

		KeyReader K;
		Player player;
		Terrain T;
		std::list<Item> Items;
		Train train;
		int train_tick;

	public:
		GameEngine()
		{
			Items.push_back(Item(ItemType::Axe, 1, fieldY - 5));
			Items.push_back(Item(ItemType::Pickaxe, 3, fieldY - 5));

			Items.push_back(Item(ItemType::Rails, 1, fieldY - 6));
			Items.push_back(Item(ItemType::Rails, 2, fieldY - 6));
			Items.push_back(Item(ItemType::Rails, 3, fieldY - 6));
		}
		//~GameEngine();
		int CalculateScore(int dist, bool is_on_station);

		const Player& GetPlayer() const { return player; }

		void Run();
		void SetGameOptions();
		void StartGame();

		void UpdateAll();
		void DrawAll();

		void MoveLocation();
	};
}