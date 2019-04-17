# ffmpeg混音(将多个声音合成一个)命令 - DoubleLi - 博客园






ffmpeg命令中可以使用filter amix实现这个功能。

## 官方文档

[http://ffmpeg.org/ffmpeg-filters.html](http://ffmpeg.org/ffmpeg-filters.html)
6.8 amix 
Mixes multiple audio inputs into a single output.

Note that this filter only supports float samples (the amerge and pan audio filters support many formats). If the amix input has integer samples then aresample will be automatically inserted to perform the conversion to float samples.

For example

ffmpeg -i INPUT1 -i INPUT2 -i INPUT3 -filter_complex amix=inputs=3:duration=first:dropout_transition=3 OUTPUT 
will mix 3 input audio streams to a single output with the same duration as the first input and a dropout transition time of 3 seconds.

It accepts the following parameters:

inputs 
The number of inputs. If unspecified, it defaults to 2.

duration 
How to determine the end-of-stream.

longest 
The duration of the longest input. (default)

shortest 
The duration of the shortest input.

first 
The duration of the first input.

dropout_transition 
The transition time, in seconds, for volume renormalization when an input stream ends. The default value is 2 seconds.

## 例子

把当前电脑播放的声音混合到文件中的例子（”audio=”后面是音频设备名，ffmpeg -f dshow -list_devices 1 -i dummy可以在win下获取音频设备名)：

ffmpeg.exe -re -i 1234.mp4 -f dshow -i audio=”立体声混音 (Realtek High Definition ” -filter_complex amix=inputs=2:duration=first:dropout_transition=0 -t 10 out.mp4 -y









