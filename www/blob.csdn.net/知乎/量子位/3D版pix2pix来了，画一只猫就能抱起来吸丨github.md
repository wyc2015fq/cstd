# 3D版pix2pix来了，画一只猫就能抱起来吸丨github - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

上帝画了一只猫，程序员把它变成了真猫，还抱起来吸了一口。

是的，现在pix2pix图像转换可以在3D模型上实现了。
![](https://pic4.zhimg.com/80/v2-54d15c72ba0a2b533954b8939971893b_b.jpg)pix2pix生成3D猫https://www.zhihu.com/video/1025437162237358080
推特博主高桥启治郎（Keijiro Takahashi）最近上传了一个3D猫猫，用pix2pix来给3D模型加上一层“猫皮”，而3D模型则用的是（打游戏用的）Unity。
![](https://pic3.zhimg.com/v2-06042cc2ecfcdea67ede559cc196417e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='294'></svg>)△ 这个logo是不是经常在游戏启动页面上见到？
所以，这样的3D猫是怎么做出来的呢？

## **学会画画，你一定会有猫的**

上帝说，要有猫。

于是，高桥拿出了Sketch Pad。

Sketch Pad是一个吸猫神器，想吸什么样的猫就有什么样的猫，想让猫摆成什么姿势就可以让猫摆成什么姿势。

前提是，你画得出来。

比如这样：
![](https://pic3.zhimg.com/v2-b5fa9a6f586084d22476bf04929cf54e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='338' height='128'></svg>)
或者这样：
![](https://pic3.zhimg.com/v2-f204f4dd4a1ffcf509b7e9efc4743a7e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='338' height='128'></svg>)
## **系统和模型准备**

除了Sketch Pad，你还需要装好Unity 2018.1以及计算着色器功能，包含DX11、 Metal和Vulkan。

另外，因为作者没有准备模型，所以最好先下载一个训练好的edges2cats模型，最好先学会pix2pix-tensorflow训练模型。

## **猫是如何成精的？**

罗马不是一天建成的，猫猫不是一天成精的。

高桥从本月初开始就一直在研究如何用Unity结合pix2pix来生成3D猫。

但猫的生成并非一帆风顺的，刚开始，是这样的混沌状态。
![](https://pic3.zhimg.com/v2-8faf90c4596363a1c5fdb9553295944a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='336' height='329'></svg>)
突然就有了出息，知道如何“做猫”。
![](https://pic4.zhimg.com/v2-c6435b1e3281cacd1c1deff61fd566b3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='346' height='382'></svg>)
外形有了，但精神状态似乎不太好，所以就让它学会了运动。
![](https://pic1.zhimg.com/v2-85330cbeca39c8bf4b14546bcb6f5430_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='421' height='418'></svg>)
最后，才能修炼成真猫。
![](https://pic3.zhimg.com/v2-f77d9969ea72e93c8ee57d9e358e82c6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='294' height='298'></svg>)
## **One More Thing**

其实作者高桥启治郎一直沉迷用Unity生成各种奇怪的玩意，从场景、雕塑到卡通角色。

为什么他对Unity爱的这么深沉？

因为……Unity给他发工资。

高桥是一位游戏开发工程师，之前在索尼工作了十年，后来跳到了Unity，就专注创造各种3D作品。比如下面这个萌妹子**Unity酱**就是他参与创作的。
![](https://pic2.zhimg.com/v2-4221e70d23d1e51eb8c21e770cfba275_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='357'></svg>)
觉得妹子萌的话可以戳github围观开源代码：

[https://github.com/keijiro/unitychan-crs](https://link.zhihu.com/?target=https%3A//github.com/keijiro/unitychan-crs)

内附妹子唱歌跳舞的油管视频，有好心的up主搬运到了B站：

[https://www.bilibili.com/video/av7622460/](https://link.zhihu.com/?target=https%3A//www.bilibili.com/video/av7622460/)

## **传送门**

github：

[https://github.com/keijiro/Pix2Pix](https://link.zhihu.com/?target=https%3A//github.com/keijiro/Pix2Pix)

高桥启治郎推特：

[https://twitter.com/_kzr/status/1041611079082663937](https://link.zhihu.com/?target=https%3A//twitter.com/_kzr/status/1041611079082663937)

就酱，吸猫愉快~

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




