# [SimplePlayer] 5. 向音频设备输出音频 - TaigaComplex求职中 - 博客园







# [[SimplePlayer] 5. 向音频设备输出音频](https://www.cnblogs.com/TaigaCon/p/9906737.html)





# 两种SDL音频输出方式

我们这里采用SDL来进行音频输出。SDL提供两种音频输出的方式：
- 如果在[SDL_OpenAudio](https://wiki.libsdl.org/SDL_OpenAudio)时不指定callback，那么可以调用[SDL_QueueAudio](https://wiki.libsdl.org/SDL_QueueAudio)主动地向音频输出设备输送音频。这种方式的实现中，SDL维护一个音频队列，并从该队列中提取音频数据输出到音频设备，用户只需要源源不断地向队列添加音频即可。如果用户无法提供所需的音频，那么音频设备会输出静音。
- 如果在SDL_OpenAudio时指定了callback，那么就是通过该callback函数向音频输出设备输送音频。这种方式的实现中，SDL会维护一个固定大小的缓冲区，并创建一个轮询调用callback函数的线程（SDL_RunAudio），该实现要求callback函数每次向该缓冲区输送固定长度的音频数据，然后将该缓冲区的音频数据输出到音频设备。如果用户无法提供所需数据，则应该通过callback提供静音数据（填充0）。



我们这里采用的是SDL传统的音频输出方式，即指定callback函数。





# 初始化

首先需要初始化SDL的音频子系统
    SDL_Init(SDL_INIT_AUDIO);


设置音频输出的采样率、样本格式、声道、一帧的有多少个样本（每次输出一帧）、以及回调函数。
    SDL_AudioSpec wanted, obtained;

    memset(&wanted, 0, sizeof(wanted));
    wanted.freq = DEF_SAMPLERATE;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = DEF_CHANNELS;
    wanted.samples = DEF_SAMPLES;
    wanted.silence = 0;
    wanted.callback = SimpleCallback;

    
    if(0>SDL_OpenAudio(&wanted, &obtained)){
        fprintf(stderr, "SDL Open Audio failed, reason:%s\n", SDL_GetError());
        return -1;
    }
    fprintf(stdout, "wanted freq:%d, format:%d, channels:%d, samples:%d\n", wanted.freq, wanted.format, wanted.channels, wanted.samples);
    fprintf(stdout, "obtained freq:%d, format:%d, channels:%d, samples:%d\n", obtained.freq, obtained.format, obtained.channels, obtained.samples);
不过由于系统（声卡）的限制，系统所能接受的参数不一定就是我们设置的参数。





# 音频输出

如前面所述，我们需要在callback函数中填充一块固定大小的buffer，该buffer的大小为SDL_OpenAudio时设定的音频帧大小，buffer中的音频数据会被输出到音频输出设备。

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181105000132538-333390281.png)
void SimpleCallback(void* userdata, Uint8 *stream, int queryLen){
    unsigned char *buf, *itr;
    int readsize = 0, len, bufsize, lengthOfRead;

    len = queryLen;
    buf = (unsigned char *)malloc(len);
    memset(buf, 0, len);
    
    itr = buf;
    while(len > 0){
        readsize = fread(itr, 1, len, pFile);
        if(!readsize)
            len = 0;
        len = len - readsize;
        itr = itr + readsize;
    }

    memcpy(stream, buf, queryLen);
    free(buf);
}











