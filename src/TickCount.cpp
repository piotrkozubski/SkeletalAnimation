
#include "TickCount.h"
#include <chrono>
#include "Logger.h"


double GetTickCount(void)
{
	static auto start = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

	//LOG_INF(delta);

	return delta;
}
