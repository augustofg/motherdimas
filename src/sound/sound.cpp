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

	/*
	 * This method run in a diferent thread and is responsible for
	 * appling the fade out effect
	 */
	void SoundSample::play_sample_thread(int start_fade_out, int stop_fade_out, int replay)
	{
		if (this->sample == NULL)
			return;
		if (start_fade_out > stop_fade_out)
			return;
		if (start_fade_out < 1 || stop_fade_out < 1)
			return;

		std::unique_lock<std::mutex> lk(this->play_mutex);
		this->channel = Mix_PlayChannel(-1, this->sample, replay);

		std::cv_status wait_result;

		if (replay < 0)
		{
			halt_sound_var.wait(lk);
			wait_result = std::cv_status::no_timeout;
		}
		else
		{
			wait_result = halt_sound_var.wait_for(lk, start_fade_out*1ms);
		}

		if (wait_result == std::cv_status::timeout)
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

	/*
	 * Load a wav file to memory
	 */
	int SoundSample::load_media_file(const std::string &filename)
	{
		this->sample = Mix_LoadWAV(filename.c_str());
		if (this->sample == NULL)
		{
			return -1;
		}
		return 0;
	}

	/*
	 * Play an audio sample
	 */
	int SoundSample::play(int start_fade_out, int stop_fade_out, int replay)
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

		this->play_thread.reset(new std::thread([this, start_fade_out, stop_fade_out, replay] {this->play_sample_thread(start_fade_out, stop_fade_out, replay);}));

		return 0;
	}

	/*
	 * Stops the audio
	 */
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

	/*
	 * Clear resources, finishes the play thread
	 */
	SoundSample::~SoundSample()
	{
		this->stop();
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
