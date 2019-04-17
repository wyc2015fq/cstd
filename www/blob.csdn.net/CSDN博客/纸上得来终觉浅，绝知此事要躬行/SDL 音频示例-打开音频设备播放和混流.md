# SDL 音频示例-打开音频设备播放和混流 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月31日 16:17:05[boonya](https://me.csdn.net/boonya)阅读数：1019








SDL，即简单的DirectMedia Layer是一个跨平台开发库，旨在通过OpenGL和Direct3D提供对音频，键盘，鼠标，游戏杆和图形硬件的低级访问。 它被视频播放软件，模拟器和流行游戏使用，包括Valve的获奖产品目录和许多Humble Bundle游戏。
SDL正式支持Windows，Mac OS X，Linux，iOS和Android。 其他平台的支持可以在源代码中找到。
SDL是用C语言编写的，使用C ++进行本地工作，并且还有其他几种语言的绑定，包括C＃和Python。
SDL 2.0是根据zlib许可证分发的。 此许可证允许您在任何软件中免费使用SDL。


SDL官网地址：[https://www.libsdl.org/](https://www.libsdl.org/)

SDL2-CS 是C#版本的SDL工具库：[https://github.com/flibitijibibo/SDL2-CS](https://github.com/flibitijibibo/SDL2-CS)。

英文原文地址（SDL2.0.7）：[http://sdl.beuc.net/sdl.wiki/Audio_Examples](http://sdl.beuc.net/sdl.wiki/Audio_Examples)

英文原文地址（SDL1.2.15）：[https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideaudioexamples.html](https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideaudioexamples.html)

## 打开音频设备



```cpp
SDL_AudioSpec wanted;
    extern void fill_audio(void *udata, Uint8 *stream, int len);

    /* Set the audio format */
    wanted.freq = 22050;
    wanted.format = AUDIO_S16;
    wanted.channels = 2;    /* 1 = mono, 2 = stereo */
    wanted.samples = 1024;  /* Good low-latency value for callback */
    wanted.callback = fill_audio;
    wanted.userdata = NULL;

    /* Open the audio device, forcing the desired format */
    if ( SDL_OpenAudio(&wanted, NULL) < 0 ) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return(-1);
    }
    return(0);
```

## 播放音频



```cpp
static Uint8 *audio_chunk;
    static Uint32 audio_len;
    static Uint8 *audio_pos;

    /* The audio function callback takes the following parameters:
       stream:  A pointer to the audio buffer to be filled
       len:     The length (in bytes) of the audio buffer
    */
    void fill_audio(void *udata, Uint8 *stream, int len)
    {
        /* Only play if we have data left */
        if ( audio_len == 0 )
            return;

        /* Mix as much data as possible */
        len = ( len > audio_len ? audio_len : len );
        SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
        audio_pos += len;
        audio_len -= len;
    }

    /* Load the audio data ... */

    ;;;;;

    audio_pos = audio_chunk;

    /* Let the callback function play the audio chunk */
    SDL_PauseAudio(0);

    /* Do some processing */

    ;;;;;

    /* Wait for sound to complete */
    while ( audio_len > 0 ) {
        SDL_Delay(100);         /* Sleep 1/10 second */
    }
    SDL_CloseAudio();
```

## 播放流音频，混合2个（或更多）音频流


```cpp
#include <iostream>
#include <cmath>
#include "SDL/SDL.h"
#include "SDL/SDL_main.h"

/* linker options: -lmingw32 -lSDLmain -lSDL -mwindows */

using namespace std;

unsigned int sampleFrequency = 0;
unsigned int audioBufferSize = 0;
unsigned int outputAudioBufferSize = 0;

unsigned int freq1 = 1000;
unsigned int fase1 = 0;
unsigned int freq2 = 5000;
unsigned int fase2 = 0;

void example_mixaudio(void *unused, Uint8 *stream, int len) {

    unsigned int bytesPerPeriod1 = sampleFrequency / freq1;
    unsigned int bytesPerPeriod2 = sampleFrequency / freq2;

    for (int i=0;i<len;i++) {
        int channel1 = int(150*sin(fase1*6.28/bytesPerPeriod1));
        int channel2 = int(150*sin(fase2*6.28/bytesPerPeriod2));

        int outputValue = channel1 + channel2;           // just add the channels
        if (outputValue > 127) outputValue = 127;        // and clip the result
        if (outputValue < -128) outputValue = -128;      // this seems a crude method, but works very well

        stream[i] = outputValue;

        fase1++;
        fase1 %= bytesPerPeriod1;
        fase2++;
        fase2 %= bytesPerPeriod2;
    }
}

int main(int argc, char *argv[])
{

    if( SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO ) <0 ) {
        cout << "Unable to init SDL: " << SDL_GetError() << endl;
        return 1;
    }

    /* setup audio */
    SDL_AudioSpec *desired, *obtained;

    /* Allocate a desired SDL_AudioSpec */
    desired = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));

    /* Allocate space for the obtained SDL_AudioSpec */
    obtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));

    /* choose a samplerate and audio-format */
    desired->freq = 44100;
    desired->format = AUDIO_S8;

    /* Large audio buffers reduces risk of dropouts but increases response time.
     *
     * You should always check if you actually GOT the audiobuffer size you wanted,
     * note that not hardware supports all buffer sizes (< 2048 bytes gives problems with some
     * hardware). Older versions of SDL had a bug that caused many configuration to use a
     * buffersize of 11025 bytes, if your sdl.dll is approx. 1 Mb in stead of 220 Kb, download
     * v1.2.8 of SDL or better...)
     */
    desired->samples = 4096;

    /* Our callback function */
    desired->callback=example_mixaudio;
    desired->userdata=NULL;

    desired->channels = 1;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(desired, obtained) < 0 ) {
        fprintf(stderr, "AudioMixer, Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }

    audioBufferSize = obtained->samples;
    sampleFrequency = obtained->freq;

    /* if the format is 16 bit, two bytes are written for every sample */
    if (obtained->format==AUDIO_U16 || obtained->format==AUDIO_S16) {
        outputAudioBufferSize = 2*audioBufferSize;
    } else {
        outputAudioBufferSize = audioBufferSize;
    }

    SDL_Surface *screen = SDL_SetVideoMode(200,200, 16, SDL_SWSURFACE);
    SDL_WM_SetCaption("Audio Example",0);

    SDL_PauseAudio(0);

    bool running = true;

    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            /* GLOBAL KEYS / EVENTS */
            switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                default: break;
                }
                break;
            case SDL_QUIT:
                running = false;
                break;
            }
            SDL_Delay(1);
        }
        SDL_Delay(1);
    }
    SDL_Quit();
    return EXIT_SUCCESS;
}
```



## SDL 更多API参考

[FFMPEG + SDL音频播放分析](http://www.cnblogs.com/lidabo/p/3701074.html)


[https://www.libsdl.org/release/SDL-1.2.15/docs/html/index.html](https://www.libsdl.org/release/SDL-1.2.15/docs/html/index.html)





