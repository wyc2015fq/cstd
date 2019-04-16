# FFMPEG列出DirectShow支持的设备 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月22日 13:13:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：40









FFMPEG列出dshow支持的设备：


` ffmpeg -list_devices true -f dshow -idummy`


举例：

采集摄像头和麦克风


`ffmpeg -f dshow -i video="Camera":audio="Microphone"`


（摄像头名称是Camera，麦克风名称是Microphone）



比如说，在我的笔记本上采集10秒的视频，并且使用x264进行编码，可以用如下命令：


`ffmpeg -f dshow -i video="Integrated Camera" -t 10 -vcodec libx264 test.mp4`
=================================================================



以下是帮助信息：



http://ffmpeg.org/ffprobe.html



11.3 dshow

Windows DirectShow input device.

DirectShow support is enabled when FFmpegis built with mingw-w64. Currently only audio and video devices are supported.

Multiple devices may be opened as separateinputs, but they may also be opened on the same input, which should improvesynchronism between them.

The input name should be in the format:

TYPE=NAME[:TYPE=NAME]

where TYPE can be either audio or video,and NAME is the device’s name.



11.3.1 Options

If no options are specified, the device’sdefaults are used. If the device does not support the requested options, itwill fail to open.

‘video_size’

Set the video size in the captured video.

‘framerate’

Set the framerate in the captured video.

‘sample_rate’

Set the sample rate (in Hz) of the capturedaudio.

‘sample_size’

Set the sample size (in bits) of thecaptured audio.

‘channels’

Set the number of channels in the capturedaudio.

‘list_devices’

If set to ‘true’, print a list of devicesand exit.

‘list_options’

If set to ‘true’, print a list of selecteddevice’s options and exit.

‘video_device_number’

Set video device number for devices withsame name (starts at 0, defaults to 0).

‘audio_device_number’

Set audio device number for devices withsame name (starts at 0, defaults to 0).



11.3.2 Examples

Print the list of DirectShow supporteddevices and exit:

$ ffmpeg -list_devices true -f dshow -idummy

Open video device Camera:

$ ffmpeg -f dshow -ivideo="Camera"

Open second video device with name Camera:

$ ffmpeg -f dshow -video_device_number 1 -ivideo="Camera"

Open video device Camera and audio deviceMicrophone:

$ ffmpeg -f dshow -ivideo="Camera":audio="Microphone"

Print the list of supported options inselected device and exit:

$ ffmpeg -list_options true -f dshow -ivideo="Camera"



原文地址：[http://blog.sina.com.cn/s/blog_3cb6a78c0100zti7.html](http://blog.sina.com.cn/s/blog_3cb6a78c0100zti7.html)




