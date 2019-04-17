# wave和pyaudio读取wav语音文件并播放 - 墨流觞的博客 - CSDN博客





2018年10月30日 12:40:43[墨氲](https://me.csdn.net/dss_dssssd)阅读数：278








需要模块：wave , pyAudio

程序如下：

```python
import pyaudio
import wave


CHUNK = 1024
# 从目录中读取语音
wf = wave.open('./2.wav', 'rb')
# read data
data = wf.readframes(CHUNK)
# 创建播放器
p = pyaudio.PyAudio()

# 获得语音文件的各个参数
FORMAT = p.get_format_from_width(wf.getsampwidth())
CHANNELS = wf.getnchannels()
RATE = wf.getframerate()

print('FORMAT: {} \nCHANNELS: {} \nRATE: {}'.format(FORMAT, CHANNELS, RATE))
# 打开音频流， output=True表示音频输出
stream = p.open(format=FORMAT,

                channels=CHANNELS,
                rate=RATE,
                frames_per_buffer=CHUNK,
                output=True)
# play stream (3) 按照1024的块读取音频数据到音频流，并播放
while len(data) > 0:
    stream.write(data)
    data = wf.readframes(CHUNK)
```



