# 使用Autolayout实现自适应UITableViewCell - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月19日 14:06:38[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2243
今天在写医生端，查看报告模块时，发现个人端相同模块的tableViewCell竟然没有自适应，现在更改了题目内容后就成了这个样子：
![](http://upload-images.jianshu.io/upload_images/844086-a15da95e15993b24.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
题目一长就没了
突然想起来年前看到过很简单的使用autolayout实现自适应的方法，就动手做了一下。
我是使用storyboard来实现的，首先搭建界面：
![](http://upload-images.jianshu.io/upload_images/844086-4923c2e6b709cb27.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
界面
然后首先拉左侧绿色方块的约束，先确定一个view的固定位置，好为右侧的其他view提供参照：
![](http://upload-images.jianshu.io/upload_images/844086-4a5133c58bcc010e.gif?imageMogr2/auto-orient/strip)
左侧button约束.gif
因为这次设定为中间蓝色的label为自适应变化的，变化时高度是根据内容来改变的，所以中部label的上下约束应该相对于superview来拉：
![](http://upload-images.jianshu.io/upload_images/844086-e8b0081c72d30c43.gif?imageMogr2/auto-orient/strip)
中间contentLabel约束.gif
最后剩下的就是橙色和黄色的label啦，我把这两个设置的为距中部label的距离固定，左右分别与蓝色label对齐。步骤如下：
![](http://upload-images.jianshu.io/upload_images/844086-8fa791af84ce8c6a.gif?imageMogr2/auto-orient/strip)
上下label约束.gif
好了约束拉好了，把label的行数设为0：
![](http://upload-images.jianshu.io/upload_images/844086-37ed3b0914af3ca6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
行数
然后加上这两行代码(/(ㄒoㄒ)/~~ 没在sb里面找到相应的属性)：
```
//设行高为自动计算
    self.tableView.rowHeight = UITableViewAutomaticDimension;
    //预计行高
    self.tableView.estimatedRowHeight = 100;
     //在cellForRowAtIndexPath方法中加上，此方法不加也没有问题.
     [cell layoutIfNeeded];
```
到这里，简单地自适应cell就完成了。效果图如下：
![](http://upload-images.jianshu.io/upload_images/844086-cc66a8247201b705.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果图
[DEMO地址：UITableViewAutolayoutCell](https://github.com/SouthCity/iOSBlogDemo)
努力进阶中/(ㄒoㄒ)/~~，写的不对的欢迎指出。
文／南城_（简书作者）
原文链接：http://www.jianshu.com/p/efafb747f110
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
