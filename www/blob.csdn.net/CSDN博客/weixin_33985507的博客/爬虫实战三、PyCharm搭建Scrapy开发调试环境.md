# 爬虫实战三、PyCharm搭建Scrapy开发调试环境 - weixin_33985507的博客 - CSDN博客
2019年02月19日 14:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：44
# 一、环境准备
- 
Python开发环境以及Scrapy框架安装，参考：[爬虫实战一、Scrapy开发环境（Win10+Anaconda）搭建](https://www.jianshu.com/p/9da3e50ec240)
- 
PyCharm安装和破解，参考：[爬虫实战二、2019年PyCharm安装（激活到2100年）](https://www.jianshu.com/p/e8215dfafd3c)
# 二、PyCharm配置Scrapy
### 2-1、创建Python项目
**注意，由于无法使用PyCharm直接创建Scrapy项目，因此需要先创建普通的Python项目。**
创建普通Python项目mySpider。
![3163615-588cd1f1f62b8af0.png](https://upload-images.jianshu.io/upload_images/3163615-588cd1f1f62b8af0.png)
图片.png
选择项目位置。
![3163615-2091f2fd7c132a05.png](https://upload-images.jianshu.io/upload_images/3163615-2091f2fd7c132a05.png)
TIM截图20190215160105.png
### 2-2、配置Project Interpreter
点击File-> Settings->Project: mySpider->Project Interpreter。
![3163615-b5de400587d083e3.png](https://upload-images.jianshu.io/upload_images/3163615-b5de400587d083e3.png)
TIM截图20190215161633.png
注意此截图已经配置好完毕，如果没有Project Interpreter，点击下图红框处添加。
![3163615-c8d66c1c0788bf2d.png](https://upload-images.jianshu.io/upload_images/3163615-c8d66c1c0788bf2d.png)
TIM截图20190215163513.png
添加Project Interpreter。
![3163615-6efd4e253f749bc0.png](https://upload-images.jianshu.io/upload_images/3163615-6efd4e253f749bc0.png)
TIM截图20190215163542.png
注意：如果有多个Python版本，为防止冲突可以选择第一个虚拟环境，此处我只有一个环境，选择配置系统解释器。
![3163615-d05d138e804dcbd5.png](https://upload-images.jianshu.io/upload_images/3163615-d05d138e804dcbd5.png)
TIM截图20190215164629.png
根据自身实际情况进行选择。
![3163615-58eadb007856fa1d.png](https://upload-images.jianshu.io/upload_images/3163615-58eadb007856fa1d.png)
TIM截图20190215164832.png
### 2-3、添加Scrapy的相关包
解释器安装完毕，点击右下方的+号，查询包
![3163615-c2dca2ef01014da2.png](https://upload-images.jianshu.io/upload_images/3163615-c2dca2ef01014da2.png)
TIM截图20190215164951.png
安装包，依次安装：
```
lxml -> zope.interface -> pyopenssl -> twisted -> scrapy
```
一般情况下Anaconda已经安装了很多类库，我们只需安装scrapy即可。显示蓝色代表已经安装成功。
![3163615-22c6a8c8160ba2e5.png](https://upload-images.jianshu.io/upload_images/3163615-22c6a8c8160ba2e5.png)
TIM截图20190215165030.png
