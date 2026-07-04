// Painter.h:
#pragma once

#include <iostream>
#include <list>
#include <windows.h>

#include "Constants.h"
#include "Terrain.h"
#include "GameObject.h"

namespace painter
{
	class Painter
	{
	private:
		const int field_position = 3; // Расположение поля по Y относительно экрана

		const int real_width;
		const int real_height;

		HANDLE console;
		CHAR_INFO* buffer; // Буфер, хранящий следующее состояние консоли.
		const int buffer_size;
	public:
		Painter() : 
			real_width(2*fieldX),
			real_height(field_position + fieldY),
			buffer_size(real_width * real_height),
			console(GetStdHandle(STD_OUTPUT_HANDLE))
		{
			system("cls");
			// Инициализация буфера
			buffer = new CHAR_INFO[buffer_size];
			for (int i = 0; i < buffer_size; i++)
			{
				buffer[i].Char.UnicodeChar = L' ';
				buffer[i].Attributes = 7;
			}
			// Скрываем курсор
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(console, &cursorInfo);
			cursorInfo.bVisible = FALSE;
			SetConsoleCursorInfo(console, &cursorInfo);
		}
		~Painter()
		{
			ClearAll();
			delete[] buffer;
		}
		void ClearAll()
		{
			system("cls");
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(console, &cursorInfo);
			cursorInfo.bVisible = TRUE;
			SetConsoleCursorInfo(console, &cursorInfo);
		}

		void Update(); // Обновляет консоль
		void Clear();
		void set_char(int x, int y, wchar_t ch, bool right = false, int color = 7);
		void set_string(int x, int y, const std::wstring& str, int color = 7);
		void set_char_in_field(int x, int y, wchar_t ch, bool right = false, int color = 7);
		void set_string_in_field(int x, int y, const std::wstring& str, int color = 7);

		void DrawInfo(bool pause, int difficulty, int score, int distantion, bool lose, int train_speed, int train_tick);
		void DrawPlayer(unrailed_game::Player& p, const unrailed_game::terrain_t& terrain, int LocationPosition);
		void DrawPlayerArm(unrailed_game::Player& p, const unrailed_game::terrain_t& terrain, int LocationPosition);
		void DrawTerrain(const unrailed_game::terrain_t & terrain, int LocationPosition, const unrailed_game::Train& train);
		void DrawItems(const std::list<unrailed_game::Item>& Items);
		void DrawTrain(const unrailed_game::Train& train, const unrailed_game::terrain_t& terrain, int LocationPosition);
	};
}