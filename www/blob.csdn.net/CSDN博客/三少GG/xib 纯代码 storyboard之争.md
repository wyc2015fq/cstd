# xib/纯代码/storyboard之争 - 三少GG - CSDN博客
2016年02月20日 15:00:39[三少GG](https://me.csdn.net/scut1135)阅读数：831
**选xib**
**###**
# [代码手写UI，xib和StoryBoard间的博弈，以及Interface Builder的一些小技巧](http://onevcat.com/2013/12/code-vs-xib-vs-storyboard/)

**[https://www.zhihu.com/question/20256649](https://www.zhihu.com/question/20256649)**
什么时候使用storyboard？
- 组织多种view的层级关系，也就是传说中的segue。
- 使用一些列表或表格单元的模板的时候。
- 除过不能使用storyboard的情况下尽量用storyboard。
什么时候不建议使用storyboard？
- 动态或复杂布局，这时候可能需要用代码来计算相关view的位置。
- 如果一个view已经用NIB或代码实现
上面几乎所有的文字都在描述storyboard，下面讨论一下NIB（本人对nib/xib不太熟，有错误的地方欢迎指正）。
什么时候使用nib？
- 模态框（如登录提示什么的）
- 可复用视图组件或模板
什么时候不推荐使用nib？
- 有动态内容的视图
- 不方便在IB中进行设计的试图
然后说说代码。
什么时候使用代码？
- 动态布局
- 试图特效
什么时候不推荐使用代码？
什么时候使用代码都是一个好方法，但不一定是最好的。
以上
