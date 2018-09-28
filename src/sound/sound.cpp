#include "sound.hpp"
#include "../utils/msglog.hpp"

#include <sstream>

using namespace std::chrono_literals;
using namespace msglog;

namespace sound
{
	SoundSample::SoundSample()
	{

	}

	void SoundSample::play_sample_thread(int start_fade_out, int stop_fade_out)
	{
		if (this->sample == NULL)
			return;
		if (start_fade_out > stop_fade_out)
			return;
		if (start_fade_out < 1 || stop_fade_out < 1)
			return;

		std::unique_lock<std::mutex> lk(this->play_mutex);
		this->channel = Mix_PlayChannel(-1, this->sample, 0);

		if (halt_sound_var.wait_for(lk, start_fade_out*1ms) == std::cv_status::timeout)
		{
			std::stringstream dbgmsg;
			dbgmsg << "(0x" << this << ") Sound timeout (fade out)";
			logmsg.debug_msg(dbgmsg.str(), 1);
			Mix_FadeOutChannel(this->channel, stop_fade_out - start_fade_out);
		}
		else
		{
			std::stringstream dbgmsg;
			dbgmsg << "(0x" << this << ") Sound halted by an external event";
			logmsg.debug_msg(dbgmsg.str(), 1);
			return;
		}

		if (halt_sound_var.wait_for(lk, (stop_fade_out - start_fade_out)*1ms) == std::cv_status::timeout)
		{
			Mix_HaltChannel(this->channel);
		}
	}

	int SoundSample::load_media_file(const std::string &filename)
	{
		this->sample = Mix_LoadWAV(filename.c_str());
		if (this->sample == NULL)
		{
			return -1;
		}
		return 0;
	}

	int SoundSample::play(int start_fade_out, int stop_fade_out)
	{
		if (this->sample == NULL)
		{
			return -1;
		}

		if (this->play_thread.get() != nullptr)
		{
			this->halt_sound_var.notify_all();
			this->play_thread->join();
		}

		this->play_thread.reset(new std::thread([this, start_fade_out, stop_fade_out] {this->play_sample_thread(start_fade_out, stop_fade_out);}));

		return 0;
	}

	int SoundSample::stop()
	{
		if (this->sample == NULL)
		{
			return -1;
		}
		this->halt_sound_var.notify_all();
		Mix_HaltChannel(this->channel);
		return 0;
	}

	SoundSample::~SoundSample()
	{
		if (this->play_thread.get() != nullptr)
		{
			this->play_thread->join();
		}

		if (this->sample != NULL)
		{
			Mix_FreeChunk(this->sample);
		}
	}
}
