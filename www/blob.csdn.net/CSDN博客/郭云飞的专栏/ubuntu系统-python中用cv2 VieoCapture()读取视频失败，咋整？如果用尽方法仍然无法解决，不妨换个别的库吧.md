
# ubuntu系统 - python中用cv2.VieoCapture()读取视频失败，咋整？如果用尽方法仍然无法解决，不妨换个别的库吧 - 郭云飞的专栏 - CSDN博客


2017年09月14日 11:09:58[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：4272


近日在Python环境中用cv2.VideoCapture()读取视频时，cv2.VideoCapture.open()始终返回False。去百度一查，遇到该情况的一大堆。然后查看opencv的官方文档，发现如下内容：

```python
以下内容的出处：https://pypi.python.org/pypi/opencv-python
Q: Why I can’t open video files on GNU/Linux distribution X or on macOS?
A: OpenCV video I/O depends heavily on FFmpeg. Manylinux and macOS OpenCV binaries are not compiled against it. The purpose of these packages is to provide as easy as possible installation experience for OpenCV Python bindings and they should work directly out-of-the-box. Adding FFmpeg as an additional dependency without a “universal” FFmpeg build (e.g. LGPL licensed build like in the Windows wheels) the goal is considerably harder to achieve. This might change in the future.
```
大致原因是没有编解码器，或者编解码器安装的不正确。那咋解决呢？在stackoverflow有一篇帖子基本上总结的比较全：

```python
// https://stackoverflow.com/questions/31040746/cant-open-video-using-opencv
```
基本思路就是：
1. git下载ffmpeg源码，编译（具体编译过程帖子里很详细）
2. gitc下载opencv源码，编译（具体编译过程帖子里很详细）。最最最重要的一点，cmake选项一定要加python支持：

```python
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_QT=OFF -D WITH_V4L=ON -D CMAKE_SHARED_LINKER_FLAGS=-Wl,-Bsymbolic ..
```
当然其他的选项，看自己情况加，比如opencv的contrib模块等。可以用cmake-gui，能直观的看到那些功能勾上了，那些没勾上。
编译opencv大家都知道，需要半个小时左右，可以去喝杯咖啡再来。
完成后，本帖题目的问题就搞定了。但各种奇怪的事情总会发生，如何还不行，咋整！？
----分割线----
在Python中，可以读视频的库，除了opencv外，还有一个——imageio，这个库的具体介绍见：

```python
https://github.com/imageio/imageio
```
这个库的核心函数如下：

```python
imread() and imwrite() - for single images
mimread() and mimwrite() - for image series (animations)
volread() and volwrite() - for volumetric image data
get_reader() and get_writer() - for more control (e.g. streaming)
```
可以看到它就是用来读图像、视频文件、camera等的。
imageio依赖一下环境：

```python
Python 3.x, 2.7 or 2.6
Numpy
Pillow
```
所有，如果想自己编译源码，要提前安装所依赖的库；如果用pip，就一条命令就够了：

```python
sudo pip install imageio
```
----分割线----
实际上，很多人都还是习惯用opencv。在Python中，opencv的图像是一个np.array。我们用imageio读取的图像数据要想转换成np.array，还需要一个库，就是skimage：
这个库的官网：

```python
http://www.scikit-image.org/
```
安装很简单：

```python
sudo pip install scikit-image
```
----分割线----
好了接下来是一个实例：

```python
import sys
import os
import random
import math
import numpy as np
import linecache
import string
import imageio
import skimage
if __name__ == "__main__":
    print '--STA--'
    strPath = '/home/raintai/local_visual_algorithm/samples/5cc4bc32-64ad-40e4-93e8-b2ab88aa684f.init.mp4'
    if (os.path.exists(strPath) == False):
        print 'file not exist'
    # use imageio to read video file
    curVideo = imageio.get_reader(strPath)
    for i, img in enumerate(curVideo):
        opencvImg = skimage.img_as_ubyte(img, True)
        # 下边就回到熟悉的opencv套路上了
		print opencvImg.shape
    print '--END--'
```

```python
import sys
import os
import random
import math
import numpy as np
import linecache
import string
import imageio
import skimage
import cv2
if __name__ == "__main__":
    print '--STA--'
    strPath = '/home/raintai/local_visual_algorithm/samples/5cc4bc32-64ad-40e4-93e8-b2ab88aa684f.init.mp4'
    if (os.path.exists(strPath) == False):
        print 'file not exist'
    # use imageio to read video file
    videoReader = imageio.get_reader(strPath)
    FrameNum = videoReader.get_length()
    print 'frame num = ', FrameNum
    for i in range(0, FrameNum):
        img1 = videoReader.get_data(i)
        grayImg = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
        print '%5d'%i, grayImg.shape
    print '--END--'
```










