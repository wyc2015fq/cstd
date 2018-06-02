#ifndef NANA_AUDIO_DETAIL_AUDIO_DEVICE_HPP
#define NANA_AUDIO_DETAIL_AUDIO_DEVICE_HPP

#include <nana/deploy.hpp>

#ifdef NANA_ENABLE_AUDIO

#include <nana/audio/detail/buffer_preparation.hpp>
#include <vector>
#if defined(NANA_WINDOWS)
	#include <windows.h>
#elif defined(NANA_LINUX)
	#include <alsa/asoundlib.h> 
#endif

namespace nana{	namespace audio
{
	namespace detail
	{
		class audio_device
		{
		public:
			audio_device();
			~audio_device();

			bool empty() const;
			bool open(size_t channels, size_t rate, size_t bits_per_sample);
			void close();
			void prepare(buffer_preparation & buf_prep);
			void write(buffer_preparation::meta * m);
			void wait_for_drain() const;
		private:
#if defined(NANA_WINDOWS)
			static void __stdcall _m_dev_callback(HWAVEOUT handle, UINT msg, audio_device * self, DWORD_PTR, DWORD_PTR);
#endif

	
#if defined(NANA_WINDOWS)
			HWAVEOUT handle_;
			recursive_mutex queue_lock_;
			vector<buffer_preparation::meta*> done_queue_;
#elif defined(NANA_LINUX)
			snd_pcm_t * handle_;
			size_t rate_;
			size_t channels_;
			size_t bytes_per_sample_;
			size_t bytes_per_frame_;
#endif
			buffer_preparation * buf_prep_;
		};

	}//end namespace detail
}//end namespace audio
}//end namespace nana

#endif	//NANA_ENABLE_AUDIO
#endif