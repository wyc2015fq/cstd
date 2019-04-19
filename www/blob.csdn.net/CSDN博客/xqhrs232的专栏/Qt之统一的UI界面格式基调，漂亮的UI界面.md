# Qt之统一的UI界面格式基调，漂亮的UI界面 - xqhrs232的专栏 - CSDN博客
2016年05月20日 15:08:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1472
原文地址::[http://www.cnblogs.com/appsucc/archive/2012/11/05/2754767.html](http://www.cnblogs.com/appsucc/archive/2012/11/05/2754767.html)
相关文章
1、使用QT designer 设计界面----[http://wenku.baidu.com/link?url=OdfnV0SK7mUfvIk0hX_VfGaDwmlqXX-yTA9ohAl9pwmj9_3IoJBFJmf87T6FqhYYzb0NUNM0J7UEWOa5nFg3xV51Kqy_rstGSPw3gXqavay](http://wenku.baidu.com/link?url=OdfnV0SK7mUfvIk0hX_VfGaDwmlqXX-yTA9ohAl9pwmj9_3IoJBFJmf87T6FqhYYzb0NUNM0J7UEWOa5nFg3xV51Kqy_rstGSPw3gXqavay)
2、Qt Designer入门教程 初学者必看----[http://mobile.51cto.com/symbian-271584.htm](http://mobile.51cto.com/symbian-271584.htm)
最近一直在忙，忙工作，忙私活；也很久没有写博客了；
　　今天主要谈谈Qt UI界面统一样式(格式基调)的问题；例如在window系统上，几乎所有的窗口都有标题栏和状态栏以及中央部件，而且每一个标题栏和状态栏以及中央部件样式都保持一致的；但是在实际开发中我们又不想使用window系统自带的标题栏和状态栏等，因此需要自定义标题栏和状态栏以及中央部件，同时保持项目中的所有窗口的标题栏和状态栏和中央部件的格式样式一致，这样在项目开发中，既可以保持拥有漂亮的ＵＩ界面，又能大大的提高开发效率，因为你不需要在每一个窗口中对标题栏和状态栏以及中央部件进行重复开发了，例如不需要对每一个窗口进行鼠标移动判断从而对窗口进行移动，使用户更加专注于业务；下面谈谈具体怎样实现：
**一、将标题栏和状态栏以及中央部件集成于一个类中**
　　单独作为一个类，提供标题栏和状态栏以及中央部件指针成员；
```
class QBaseWidget : public QFrame
{
QVBoxLayout *m_pMainLayout;     //主布局
QTitleBar *m_pTitleBar;         //标题栏
QWidget *m_pContentWidget;      //内容区域，中央部件
QStatuBar *m_pStatuBar;         //状态栏
}
```
　　代码如上所示，当然QTitleBar和QStatuBar都是自定义的类，需要自己处理，如增加最小化按钮，最大化按钮按钮，关闭按钮，鼠标位置判断以及窗口多动，这样你可以做成你自己想要的任何标题栏和状态栏以及中央部件，m_pContentWidget成员变量只是保持一个指针而已，以后如果你要把你做的内容窗口嵌入到该类中，只是赋值即可，m_pMainLayout就是类的主布局了；写到这里，大家觉得是不是有点像QMainWindow类的处理过程了；
**二、为标题栏和状态栏提供修改接口**
　　在有些窗口中，可能不需要状态栏，例如程序的关于对话框，因此这时就应该提供外部接口供调用者使用，表示标题栏和状态按是否需要；
```
QBaseWidget(bool bShowTitleBar,QWidget *pContentWidget,bool bShowStatuBar,QWidget *parent = 0);
```
**三、为标题栏按钮提供修改接口**
　　并不是所有的窗口都有最大化和最小化以及关闭按钮，例如对话框窗口，就只有关闭按钮了，那么最小化以及最大化按钮当然就不要了；所以需要提供外部接口给调用者使用：
```
void setWidgetBtnShow(bool bMinBtnShow,bool bMaxBtnShow,bool bCloseBtnShow);
```
　　参数的含义依次表示是否显示最小化按钮，是否显示最大化按钮，是否显示关闭按钮.
**四、将自己的内容部件嵌入到QBaseWidget中**
　　上述只是讲解了怎么修改自己的外观；当然，最主要的还是要把自己的内容部件潜入到其中，例如你在Qt Designer中设计了一个窗口，你想把这个窗口套在其中，则：
```
QBaseWidget(bool bShowTitleBar,QWidget *pContentWidget,bool bShowStatuBar,QWidget *parent = 0);
```
　　只要将自己设计的内容部件的指针赋值给pContentWidget即可，这样就自然套上了统一的标题栏和状态栏以及中央部件样式.
**五、统一外观设置**
　　在整个项目中，当然希望所有的窗口界面的色彩格调保持一致了，这时只要美化上述中所述的标题栏，状态栏以及中央部件即可，而且只要写一处代码即可哦.
**六、程序示例**
　　最后，给大家截图看看实现效果，统一格式的标题栏，状态栏以及中央部件的格式；由于工程是用于实际项目中，所以这里只截取了用户登录对话框以及关于对话框界面，其他界面不提供，谢谢
![](http://pic002.cnblogs.com/images/2012/371367/2012110510535564.png)
![](http://pic002.cnblogs.com/images/2012/371367/2012110510540927.png)
**七、界面设计**
　　用户只需要设计中央部件即可，其他的就不要了，如上图中的用户登录对话框，用户只需要在Qt Designer中设计用户登录对话框即可，然后嵌入其中即可，如下图所示：
![](http://pic002.cnblogs.com/images/2012/371367/2012110510550010.png)
**八、优势**
1、统一的样式格调
2、漂亮的UI界面
3、大大节省开发时间
