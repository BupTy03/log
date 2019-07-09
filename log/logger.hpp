#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "active.hpp"
#include <iostream>
#include <string>

class logger
{
public:
	void println(std::string msg)
	{
		a_.send([str = std::move(msg)]{ std::cout << str << std::endl; });
	}

private:
	active a_;
};

#endif // !LOGGER_HPP
