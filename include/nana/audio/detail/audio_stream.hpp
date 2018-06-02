#ifndef NANA_AUDIO_DETAIL_AUDIO_STREAM_HPP
#define NANA_AUDIO_DETAIL_AUDIO_STREAM_HPP

#include <nana/deploy.hpp>

#ifdef NANA_ENABLE_AUDIO

#include <fstream>

namespace nana{	namespace audio{
	namespace detail
	{
		namespace wave_spec
		{
		#if defined(NANA_WINDOWS)
			#pragma pack(1)
				struct master_riff_chunk
				{
					unsigned	ckID;	//"RIFF"
					unsigned	cksize;
					unsigned	waveID;	//"WAVE"
				};

				struct format_chunck
				{
					unsigned		ckID;	//"fmt "
					unsigned		cksize;
					unsigned short	wFormatTag;
					unsigned short	nChannels;
					unsigned		nSamplePerSec;
					unsigned		nAvgBytesPerSec;
					unsigned short	nBlockAlign;
					unsigned short	wBitsPerSample;
				};
			#pragma pack()
		#elif defined(NANA_LINUX)
			struct master_riff_chunk
			{
				unsigned	ckID;	//"RIFF"
				unsigned	cksize;
				unsigned	waveID;	//"WAVE"
			}__attribute__((packed));

			struct format_chunck
			{
				unsigned		ckID;	//"fmt "
				unsigned		cksize;
				unsigned short	wFormatTag;
				unsigned short	nChannels;
				unsigned		nSamplePerSec;
				unsigned		nAvgBytesPerSec;
				unsigned short	nBlockAlign;
				unsigned short	wBitsPerSample;
			}__attribute__((packed));
		#endif
		}

		class audio_stream
		{
			struct chunck
			{
				unsigned ckID;
				unsigned cksize;
			};
		public:
			bool open(const char* file);
			void close();
			bool empty() const;
			const wave_spec::format_chunck & format() const;
			size_t data_length() const;
			void locate();
			size_t read(void * buf, size_t len);
		private:
			size_t _m_locate_chunck(unsigned ckID);
		private:
			ifstream fs_;
			wave_spec::format_chunck ck_format_;
			size_t pcm_data_pos_;
			size_t pcm_data_size_;
			size_t data_size_;
		}; //end class audio_stream
	}
}//end namespace audio
}//end namespace nana
#endif	//NANA_ENABLE_AUDIO
#endif