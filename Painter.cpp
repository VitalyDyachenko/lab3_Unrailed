// Painter.cpp:
#include "Painter.h"

using namespace unrailed_game;
namespace painter
{
	constexpr wchar_t GetItemTypeTexture(ItemType type)
	{
		switch (type)
		{
		case ItemType::Axe: return TEXTURE_AXE;
		case ItemType::Pickaxe: return TEXTURE_PICKAXE;
		case ItemType::Stone: return TEXTURE_STONE;
		case ItemType::Wood: return TEXTURE_WOOD;
		case ItemType::Rails: return TEXTURE_RAILS;
		default: return L'?';
		}
	}
	constexpr int GetItemTypeColor(ItemType type)
	{
		switch (type)
		{
		case ItemType::Axe: return COLOR_AXE;
		case ItemType::Pickaxe: return COLOR_PICKAXE;
		case ItemType::Stone: return COLOR_STONE;
		case ItemType::Wood: return COLOR_WOOD;
		case ItemType::Rails: return COLOR_RAILS;
		default: return COLOR_PLAYER;
		}
	}

	void Painter::Update()
	{
		COORD buffer_sizes = { (SHORT)real_width, (SHORT)real_height };
		COORD buffer_coord = { 0, 0 };
		SMALL_RECT write_region = { 0, 0, (SHORT)(real_width - 1), (SHORT)(real_height - 1) };

		//system("cls");
		WriteConsoleOutputW(console, buffer, buffer_sizes, buffer_coord, &write_region);
	}
	void Painter::Clear()
	{
		for (int i = 0; i < buffer_size; i++)
		{
			buffer[i].Char.UnicodeChar = ' ';
			buffer[i].Attributes = 7;
		}
	}
	void Painter::set_char(int x, int y, wchar_t ch, bool right, int color)
	{
		// Перевод из координат внутри холста в реальные коордтнаты
		x = x * 2 + right;
		y = y + field_position;

		if (x >= 0 && x < real_width && y >= 0 && y < real_height)
		{
			int index = y * real_width + x;
			buffer[index].Char.UnicodeChar = ch;
			buffer[index].Attributes = color;
		}
	}
	void Painter::set_string(int x, int y, const std::wstring& str, int color)
	{
		for (size_t i = 0; i < str.size(); i += 2)
		{
			set_char(x + i/2, y, str[i], 0, color);
			if (i + 1 != str.size()) set_char(x + i/2, y, str[i+1], 1, color);
		}
	}
	void Painter::set_char_in_field(int x, int y, wchar_t ch, bool right, int color)
	{
		if (x >= 0 && x < fieldX && y >= 0 && y < fieldY) set_char(x, y, ch, right, color);
	}
	void Painter::set_string_in_field(int x, int y, const std::wstring& str, int color)
	{
		for (size_t i = 0; i < str.size(); i += 2)
		{
			set_char_in_field(x + i / 2, y, str[i], 0, color);
			if (i + 1 != str.size()) set_char_in_field(x + i / 2, y, str[i + 1], 1, color);
		}
	}

