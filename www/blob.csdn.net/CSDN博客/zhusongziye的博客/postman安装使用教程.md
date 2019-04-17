# postman安装使用教程 - zhusongziye的博客 - CSDN博客





2017年09月16日 21:50:57[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1995








        无聊的夜晚，没有酒喝，也没妹子陪，闲来没事研究下postman，之前接触过有点，还不错的工具。先从最基本的开始。。。。

postman是谷歌的一款非常好用的工具，可用来做手工的接口测试，也能实现简单的接口自动化持续集成，如果大家觉得postman已经能满足自己的需要，也不想去学习

Python+requests，那要好好学习了！


- **Postman功能**（[https://www.getpostman.com/features](https://www.getpostman.com/features)）
- 主要用于模拟网络请求包
- 快速创建请求
- 回放、管理请求
- 快速设置网络代理





**我的环境：**

        Windows7 32bit

        chrome 版本58.0.3029.110




**一、安装nodejs**


安装postman需要先安装nodejs，这个很简单下载下来一路点next安装上即可，大家都会就不说了。

下载地址：[http://nodejs.cn/download/](http://nodejs.cn/download/)


安装后，打开cmd，输入node -v ，看到输出node的版本信息，即代表安装成功，环境变量设置也正确。

![](https://img-blog.csdn.net/20170916220201035?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





二、**安装newman**


         命令行输入指令 npm install newman –global 


![](https://img-blog.csdn.net/20170916221414485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


         安装完成后，输入指令 newman -v 查看版本，检测是否安装成功，这里注意，newman的包被安装在了npm工具的包下 


![](https://img-blog.csdn.net/20170916221721962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


          出现了newman的版本，说明我们安装ok。




**三、安装postman**

          下载地址：[https://www.getpostman.com/](https://www.getpostman.com/)

          直接安装，安装完成后，会弹出登陆页面，完成登陆，进入Postman主页面

![](https://img-blog.csdn.net/20170916230517818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

          postman界面的左侧边栏，选择collections，找到自己需要导出的集合，选择更多按键，点击Export

![](https://img-blog.csdn.net/20170916231453390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170916231922178?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




           点击export之后，弹出的窗口，我们选择v2，并导出到某一个我们选择的文件夹下

![](https://img-blog.csdn.net/20170916232120783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




            导出的collection是json文件


![](https://img-blog.csdn.net/20170916232314601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



##        命令行运行collection

            cmd进入到collection的json文件的目录下，输入指令



            newman run postmanEcho.postman_collection.json

![](https://img-blog.csdn.net/20170916233109639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## 三、Jenkins创建项目

> 
我们可以使用Jenkins新建一个项目，按照流程配置一下（参考Jenkins的教程） 

看一下执行构建的windows命令，给大家一点tip 
![这里写图片描述](https://img-blog.csdn.net/20170910181746028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTQ5MDgwMjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的”cd\”是因为博主本身的Jenkins默认的工作环境就是G盘的Apache目录，所以需要切换回根目录


## 四、newman指令非内部或外部指令

> 
如果在测试构建的过程中碰到以下问题 
![这里写图片描述](https://img-blog.csdn.net/20170910182006671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTQ5MDgwMjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是因为Jenkins的工作环境下与windows的默认环境有所区别，我们只需要配置一下Jenkins的环境变量即可 

系统管理->管理节点->选择需要使用的主机->配置从节点 

新建一个键值对列表，键名PATH，把npm和nodejs的路径放进来即可 
![这里写图片描述](https://img-blog.csdn.net/20170910182338893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTQ5MDgwMjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

点击保存之后我们再去构建就没有问题了。


## 五、newman生成测试报告

> 
newman运行可以生成各类测试报告，有html,cli,json格式等 

指令：newman run xxx.postman_collection.json -r html[,cli,json,junit] 

如果要生成多个格式，用逗号隔开不要留空格 

当然支持给测试报告命名和指定生成路径 

指令：newman run xxx.postman_collection.json -r html –reporter-html-export G:\name.html


可以定义测试报告的生成位置以及它的名称







