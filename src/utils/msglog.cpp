#include <iostream>

#include "msglog.hpp"

namespace msglog
{
	msglog::msglog()
	{
		this->dbg_level = -1;
	}

	void msglog::set_debug_level(int level)
	{
		this->dbg_level = level;
	}

	void msglog::debug_msg(const std::string& msg, int level)
	{
		if (this->dbg_level >= level)
		{
			std::cout << "[debug]: " << msg << std::endl;
		}
	}

	void msglog::warning_msg(const std::string& msg)
	{
		std::cerr << "[warning]: " << msg << std::endl;
	}

	void msglog::error_msg(const std::string& msg)
	{
		std::cerr << "[error]: " << msg << std::endl;
	}
}
