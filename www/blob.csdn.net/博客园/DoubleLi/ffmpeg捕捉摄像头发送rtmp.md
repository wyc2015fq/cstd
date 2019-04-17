# ffmpeg捕捉摄像头发送rtmp - DoubleLi - 博客园






打印 DirectShow 支持的设备列表（true 可用1替换）：
ffmpeg -list_devices true -f dshow -i dummy

本计算机打印出的信息如下：
[dshow @ 0000000002f282c0] DirectShow video devices (some may be both video and audio devices)
[dshow @ 0000000002f282c0] "Integrated Webcam"
[dshow @ 0000000002f282c0] Alternative name "@device_pnp_\\?\usb#vid_1bcf&pid_2c0e&mi_00#7&2d59f11c&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\global"
[dshow @ 0000000002f282c0] DirectShow audio devices
[dshow @ 0000000002f282c0] "Internal Microphone Array (IDT High Definition Audio CODEC)"
[dshow @ 0000000002f282c0] Alternative name "@device_cm_{33D9A762-90C8-11D0-BD43-00A0C911CE86}\wave_{0384A54D-E450-4FE5-909B-075A45EC97C1}"
[dshow @ 0000000002f282c0] "Stereo Mix (IDT High Definition Audio CODEC)"
[dshow @ 0000000002f282c0] Alternative name "@device_cm_{33D9A762-90C8-11D0-BD43-00A0C911CE86}\wave_{8297745C-A133-46C5-A7BD-1056E6064A3E}"
打开电脑摄像头和麦克风：
ffplay -f dshow -i video="HP Integrated Webcam":audio="Internal Microphone Array (IDT High Definition Audio CODEC)"

录一段视频，按 q 键停止：
ffmpeg -f dshow -i video="HP Truevision HD":audio="Internal Microphone Array (IDT High Definition Audio CODEC)" -s 640x360 -b:v 1000k -b:a 128k output.mkv打开摄像头生成rtmp流：ffmpeg -f dshow -i video="Integrated Webcam" -s 640x360 -vcodec libx264 -b:v 1000k   -ab 128k -f flv rtmp://192.168.31.102:1935/myapp/test1
推送本地视频文件，视音频的编码格式只能为H264、AACffmpeg -re -i "E:\片源\葫芦娃720p.mov" -vcodec copy -acodec copy -f flv rtmp://192.168.31.102:1935/myapp/test1










