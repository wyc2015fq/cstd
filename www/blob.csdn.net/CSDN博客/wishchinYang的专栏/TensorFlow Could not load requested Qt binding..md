# TensorFlow: Could not load requested Qt binding. - wishchinYang的专栏 - CSDN博客
2017年12月12日 17:03:18[wishchin](https://me.csdn.net/wishchin)阅读数：395
         使用Eclipse 引入tensorflow，出现 Could not load requested Qt binding.  问题
         ImportError: Could not load requested Qt binding.
         Please ensure that PyQt4 >= 4.7, 
         PyQt5 or PySide >= 1.0.3 is available, 
         and only one is imported per session。
解决方法：
       可以在其他IDE中引入TensorFlow，但是仍然不能使用，因为老的matplotlib文件使用了pyqt4.
       卸载matlpotlib，重装。
       安装keras，可以使用TensorFlow。
