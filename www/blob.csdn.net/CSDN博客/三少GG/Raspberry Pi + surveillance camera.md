# Raspberry Pi + surveillance camera - 三少GG - CSDN博客
2013年06月04日 20:43:12[三少GG](https://me.csdn.net/scut1135)阅读数：3915
个人分类：[开源硬件Raspberry](https://blog.csdn.net/scut1135/article/category/712785)

# Raspberry Pi – Surveillance WebCam in the Cloud
Why ?
- Why Surveillance Camera: Well it’s fun to be in vacation and to check how the garden looks like, if a friend should be called and sent to wet the flowers, aso …
- Why Web Cam: Together with a Pi it’s flexible and very cheap compared with other solutions
- Why Cloud: I want to see my garden when I’m NOT at home. And I don’t want to open ports in my own firewall. Hence, if I cannot come IN to the content, I obviously have to put the content
 OUT.
--------------------------------------------------------------------------------------------------------------------------------------
# [Using the Raspberry Pi for camera surveillance using Motion and cloud storage](http://astroland.funxiun.com/using-the-raspberry-pi-for-camera-surveillance-using-motion-and-cloud-storage/)
If you are looking for a cheap camera surveillance setup, the [Raspberry Pi](http://www.raspberrypi.org/) is
 a great solution.
It is small, easy to install and, most important, has low energy usage.
I own Model B (see the [Raspberry Pi Wikipedia
 page](http://en.wikipedia.org/wiki/Raspberry_Pi#Specifications) for detail about the models) which uses 3.5 Watts.
There are several open source motion detection applications for Linux such as [ZoneMinder](http://www.zoneminder.com/)and [Motion](http://www.lavrsen.dk/foswiki/bin/view/Motion/WebHome).
Both programs are available through the Rasbian repository.
ZoneMinder looks like a real security control center with all those nice cam screens, but it’s too bloated and too CPU heavy for the Raspberry Pi.
Motion is more lightweight and doesn’t stress the CPU too much when processing 320×200 camera data.
With 2 IP camera’s my average load is less than 0.50 on my RasPi, which also serves as an caching DNS server.
F**or storage of the AVI video files which contain the captured motion frames,** I use a free [Box.com](https://www.box.com/) account.
Box.com provides the same service as the popular Dropbox.
The reason for choosing Box.com is because Dropbox has no open source client. The Dropbox Linux client is closed source and has, at the moment, no support for ARM devices like the Raspberry Pi.
Furthermore the free Box.com account gives you 5 GB of storage. Way enough to store home camera data for a couple of months.
[BoxFS](http://code.google.com/p/boxfs/) is the weapon of choice to mount your Box.com account as a drive partition on your Raspberry Pi.
Some basic Linux knowledge is preferred.
Setting up BoxFS
Read the [Compiling page](http://code.google.com/p/boxfs/wiki/Compiling) on the BoxFS site carefully ![:-)](http://astroland.funxiun.com/wp-includes/images/smilies/icon_smile.gif)
From release 63 of BoxFS [libapp](https://github.com/drotiro/libapp) is used for parsing config files. Be sure
 to compile and install this first.
When libapp is compiled and installed properly, install the rest of the BoxFS prerequisites:
```
sudo apt-get install subversion libxml2-dev libfuse-dev libcurl4-gnutls-dev libzip-dev
```
I recommend using the latest commit of BoxFS in the repository.
The latest release, version 0.7, does not support user/group id’s and file/directory permissions. The latest commit (>=release 63) does.
`svn checkout http://boxfs.googlecode.com/svn/trunk/ boxfs-read-only`
After checking out the BoxFS source code, go to the boxfs-read-only directory to compile and install BoxFS:
`sudo make install`
The BoxFS binary will be installed in /usr/local/bin.
Mounting your Box.com account using BoxFS
I found out that running BoxFS as root gives the best results. I tried running BoxFS as a non-root user and it gave me some curious and unpredictable results.
Create a configuration file (for example boxfs.cfg) first.
A valid configuration file looks like this:
```
#This is a comment line
username   = me@domain.com
password   = mystrongpass
mountpoint = /path/to/folder
# from version 0.6:
verbose    = yes
secure     = no
# from version 0.7:
largefiles = yes
# from commit r86
uid        = 1000
gid        = 1000
fperm      = 644
dperm      = 755
```
The mount point must be provided in either way.  This can be an empty directory called /motion for
 example.
To start the BoxFS client, simply run this command as root:
boxfs -f boxfs.cfg
To check if everything is doing what it should be doing, go to your BoxFS mount point and try to create or copy some files.
For debugging I recommend to be logged in to your Box.com account with your browser to see if things are actually written in to the cloud storage.
I noticed that the BoxFS partition doesn’t appear when you run the df command as a non-root user. When you run the df command
 as root (or something like sudo df), the BoxFS partition is shown.
Just keep that in mind.
Installing and configuring Motion
To install [Motion](http://www.lavrsen.dk/foswiki/bin/view/Motion/WebHome) just run this command:
`sudo apt-get install motion```
Be sure that your camera’s and/or webcams output a 320×200 (or 240) image and Motion is configured with the same resolution.
**I’ve tried a resolution like 640×400 pixels, but the CPU load went sky high on the RasPi, so I’ve settled for a lower resolution.**
The images generated by my camera’s are quite clear and useful, so I’m happy.
I have no experience over clocking the Pi in combination with Motion, but if you do, please share your findings in the comments.
Same goes for solving the above resolution problem ![:)](http://astroland.funxiun.com/wp-includes/images/smilies/icon_smile.gif)
Further configuring Motion is something you have to do on your own.
The on line manual is quite clear about the numerous settings which can be done.
Mailing an image snapshot or a simple message when motion is detected is just one of the many possibilities.
Oh, a**nd don’t forget to set the path to your BoxFS mount point to store your videos **![;-)](http://astroland.funxiun.com/wp-includes/images/smilies/icon_wink.gif)
When you’ve got an mobile device, you can use the Box.com app to download an AVI file and watch it using a movie player which supports the file format.
Great when you’re on holiday for example, and want to know what’s going on at home.
And all of this without tampering  your firewall, keeping your home network closed and secure.
I hope this post helps you to set up your own budget camera surveillance system.
For less than 180 euro (that’s [2 decent IP camera’s](http://mydlink.dlink.com/products/DCS-932L) and
 a RasPi) you’re all set to go.
----------------------------------------------------------------------------
## 基于树莓派实现的开门君
开门君是一个基于树莓派(raspberry pi)实现可以通过手机开门的小项目，也许是为了方便搞基，总之他就是被设定成为开门君了。
第一问:为什么通过浏览器访问一个地址就能把门禁开了？请看以下简略的流程解释:
- 
1、浏览器向服务器端发送请求。
- 
2、树莓派接收到请求后控制GPIO输出高电平。
- 
3、高电平信号会让继电器模块吸合，相当于按下了门禁开关,于是开门。
第二问:树莓派是什么?树莓派起到了什么作用?
![](http://m3.img.libdd.com/farm4/2013/0304/00/7271AB6975E3709EFE88289C405E338C1D9052B75C2AC_500_374.jpg)
上图的电路板就是树莓派
> 
树莓派是一款基于ARM的微型电脑主板，以SD卡为内存硬盘，卡片主板周围有两个USB接口和一个网口，可连接键盘、鼠标和网线，同时拥有视频模拟信号的电视输出接口和HDMI高清视频输出接口，以上部件全部整合在一张仅比信用卡稍大的主板上...
树莓派上面可以运行Raspbian,一个基于Debian为树莓派优化的Linux,这里树莓派就是一个Linux服务器.你可以在上面使用你熟悉的编程语言进行开发.
同时更方便的是树莓派带有8个GPIO接口,可以非常方便地用程序控制GPIO口输入或者输出高低电位来控制继电器开或者关.
python代码:
web.py是python下的一个小型web框架.
wiringpi是一个开源的GPIO控制库.
sesame模块是我基于wiringpi用来实现开关门的动作.
index.py
#yishanju 2013/02/27 18:07
#
    
import web
import sesame
    
urls = ("/","index",)
app =web.application(urls,globals())
    
class index:
        def GET(self):
                sesame.k()
                return "Open sesame."
    
if __name__ == '__main__':
        app.run()
sesame.py
#!/bin/env python
#coding=GB18030
#yishanju 2013/02/27
   
import wiringpi
from time import sleep
  
pin=6
t=1
  
io=wiringpi.GPIO(wiringpi.GPIO.WPI_MODE_PINS)
io.pinMode(pin,io.OUTPUT)
  
def Open ():
  
        io.digitalWrite(pin,io.HIGH)
        if io.digitalRead(pin):
                return 1
  
def Close():
        io.digitalWrite(pin,io.LOW)
        return io.digitalRead(pin)
  
def k():
        Open()
        sleep(t)
        Close()
 
if __name__=='__main__':
        k()
硬件: 
树莓派 x1 约300元
继电器模块 (工作电压5V,高电平吸合,单刀双掷地) 5元
无线网卡 x1 约 50元
HDMI转DVI X1 约40元
SD卡 x1 约70元
万用表 x1
杜邦线 3根 用来连接树莓派与继电器
测试夹 方便测量测试用.
这个开门小项目的基础上还可以做得出什么?:
- 
增加身份验证
- 
加上声音接收模块,简单的可以实现声控开门,
- 
更难一些的通过接收声音二维码开门.
- 
加上摄像头,可以当监控用,也许可以实现人脸识别~
- 
加上指纹模块,可以自己来实现指纹识别..
- 
更NB也许还可以通过脚步声,步态进行身份识别呢?
- 
加上粉尘传感器,还可以兼职PM2.5检测器.其它什么温湿度压力依此类推.
收获:
在想如何实现开门君的过程当中也想明白了饮水机娘的一个实现方式.
做的过程里发现自己需要更深的知识和技术才能发挥出价值~~ T_T这种浮浅的水平根本做不好事情啊.
别的没什么,可是,电子学,Linux,python这些技能都用到了,放组合技能的感觉太爽了,哈哈.
其它:
针对树莓派GPIO这一块还有一些不明白的地方.争取弄明白后补上文章.
最后向web.py创始人Aaron Swartz致敬.
参考资料:
[使用树莓派制作的远程开门器](http://www.cnblogs.com/guanhe/archive/2012/12/25/2832982.html)[http://www.cnblogs.com/guanhe/archive/2012/12/25/2832982.html](http://www.cnblogs.com/guanhe/archive/2012/12/25/2832982.html)
wiringpi作者Gordon的博客[https://projects.drogon.net/raspberry-pi/wiringpi/](https://projects.drogon.net/raspberry-pi/wiringpi/)
WiringPi的github主页 [https://github.com/WiringPi](https://github.com/WiringPi)
[树莓派外部接口详细说明](http://blog.csdn.net/wgluser/article/details/8494086)
[SPI、I2C接口的定义以及区别](http://blog.21ic.com/user1/5106/archives/2008/53314.html)
用NodeJs来控制硬件（基于Raspberry Pi）（一）[http://cnodejs.org/topic/509a960c637ffa41553bbd4c](http://cnodejs.org/topic/509a960c637ffa41553bbd4c)
让你们通过超链接跳转到外星球去吧！！：
DIY NB到极致的比尔•博鲁茨基: 送望远镜上天，去寻找外星行星！[http://www.guokr.com/article/425871/](http://www.guokr.com/article/425871/)
自制Arduino检测器，随时关注空气质量 [http://www.guokr.com/article/434130/](http://www.guokr.com/article/434130/)
用Siri语音打开车库门 树莓派发烧友新玩法[http://iphone.tgbus.com/news/class/201212/20121225110024.shtml](http://iphone.tgbus.com/news/class/201212/20121225110024.shtml)
电子点火器 [](http://www.guokr.com/article/436640/)[http://www.guokr.com/article/436640/](http://www.guokr.com/article/436640/)
揭开“饮水机娘”的神秘面纱 [http://www.guokr.com/article/81621/](http://www.guokr.com/article/81621/)
扫地机器人也发微博，才是真正的微博时代[http://www.guokr.com/article/49759/](http://www.guokr.com/article/49759/)
[#Raspberry PI](http://yishanju.diandian.com/?tag=Raspberry%20PI)[#树莓派](http://yishanju.diandian.com/?tag=%E6%A0%91%E8%8E%93%E6%B4%BE)[#wiringpi](http://yishanju.diandian.com/?tag=wiringpi)[#web.py](http://yishanju.diandian.com/?tag=web.py)[#python](http://yishanju.diandian.com/?tag=python)
参考内容：
1. [http://www.instructables.com/id/Pan-Tilt-face-tracking-with-the-raspberry-pi/?ALLSTEPS](http://www.instructables.com/id/Pan-Tilt-face-tracking-with-the-raspberry-pi/?ALLSTEPS)
## Step 4: Connecting the servos
[](http://www.instructables.com/files/deriv/FT9/LSIP/HDYZIQXT/FT9LSIPHDYZIQXT.LARGE.jpg)
![RPi-GPIO-Pins.png](http://www.instructables.com/files/deriv/FT9/LSIP/HDYZIQXT/FT9LSIPHDYZIQXT.LARGE.jpg)
2. 
# [The Dark Pi Rises](http://www.aonsquared.co.uk/the_dark_pi_rises)
To use it, let's install some required programs:
`sudo apt-get install gstreamer-tools gstreamer0.10-plugins-bad gstreamer0.10-plugins-good v4l-utils`
There are several ways to stream the webcam output to the network, but I settled on GStreamer because 1) I couldn't get VLC to work, 2) ffserver, motion and mjpeg consumed more CPU when I tried them. The following command gives me 10fps at 320x240, at only
 25-30% CPU usage:
UPDATE: The Raspberry Pi Foundation has updated the firmware to enable H.264 encode on the GPU - I'll be working on getting this working as this could enable phenomenal improvement in the webcam streaming while using *less* CPU!
To open the Raspberry Pi's stream on your desktop, just fire up VLC and click 'File->Open Network Stream' and type in 'tcp://192.168.1.135:5000' in the location bar (replace the IP address with your Raspberry Pi's IP address!).
Now you should have 'live' streaming from your Raspberry Pi!

3. 
## [Object Tracking on the Raspberry Pi with C++, OpenCV, and cvBlob](http://programmaticponderings.wordpress.com/2013/02/09/opencv-and-cvblob-with-raspberry-pi/)
Frame-rates on the Intel processor-based Ubuntu laptops easily performed at or beyond the maximum 30 fps rate of the webcams, at
 640 x 480 pixels. On a positive note, the Raspberry Pi was able to compile and execute the tests of OpenCV and cvBlob (see bug noted at end of article).
**Unfortunately, at least in my tests, the Raspberry Pi could not achieve more than 1.5 – 2 fps at most, even in the most basic tests, and at a reduced capture size of 320 x 240 pixels.**This can be seen in the first and second screen-grabs of Test #1, above. Although, I’m sure there are ways to improve the code and optimize the image capture, the results were much to slow to provide accurate, real-time
 data to the robot’s targeting system.
