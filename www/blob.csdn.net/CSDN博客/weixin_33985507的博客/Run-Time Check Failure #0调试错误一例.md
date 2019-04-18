# Run-Time Check Failure #0调试错误一例 - weixin_33985507的博客 - CSDN博客
2014年06月12日 21:19:07[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
今天在调试自己的Cocos2d-x游戏时出现如下错误提示：
Run-Time Check Failure #0 - The value of ESP was not properly saved 
across a function call.  This is usually a result of calling a function 
declared with one calling convention with a function pointer declared 
with a different calling convention.同时出现如下错误提示对话框：
![](http://hi.csdn.net/attachment/201201/25/0_1327486107LXb7.gif)
颇感疑惑！因为很长时间没有遇到这样的错误提示了。也许习惯了，一下就打开网络先搜索一下，[在这里](http://blog.csdn.net/kingli_2008/article/details/7215175)。从作者代码来看，是函数调用格式发生了变化所致。
于是，结合提示并根据自己的情形仔细分析，看上去是函数调用时有关参数栈不一致所致。忽然明白了－－这部分代码已经是一个多月前的了，今天作了粗糙的修改，竟然把这个给忘记了。
我的第一部分代码如下：
```cpp
Label* guidLabelText = dynamic_cast<Label*>(ul->getWidgetByName("guidLabelText"));
    guidLabelText->setTouchEnabled(true);
    guidLabelText->addTouchEventListener(this,toucheventselector(TutorialScene::nextCallback));
```
对应的自然要有回调函数的正确格式了，没有细细修改，就胡乱地复制代码了。
```
void TutorialScene::nextCallback( CCObject* pSender )
{
}
//修改一下便OK了，如下所示：
void TutorialScene::nextCallback( CCObject *pSender, TouchEventType type)
```
