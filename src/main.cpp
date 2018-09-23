#include <iostream>
#include <string>

#include "docopt.h"
#include "utils/msglog.hpp"

msglog::msglog logmsg;

static const char _USAGE[] =
    R"(MotherDimas.
Usage:
  __PROGNAME__ [--verbose]
  __PROGNAME__ (-h | --help)
  __PROGNAME__ --version
Options:
  -h --help           Show this screen.
  --version           Show version.
  --verbose           Verbose output for debugging
)";

void search_and_replace(std::string& str, const std::string& old_str, const std::string& new_str)
{
	std::string::size_type index = 0;
	std::string::size_type old_str_len = old_str.length();
	std::string::size_type new_str_len = new_str.length();
	while ((index = str.find(old_str, index)) != std::string::npos)
	{
		str.replace(index, old_str_len, new_str);
		index += new_str_len;
	}
}

int main(int argc, char *argv[])
{
	std::string USAGE = _USAGE;
	search_and_replace(USAGE, "__PROGNAME__", argv[0]);

	std::map<std::string, docopt::value> args
		= docopt::docopt(USAGE,
						 { argv + 1, argv + argc },
						 true,               // show help if requested
						 "MotherDimas V0.1"); // version string

	if (args["--verbose"].asBool())
	{
		logmsg.set_debug_level(0);
		logmsg.debug_msg("Verbose mode", 0);
	}
	std::cout << "Hello!" << std::endl;
    return 0;
}
