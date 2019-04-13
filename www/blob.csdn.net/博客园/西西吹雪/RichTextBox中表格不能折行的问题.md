
# RichTextBox中表格不能折行的问题 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[RichTextBox中表格不能折行的问题](https://www.cnblogs.com/watsonyin/archive/2006/12/02/579943.html)
数据库编程中，有时要保存一些复杂的文档，如果用HTML，可以保存文档的格式，但文档中的图片却要单独存放，比较麻烦。如果用RTF格式的话，就可以将所有内容存为一个整体，放到数据库中。但vs自带的RICHTEXTBOX控件，在处理表格时，如果单元格中的内容太多，不会自动折行，郁闷。打开写字板看看，人家是可以的，奇怪了。
上网查了一下，说写字板用的可能是msftedit.dll，而不是riched20.dll。我试着把system32目录下的msftedit.dll复制一份，改名成riched20.dll，替换原有的文件，试一试，还是不行。我想以前VB编程的时候也有一个COM的richtextbox，不知道还行不行，试一下，行了！太高兴了！但再打开word，文档不能可存，提示“内存不足，是否保存为挽救的文档”，显示是riched20.dll被替换了的缘故，恢复了该文件，word就正常了。
看来这是一个矛盾。我“灵机一动”，将msftedit.dll复制到应用程序目录下，改名为riched20.dll，运行，正常！
归结起来，有两点：
1、不用vs自带的richtextbox 控件，而是用com的richtextbox组件（即windows richtextbox control (sp4)  richtx32.ocx 好像是这个文件名，在此system32目录下，没有话，到装了vb的机器上copy一个）；
2、将msftedit.dll复制到应用程序目录下，改名为riched20.dll。如果你没有msftedit.dll文件，到装了windows xp或windows 2003的机器上复制一份。





posted on2006-12-02 17:38[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=579943)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
