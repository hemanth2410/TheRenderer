#include "PersistantTimer.h"

using namespace std::chrono;
PersistantTimer::PersistantTimer()
{
	last = steady_clock::now();
}
float PersistantTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}
float PersistantTimer::Peek() const {
	return duration<float>(steady_clock::now() - last).count();
}