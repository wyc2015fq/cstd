# PCM音频设备的操作(转) - h13 - 博客园
 对音频设备的操作主要是初始化音频设备以及往音频设备发送 PCM（Pulse Code Modulation）数据。为了方便，本文使用 ALSA（Advanced Linux Sound Architecture）提供的库和驱动。在编译和运行本文中的 MP3 流媒体播放器的时候，必须先安装 ALSA 相关的文件。
 本文用到的主要对 PCM 设备操作的函数分为 PCM 设备初始化的函数以及 PCM 接口的一些操作函数。
PCM 硬件设备参数设置和初始化的函数有：
- int  snd_pcm_hw_params_malloc (snd_pcm_hw_params_t **ptr)  
- int  snd_pcm_hw_params_any (snd_pcm_t *pcm, snd_pcm_hw_params_t *params)  
- void snd_pcm_hw_params_free (snd_pcm_hw_params_t *obj)  
- int  snd_pcm_hw_params_set_access ( snd_pcm_t *pcm,   
-                                     snd_pcm_hw_params_t *params,   
-                                     snd_pcm_access_t _access)  
- int  snd_pcm_hw_params_set_format ( snd_pcm_t *pcm,   
-                                     snd_pcm_hw_params_t *params,   
-                                     snd_pcm_format_t val)  
- int  snd_pcm_hw_params_set_channels(snd_pcm_t *pcm,   
-                                     snd_pcm_hw_params_t *params,   
-                                     unsigned int val)  
- int snd_pcm_hw_params_set_rate_near(snd_pcm_t *pcm,   
-                                     snd_pcm_hw_params_t *params,   
-                                     unsigned int *val, int *dir) 
PCM 接口函数有：
- int   snd_pcm_hw_params (snd_pcm_t *pcm, snd_pcm_hw_params_t *params)  
- int   snd_pcm_prepare (snd_pcm_t *pcm)  
- int   snd_pcm_open (snd_pcm_t **pcm, constchar *name,   
-                     snd_pcm_stream_t stream, int mode)  
- int   snd_pcm_close (snd_pcm_t *pcm)  
- snd_pcm_sframes_t   snd_pcm_writei (snd_pcm_t *pcm,   
- constvoid *buffer, snd_pcm_uframes_t size) 
这些函数用到了 snd_pcm_hw_params_t 结构，此结构包含用来播放 PCM 数据流的硬件信息配置。在往音频设备（声卡）写入音频数据之前，必须设置访问类型、采样格式、采样率、声道数等。
首先使用 snd_pcm_open () 打开 PCM 设备，在 ALSA 中，PCM 设备都有名字与之对应。比如我们可以定义 PCM 设备名字为 char *pcm_name = "plughw:0,0"。 最重要的 PCM 设备接口是“plughw”以及“hw”接口。 使用“plughw”接口，程序员不必过多关心硬件，而且如果设置的配置参数和实际硬件支持的参数不一致，ALSA 会自动转换数据。如果使用“hw”接口，我们就必须检测硬件是否支持设置的参数了。Plughw 后面的两个数字分别表示设备号和次设备（subdevice）号。
snd_pcm_hw_params_malloc( ) 在栈中分配 snd_pcm_hw_params_t 结构的空间，然后使用 snd_pcm_hw_params_any( ) 函数用声卡的全配置空间参数初始化已经分配的 snd_pcm_hw_params_t 结构。snd_pcm_hw_params_set_access ( ) 设置访问类型，常用访问类型的宏定义有：
- SND_PCM_ACCESS_RW_INTERLEAVED 
交错访问。在缓冲区的每个 PCM 帧都包含所有设置的声道的连续的采样数据。比如声卡要播放采样长度是 16-bit 的 PCM 立体声数据，表示每个 PCM 帧中有 16-bit 的左声道数据，然后是 16-bit 右声道数据。
- SND_PCM_ACCESS_RW_NONINTERLEAVED 
非交错访问。每个 PCM 帧只是一个声道需要的数据，如果使用多个声道，那么第一帧是第一个声道的数据，第二帧是第二个声道的数据，依此类推。
函数 snd_pcm_hw_params_set_format() 设置数据格式，主要控制输入的音频数据的类型、无符号还是有符号、是 little-endian 还是 bit-endian。比如对于 16-bit 长度的采样数据可以设置为：
- SND_PCM_FORMAT_S16_LE      有符号16 bit Little Endian   
- SND_PCM_FORMAT_S16_BE      有符号16 bit Big Endian   
- SND_PCM_FORMAT_U16_LE      无符号16 bit Little Endian   
- SND_PCM_FORMAT_U16_BE      无符号 16 bit Big Endian  
- 比如对于 32-bit 长度的采样数据可以设置为：  
- SND_PCM_FORMAT_S32_LE      有符号32 bit Little Endian   
- SND_PCM_FORMAT_S32_BE      有符号32 bit Big Endian   
- SND_PCM_FORMAT_U32_LE      无符号32 bit Little Endian   
- SND_PCM_FORMAT_U32_BE      无符号 32 bit Big Endian 
函数 snd_pcm_hw_params_set_channels() 设置音频设备的声道，常见的就是单声道和立体声，如果是立体声，设置最后一个参数为2。snd_pcm_hw_params_set_rate_near () 函数设置音频数据的最接近目标的采样率。snd_pcm_hw_params( ) 从设备配置空间选择一个配置，让函数 snd_pcm_prepare() 准备好 PCM 设备，以便写入 PCM 数据。snd_pcm_writei() 用来把交错的音频数据写入到音频设备。
初始化 PCM 设备的例程如下：
**初始化 PCM 设备的例程**
- /* open a PCM device */
- int open_device(struct mad_header const *header)  
- {  
- int err;  
-    snd_pcm_hw_params_t *hw_params;  
- char  *pcm_name = "plughw:0,0";  
- int rate = header->samplerate;  
- int channels = 2;  
- 
- if (header->mode == 0) {  
-       channels = 1;  
-    } else {  
-       channels = 2;  
-    }  
- 
- if ((err = snd_pcm_open (&playback_handle,   
-                             pcm_name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {  
-       printf("cannot open audio device %s (%s)\n",  
-       pcm_name,  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
- if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {  
-       printf("cannot allocate hardware parameter structure (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
- if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {  
-       printf("cannot initialize hardware parameter structure (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
- 
- if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params,   
-               SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {  
-       printf("cannot set access type (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
- 
- if ((err = snd_pcm_hw_params_set_format (playback_handle,   
-               hw_params, SND_PCM_FORMAT_S32_LE)) < 0) {  
-       printf("cannot set sample format (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- if ((err = snd_pcm_hw_params_set_rate_near (playback_handle,   
-               hw_params, &rate, 0)) < 0) {  
-       printf("cannot set sample rate (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
- if ((err = snd_pcm_hw_params_set_channels (playback_handle,   
-               hw_params, channels)) < 0) {  
-       printf("cannot set channel count (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
- if ((err = snd_pcm_hw_params (playback_handle,   
-               hw_params)) < 0) {  
-       printf("cannot set parameters (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
-    snd_pcm_hw_params_free (hw_params);  
- if ((err = snd_pcm_prepare (playback_handle)) < 0) {  
-       printf("cannot prepare audio interface for use (%s)\n",  
-       snd_strerror (err));  
- return -1;  
-    }  
- 
- return 0;  
- }  
这里配置的 PCM 格式是`SND_PCM_FORMAT_S32_LE`，采样的格式是每个采样有 32-bit 的数据，数据按照`little-endian`存放。如果通过`mad_frame_decode()`函数得到 PCM 数据后，要求每个采样数据只占 16-bit，需要把数据进行MAD的定点类型到`signed short`类型进行转换。那么，PCM 数据如何写入声卡中呢？函数实现例程如下所示：
**PCM 数据写入声卡函数实现例程**
- while (nsamples--) {  
- /* nsamples 是采样的数目 */
- signedint sample;  
- 
-        sample = pcm->samples[0][j];  
-        *(OutputPtr++) = sample & 0xff;  
-        *(OutputPtr++) = (sample >> 8);  
-        *(OutputPtr++) = (sample >> 16);  
-        *(OutputPtr++) = (sample >> 24);  
- 
- if (nchannels == 2) {  
-           sample = pcm->samples[1][j];  
-           *(OutputPtr++) = sample  & 0xff;  
-           *(OutputPtr++) = sample >> 8;  
-           *(OutputPtr++) = (sample >> 16);  
-           *(OutputPtr++) = (sample >> 24);  
- 
-        }  
-        j++;  
- 
-    }  
- if ((err = snd_pcm_writei (playback_handle, buf, samples)) < 0) {  
-       err = xrun_recovery(playback_handle, err);  
- if (err < 0) {  
-          printf("Write error: %s\n", snd_strerror(err));  
- return -1;  
-       }  
-    }  
这里用到了[http://www.alsa-project.org/](http://www.alsa-project.org/)关于 ALSA 文档中的例子函数`xrun_recovery( )`。详细例子请参见[http://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_8c-example.html](http://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_8c-example.html)。使用此函数的目的是避免出现由于网络原因，声卡不能及时得到音频数据而使得`snd_pcm_writei()`不能正常连续工作。实际上在`xrun_recovery( )`中，又调用`snd_pcm_prepare()`和`snd_pcm_resume()`以实现能“恢复错误”的功能。`-EPIPE`错误表示应用程序没有及时把 PCM 采样数据送入ASLA 库。`xrun_recovery()`函数如下所示：
**`xrun_recovery()`函数**
- int xrun_recovery(snd_pcm_t *handle, int err)  
- {  
- if (err == -EPIPE) {    /* under-run */
-       err = snd_pcm_prepare(handle);  
- 
- if (err < 0)  
-       printf("Can't recovery from underrun, prepare failed: %s\n",  
-          snd_strerror(err));  
- return 0;  
-    } elseif (err == -ESTRPIPE) {  
- while ((err = snd_pcm_resume(handle)) == -EAGAIN)  
-          sleep(1);       /* wait until the suspend flag is released */
- if (err < 0) {  
-             err = snd_pcm_prepare(handle);  
- if (err < 0)  
-             printf("Can't recovery from suspend, prepare failed: %s\n",  
-               snd_strerror(err));  
-       }  
- return 0;  
-    }  
- return err;  
- }  
知道了具体的音频设备操作方法，就该使用 MAD 提供的函数具体实现解码了。函数`mp3_decode_buf( )`提供了使用 libmad 解码的方法。首先调用`mad_stream_buffer()`函数把 MP3 流数据和`decode_stream`关联，然后开始循环解码数据。如果在解码数据过程中，有不完整 PCM 数据帧，那么`decode_stream.error`的值就是`MAD_ERROR_BUFLEN`，且`decode_stream.next_frame`不为`NULL`。这时候，把剩余的未解码的数据再拷贝到数据解码缓冲区里。`mad_frame_decode( )`函数从`decode_stream`中得到 PCM 数据。
**mad_frame_decode( )函数从`decode_stream`中得到 PCM 数据**
- int mp3_decode_buf(char *input_buf, int size)  
- {  
- int decode_over_flag = 0;  
- int remain_bytes = 0;  
- int ret_val = 0;  
-   mad_stream_buffer(&decode_stream, input_buf, size);  
-   decode_stream.error = MAD_ERROR_NONE;  
- while (1)  
-   {  
- if (decode_stream.error == MAD_ERROR_BUFLEN) {  
- if (decode_stream.next_frame != NULL) {  
-            remain_bytes = decode_stream.bufend - decode_stream.next_frame;  
-            memcpy(input_buf, decode_stream.next_frame, remain_bytes);  
- return remain_bytes;  
-         }  
-       }  
-       ret_val = mad_frame_decode(&decode_frame, &decode_stream);  
- /* 省略部分代码 */
-      ...  
- if (ret_val == 0) {  
- if (play_frame(&decode_frame) == -1) {  
- return -1;  
-          }  
-       }  
- /* 后面代码省略 */
-       ...  
-    }  
- 
- return 0;  
- }  
recommend from :[http://www.ibm.com/developerworks/cn/linux/l-cn-libmadmp3player/index.html](http://www.ibm.com/developerworks/cn/linux/l-cn-libmadmp3player/index.html)
本文出自 “[驿落黄昏](http://yiluohuanghun.blog.51cto.com/)” 博客，请务必保留此出处[http://yiluohuanghun.blog.51cto.com/3407300/868048](http://yiluohuanghun.blog.51cto.com/3407300/868048)
