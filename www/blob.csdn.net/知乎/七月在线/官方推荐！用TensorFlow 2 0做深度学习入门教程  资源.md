# 官方推荐！用TensorFlow 2.0做深度学习入门教程 | 资源 - 知乎
# 



最近，TensorFlow 2.0版的开发者预览版发布没多久，这不，又有一篇优质教程来了。

前Youtube视频分类的产品经理、Hands-On MachineLearning with ScikitLearn and TensorFlow一书的作者Aurélien Geron

就发布了用tf.keras和TensorFlow2.0做深度学习任务的Jupyter教程。



![](https://pic4.zhimg.com/v2-0983d44f69d0719f035241ef484495e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='591'></svg>)



教程一发，网友点赞转发不在少数，不少网友表示教程简洁重点“真的很棒”！



![](https://pic4.zhimg.com/v2-2e204279935055e245c0145913d7b3e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='241'></svg>)
## **安装**

教程开始——

如果之前你没安装过git那就先开始安装吧，输入以下命令复制存储库。



![](https://pic1.zhimg.com/v2-1d5deafb90e6b7847d9acbae56e807b4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1052' height='168'></svg>)
如果熟悉Python并知道如何安装Python库，就继续安装NumPy、Matplotlib、Jupyter和TensorFlow吧，可以在requirements.txt中查看详细信息，然后跳转到Starting Jupyter部分。

相关地址：

[https://github.com/ageron/tf2_course#starting-jupyter](https://link.zhihu.com/?target=https%3A//github.com/ageron/tf2_course%23starting-jupyter)

很多人可能已经安装了2.0的预览版，有些人甚至安装了3.0版本，所以先检查你的Python版本，确定下自己的是哪一种：
![](https://pic1.zhimg.com/v2-19d92958dc03a0ea189da79e50dfa81c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='672' height='128'></svg>)
这个教程需要Python 3.5或者Python 3.6，TensorFlow目前还不支持Python 3.7。你可以稍微调整后，在Python 2中运行这段代码，但还是建议升级到Python 3。

这时的第一种选择因系统而异，在Windows或MacOSX系统上可以从官网下载，MacOSX系统也可以使用MacPorts或Homebrew。Linux要用系统封装。

官网地址：

[https://www.python.org/downloads/](https://link.zhihu.com/?target=https%3A//www.python.org/downloads/)

比如，在Debian或Ubuntu中输入：
![](https://pic1.zhimg.com/v2-584c05acd2bef15fb32dd64f64bc5198_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='716' height='136'></svg>)
第二种选择是下载并安装Anaconda，包含Python和很多库。

## **Anaconda的使用**

如果刚刚你选择了其二Anaconda，现在就能创建本教程专用的独立Python环境了：
![](https://pic1.zhimg.com/v2-6185935ffad8dafc2adf7019881a0404_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='138'></svg>)
新的Python 3.6环境tf2course出来后激活它，此外，我们还需要继续安装：
![](https://pic1.zhimg.com/v2-fc8e6672c9b10e5acb4b0fcf7a30a928_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='95'></svg>)
这一步是在tf2course环境中安装TensorFlow 2，如果上一步没有创建这个环境，需要删除-n tf2course选项。

好了准备阶段马上结束，下面即将步入Starting Jupyter的校车。

## **pip的使用**

如果上一步你没有使用Anaconda，则需要安装本课程所需的几个科学Python库：NumPy，Jupyter，Matplotlib和TensorFlow。

因此可以借用Python的集成封装系统pip，你也可能更喜欢使用系统自己的封装系统。使用系统封装系统优势也很明显，就是库版本与系统其他软件包之间存在冲突的风险较小。

而pip的优点是可以容易地创建包含不同库和不同库版本的多个Python环境，且每个环境都相互隔离。

如果想用pip安装所需的库，试试下面这些指令。

首先确保安装了最新版本的pip：
![](https://pic1.zhimg.com/v2-d1d8a7c8e29ca3b7a1c96b007268bfc8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='736' height='102'></svg>)



接下来需要创建隔离环境，这样做可以为每个项目提供不同的环境，方便以后的不同任务：
![](https://pic3.zhimg.com/v2-767b59738a154cb2945e5e8ecf712932_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='808' height='92'></svg>)
这是系统会自动创建一个env模块来调用当前目录，也包括使用python 3隔离出的单独环境。如果你在系统上安装了多个版本的Python 3，可以将which python3替换成你希望使用的可执行文件路径。

在每次激活环境前，都需要你运行此命令：
![](https://pic3.zhimg.com/v2-767b59738a154cb2945e5e8ecf712932_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='808' height='92'></svg>)



接下来用pip安装所需的Python安装包：
![](https://pic1.zhimg.com/v2-3fa3da291cdce1b3406cafa46250e3b4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='964' height='114'></svg>)
大功告成！你只用启动Jupyter就可以了。

## **启用Jupyter**

打开Jupyter，输入下面这段代码：
![](https://pic4.zhimg.com/v2-efa010b12e4b23c79144cc1cf451af87_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='746' height='108'></svg>)



现在打开你的浏览器可以看到Jupyter的树状图，如果浏览器没有自动打开，你需要先跳转到localhost：8888。

localhost：8888地址：

http://localhost:8888/tree

接下来单击*.ipynb打开Jupyter笔记本。好了，祝你在TensorFlow 2中玩得开心！

作者提示，因为TensorFlow 2.0版还未正式发布，所以预览版与最终版可能存在一定区别，正式发布后还请各位稍加留意。

## **传送门**

GitHub教程：

[https://github.com/ageron/tf2_course](https://link.zhihu.com/?target=https%3A//github.com/ageron/tf2_course)

> 铜灵 编译整理  量子位 出品

**今日学习推荐：**
为了让小伙伴们更好的学习，我们为你准备了**机器学习、CV就业、推荐系统就业，VIP年会员、无人驾驶**相关方面知识，可免费试听喔，详情点击下方学习~

[机器学习集训营 第八期「线上线下结合，线下在北上深广杭沈济郑成武西长十二城」- 七月在线​www.julyedu.com![图标](https://pic2.zhimg.com/v2-0d5b82db757beb2dcc9eb1c23938bf99_180x120.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/weekend/train8%3Fv%3Dm1)[CV就业班 第一期「BAT大咖小班教学，保证就业」- 七月在线​www.julyedu.com![图标](https://pic2.zhimg.com/v2-4e6d94ebbf70294325a2e6e6a9f27d61_ipico.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/Employment/gjob)[推荐系统就业班 第二期「BAT大咖小班教学，保证就业」- 七月在线​www.julyedu.com](https://link.zhihu.com/?target=http%3A//www.julyedu.com/employment/rs2)[VIP年会员新春特价：¥3299包年全平台在线课程和GPU云实验平台 - 七月在线​www.julyedu.com![图标](https://pic2.zhimg.com/v2-f1a1207cd9519e899fde0c0c54701b61_180x120.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/vip/newvip)[无人驾驶实战 [首次全面公开L4级别的全自动驾驶技术，帮我砍价直降100多] - 七月在线​www.julyedu.com![图标](https://pic3.zhimg.com/v2-a5f964d8f54e9a2a46dafeec421c6d12_ipico.jpg)](https://link.zhihu.com/?target=https%3A//www.julyedu.com/course/getDetail/136)

