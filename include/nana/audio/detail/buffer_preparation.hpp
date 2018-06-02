#ifndef NANA_AUDIO_DETAIL_BUFFER_PREPARATION_HPP
#define NANA_AUDIO_DETAIL_BUFFER_PREPARATION_HPP
#include <nana/deploy.hpp>

#ifdef NANA_ENABLE_AUDIO

#include <nana/audio/detail/audio_stream.hpp>

#if defined(STD_THREAD_NOT_SUPPORTED)
    #include <nana/std_thread.hpp>
    #include <nana/std_mutex.hpp>
    #include <nana/std_condition_variable.hpp>
#else
    #include <mutex>
    #include <condition_variable>
    #include <thread>
#endif

#include <sstream>
#include <vector>

#if defined(NANA_WINDOWS)
	#include <windows.h>
#endif

namespace nana{	namespace audio
{
	namespace detail
	{
		class buffer_preparation
		{
		public:
#if defined(NANA_WINDOWS)
			typedef WAVEHDR meta;
#elif defined(NANA_LINUX)
			struct meta
			{
				char * buf;
				size_t bufsize;
			};
#endif

		public:
			buffer_preparation(audio_stream& as, size_t seconds);

			~buffer_preparation();

			meta * read();
			//Revert the meta that returned by read()
			void revert(meta * m);
			bool data_finished() const;
		private:
			void _m_prepare_routine();
		private:
			volatile bool running_;
			volatile bool wait_for_buffer_;
			thread thr_;
			mutable mutex token_buffer_, token_prepared_;
			mutable condition_variable	cond_buffer_, cond_prepared_;

			vector<meta*> buffer_, prepared_;
			size_t block_size_;
			audio_stream & as_;
		};
	}//end namespace detail
}//end namespace audio
}//end namespace nana
#endif	//NANA_ENABLE_AUDIO
#endif