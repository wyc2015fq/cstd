# pom - 三少GG - CSDN博客
2010年11月04日 11:09:00[三少GG](https://me.csdn.net/scut1135)阅读数：1147标签：[file																[include																[build																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)
1.  misc.cc rgb_image.cc       include <stdlib.h>
2.  **$sudo apt-get install libpng-dev**
**3.  make**
**4.  ./pom test.pom**
## linux下build的时候遇到的问题及解决
 http://news.ccidnet.com/art/32859/20100709/2110913_1.html
1. Could not find wxWidgets library. 
Solution: $sudo apt-get install libwxgtk2.8-0 libwxgtk2.8-dev wx2.8-headers wx-common
2. Need libsmbclient.h for compiling on Unix. 
Solution: $sudo apt-get install libsmbclient-dev 
3. Need cups.h for compiling on Unix. 
Solution: $sudo apt-get install libcupsys2-dev 
4. error: Need opensc/opensc.h. 
Solution: $sudo apt-get install libopensc2-dev 
5. exec: g++: not found. 
Solution: $sudo apt-get install build-essential 
6. error: X11/Xlib.h: no such file or directory. 
Solution: $sudo apt-get install libx11-dev 
7. error: X11/Xmu/WinUtil.h: no such file or directory.
Solution: $sudo apt-get install libxmu-dev 
8. error: jpeglib.h: no such file or directory. 
Solution: $sudo apt-get install libjpeg-dev 
**9. error: png.h: no such file or directory.**
**Solution: $sudo apt-get install libpng-dev**
