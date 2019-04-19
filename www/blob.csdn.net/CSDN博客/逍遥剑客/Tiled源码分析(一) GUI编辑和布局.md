# Tiled源码分析(一): GUI编辑和布局 - 逍遥剑客 - CSDN博客
2014年01月14日 23:55:22[xoyojank](https://me.csdn.net/xoyojank)阅读数：3154
MFC, WinForms, WPF, Qt算是都折腾过, 就差WxWigets了
比较下来, 做游戏(引擎)工具开发的话, 最重要的是易上手, 好维护, 开发快, 综合下来还是滚回C++阵营, 选择了Qt
不用不知道, 其实传统的Qt Widgets用起来跟WinForms非常像, 甚至Qt Quick的特性跟WPF有的一拼
想当初强烈地想用C#, 很大部分原因是WinForms那个可视化GUI编辑器比较吸引人
不过QtDesigner虽说还没有生成事件代码的功能, 但是对于C++来说, 已经很不错了, 再说还有signal-slot的支持
# 控件布局
回归正题, Tiled中的主要对话框都是.ui文件, 用QtDesigner就可以打开
![](https://img-blog.csdn.net/20140114232112593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140114232137296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
仔细观察发现, 工具栏上的按钮只有固定几个, 好多窗口也没有在.ui里找到
虽说很多人会鄙视可视化的UI编辑器, 但是不得不说, 有些地方的确很方便, 但所有界面都依赖这个就份了
因为从架构设计的角度来讲, 越抽象, 其实就离UI编辑器越远
我想这也是Tiled的一些功能模块窗口没有使用设计器的原因
.ui文件与C++类之间的关系就不说了, 之前都用过
# 功能面板
通过[Tiled截图](http://blog.csdn.net/xoyojank/article/details/18278313)来看, 其它功能窗口布局都很简单, 而且都是Dock Window
所以在代码中可以看到它们都从QDockWidget派生的
![](https://img-blog.csdn.net/20140114233258203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140114233851812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 状态栏
状态栏其实就是一个窗器, 可以创建一些控件扔进去
![](https://img-blog.csdn.net/20140114234806953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 菜单和工具栏命令
这里也是基于Qt比较不错的的QAction, 比MFC的ID方式强太多了. 工具栏那些没在Designer中出现的按钮是跟操作/插件功能相关的, 后续再分析
