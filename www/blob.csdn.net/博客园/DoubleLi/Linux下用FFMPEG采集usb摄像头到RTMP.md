# Linux下用FFMPEG采集usb摄像头到RTMP - DoubleLi - 博客园






Linux下用 FFMPEG 采集 usb摄像头视频 和 摄像头内置麦克风音频 到RTMP服务



ffmpeg -f video4linux2 -qscale 10 -r 12 -s 640x480 -i /dev/video0 -f alsa -i hw:1 -ab 16 -ar 22050 -ac 1 -f mp3 -f flv rtmp://127.0.0.1/rtmpsvr/rtmp1









