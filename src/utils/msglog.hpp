/*
 * Description
 */

#ifndef MSGLOG_HPP_
#define MSGLOG_HPP_

#include <atomic>
#include <mutex>
#include <string>

namespace msglog
{
	class msglog
	{
        std::atomic<std::int32_t> dbg_level;
		std::mutex log_mutex;
	public:
		msglog();
		void set_debug_level(int level);
		void debug_msg(const std::string& msg, int level);
		void debug_msg(const char* msg, int level);
		void warning_msg(const std::string& msg);
		void warning_msg(const char* msg);
		void error_msg(const std::string& msg);
		void error_msg(const char* msg);
	};

}
extern msglog::msglog logmsg;

#endif
