#include "barrier.h"

#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
int main(int, char**)
{
	std::atomic<unsigned> x(0);
	unsigned T = std::thread::hardware_concurrency();
	barrier bar{T};
	auto thread_proc = [&x, &bar, T]()
	{
		if (++x == T)
			std::cout << "Reached T (1)\n";
		bar.arrive_and_wait();
		if (--x == 0u)
			std::cout << "Reached 0\n";
		bar.arrive_and_wait();
		if (++x == T)
			std::cout << "Reached T (2)\n";
	};
	std::vector<std::thread> threads;
	for (unsigned t = 0; t < T; ++t)
		threads.emplace_back(thread_proc);
	for (auto& thread:threads)
		thread.join();
	return 0;
}

/*
Reached T (1)
Reached 0
Reached T (2)
*/

