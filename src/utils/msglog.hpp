/*
 * Description
 */

#ifndef MSGLOG_HPP_
#define MSGLOG_HPP_

#include <atomic>
#include <string>

namespace msglog
{
	class msglog
	{
		int dbg_level;
	public:
		msglog();
		void set_debug_level(int level);
		void debug_msg(const std::string& msg, int level);
		void warning_msg(const std::string& msg);
		void error_msg(const std::string& msg);
	};

	extern std::atomic<msglog> logmsg;
}

#endif
