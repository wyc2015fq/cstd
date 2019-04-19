# Ubuntu14.04 安装配置Opencv3.0和Python2.7 - =朝晖= - 博客园
# [Ubuntu14.04 安装配置Opencv3.0和Python2.7](https://www.cnblogs.com/dhcn/p/7124948.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/u010381648/article/details/49452023
Install [OpenCV](http://lib.csdn.net/base/opencv) 3.0 and [Python](http://lib.csdn.net/base/python) 2.7+ on Ubuntu: http://www.pyimagesearch.com/2015/06/22/install-[opencv](http://lib.csdn.net/base/opencv)-3-0-and-[python](http://lib.csdn.net/base/python)-2-7-on-ubuntu/
Install OpenCV 3.0 and Python 2.7+ on OSX: http://www.pyimagesearch.com/2015/06/15/install-opencv-3-0-and-python-2-7-on-osx/
ffmpeg安装：https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu\
How to install Python 2.7 bindings for OpenCV using MacPorts:http://stackoverflow.com/questions/8410443/how-to-install-python-2-7-bindings-for-opencv-using-macports/8547012
对以上的内容做个补充吧:
      Ubuntu14.04安装的时候，gitclone速度很慢，因为下载量太大，建议去官网下载zip，然后wget Zip地址，cmake的时候如果碰到ippicv_linux_20151201.tgz下载过慢，可以手动下载：https://raw.githubusercontent.com/Itseez/opencv_3rdparty/81a676001ca8075ada498583e4166079e5744668/ippicv/ippicv_linux_20151201.tgz  并把它放到3rdparty相关的位置(find一下文件名，这个文件位置可以找到)，3.1的cmake命令有所变动，这个具体看上面的那个老外帖子,最后的安装目录在python库的sites-packages目录下面，这个目录不一定是sys.path目录，如果找不到，就自己往dist-packages这类的sys.path目录里面copy一份就Ok了。
     stackoverflow那个帖子的核心内容：
- sudo port uninstall opencv  
- sudo port install py27-numpy  
- sudo port install opencv +python27  
- 
