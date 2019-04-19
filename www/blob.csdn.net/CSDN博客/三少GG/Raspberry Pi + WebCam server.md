# Raspberry Pi + WebCam server - 三少GG - CSDN博客
2013年06月12日 20:28:25[三少GG](https://me.csdn.net/scut1135)阅读数：2184
共有四种解决方案:
**1.  fswebcam**
可以定时截图。
**[Webcam capture using fswebcam](http://www.r3uk.com/index.php/home/38-software/100-webcam-capture-using-fswebcam)**
**There are two video capture API's for Linux - Video4Linux (V4L) and Video4Linux2 (V4L2). As you may have guessed from the name, V4L2 is the second version of V4L.Why do you care?? Well, because it can be terribly confusing if you want to set up a simple webcam in Linux. Some cameras only support V4L or V4L2 and some apps only support V4L or V4L2.Wanna test your camera? Install a nice little app called cheese. You'll find it in your repository and it works with V4L or V4L2 compliant devices.**
**fswebcam**配置：
**device /dev/video0input 0loop 15skip 20backgroundresolution 320x240set brightness=60%set contrast=13%top-bannerfont /usr/share/fonts/truetype/msttcorefonts/arial.ttftitle "EvilEye cam-O-tron"timestamp "%d-%m-%Y %H:%M:%S (%Z)"jpeg 95save /home/user/pictures/viewcam.jpgpalette MJPEG**
**2.   motion**
**[http://www.palebluedot.nl/jml/computer-stuff/33-webcam-streaming-with-raspberry-pi.html](http://www.palebluedot.nl/jml/computer-stuff/33-webcam-streaming-with-raspberry-pi.html)**
**3.  mjpeg-streamer**
# [Stream Video from the Raspberry Pi Camera to Web Browsers, Even on iOS and Android](http://blog.miguelgrinberg.com/post/stream-video-from-the-raspberry-pi-camera-to-web-browsers-even-on-ios-and-android)
This is an efficient method of streaming video from the Pi to another computer, but it has a few problems:
- The Raspberry Pi needs to know the address of the computer that is playing the video
- The playing computer needs to have an advanced player that can play a raw H.264 video stream. No mobile device that I know can do this, for example.
- Since this system relies on a direct connection between the Pi and the player, it is impossible to have the player computer connect and/or disconnect from the stream, the connection needs to be on at all times.
- What if there are two or three concurrent players? Things get awfully complicated for the Pi.
**4. gstreamer**
**5. **
