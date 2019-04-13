
# AWS之搭建深度学习主机 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[AWS之搭建深度学习主机](https://www.cnblogs.com/eniac1946/p/7804212.html)
|Posted on|2017-11-08 15:18|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7804212)|[收藏](#)
|B.G
|至2017.11
|GPU选型：（参考知乎、CSDN）
|高性价比的两款：GTX1080ti, Titan X
|--通常调试算法  i7CPU+32G内存+SSD+2*GPU(单块显存>6G)，总计成本2.7 - 4w+RMB
|土豪系列：Tesla K80, M40
|云：（每小时成本高于物理机，优势是托管环境系统）
|AWS、UClound
|阿里云HPC
|一、准备工作
|Amazon账号|[在AWS上配置深度学习主机](https://zhuanlan.zhihu.com/p/25066187)
|AWS 命令行界面 (CLI)|[https://aws.amazon.com/cn/cli/](https://aws.amazon.com/cn/cli/)
|-使用命令行调用 AWS 服务
|-可以在命令行上获得帮助，以查看支持的服务。
|SSH|（PuTTY+文件导入/导出Linux实例）|[https://docs.aws.amazon.com/zh_cn/AWSEC2/latest/UserGuide/putty.html?icmpid=docs_ec2_console](https://docs.aws.amazon.com/zh_cn/AWSEC2/latest/UserGuide/putty.html?icmpid=docs_ec2_console)
|\#划分一个VPC|[http://blog.csdn.net/dongdong9223/article/details/47153421](http://blog.csdn.net/dongdong9223/article/details/47153421)
|\#申请弹性IP EIP|[亚马逊EC2 绑定Elastic IP（固定ip）_百度经验](https://www.baidu.com/link?url=Fs-Q6R34JTCna_oSmSDt4xvXvTEo1Q2Hg91TTgaOU20-n-nQumjnFR_Osut6DAvIjDuEaptYJxX7fGqqpysofAGI8OdBkmKZ_8jcJd7PsDW&wd=&eqid=be941b940000575b000000055a0185cc)
|二、开启一个实例
|以EC2 p2 xlarge为例
|0. 创建一个新EC2
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171108151207638-1224931184.png)
|共以下几个步骤
![](https://images2017.cnblogs.com/blog/1181483/201711/1181483-20171108151330434-926258272.png)
|1/选择AMI|[Deep Learning AMI](http://link.zhihu.com/?target=https%3A//aws.amazon.com/marketplace/pp/B01M0AXXQB)|（官方，包含Keras2.0）
|-AMI是一个OS的镜像
|-software还需补充 Jupyter Notebook等
|2-7 一路进行下去
|-“添加存储”按照EBS使用量每月计费，按需申请
|配置密钥对
|如果你有自己常用的私钥和公钥，可以从|[密钥对](http://link.zhihu.com/?target=https%3A//console.aws.amazon.com/ec2/v2/home%23KeyPairs)|这里导入，否则你可以生成一个新的密钥，然后下载密钥对。
|启动后|，安装必要的软件，可创建自己的镜像（AMI）供开启竞价实例的时候调用。
|三、开启竞价实例
|打开|[EC2 控制面板](http://link.zhihu.com/?target=https://console.aws.amazon.com/ec2/v2/home)|，如下：
![](//upload-images.jianshu.io/upload_images/2301760-5c24c4b24d73f09b.png?imageMogr2/auto-orient/strip%7CimageView2/2)
|然后选择左侧栏的竞价请求（如果你点击"启动实例"的蓝色按钮，你将进行正常的实例开启进去之后选择操作系统，然后一路进行下去就行，过程中需要创建并下载秘钥对），进去之后，点击"请求竞价实例"的蓝色按钮。然后开始填写，其中AMI选择你想要的（选择Windows系统时一定要带有桌面！），其他按如下填写就行：
![](//upload-images.jianshu.io/upload_images/2301760-fc3e894a7a445665.png?imageMogr2/auto-orient/strip%7CimageView2/2)
|然后进入下一页，EBS 卷大小选大一点，比如500G，如果你选的 Windows 系统那就是 C 盘的大小。你也可以再添加其他卷（盘）。接下来 "密钥对名称" 那一栏，没有的话就创建一下，然后下载保存好，后面会用到。"安全组" 一栏可以 default 或者创建一个。 "有效时间" 自己决定就好，反正中途可以自行结束。然后点击审核按钮，然后点击启动。然后你会看到如下页面：
![](//upload-images.jianshu.io/upload_images/2301760-9125738b4b70d7e7.png?imageMogr2/auto-orient/strip%7CimageView2/2)
|看到状态激活后，点击红框框里的东西，然后你会看到如下页面（刚开始会进行2轮状态检查，全部通过后就是下面这张图）：
![](//upload-images.jianshu.io/upload_images/2301760-c75eaeacd51e4e0b.png?imageMogr2/auto-orient/strip%7CimageView2/2)
|一旦状态检查通过后，便开始计费了（不足一小时按一小时计费）。下面就可以进行远程桌面连接了。
|Note: 竞价实例虽然便宜，但是一旦开启后不能停止，只能终止，一旦终止所有的在远程系统上的配置、数据都将清空，你只能重新开启一次竞价请求。在开启实例之前，如果你有无法从网上直接下载的数据，那么一定要在开启实例之前把所有数据传到网上（如 Google drive 或者 Dropbox 或者百度盘），然后在服务器上下载这些数据，以节省开支。
|3. 远程桌面连接
|点击上面那张图里的 "连接" 按钮，然后下载远程桌面连接程序，顺便获取一下登陆密码，这时就需要用到之前下载的秘钥对。运行远程桌面连接程序，选择Administrator，然后输入密码。连接成功后，你就可以远程控制服务器了（桌面上的 Chrome，NV 和 Pycharm 都是我后装的）：
![](//upload-images.jianshu.io/upload_images/2301760-9cffc96ca30d833a.png?imageMogr2/auto-orient/strip%7CimageView2/2)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
