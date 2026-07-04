// GameObject.cpp:
#include "GameObject.h"

namespace unrailed_game
{
	void Player::Update(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train)
	{
		TakeItem(K, T, Items, train);
		DropItem(K, T, Items, train);
		Move(K, T, Items, train);
	}
	void Player::TakeItem(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train)
	{
		if (K.WasKeyPressed(VK_GET))
		{
			// Попытка взять с поезда:
			if (train.TryToTakeRails(*this)) return;
			// Взять с пола:
			if (Stackable(item) && item_count < player_inventory_capacity)
			{
				for (auto i = Items.begin(); i != Items.end();)
				{
					if (i->GetX() == X && i->GetY() == Y && AddItem(i->GetType()))
					{
						i = Items.erase(i);
						break;
					}
					else i++;
				}
			}	
		}
	}
	bool Train::TryToTakeRails(Player& p)
	{
		int X = p.GetX(), Y = p.GetY();
		for (auto& c : train)
		{
			if (c.type == CarriageType::Crafting && c.Rails() > 0 &&
				abs(c.GetX() - X) <= 1 && abs(c.GetY() - Y) <= 1)
			{
				bool was_taken = false;
				while (c.Rails() > 0 && p.AddItem(ItemType::Rails))
				{
					was_taken = true;
					c.Remove(ItemType::Rails);
				}
				return was_taken;
			}
		}
		return 0;
	}
	void Player::DropItem(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train)
	{
		if (K.WasKeyPressed(VK_DROP) && item != ItemType::Empty)
		{
			if (T.Get()[X + T.Position()][Y].type == TerrainType::Empty)
			{
				ItemType it = ItemType::Empty;
				for (auto& i : Items)
				{
					if (i.GetX() == X && i.GetY() == Y)
					{
						it = i.GetType();
						break;
					}
				}
				bool is_items_under_player = (it != ItemType::Empty);

				// Попытка поставить рельсы:
				if (item == ItemType::Rails && !is_items_under_player)
				{
					bool rail_up = (Y != 0) && T.Get()[X + T.Position()][Y - 1].type == TerrainType::Rails;
					bool rail_down = (Y != fieldY - 1) && T.Get()[X + T.Position()][Y + 1].type == TerrainType::Rails;
					bool rail_right = (X + T.Position() != locationX - 1) && T.Get()[X + 1 + T.Position()][Y].type == TerrainType::Rails;
					bool rail_left = (X + T.Position() != 0) && T.Get()[X - 1 + T.Position()][Y].type == TerrainType::Rails;
					// Проверка на то, что вокруг игрока ровно 1 рельса 
					if (int(rail_up) + int(rail_down) + int(rail_right) + int(rail_left) == 1)
					{
						int x = X;
						int y = Y;
						if (rail_right) x++;
						if (rail_left) x--;
						if (rail_down) y++;
						if (rail_up) y--;
						bool rail_up = (y != 0) && T.Get()[x + T.Position()][y - 1].type == TerrainType::Rails;
						bool rail_down = (y != fieldY - 1) && T.Get()[x + T.Position()][y + 1].type == TerrainType::Rails;
						bool rail_right = (x + T.Position() != locationX - 1) && T.Get()[x + 1 + T.Position()][y].type == TerrainType::Rails;
						bool rail_left = (x + T.Position() != 0) && T.Get()[x - 1 + T.Position()][y].type == TerrainType::Rails;
						// Проверка, что вокруг этой рельсы не более 1 рельсы
						if (int(rail_up) + int(rail_down) + int(rail_right) + int(rail_left) <= 1)
						{
							T.SetRail(X + T.Position(), Y);
							RemoveItem();
							return;
						}
					}
				}
				// Попытка положить в поезд:
				if (train.TryToPutItems(*this)) return;
				// Попытка положить на землю:
				if (!is_items_under_player)
				{
					Items.push_back(Item(item, X, Y));
					RemoveItem();
				}
			}
			else if (T.Get()[X + T.Position()][Y].type == TerrainType::Water)
			{
				T.SetBridge(X + T.Position(), Y);
				RemoveItem();
				return;
			}
		}
	}
	bool Train::TryToPutItems(Player& p)
	{
		if (p.GetItem() == ItemType::Wood || p.GetItem() == ItemType::Stone)
		{
			for (auto& c : train)
			{
				// Если вагон-хранилище находится возле игрока
				if (c.type == CarriageType::Storage && c.Item(p.GetItem()) < train_inventory_capacity &&
					abs(c.GetX() - p.GetX()) <= 1 && abs(c.GetY() - p.GetY()) <= 1)
				{
					c.Add(p.GetItem());
					p.RemoveItem();
					return 1;
				}
			}
		}
		return 0;
	}
	bool Player::CanMove(int x, int y, Terrain& T, Train& train) const
	{
		if (x >= fieldX || x < 0 || y >= fieldY || y < 0) return 0;
		return CanMoveWithoutWater(x, y, T, train) ||
			T.Get()[x + T.Position()][y].type == TerrainType::Water && item == ItemType::Wood &&
			(CanMoveWithoutWater(x + 1, y, T, train) || CanMoveWithoutWater(x - 1, y, T, train) ||
			CanMoveWithoutWater(x, y + 1, T, train) || CanMoveWithoutWater(x, y - 1, T, train));
	}
	bool Player::CanMoveWithoutWater(int x, int y, Terrain& T, Train& train) const
	{
		if (x >= fieldX || x < 0 || y >= fieldY || y < 0) return 0;
		return !train.IsOnPosition(x, y) &&
			(T.Get()[x + T.Position()][y].type == TerrainType::Empty ||
			T.Get()[x + T.Position()][y].type == TerrainType::Rails ||
			T.Get()[x + T.Position()][y].type == TerrainType::Station);
	}
	void Player::Move(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train)
	{
		int x = X, y = Y;
		int break_x = X, break_y = Y;
		if (K.WasKeyPressed(VK_MOVE_RIGHT) && X < fieldX - 1)
		{
			if (CanMove(x + 1, y, T, train)) x += 1;
			break_x = X + 1;
			break_y = Y;
		}
		if (K.WasKeyPressed(VK_MOVE_LEFT) && X > 0)
		{
			if (CanMove(x - 1, y, T, train)) x -= 1;
			break_x = X - 1;
			break_y = Y;
		}
		if (K.WasKeyPressed(VK_MOVE_DOWN) && Y < fieldY - 1)
		{
			if (CanMove(x, y + 1, T, train)) y += 1;
			break_x = X;
			break_y = Y + 1;
		}
		if (K.WasKeyPressed(VK_MOVE_UP) && Y > 0)
		{
			if (CanMove(x, y - 1, T, train)) y -= 1;
			break_x = X;
			break_y = Y - 1;
		}
		if (T.Get()[break_x + T.Position()][break_y].type == TerrainType::Forest && item == ItemType::Axe ||
			T.Get()[break_x + T.Position()][break_y].type == TerrainType::Moutains && item == ItemType::Pickaxe)
		{
			if (T.Break(break_x + T.Position(), break_y))
			{
				if (item == ItemType::Axe) Items.push_back(Item(ItemType::Wood, break_x, break_y));
				if (item == ItemType::Pickaxe) Items.push_back(Item(ItemType::Stone, break_x, break_y));
			}
		}
		X = x;
		Y = y;
	}
	bool Train::IsOnPosition(int x, int y) const
	{
		for (auto& c : train)
		{
			if (c.GetX() == x && c.GetY() == y) return true;
		}
		return false;
	}
	bool Train::Move(const Terrain& T, int& distation)
	{
		int last_x = -1;
		int last_y = -1;
		for (auto& c : train)
		{
			if (last_x == -1)
			{
				int X = c.GetX();
				int Y = c.GetY();
				if (waiting > 0 && T.Get()[X + T.Position()][Y].type == TerrainType::Station)
				{
					// Поезд ждёт на станции:
					waiting--;
					break;
				}

				last_x = X;
				last_y = Y;
				auto& second_carriage = *std::next(train.begin());
				int x = second_carriage.GetX();
				int y = second_carriage.GetY();
				// Поиск рельсы вокруг, по которой он не проезжал:
				if ((Y != 0) && T.Get()[X + T.Position()][Y - 1].type == TerrainType::Rails && Y - 1 != y)
					Y--;
				else if ((Y != fieldY - 1) && T.Get()[X + T.Position()][Y + 1].type == TerrainType::Rails && Y + 1 != y)
					Y++;
				else if ((X + T.Position() != locationX - 1) && T.Get()[X + 1 + T.Position()][Y].type == TerrainType::Rails && X + 1 != x)
					X++;
				else if ((X + T.Position() != 0) && T.Get()[X - 1 + T.Position()][Y].type == TerrainType::Rails && X - 1 != x)
					X--;
				else
				{
					waiting = train_waiting; // Считаем, что поезд у станции и начиает ждать
					// Поиск станции вокруг:
					if ((Y != 0) && T.Get()[X + T.Position()][Y - 1].type == TerrainType::Station)
						Y--;
					else if ((Y != fieldY - 1) && T.Get()[X + T.Position()][Y + 1].type == TerrainType::Station)
						Y++;
					else if ((X + T.Position() != locationX - 1) && T.Get()[X + 1 + T.Position()][Y].type == TerrainType::Station)
						X++;
					else if ((X + T.Position() != 0) && T.Get()[X - 1 + T.Position()][Y].type == TerrainType::Station)
						X--;
					else return 1; // Конец игры - вокруг нет ни рельс, ни станции
				}
				c.SetX(X);
				c.SetY(Y);
				distation += X - last_x;
			}
			else
			{
				int X = last_x;
				int Y = last_y;
				last_x = c.GetX();
				last_y = c.GetY();
				c.SetX(X);
				c.SetY(Y);
			}
		}
		return 0;
	}
	bool Carriage::Craft()
	{
		crafting_progress++;
		if (crafting_progress == crafting_time)
		{
			crafting_progress = 0;
			return true;
		}
		return false;
	}
	void Train::Craft()
	{
		for (auto& c : train)
		{
			if (c.type == CarriageType::Crafting && c.Rails() < train_inventory_capacity)
			{
				if (!c.Crafting())
				{
					// Поиск хранилищ с ресурсами:
					Carriage* wood = nullptr;
					Carriage* stone = nullptr;
					for (auto& s : train)
					{
						if (s.type == CarriageType::Storage)
						{
							if (!wood && s.Wood() > 0) wood = &s;
							if (!stone && s.Stone() > 0) stone = &s;

							if (wood && stone)
							{
								wood->Remove(ItemType::Wood);
								stone->Remove(ItemType::Stone);
								break;
							}
						}
					}
					if (!(wood && stone)) break;
					// Начало крафта в случае нахождения ресурсов:
					c.Craft();
				}
				else
				{
					if (c.Craft()) c.Add(ItemType::Rails);
				}
			}
		}
	}
}