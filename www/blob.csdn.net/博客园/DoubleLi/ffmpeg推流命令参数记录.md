# ffmpeg推流命令参数记录 - DoubleLi - 博客园






列出我们本机的设备：ffmpeg -list_devices true -f dshow -i dummy



.\ffmpeg -r 25 -f dshow -s 640*480 -i video="WebCam SC-10HDD12636N":audio="麦克风 (Realtek High Definition Au" -vcodec libx264 -b:v 600k -preset:v ultrafast -acodec aac -ab 128k -f flv rtmp://192.168.0.143/live/stream


.\ffmpeg -f dshow -s 640*480 -i video="WebCam SC-10HDD12636N":audio="麦克风 (Realtek High Definition Au" -vcodec libx264  -acodec aac -preset:v ultrafast -tune:v zerolatency -f flv rtmp://192.168.0.143/live/stream 

ffmpeg -f dshow -i video="WebCam SC-10HDD12636N":audio="麦克风 (Realtek High Definition Au" -vcodec libx264 -acodec aac -preset:v ultrafast -tune:v zerolatency -f rtsp rtsp://127.0.0.1/live.sdp

ffmpeg -f dshow -i video="WebCam SC-10HDD12636N" -s 640x360 -vcodec libx264 -b:v 1000k   -ab 128k -f flv rtmp://192.168.0.143/live/stream 









