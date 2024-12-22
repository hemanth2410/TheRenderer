#pragma once
#include <chrono>

class PersistantTimer
{
public:
	PersistantTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};