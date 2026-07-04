// KeyReader.h:
#pragma once

#include <map>
#include <windows.h>
#include <stdexcept>

#include "Constants.h"

constexpr int holding_min_time = 10; // Минимальное число Update, чтоб нажатие считать зажаием
//constexpr int max_pressed_count = 5;

class KeyInfo
{
public:
	int holding_time; // Сколько прошло Update с зажатия
	int pressed_count;
	bool used_this_key; // Было ли уже использовано, что эта клавиша нажата.
};
class KeyReader
{
private:
	std::map<int, KeyInfo> pressed_keys;
public:
	KeyReader()
	{
		pressed_keys[VK_GET];
		pressed_keys[VK_DROP];
		pressed_keys[VK_MOVE_UP];
		pressed_keys[VK_MOVE_DOWN];
		pressed_keys[VK_MOVE_RIGHT];
		pressed_keys[VK_MOVE_LEFT];
		pressed_keys[VK_GAME_PAUSE];
		pressed_keys[VK_EXIT]; 
		pressed_keys[VK_MOVE_TRAIN];
	}
	static bool IsKeyPressed(int key)
	{
		return (GetAsyncKeyState(key) & 0x8000) != 0;
	}
	bool WasKeyPressed(int key)
	{
		try
		{
			auto& key_info = pressed_keys.at(key);

			// Если есть буферизованные нажатия - используем их
			if (key_info.pressed_count > 0)
			{
				key_info.pressed_count--;
				key_info.used_this_key = true;  // Помечаем как использованное
				return true;
			}
			// Если клавиша нажата и еще не использована в этом тике
			if (IsKeyPressed(key) && !key_info.used_this_key)
			{
				key_info.used_this_key = true;
				return true;
			}

			return false;
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
	}

	void Update()
	{
		for (auto& a : pressed_keys)
		{
			auto& key_info = a.second;
			if (IsKeyPressed(a.first))
			{
				key_info.holding_time++;

				// Если только что нажали (holding_time == 1) - добавляем в буфер
				if (key_info.holding_time == 1 && !key_info.used_this_key)
				{
					key_info.pressed_count++;
				}
			}
			else
			{
				// При отпускании сбрасываем состояние
				key_info.holding_time = 0;
			}
		}
	}

	void NewTick()
	{
		for (auto& a : pressed_keys)
		{
			pressed_keys[a.first].used_this_key = 0;
		}
	}
	void Clear()
	{
		for (auto& a : pressed_keys)
		{
			//pressed_keys[a.first].this_tick_state = 0;
			//pressed_keys[a.first].last_tick_state = 0;
		}
	}
};