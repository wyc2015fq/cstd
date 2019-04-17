# SDL适合多窗口播放音频打开方式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月19日 15:10:16[boonya](https://me.csdn.net/boonya)阅读数：655








在实现OCX播放控件中要求可以多个窗口播放，注意这里的需求不是分屏。

##### 官方例子

参考：[SDL_OpenAudioDevice](https://wiki.libsdl.org/SDL_OpenAudioDevice?highlight=%28%5CbCategoryAPI%5Cb%29%7C%28SDLFunctionTemplate%29)

```
SDL_AudioSpec want, have;
SDL_AudioDeviceID dev;

SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
want.freq = 48000;
want.format = AUDIO_F32;
want.channels = 2;
want.samples = 4096;
want.callback = MyAudioCallback; /* you wrote this function elsewhere -- see SDL_AudioSpec for details */

dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
if (dev == 0) {
    SDL_Log("Failed to open audio: %s", SDL_GetError());
} else {
    if (have.format != want.format) { /* we let this one thing change. */
        SDL_Log("We didn't get Float32 audio format.");
    }
    SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
    SDL_Delay(5000); /* let the audio callback play some sound for 5 seconds. */
    SDL_CloseAudioDevice(dev);
}
```

音频播放工具类

```
public unsafe class SDLAudio
    {
        // 设备编号索引
        uint devIndex;

        class aa
        {
            public byte[] pcm;
            public int len;
        }
        int lastIndex = 0;

        private List<aa> data = new List<aa>();

        //private List<byte> data = new List<byte>();
        SDL.SDL_AudioCallback Callback;
        public void PlayAudio(IntPtr pcm, int len)
        {
            lock (this)
            {
                byte[] bts = new byte[len];
                Marshal.Copy(pcm, bts, 0, len);
                data.Add(new aa
                {
                    len = len,
                    pcm = bts
                });
            }

            //SDL.SDL_Delay(10);
        }
        void SDL_AudioCallback(IntPtr userdata, IntPtr stream, int len)
        {
            if (data.Count == 0)
            {
                for (int i = 0; i < len; i++)
                {
                    ((byte*)stream)[i] = 0;
                }
                return;
            }
            for (int i = 0; i < len; i++)
            {
                if (data[0].len > i)
                {
                    ((byte*)stream)[i] = data[0].pcm[i];
                }
                else
                    ((byte*)stream)[i] = 0;
            }
            data.RemoveAt(0);


           
        }
        public int SDL_Init()
        {
            Callback = SDL_AudioCallback;


            SDL.SDL_AudioSpec wanted_spec = new SDL.SDL_AudioSpec();
            wanted_spec.freq = 8000;
            wanted_spec.format = SDL.AUDIO_S16;
            wanted_spec.channels = 1;
            wanted_spec.silence = 0;
            wanted_spec.samples = 320;
            wanted_spec.callback = Callback;

            SDL.SDL_AudioSpec out_spec= wanted_spec;

            // 打开当前索引设备: 传入NULL的设备名称会请求最合理的默认值
            devIndex = SDL.SDL_OpenAudioDevice(null, 0, ref wanted_spec, out out_spec, (int)SDL.SDL_AUDIO_ALLOW_FORMAT_CHANGE);
            if (devIndex < 0)
            {
                throw new ApplicationException("Can't open audio device." );
            }
            //Play  
            SDL.SDL_PauseAudioDevice(devIndex, 0);
            return 0;
        }

        /// <summary>
        ///  暂停
        /// </summary>
        public void PauseAudio()
        {
            SDL.SDL_PauseAudioDevice(devIndex,1);// 暂停声音
        }

        /// <summary>
        /// 播放
        /// </summary>
        public void PlayAudio()
        {
            SDL.SDL_PauseAudioDevice(devIndex,0);//播放声音
        }

        /// <summary>
        /// 关闭设备
        /// </summary>
        public void CloseAudioDevice()
        {
            SDL.SDL_CloseAudioDevice(devIndex);// 关闭播放设备
        }
    }
```







