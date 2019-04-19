# jabberNet 发送出席信息 - 左直拳的马桶_日用桶 - CSDN博客
2014年06月19日 21:45:19[左直拳](https://me.csdn.net/leftfist)阅读数：1908标签：[jabberNet IM XMPP 即时](https://so.csdn.net/so/search/s.do?q=jabberNet IM XMPP 即时&t=blog)
个人分类：[.NET																[IM																[学习笔记](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/2457179)](https://blog.csdn.net/leftfist/article/category/94497)
没代码我说个J8：
```
public void Presence(User.EStatus status)
{
    string statustxt = "";  //说明文字。比如，离开的原因：外出吃饭
    string show = "";       //真正起作用的命令。
    switch (status)
    {
        case User.EStatus.OnLine:
            show = null;
            break;
        case User.EStatus.Chat:
            show = "chat";
            break;
        case User.EStatus.Busy:
            show = "dnd";
            break;
        case User.EStatus.Leave:
            show = "away";
            break;
        default:
            show = null;
            break;
    }
    jc.Presence(PresenceType.available, statustxt, show, jc.Priority);
}
```
这样子的话，我方切换状态，对方也能收到相应事件，做出相应的，关于我的状态调整。
具体的命令和说明，可以参照XMPP：
[http://wiki.jabbercn.org/RFC6121#.E5.87.BA.E5.B8.AD.E4.BF.A1.E6.81.AF.E8.AF.AD.E6.B3.95](http://wiki.jabbercn.org/RFC6121#.E5.87.BA.E5.B8.AD.E4.BF.A1.E6.81.AF.E8.AF.AD.E6.B3.95)
