# OpenCV：Python3使用OpenCV - wishchinYang的专栏 - CSDN博客
2017年11月12日 18:14:02[wishchin](https://me.csdn.net/wishchin)阅读数：2831
       Python3使用OpenCV安装过程应该是这样的，参考：[http://blog.csdn.net/lixintong1992/article/details/61617025](http://blog.csdn.net/lixintong1992/article/details/61617025)    ，使用conda安装OpenCV，  但是没有多少效果。在Eclipse-PyDev里面不能使用cv2.
        另一种方法：直接下载       opencv_python‑3.3.1‑cp35‑cp35m‑win_amd64.whl 文件，下载到本地，使用pip 安装。
命令：
        pip install opencv_python-3.3.1-cp35-cp35m-win_amd64.whl
问题：
      若出现 import numpy.core问题，重装numpy最新版本即可。
        pip uninstall numpy
   或者直接使用 
        pip install -U numpy 
      可重新使用OpenCV
