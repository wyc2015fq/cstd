# Intellij IDEA神器那些让人爱不释手的小技巧 - 纯洁的微笑 - CSDN博客
2018年06月14日 07:36:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：1159
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnq2h8QiaicAacfSDadkaFbxX3z0EhJ5lAErkQTvibmyIZc9Uw3OAA0TIlQDuv0Lhg01cqCknU0yceNgA/640?wx_fmt=png)
# 概述
之前写了一篇介绍IntellIJ IDEA的文章，主要是列出一些平时大家可能没用过或者没怎么用，但是又非常好用的`IntellIJ IDEA`小技巧。由于篇幅原因，只是列出了一小部分，那么接下来的这篇文章，会继续补充一些`IntellIJ IDEA`的小技巧。
# 别轻易自定义快捷键
有蛮多操作，`IntellIJ IDEA`并没有给我们设置默认快捷键，需要使用者自己去定义快捷键。比如说：
- 
Rebuild Project
- 
Compare With Branch
为了能在`IntellIJ IDEA`里进行`无鼠标编程`，很多程序员都会自定义快捷键，但是有三个地方你可能需要注意一下。
- 
经常会出现快捷键与其他应用的快捷键冲突的情况；
- 
自定义太多快捷键了，你也不太好记住；
- 
使用其他同事的IDEA时(比如说帮忙定位问题)，你自定义的快捷键没法用。
其实在`IntellIJ IDEA`里的每个操作，都可以看出一个`action`。我们可以使用`ctrl+shift+a`来输入我们要使用的操作。比如说，上面提到的`Rebuild Project`。你可以先使用`ctrl+shift+a`快捷键，然后输入`Rebuild Project`并回车,即可执行你要的操作。 
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbOQWjVLPeEfZcGGuh8smokY7hCibudC7ByTF92eiaqtUuL28uibXvdTomQ/640?)
对我自己来说，除了基础的快捷键，`ctrl+shift+a`是我用最频繁的快捷键了。
# 使用ctrl+alt+h要小心
`ctrl+alt+h`非常好用,但是有个坑,当同一个方法里,调用某个方法多次的时候,比如说下面的代码：
```
public class TestService {
   public void test1() {
       System.out.println("aa");
   }
   public void test2() {
       test1();
   }
   public void test3() {
       test1();
       //无数业务操作后,再次电影test1()方法
       test1();
   }
}
```
如果我们想知道有哪些地方调用了`test1()`方法，使用`ctrl+alt+h`无法正确列出来的。因为`ctrl+alt+h`只能告诉你调用的层次。 
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbW9eyYSuqJJToTjGibic9VRlOicH9NoeZqZxke4NP2fFwJYPDJMz2Fw1og/640?)
`ctrl+alt+h`只是会在某个隐蔽的地方，告诉你，`test3()`方法调用了`test1（）`方法两次。这样就很容易坑到开发者，因为大部分人可能不太注意后面的调用次数，导致改`bug`的时候，以为全部都改了呢？
如果你想精确的列出到底哪些地方调用了`test1（）`方法，你需要使用`alt+f7`这个快捷键。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbJHOgJVsWBHOIYVibsK9eCtHU44BVNh3luDjZ4JPfxPhZdmkcwkAdkHg/640?)
尤其是我们在阅读极其复杂的业务代码时，使用`alt+f7`就非常合适。
当然`alt+f7`也可以作用在变量上，列出某个类里，哪些地方使用了该变量。
# ctrl+alt+h被问的最多的两个问题
经常有同事和网友问我。
> 
使用ctrl+alt+h怎么跳转到源代码，又如何重新回到ctrl+alt+h对应的视图里面。
## 调转到源代码
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbI1mP7HcdibrDfVhHq6bPMfWvibIKO8auqZfvGqiaE3wMBSK4te5sENbqw/640)
其实很简单，当你使用`ctrl+alt+h`后，使用向下或者向上箭头，选择某个调用，然后按下`f4`即可跳转到源代码。
## 如何回到ctrl+alt+h视图
这个真心被问了好几百遍，其实很简单，当你使用`f4`跳转到源代码后，直接使用`alt+8`就可以跳回去了。就又可以继续看下一个调用的地方了。
# 快速找到Controller方法
如果你的项目里有非常多的`controller`，里面有非常多的`http`或者`resful`方法。如何快速找到这些方法呢？这个时候，`ctrl+alt+shift+n`就可以派上用场了。
比如说，你依稀记得入账单相关的接口，都有个`bill`的url路径，那么使用`ctrl+alt+shift+n`后，直接输入`/bill`即可。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbFRtib4CbkVNR6C0GfkiaK4DIgc0hyR3P9UCJWtn9J85YPQ7JUEI6P3ibA/640)
当你在成千上万的`Controller`里寻找方法时，这一招就可以大大提高效率。
# 了解项目关键业务流程方法的利器-bookmark
在一些创业公司里，很多核心的模块都是放置在同一个项目里的。比如说，订单相关的接口，支付相关的接口，商品相关的接口。这个时候，你可以将这些关键业务方法，使用`bookmark`统一放置到某个地方，方便你阅读。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbWsyicdyAurxnJdlp377c2qtK8V25BSc668E7pbEOgnwpzB1ib5VmycEw/640)
那么如何使用快捷键来达到上面的效果呢？
```
public class TestService {
   public void test1() {
       System.out.println("aa");
   }
   public void test2() {
       test1();
   }
   public void test3() {
       test1();
       test1();
   }
}
```
比如像上面的方法，我想将`test1()`方法放置到`bookmark`里，可以通过如下操作来完成： 
1、使用`ctrl+f12`,列出该类的所有方法，然后输入`test1`，将光标定位在`test1`上； 
2、按下`f11`,将`test1()`加入到`bookmark`； 
3、按下`shift+f11`，将`bookmark`列表弹出来; 
4、按下`ctrl+enter`修改`bookmark`名字。
# 只留下一个tab
这个是我目前正在用的，就是整个工程里面，只有一个代码tab。也即是说，无论你打开多少个文件，都是在同一个tab里面显示。如果这样设置了，有些网友可能会问,我想看看我最近操作哪些类了，怎么看？ 可以直接使用`ctrl+e`来显示最近操作的文件。
我是比较推荐只是保留一个代码tab的，非常简洁。如果每打开一个文件，就是一个新的tab，很快你就会乱掉，而且还得关闭部分tab。
可以通过下面的方式来设置成用一个tab显示代码。按下`ctrl+shif+a`,然后输入`Editor Tabs`，然后回车进入编辑页面。 
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbbWRo14xLMD20XbJLvWGCOFqhCqALnlibVn15ZhpBW7ANfpwrHKeWRKw/640)
然后在`Placement`那里,选择`None`
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbT6wWFBGtX6799wrLpJtmvOicEqzgY6pxhUibCZLqLAD6CwW8KNUmzRicw/640)
# 如何阅读又长又臭的代码
由于历史原因，项目里总会存在那种无法理解的，又长又臭的业务代码。阅读这种代码，简直就是一种煎熬。但是在`IntellIJ IDEA`里，只要使用5个小技巧，便可大大提高阅读质量和速度。
## 创建任意代码折叠块
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbFeTXnQ90ktVAlDBLrvVicib1xlbxQTz0p6MFSibcOu8ad8mkjEboIgPUw/640)
像上面的for循环，我想直接将其折叠起来，因为代码太长的时候，使用折叠块，可以帮助你快速理清代码的主脉络。
可以将光标定位在for循环的左大括号里，然后使用`ctrl+shift+.` 即可。 
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbAxpNcjv1NjSX6nyOnibRTPdWiacsJ8D0Hf1Eh3NdjOibE1QF9F9OMBszw/640)
如果你想让这个折叠快消失，直接使用`ctrl` 加上一个`+`即可。
## 大括号匹配
这个也非常有用，因为代码太长，某个for循环，可能已经撑满整个屏幕了。这个时候，找到某个大括号对应的另外一边就很费劲。你可以将光标定位在某个大括号一边，然后使用`ctrl+]`或者`ctrl+[`来回定位即可。
## ctrl+shift+f7结合f3
`ctrl+shift+f7`可以高亮某个变量，而且随着鼠标的移动，这个高亮是不会消失的(`这个很重要`)。然后使用`f3`找到下一个使用该变量的地方。
## 使用ctrl+shift+i
这个也是阅读长段代码的法宝，当你阅读的代码很长的时候，突然想看代码里某个类的定义，那么直接使用`ctrl+shift+i`,就可以在当前类里再弹出一个窗口出来。比如说： 
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbfJhsytLgy34Kgb8hbMMWpRSL9ylHN3Dm9L45OmldmLpYjibBU9PIqxw/640?)
在这个代码块里，你想看看`TestTemp`类的定义，那么将光标定位在`TestTemp`上，然后直接使用`ctrl+shift+i`，就会弹出如下的窗口。 
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbOv7VHsiblCfZ9oMfD3pIfXyXsdE84ibWtJOhkp2Fnx1oOIgdOSJ1ZicVg/640?)
按下`esc`，可以关闭这个窗口。
# 使用alt+f7
这个我在上面已经介绍过了。可以列出变量在哪些地方被使用了。
结合这5个技巧，相信可以大大提高长段代码的阅读效率。
# 跳到父类接口
我们经常会定义一个`service` 接口，比如说`UserService`,然后使用一个`UserServiceImpl`类去实现`UserService`里面的接口。
```
public interface UserService {
   void test1();
}
```
```
public class UserServiceImpl implements UserService {
   @Override
   public void test1() {
   }
}
```
那么在`UserServiceImpl`里的`test1()`方法上，如何跳转到`UserService`的`test1()`,直接使用`ctrl+u`即可。
# 后悔药
如果修改了部分代码，突然觉得不合适，使用`ctrl+z`回滚掉后。突然又觉得刚才的修改是可以的。那你可以使用`ctr+shift+z`再次恢复你刚才修改的内容。
# 切换皮肤最快的方式
可以直接使用`ctrl`,然后加上一个`,就可以立刻弹出如下界面： 
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbuJac5tQ7dwrVjrvnaovGucpNBwDiauk0LiaH2ZHhCoaj6UYotYMQo7Vg/640?)
选择`Color Scheme`，然后回车，就可以弹出修改皮肤的窗口。 ![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPSH11OTichTx6YYOFbx5NjbdyKGSec1ayiamfRyTj0YTSFebOicbI3OjJ3SSiafqibNU9oAPbn7HAribDg/640?)
> 
**原文出处：blog.csdn.net/linsongbin1/article/details/80560332**
END
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpcR0YpEk9cSITT9srthjLfnyo5a7BroaQnXA0uMk8tvhZtMVLkQDYSsXP1ibtlmYibkvBLRRe6ibD7g/640?wx_fmt=png)
更多精彩内容，尽在阅读原文
