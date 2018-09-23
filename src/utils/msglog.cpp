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
		debug_msg(msg.c_str(), level);
	}

	void msglog::debug_msg(const char* msg, int level)
	{
		if (this->dbg_level >= level)
		{
			std::lock_guard<std::mutex> lock(log_mutex);
			std::cout << "[debug]: " << msg << std::endl;
		}
	}

	void msglog::warning_msg(const std::string& msg)
	{
		warning_msg(msg.c_str());
	}

	void msglog::warning_msg(const char* msg)
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		std::cerr << "[warning]: " << msg << std::endl;
	}

	void msglog::error_msg(const std::string& msg)
	{
		error_msg(msg.c_str());
	}

	void msglog::error_msg(const char* msg)
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		std::cerr << "[error]: " << msg << std::endl;
	}
}
