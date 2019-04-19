# stream rtmp (rtsp) to RTMP server - 三少GG - CSDN博客
2013年06月16日 21:46:10[三少GG](https://me.csdn.net/scut1135)阅读数：1844
个人分类：[开源硬件Raspberry](https://blog.csdn.net/scut1135/article/category/712785)
1. 
Here's the
**lsmod** for it:
**v4l-utils **contains the following video4linux command line utilities:
 decode_tm6000: decodes tm6000 proprietary format streams
 v4l2-compliance: tool to test v4l2 API compliance of drivers
 v4l2-ctl, cx18-ctl, ivtv-ctl: tools to control v4l2 controls from the cmdline
 v4l2-dbg: tool to directly get and set registers of v4l2 devices
 v4l2-sysfs-path: sysfs helper tool
And the script file I use to start ffserver and ffmpeg
```
ffserver -f /etc/ff.conf & ffmpeg -v quiet -r 5 -s 320x240 -f video4linux2 -i /dev/video0 http://localhost/webcam.ffm
```
When I tried streaming, I found out that the webcam only supports 30fps and 15fps, so the driver automatically forces it to 15fps.
For some reason, ffserver also has trouble streaming mjpeg when the source is also mjpeg.
To stream straight mjpeg, I used the `-vcodec mjpeg` parameter by itself
 in the script to start ffmpeg. The output of ffmpeg shows that it is definitely streaming straight mjpeg but for some reason I can’t tap into the stream.
2. 
# [Raspberry Pi – Surveillance WebCam in the Cloud](http://jasmeu.wordpress.com.sixxs.org/2013/03/17/raspberry-pi-surveillance-webcam-in-the-cloud/)
Here is my first project using the Raspberry Pi: Surveillance Camera connected to the cloud.
First of all, a short: Why ?
- Why Surveillance Camera: Well it’s fun to be in vacation and to check how the garden looks like, if a friend should be called and sent to wet the flowers, aso …
- Why Web Cam: Together with a Pi it’s flexible and very cheap compared with other solutions
- Why Cloud: I want to see my garden when I’m NOT at home. And I don’t want to open ports in my own firewall. Hence, if I cannot come IN to the content, I obviously have to put the content OUT.
Some other constraints I had to consider:
- The Web Cam will be placed about 10-15 m away from my Pi
- The cable to the Cam will have to go through some walls (physical- not fire-), so the smaller the holes I’ll have to do in the walls, the more peaceful will remain my family-life ![;)](http://s1.wp.com.sixxs.org/wp-includes/images/smilies/icon_wink.gif?m=1129645325g)
- I must avoid writing too many content on the SD Card; It would be a pitty to break it due to high number of Write operations.
- I considered being as “slim” as possible with using the Pi ressources. This meant to me scripting in Bash rather than other scripting languages.
- I somehow had to upload to the cloud: Best case: via some robust SDK API, hopefully implementing very efficiently the communication to the cloud. So here I had to give up the Bash and consider for example Java.
