// Random.cpp:
#include "Random.h"

// Глобальный генератор случайных чисел
random_device rd;
mt19937 gen(rd());

double randomDouble()
{
	uniform_real_distribution<> dis(0.0, 1.0);
	return dis(gen);
}
int randomInt(int min, int max)
{
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}