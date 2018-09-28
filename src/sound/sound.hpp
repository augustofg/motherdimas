/*
 * Description
 */

#ifndef SOUND_HPP_
#define SOUND_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <atomic>

namespace sound
{
	class SoundSample
	{
		Mix_Chunk *sample = NULL;
		int channel = -1;
		std::condition_variable halt_sound_var;
		std::unique_ptr<std::thread> play_thread;
		std::mutex play_mutex;
		void play_sample_thread(int start_fade_out, int stop_fade_out);
	public:
		SoundSample();
		int load_media_file(const std::string& filename);
		int play(int start_fade_out, int stop_fade_out);
		int stop();
		~SoundSample();
	};
}

#endif
