// GameEngine.cpp:

#include "GameEngine.h"

namespace unrailed_game
{
	void GameEngine::Run()
	{
		SetGameOptions();
		StartGame();
	}
	void GameEngine::SetGameOptions()
	{
		pause = true;

		std::cout << "Выберите сложность:\n";
		std::cout << "1 - лёгкая\n";
		std::cout << "2 - нормальная\n";
		std::cout << "3 - сложная\n";
		std::cout << "4 - невозможная\n";
		std::cin >> difficulty;
		if (difficulty < 1 || difficulty > 4) difficulty = 2;
		if (difficulty == 1) train_speed = 100;	// Лёгкая
		if (difficulty == 2) train_speed = 80;	// Нормальная
		if (difficulty == 3) train_speed = 56;	// Сложная
		if (difficulty == 4) train_speed = 48;	// Невозможная
	}
	void GameEngine::StartGame()
	{
		auto last_time = std::chrono::steady_clock::now();
		while (true)
		{
			auto current_time = std::chrono::steady_clock::now();

			if (current_time - last_time >= tick_time)
			{
				last_time = current_time;

				if (K.WasKeyPressed(VK_GAME_PAUSE)) pause = !pause;
				if (K.WasKeyPressed(VK_EXIT))
				{
					P.ClearAll();
					return;
				}
				// Обновление игры
				if (!pause && !lose)
				{
					UpdateAll();
				}
				K.NewTick();
				// Отрисровка
				P.Clear();
				DrawAll();
				P.Update();
			}
			else
			{
				K.Update();
				std::this_thread::sleep_for(1ms);
			}
		}
	}
	void GameEngine::UpdateAll()
	{
		player.Update(K, T, Items, train);
		train_tick++;
		if (K.WasKeyPressed(VK_MOVE_TRAIN)) train_tick = train_speed;
		if (train_tick == train_speed)
		{
			int last_distation = distation;
			lose = train.Move(T, distation);
			if (lose) return;
			score += CalculateScore(distation - last_distation, train.Waiting() == train_waiting);
			train_tick = 0;
			if (train.Get().begin()->GetX() > trainX) MoveLocation();	// Сдвиг всего поля, если поезд достаточно проехал
			if (train.Waiting() == train_waiting) T.ShiftMap();			// Поезд на станции - сдвигаем карту и генерируем новую её часть
		}
		train.Craft();
	}
	void GameEngine::DrawAll()
	{
		P.DrawInfo(pause, difficulty, score, distation, lose, train_speed, train_tick);
		P.DrawTerrain(T.Get(), T.Position(), train);
		P.DrawPlayerArm(player, T.Get(), T.Position());
		P.DrawItems(Items);
		P.DrawPlayer(player, T.Get(), T.Position());
		P.DrawTrain(train, T.Get(), T.Position());
	}

	void GameEngine::MoveLocation()
	{
		if (T.Position() < locationX - fieldX)
		{
			T.Move();
			player.MoveWithLocation();
			for (auto i = Items.begin(); i != Items.end(); )
			{
				if (i->MoveWithLocation())
				{
					i = Items.erase(i);
				}
				else i++;
			}
			train.MoveWithLocation();
		}
	}

	int GameEngine::CalculateScore(int dist, bool is_on_station)
	{
		int res = 0;
		if (is_on_station) res += 20;
		res += dist;
		res *= difficulty;
		return res;
	}
}