	void Painter::DrawInfo(bool pause, int difficulty, int score, int distantion, bool lose, int train_speed, int train_tick)
	{
		if (lose)
		{
			set_string(0, 0 - field_position, L"Конец игры. Нажмите O, чтобы выйти.");
		}
		else
		{
			if (pause) set_string(0, 0 - field_position, L"Пауза. Нажмите P, чтобы продождить игру.");
			else set_string(0, 0 - field_position, L"P - поставить игру на паузу, O - выход.");
		}
		std::wstring info = L"Сложность:" + std::to_wstring(difficulty);
		info += L"  Пройдено:" + std::to_wstring(distantion);
		info += L"  Счёт:" + std::to_wstring(score);
		set_string(0, 1 - field_position, info);
		for (int x = 0; x < train_speed / 5; x++)
		{
			int color = (x > train_tick / 5) ? 8 : 11;
			set_string(x, 2 - field_position, L"==", color);
		}
	}
	void Painter::DrawPlayer(Player& p, const terrain_t& terrain, int LocationPosition)
	{
		bool on_rails = (terrain[p.GetX() + LocationPosition][p.GetY()].type == TerrainType::Rails || terrain[p.GetX() + LocationPosition][p.GetY()].type == TerrainType::Station);
		bool on_water = (terrain[p.GetX() + LocationPosition][p.GetY()].type == TerrainType::Water);
		int color;

		if (on_rails) color = COLOR_PLAYER_RAILS;
		else if (on_water) color = COLOR_PLAYER_WATER;
		else color = COLOR_PLAYER;
		bool HaveItem = (p.GetItem() != ItemType::Empty);
		set_char(p.GetX(), p.GetY(), TEXTURE_PLAYER, !HaveItem, color);
		
		if (on_rails) color = COLOR_PLAYER_RAILS;
		else if (on_water) color = COLOR_PLAYER_WATER;
		else color = GetItemTypeColor(p.GetItem());
		if (HaveItem) set_char(p.GetX(), p.GetY(), GetItemTypeTexture(p.GetItem()), 1, color);
	}
	void Painter::DrawPlayerArm(unrailed_game::Player& p, const unrailed_game::terrain_t& terrain, int LocationPosition)
	{
		bool on_rails = (terrain[p.GetX() + LocationPosition][p.GetY()].type == TerrainType::Rails || terrain[p.GetX() + LocationPosition][p.GetY()].type == TerrainType::Station);
		bool on_water = (terrain[p.GetX() + LocationPosition][p.GetY()].type == TerrainType::Water);
		int color;

		if (on_rails) color = COLOR_PLAYER_RAILS;
		else if (on_water) color = COLOR_PLAYER_WATER;
		else color = GetItemTypeColor(p.GetItem());
		if (p.GetItem() == ItemType::Empty)
		{
			set_char(p.GetX(), p.GetY(), L'·', 0, color);
		}
		
	}
	void Painter::DrawTerrain(const terrain_t& terrain, int LocationPosition, const unrailed_game::Train& train)
	{
		for (int y = 0; y < fieldY; y++)
		{
			for (int x = 0; x < fieldX; x++)
			{
				if (terrain[x + LocationPosition][y].type == TerrainType::Forest)
					set_string(x, y, terrain[x + LocationPosition][y].count <= tile_durability / 2 ? TEXTURE_FOREST_B : TEXTURE_FOREST, COLOR_FOREST);
				else if (terrain[x + LocationPosition][y].type == TerrainType::Moutains) 
					set_string(x, y, terrain[x + LocationPosition][y].count <= tile_durability / 2 ? TEXTURE_MOUNTAINS_B : TEXTURE_MOUNTAINS, COLOR_MOUNTAINS);
				else if (terrain[x + LocationPosition][y].type == TerrainType::Water) 
					set_string(x, y, TEXTURE_WATER, COLOR_WATER);
				else if (terrain[x + LocationPosition][y].type == TerrainType::Rails)
				{
					if ((y == 0 || terrain[x + LocationPosition][y - 1].type != TerrainType::Rails) &&
						(y == fieldY - 1 || terrain[x + LocationPosition][y + 1].type != TerrainType::Rails))
					{
						set_string(x, y, TEXTURE_RAILS_TERRAIN_1, COLOR_RAILS_TERRAIN);
					}
					else
					{
						set_string(x, y, TEXTURE_RAILS_TERRAIN_2, COLOR_RAILS_TERRAIN);
					}
				}
				else if (terrain[x + LocationPosition][y].type == TerrainType::Station)
				{
					set_string(x, y, TEXTURE_RAILS_TERRAIN_1, COLOR_RAILS_TERRAIN);
					set_string_in_field(x - 1, y - 1, TEXTURE_STATION, COLOR_STATION);
					if (train.Waiting()) set_char_in_field(x, y - 1, TEXTURE_NUMBER + train.Waiting(), 0, COLOR_STATION);
				}
				else set_string(x, y, L"  ", COLOR_FLOOR);
			}
		}
	}
	void Painter::DrawItems(const std::list<Item>& Items)
	{
		for (auto& item : Items)
		{
			int x, y;
			x = item.GetX(), y = item.GetY();
			set_char(x, y, GetItemTypeTexture(item.GetType()), 0, GetItemTypeColor(item.GetType()));
			if (item.GetType() == ItemType::Rails)
				set_char(x, y, GetItemTypeTexture(item.GetType()), 1, GetItemTypeColor(item.GetType()));
		}
	}
	void Painter::DrawTrain(const Train& train, const terrain_t& terrain, int LocationPosition)
	{
		for (auto& c : train.Get())
		{
			if (c.type == CarriageType::Storage)
			{
				set_char(c.GetX(), c.GetY(), L'0' + c.Wood(), 0, COLOR_TRAIN_STORAGE_WOOD);
				set_char(c.GetX(), c.GetY(), L'0' + c.Stone(), 1, COLOR_TRAIN_STORAGE_STONE);
				continue;
			}
			std::wstring texture = TEXTURE_CARRIAGE_EMPTY;
			if (c.type == CarriageType::Main)
			{
				auto& second_carriage = *std::next(train.Get().begin());
				int X = second_carriage.GetX();
				int Y = second_carriage.GetY();
				if (X == c.GetX() - 1)
				{
					texture = TEXTURE_CARRIAGE_MAIN_RIGHT;
				}
				if (X == c.GetX() + 1)
				{
					texture = TEXTURE_CARRIAGE_MAIN_LEFT;
				}
				if (Y == c.GetY() - 1)
				{
					texture = TEXTURE_CARRIAGE_MAIN_DOWN;
				}
				if (Y == c.GetY() + 1)
				{
					texture = TEXTURE_CARRIAGE_MAIN_UP;
				}
			}
			else if (c.type == CarriageType::Crafting)
			{
				if (c.Rails() == 3)
					texture = TEXTURE_CARRIAGE_CRAFTING_3;
				else if (c.Rails() == 2)
					texture = TEXTURE_CARRIAGE_CRAFTING_2;
				else if (c.Rails() == 1)
					texture = TEXTURE_CARRIAGE_CRAFTING_1;
				else if (c.Rails() == 0)
					texture = TEXTURE_CARRIAGE_CRAFTING_0;
			}
			int color = (c.type == CarriageType::Crafting) ? COLOR_TRAIN_CRAFTING : COLOR_TRAIN;
			set_string_in_field(c.GetX(), c.GetY(), texture, color);
		}
	}
}