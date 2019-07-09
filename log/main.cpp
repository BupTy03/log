#include "logger.hpp"

#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
	using namespace std::chrono_literals;

	logger lg;

	std::thread th1([&lg] {
		for (int i = 0; i < 10000; ++i)
			lg.println("Hello");
	});
	std::thread th2([&lg] {
		for (int i = 0; i < 10000; ++i)
			lg.println("world");
	});
	std::thread th3([&lg] {
		for (int i = 0; i < 10000; ++i)
			lg.println("!!!");
	});

	std::this_thread::sleep_for(20s);

	th1.join();
	th2.join();
	th3.join();

	return 0;
}