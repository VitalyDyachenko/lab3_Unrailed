// GameObject.h:
#pragma once

#include <string>
#include <list>

#include "Terrain.h"
#include "KeyReader.h"
#include "Constants.h"

namespace unrailed_game
{
	enum class ItemType
	{
		Empty,
		Pickaxe,
		Axe,
		Stone,
		Wood,
		Rails
	};
	constexpr bool Stackable(ItemType type)
	{
		return (type != ItemType::Axe && type != ItemType::Pickaxe);
	}

	class GameObject
	{
	protected:
		int X, Y;
	public:
		virtual ~GameObject() = default;
		int GetX() const { return X; }
		int GetY() const { return Y; }

		bool MoveWithLocation() // Возвращает true, если долен выйти за границу
		{
			if (X != 0)
			{
				X--;
				return false;
			}
			return true;
		}
	};

	class Item;
	class Train;

	class Player : public GameObject
	{
	private:
		ItemType item;
		int item_count;
	public:
		Player() : GameObject()
		{
			item = ItemType::Empty;
			item_count = 0;
			X = 2;
			Y = fieldY - 4;
		}
		ItemType GetItem() const { return item; }
		int GetItemCount() const { return item_count; }

		bool AddItem(ItemType type)
		{
			if (type == ItemType::Empty ||
			item_count >= player_inventory_capacity ||
			item != ItemType::Empty && item != type)
				return 0;
			if (item_count == 0) item = type;
			item_count++;
			return 1;
		}
		bool RemoveItem()
		{
			if (item == ItemType::Empty) return 0;
			item_count--;
			if (item_count == 0) item = ItemType::Empty;
			return 1;
		}

		bool CanMove(int x, int y, Terrain& T, Train& train) const;
		bool CanMoveWithoutWater(int x, int y, Terrain& T, Train& train) const;
		void Move(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train);
		void DropItem(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train);
		void TakeItem(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train);
		void Update(KeyReader& K, Terrain& T, std::list<Item>& Items, Train& train);
	};

	class Item : public GameObject
	{
	private:
		ItemType type;
	public:
		Item(ItemType Type, int x, int y) : type(Type)
		{
			X = x;
			Y = y;
		}
		ItemType GetType() const { return type; }
	};

	enum CarriageType
	{
		Empty,
		Main,
		Crafting,
		Storage
	};
	class Carriage : public GameObject
	{
	private:
		int rails_count;
		int crafting_progress;
		int wood_count;
		int stone_count;
	public:
		CarriageType type;
		Carriage() : type(CarriageType::Empty) {}
		Carriage(CarriageType t) : type(t)
		{
			rails_count = 0;
			crafting_progress = 0;
			wood_count = 0;
			stone_count = 0;
		}
		int Rails() const { return rails_count; }
		int CraftingProgress() const { return crafting_progress; }
		int Stone() const { return stone_count; }
		int Wood() const { return wood_count; }
		int Item(ItemType item) const
		{
			if (item == ItemType::Wood) return wood_count;
			if (item == ItemType::Stone) return stone_count;
			if (item == ItemType::Rails) return rails_count;
			return 0;
		}
		void Add(ItemType item)
		{
			if (item == ItemType::Wood && wood_count < train_inventory_capacity) wood_count++;
			if (item == ItemType::Stone && stone_count < train_inventory_capacity) stone_count++;
			if (item == ItemType::Rails && stone_count < train_inventory_capacity) rails_count++;
		}
		void Remove(ItemType item)
		{
			if (item == ItemType::Wood && wood_count > 0) wood_count--;
			if (item == ItemType::Stone && stone_count > 0) stone_count--;
			if (item == ItemType::Rails && rails_count > 0) rails_count--;
		}

		void SetX(int x) { X = x; }
		void SetY(int y) { Y = y; }

		bool Craft();
		bool Crafting() { return crafting_progress != 0; }
	};
	class Train
	{
	private:
		std::list<Carriage> train;
		int waiting;
	public:
		Train()
		{
			train.push_back(Carriage(CarriageType::Main));
			train.push_back(Carriage(CarriageType::Crafting));
			train.push_back(Carriage(CarriageType::Storage));
			train.push_back(Carriage());
			train.push_back(Carriage());
			train.push_back(Carriage(CarriageType::Storage));
			train.push_back(Carriage());
			train.push_back(Carriage());

			int y = fieldY / 2 - 2;
			for (auto& c : train)
			{
				c.SetX(2);
				c.SetY(y--);
			}
		}
		const std::list<Carriage>& Get() const { return train; }
		bool IsOnPosition(int x, int y) const;
		int Waiting() const { return waiting; }

		bool TryToTakeRails(Player& p);
		bool TryToPutItems(Player& p);

		void MoveWithLocation()
		{
			for (auto& c : train) c.MoveWithLocation();
		}
		bool Move(const Terrain& T, int& distation);
		void Craft();
	};
}