# pycharm下打开、执行并调试scrapy爬虫程序 - 博客堂 - CSDN博客





2017年05月14日 20:34:56[最小森林](https://me.csdn.net/u012052268)阅读数：27770















- 

首先得有一个Scrapy项目，我在Desktop上新建一个Scrapy的项目叫test，在Desktop目录打开命令行，键入命令：`scrapy startproject test1`


![1](https://img-blog.csdn.net/20161024143830765)


目录结构如下： 

![1](https://img-blog.csdn.net/20161024143930168)
![1](https://img-blog.csdn.net/20161024144009872)
![1](https://img-blog.csdn.net/20161024144048688)



- 

打开Pycharm，选择open 


![1](https://img-blog.csdn.net/20161024144211720)



- 

选择项目，ok 

![1](https://img-blog.csdn.net/20161024144333204)


- 

打开如下界面之后，按alt + 1， 打开project 面板 

![1](https://img-blog.csdn.net/20161024144503783)
![2](https://img-blog.csdn.net/20161024144602237)
- 

在test1/spiders/，文件夹下，新建一个爬虫spider.py， 注意代码中的`name="dmoz"`。这个名字后面会用到。 

![1](https://img-blog.csdn.net/20161024144749397)


- 

在test1目录和scrapy.cfg同级目录下面，新建一个begin.py文件（便于理解可以写成main.py），注意箭头2所指的名字和第5步中的`name='dmoz'` 名字是一样的。


```python
from scrapy import cmdline

    cmdline.execute("scrapy crawl dmoz".split())
```
- 1
- 2
- 3
- 1
- 2
- 3




![1](https://img-blog.csdn.net/20161024145150255)
7. 上面把文件搞定了，下面要配置一下pycharm了。点击Run->Edit Configurations



![1](https://img-blog.csdn.net/20161024145608602)
8. 新建一个运行的python模块 
![1](https://img-blog.csdn.net/20161024145802551)
9. Name：改成spider； script：选择刚才新建的那个begin.py文件；Working Direciton：改成自己的工作目录 
![1](https://img-blog.csdn.net/20161024145918443)
10. 至此，大功告成了，点击下图，右上角的按钮就能运行了。 
![1](https://img-blog.csdn.net/20161024150152071)









##### 调试




可以在其他代码中设置断点，就可以debug运行

![](http://upload-images.jianshu.io/upload_images/1908836-80d3f622ca69e5f1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


Paste_Image.png


![](http://upload-images.jianshu.io/upload_images/1908836-af33ca442139bc1a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)





转载1：http://www.jianshu.com/p/f85120fcbca0

转载2：http://blog.csdn.net/wangsidadehao/article/details/52911746




