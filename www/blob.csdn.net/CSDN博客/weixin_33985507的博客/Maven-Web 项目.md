# Maven-Web 项目 - weixin_33985507的博客 - CSDN博客
2018年07月24日 10:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
1.环境准备：JDK版本设置,编码设置
2.设置maven自动导入,webapp图标变化
![6217104-dd99a434bb57dd1c.png](https://upload-images.jianshu.io/upload_images/6217104-dd99a434bb57dd1c.png)
image.png
3.创建java目录new-Derectory并做目录转化(reimport)
4.maven项目结构熟悉
5.打包查看生成的文件(war也可以打包到仓库中，但是这个项目不是给其他团队直接依赖调用的。可以让测试工程师下载，部署到web容器中去运行测试。)
6.Web服务器配置
创建Maven-Web项目
![6217104-4b6f1fc7aba1c3f5.png](https://upload-images.jianshu.io/upload_images/6217104-4b6f1fc7aba1c3f5.png)
image.png
![6217104-73488189373ede37.png](https://upload-images.jianshu.io/upload_images/6217104-73488189373ede37.png)
image.png
仓库确认：
![6217104-7542260c5ad5e6e2.png](https://upload-images.jianshu.io/upload_images/6217104-7542260c5ad5e6e2.png)
image.png
地址：
![6217104-3ac44b295bc5bd82.png](https://upload-images.jianshu.io/upload_images/6217104-3ac44b295bc5bd82.png)
image.png
默认建立没有java目录，需要手动创建java目录
![6217104-183a09aff2d2dd7e.png](https://upload-images.jianshu.io/upload_images/6217104-183a09aff2d2dd7e.png)
image.png
发现颜色都有问题，可以通过手动刷新：
![6217104-ac1575a9fc06c22f.png](https://upload-images.jianshu.io/upload_images/6217104-ac1575a9fc06c22f.png)
image.png
再次确认颜色：
![6217104-984ee518fff42041.png](https://upload-images.jianshu.io/upload_images/6217104-984ee518fff42041.png)
image.png
Web项目结构
在main文件夹下新建一个java的文件夹，若文件夹没有变成source,右键Mark Directory As...
src文件夹下新建文件夹test并在test文件夹下新建java和resources文件夹。
src/main/java 该目录下存放项目的java文件
src/main/resources该目录下存放项目的资源文件(如spring的xml等)
src/main/webapp web文件的目录
src/test/java 该目录下存放项目测试用的java文件
src/test/resources该目录下存放项目测试用的资源文件
注意颜色：
![6217104-b9410b0aef77d4cf.png](https://upload-images.jianshu.io/upload_images/6217104-b9410b0aef77d4cf.png)
image.png
Web 项目打包格式：
![6217104-e5363bce4e870e27.png](https://upload-images.jianshu.io/upload_images/6217104-e5363bce4e870e27.png)
image.png
Web项目不适合给第三方团队直接依赖使用，war项目给第三方团队调用的是接口。ex:[http://www.java.com/demo/sendmsg.json](http://www.java.com/demo/sendmsg.json)
打包到仓库：
测试童鞋测试，运维同学线上部署升级。直接下载仓库中的war包使用。
本地如何发布测试呢？那就需要关联web容器:
