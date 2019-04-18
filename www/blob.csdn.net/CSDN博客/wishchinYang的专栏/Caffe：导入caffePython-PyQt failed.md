# Caffe：导入caffePython-PyQt failed - wishchinYang的专栏 - CSDN博客
2018年02月24日 10:15:02[wishchin](https://me.csdn.net/wishchin)阅读数：732

在另一台电脑上使用caffe python版本，显示
         Backend Qt5Agg is interactive backend. Turning interactive mode on.
         Failed to enable GUI event loop integration for 'qt5'
         ...................
         PyQt4 failed...
原因，没有安装PyQt4，而使用了PyQt5。
下载个PyQt4文件，安装完成。
出现：
Backend Qt5Agg is interactive backend. Turning interactive mode on.
Failed to enable GUI event loop integration for 'qt5'
Traceback (most recent call last):
  File "D:\Works\Android\eclipse\plugins\org.python.pydev_5.3.0.201610121612\pysrc\_pydev_bundle\pydev_console_utils.py", line 565, in do_enable_gui
    enable_gui(guiname)
  File "D:\Works\Android\eclipse\plugins\org.python.pydev_5.3.0.201610121612\pysrc\pydev_ipython\inputhook.py", line 528, in enable_gui
    return gui_hook(app)
  File "D:\Works\Android\eclipse\plugins\org.python.pydev_5.3.0.201610121612\pysrc\pydev_ipython\inputhook.py", line 215, in enable_qt5
    app, inputhook_qt5 = create_inputhook_qt5(self, app)
  File "D:\Works\Android\eclipse\plugins\org.python.pydev_5.3.0.201610121612\pysrc\pydev_ipython\inputhookqt5.py", line 84, in create_inputhook_qt5
    from PyQt5 import QtWidgets
  File "D:\Works\Android\eclipse\plugins\org.python.pydev_5.3.0.201610121612\pysrc\_pydev_bundle\pydev_import_hook.py", line 21, in do_import
    module = self._system_import(name, *args, **kwargs)
ImportError: DLL load failed: �Ҳ���ָ���ĳ���
环境优先使用了PyQt5。
1.进入 C:\Works\Anaconda2\Lib\site-packages 目录，删除PyQt5；
导入成功；
但是在使用时候仍然判定API为PyQt5，使用失败。
2.卸载Matplotlib          pip uninstall Matplotlib
3.重装Matplotlib          pip install Matplotlib
使用成功。
PlaceNet205.
![](https://img-blog.csdn.net/20180224105420151?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
