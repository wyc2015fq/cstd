# TensorFlow：Windows下使用TensorFlow-Python版本 - wishchinYang的专栏 - CSDN博客
2017年11月15日 16:04:44[wishchin](https://me.csdn.net/wishchin)阅读数：4563
              原文链接：[Win10X64下安装使用TensorFlow](http://blog.csdn.net/u010099080/article/details/53418159)
## 安装TensorFlow
由于Google那帮人已经把 TensorFlow 打成了一个 pip 安装包，所以现在可以用正常安装包的方式安装 TensorFlow 了，就是进入命令行执行下面这一条简单的语句：
```
# GPU版本
pip3 install --upgrade tensorflow-gpu
# CPU版本
pip3 install --upgrade tensorflow
```
然后就开始安装了，速度视网速而定。
问题1：   
Found existing installation: setuptools 27.2.0
Cannot remove entries from nonexistent file c:\works\anaconda3\lib\site-packages\easy-install.pth
解决方式：安装1.2版本
pip install --ignore-installed --upgrade pip setuptools https://storage.googleapis.com/tensorflow/windows/cpu/tensorflow-0.12.0-cp35-cp35m-win_amd64.whl
问题2：
没有翻墙，不能链接到google网站
解决方法：下载whl文件：
protobuf-3.4.0-py3-none-any.whl
tensorflow-1.4.0-cp35-cp35m-win_amd64.whl
直接安装
![](https://img-blog.csdn.net/20171115155740786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第三次成功...........................
测试（使用Eric6-Anaconda3）：
![](https://img-blog.csdn.net/20171115155808054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然而出现此种问题，在Eclipse下面，第二次才能导入成功.....
![](https://img-blog.csdn.net/20171115160305327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

