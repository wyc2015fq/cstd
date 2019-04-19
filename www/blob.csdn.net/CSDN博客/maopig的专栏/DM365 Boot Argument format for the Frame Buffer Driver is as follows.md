# DM365 Boot Argument format for the Frame Buffer Driver is as follows - maopig的专栏 - CSDN博客
2012年03月31日 23:25:51[maopig](https://me.csdn.net/maopig)阅读数：1537标签：[buffer																[windows																[video																[application																[deprecated																[command](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=deprecated&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)
video=[davincifb|dm64xxfb|dm355fb]
 (keywords 'dm64xxfb' and 'dm355fb' are deprecated, 'davincifb' should be used instead.)
·vid0=[off|MxNxP,S@X,Y]
·vid1=[off|MxNxP,S@X,Y]
·osd0=[MxNxP,S@X,Y]
·osd1=[MxNxP,S@X,Y]
Where MxN are the horizontal and vertical window size; P is the color depth (bits per pixel), S is the frame buffer
 size in bytes with suffixes such as ‘K’ or
‘M’ for Kilo (2^10) and Mega (2^20); X, Y are the window position. Only video windows can be turned off. Turning off a video window means that no FB device will be registered for it.
For example:
**video=davincifb:vid0=720x480x16,2025K@0,0:vid1=720x480,1350K@0,0:osd0=720x480,1350K@0,0:osd1=720x480,1350K@0,0****720*480*2*2 = 1350k**
In the above example, the vid0 is reserved with buffer size 2025K bytes, which is large enough for triple buffering
 at 720x280x16. The osd0 window is reserved with buffer size 1350K, which is for double buffering for 720x480x16. The FB driver limits video windows to triple buffering and osd windows to double buffering. The total size of the buffer for all display windows
 shall not exceed 40M bytes. Specific window can be disabled using boot argument option as below:
video=davincifb:vid0=off:vid1=off
 or video=davincifb:vid0=0,0:vid1=0,0
In this example both the vid0 and vid1 will be disabled at boot time. This will prevent the FBDev driver from creating
 devices for vid0 and vid1 (/dev/fb/1 and /dev/fb/3). If any of the windows is disabled at boot time, any FBDev driver application is not allowed to perform any IO control operation with that window. However, this will allow other video applications (e.g. V4L2)
 to access the video windows disabled by FB driver bootargs. OSD windows, however, cannot be disabled by boot arguments. Even if setting up an OSD window as
“off” in the bootargs, it will be ignored by FB driver and set it up with default values. When a video window is turned off at boot time, no /dev/fb or /proc/dev entry will be created
 for it. Alternatively, the following boot arguments can be used to prevent the FBDev driver from claiming video windows, but still reserve the frame buffer space and create FBDev devices. In other words, this will allow V4L2 applications to access vid0 and
 vid1 windows, yet FBDev devices /dev/fb/1 and /dev/fb/3 will still be created.
video=davincifb:vid0=0,2025K:vid1=0,1350K
After booting up, all FB devices are created as normal, and V4L2 applications are able to claim video windows (through
 /dev/video/2 or /dev/video/3) to use. When an FBDev application needs to use the device, use
‘fbset’ command to allow FBDev driver to re-claim the video windows (to desired resolution):
$fbset -fb /dev/fb1 -xres 720 -yres 480 -vxres 720 -vyres 1440 -depth 16
$fbset -fb /dev/fb3 -xres 720 -yres 480 -vxres 720 -vyres 1440 -depth 16
A second alternative is, instead of disabling these windows using boot arguments, using
‘fbset’ to release the windows from FBDev driver’s control for other applications to use even if FBDev devices are enabled at boot argument. The following
 example shows two console commands to “turn off” osd0 and vid0 windows, respectively.
$ fbset -fb /deb/fb0 -xres 0 $ fbset -fb /dev/fb1 -xres 0
When these display windows need to be used by an FBDev application, use
‘fbset’ again to restore the frame buffer device. The following example shows the command of setting vid0 display window to NTSC window size with triple buffering.
$ fbset -fb /dev/fb1 -xres 720 -yres 480 -vxres 720 -vyres 1440
