# 自定义ASP.NET服务器控件与用户控件 - 左直拳的马桶_日用桶 - CSDN博客
2012年08月09日 12:35:24[左直拳](https://me.csdn.net/leftfist)阅读数：1702标签：[服务器																[asp.net																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=asp.net&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
两种都是自己写的，有什么区别吗？什么情况下该写服务器控件，又什么情况下该用用户控件。
服务器控件：
编译出来是DLL，适用于不同的网站，只须引用即可。但开发起来比较麻烦，需要重写不少东西。其形式，就是个类库，开发过程中没有什么界面好讲。
用户控件：
是个.ascx后缀的文件，只适用于同一个网站，提高重用性，加强封装而已。不过开发起来比较方便，用鼠标拖来拖去，写几行代码，甚至不写都可以。但不适合几个网站共同使用。如果非要共用，只能是将该控件文件拷贝到每个网站下，网站间的重用性是不够的。
