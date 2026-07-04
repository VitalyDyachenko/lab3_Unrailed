// Constants.h:
#pragma once

#include <string>
#include <windows.h>
#include <array>

// Размеры видимой области:
constexpr int fieldX = 20;
constexpr int fieldY = 14;

constexpr int locationX = 40; // Длина всей локации:
constexpr int spawnX = 5; // Длина зоны спавна
constexpr int trainX = 8; // Положение поезда относительно экрана

constexpr int tile_durability = 4;
constexpr int player_inventory_capacity = 3;
constexpr int train_inventory_capacity = 3;
constexpr int train_waiting = 3; // Сколько тиков поезд стоит на станции
constexpr int crafting_time = 30; // Сколько тиков крафтит 1 рельсу

//const std::wstring TEXTURE_PLAYER = L"☻ ";
const wchar_t TEXTURE_NUMBER = L'1'; //①
const int COLOR_FLOOR = 96;
const wchar_t TEXTURE_PLAYER = L'☻'; //ඩ
const int COLOR_PLAYER = 110;
const int COLOR_PLAYER_RAILS = 5 * 16 + 14;
const int COLOR_PLAYER_WATER = 3 * 16 + 14;
const wchar_t TEXTURE_PICKAXE = L'↑';
const int COLOR_PICKAXE = 97;
const wchar_t TEXTURE_AXE = L'ᖰ';
const int COLOR_AXE = 100;
const wchar_t TEXTURE_WOOD = L'▲';
const int COLOR_WOOD = 98;
const wchar_t TEXTURE_STONE = L'■';
const int COLOR_STONE = 104;
const wchar_t TEXTURE_RAILS = L'፲'; //⋕╪⌗‡
const int COLOR_RAILS = 6 * 16 + 5;
const std::wstring TEXTURE_CARRIAGE_EMPTY = L"[]";
const std::wstring TEXTURE_CARRIAGE_MAIN_LEFT = L"<|";
const std::wstring TEXTURE_CARRIAGE_MAIN_UP = L"/\\";
const std::wstring TEXTURE_CARRIAGE_MAIN_RIGHT = L"|>";
const std::wstring TEXTURE_CARRIAGE_MAIN_DOWN = L"\\/";
const std::wstring TEXTURE_CARRIAGE_CRAFTING_0 = L"  ";
const std::wstring TEXTURE_CARRIAGE_CRAFTING_1 = L"1#";
const std::wstring TEXTURE_CARRIAGE_CRAFTING_2 = L"2#";
const std::wstring TEXTURE_CARRIAGE_CRAFTING_3 = L"##";
const int COLOR_TRAIN_CRAFTING = 16 * 4 + 5;
const int COLOR_TRAIN_STORAGE_WOOD = 16 * 4 + 10;
const int COLOR_TRAIN_STORAGE_STONE = 16 * 4 + 7;
const int COLOR_TRAIN = 16 * 4 + 15;
const std::wstring TEXTURE_FOREST = L"♧↟";
const std::wstring TEXTURE_FOREST_B = L"♧ ";
const int COLOR_FOREST = 42;
const std::wstring TEXTURE_MOUNTAINS = L"/\\"; //"⎕ᗋ";
const std::wstring TEXTURE_MOUNTAINS_B = L"/ ";
const int COLOR_MOUNTAINS = 135;// 127;
const std::wstring TEXTURE_WATER = L"-_";// L"░░";
const int COLOR_WATER = 16 * 3 + 11;
const std::wstring TEXTURE_RAILS_TERRAIN_1 = L"፲፲";
const std::wstring TEXTURE_RAILS_TERRAIN_2 = L"┝┥";
const int COLOR_RAILS_TERRAIN = 5 * 16 + 7;
const std::wstring TEXTURE_STATION = L"/П П\\⚐";
const int COLOR_STATION = 12 * 16 + 7;

constexpr int VK_MOVE_UP = VK_UP;
constexpr int VK_MOVE_DOWN = VK_DOWN;
constexpr int VK_MOVE_RIGHT = VK_RIGHT;
constexpr int VK_MOVE_LEFT = VK_LEFT;
constexpr int VK_GET = 'E';
constexpr int VK_DROP = 'Q';
constexpr int VK_GAME_PAUSE = 'P';
constexpr int VK_EXIT = 'O';
constexpr int VK_MOVE_TRAIN = 'M';