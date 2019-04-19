# web语音发音实现【非异地版】 - Big Smile - CSDN博客
2016年08月14日 17:48:43[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1667
这两天做食堂的订餐系统的发音功能，颇有感受，借此分享。本文软件资源：[http://download.csdn.net/detail/tr1912/9597635](http://download.csdn.net/detail/tr1912/9597635)
# 一、语言的构思
        开始提出了语音的这个词汇，听着很是高大上的感觉，听起来实现的非常难，但是看了文档，用起来还是比较简单的。在这里我们用到了两种方式来播放语音，一种是微软自带的微软TTS引擎，一种是我自己在网上找的中国做的捷通华声语音系统。其实要是论易用性呢，微软自带的这个比较好，为什么呢？因为这个语音系统的基础是封装在win7以上的电脑里面的，所以一般的使用就是直接调用系统的类库就可以了。但是，要是扩展的话就需要很大的麻烦了，因为里面的扩展并不是很多。
         语音的构思方式主要是来自报什么，怎么报来思考的。由于只是在刷卡的页面需要提示人们菜价，因此呢简单一句话，刷卡就报。
# 二、代码的实现
        由于一开始用微软TTS语音的时候是用的同步的方法，我们还不会多线程的异步，所以我只是用了一小下，发现了问题，就是只要运行了语音功能，别的代码就暂停了，不能有很好的用户体验，所以放弃了这个方案，后来在网上查才看见有这个捷通华声语音系统，看网上的评论还不错，就下载了一个版本的试了试，起初看到他有多程，就觉得肯定可以实现，就去看Demo和文档了。
        这个文档的地方很难找，位置见下图（软件安装位置默认）：
![](https://img-blog.csdn.net/20160814172517814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20160814172537044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中里面的文件名称和作用一目了然不是吗？
![](https://img-blog.csdn.net/20160814173003925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         Bin里面放的是演示程序，所有的功能都从那里体现，而Examples下面是用各种语言和方法实现的例子，我就是这样参照的。文档在Bin的help下面。
       仿照C#的例子，我首先找到了 他们用的核心就是一个CS文件：![](https://img-blog.csdn.net/20160814173344723?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中这个CS文件是以下面的DLL为核心写的，方法都在JTTS_ML.cs中实现。详情见说明文档，下载地址在最上方。
具体使用：初始化——执行——结束
C#命名空间引用：
`using InfoQuick.SinoVoice.Tts;`
**          初始化代码：**
```
public bool InitJtts()
        {
            string strLibPath = null;       //高级配置文件的初始化
            string strSerialNO = null;      //初始化序列
            int iErr = 0;            //错误代码
            iErr = Jtts.jTTS_End();       //结束正在运行的
            iErr = Jtts.jTTS_Init(strLibPath, strSerialNO);     //初始化驱动
            if (Jtts.ERR_NONE == iErr || Jtts.ERR_ALREADYINIT == iErr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
```
        每次执行命令的时候都尽量初始化：
```
InitJtts();
Jtts.jTTS_Play("您好，您未订餐，请在等候区等待！", 0);
```
       这样就可以朗读了，如果想设置语速和音量则需要查文档，看参数然后设置，如：
```
Jtts.jTTS_SetParam(3, 7);       //语速7
Jtts.jTTS_SetParam(4, 7);      //音量7
```
     如果想判断当前的阅读状态（朗读，停止，空闲）则需要status这个方法来判断：
`    int ttsStatus = Jtts.jTTS_GetStatus();     //返回的INT类型代表什么状态需要看文档。`
     如果想清空当前缓存，则用这个：
```
InitJtts();
Jtts.jTTS_End();   //释放空间，要在没哟朗读的情况下
```
       我用到的函数就是这么多，具体怎么去安排，去实现，就得看自己的程序了。
