# Juce之旅-第一个例子（图形窗口） - DoubleLi - 博客园






以这么说现在的人越来越妖精了，本来软件吗，要的是简单稳定实用，但是看现在的趋势是越来越多人注重界面的美化和和效果。比如IM类软件，QQ，飞信还有土的掉渣的MSN等，前两天看了一下YahooUI，觉得不错，ICQ的界面也可以。Windows上界面技术基本都是DirectUI或者近似于DirectUI，而迅雷7使用了WPF做界面，很漂亮，虽然有很多bug，不过可以理解。WPF估计是未来donet平台的主流开发技术了。这样做就给用户惯了个坏毛病，如果你的软件不够漂亮，就会引来很多非议或者干脆扔掉不用。呵呵， 没有办法。其实我的工作本身和界面美化没什么关系，纯粹出于兴趣。研究过很多流行的库，不过问题是开源的很难作出好的效果，效果不错的却不开源，当然我不是说不能用纯win32或者MFC做出牛X的界面哈，只是一种权衡而已，国内现在有很多公司比如UIPower，UIEasy还有直接叫DirectUI的公司，都是收费的，而且价格不是一般的贵。记得在东软的时候，项目中买了一个Skin++的授权，MD7千一个，呵呵。其实Skin++先前版本的原理很简单，采用的是子类化和钩子而已，当然这也是Win32窗体系列美化的经典做法了。如果哪位哥们有兴趣可以联系我，我们也开发个，而且免费。呵呵。



     Ok，废话少说，回到正题上来吧，说说Juce，这是个很不错的库，源码非常简练，而且注释完备，很容易看懂，整个体系就是DirectUI的思路，我一直觉得这东西就是一个精简版的Qt。我不得不说Jules大哥是个牛人也是好人，有什么问题一封邮件，他立马回你，而且Juce社区国外用户很活跃，Juce库也不断地完善的和强大。听Jules大哥说，会加入Skin的功能，Great！



     来看第一个例子，我们给他取个名字叫“SuperJucer”吧，该例子的功能就是利用png文件创建一个不规则窗口，估计大家对这个很熟悉了，想想QQ宠物哈，一个小企鹅笨笨的在你的工具栏走来走去，是不是很有意思。而该类就是一个超人叔叔，出现在你的桌面上，没有windows土里吧唧的边框和XXX按钮，哈哈！



先看图片哈：这就是我们要制作的窗口，超人叔叔。

![](http://hi.csdn.net/attachment/201011/18/0_12900749927hED.gif)



第二个图片：景甜妹妹

![](http://hi.csdn.net/attachment/201011/18/0_1290075130Bx77.gif)



Ok，素材准备好了，我们就用这两幅图片做个异形窗口哈：



第一步： 在VS2008中创建一个Win32的空项目，取名SuperJucer，创建好之后，该工程下面只有三个空的文件夹：header，source，resource. 要的就是干净。



第二步： 添加两个文件SuperJucer.h和SuperJucer.cpp.这就是我们的代码文件。



第三步：设置包含路径及链接库路径，请看我第一篇的翻译哈。



第四步：在头文件SuperJucer.h中添加以下类：







**[cpp]**[view plain](http://blog.csdn.net/wangfangjie/article/details/6019742#)[copy](http://blog.csdn.net/wangfangjie/article/details/6019742#)



- #ifndef __SUPERJUCER__  
- #define __SUPERJUCER__  
- #include "../../juce.h"  
- namespace ProjectInfo  
- {  
- const char* const  projectName    = "SuperJucer";  
- const char* const  versionString  = "1.0.0";  
- const int          versionNumber  = 0x10000;  
- }  
- //==============================================================================  
- class MainAppWindow   : public Component  
- {  
- public:  
- //==============================================================================  
-     MainAppWindow();  
-     ~MainAppWindow();  
- void closeButtonPressed();  
- virtual void paint (Graphics& g);  
- void mouseDown (const MouseEvent& e)  
- void mouseDrag (const MouseEvent& e)   
-     juce_UseDebuggingNewOperator  
- 
- private:  
-     MainAppWindow (const MainAppWindow&);  
-     MainAppWindow& operator= (const MainAppWindow&);  
-     ComponentDragger dragger;  
-     Image* m_testWindowBK;  
- };  
- #endif  // __SUPERJUCER__  






解释如下：


- #include "../../juce.h"   
- 这个就是包含juce库文件，按照文档说明，一般这行应放在stdafx.h中，理由很简单，就是这个是全局要用的文件。
- class MainAppWindow   : public Component  
- 这行就是创建一个窗口类，继承Component, Component是Juce的窗体基类。暂时知道就可以了，我会翻译后续文档。到时就明白。
- void closeButtonPressed(); 
- virtual void paint (Graphics& g);
- void mouseDown (const MouseEvent& e)
- void mouseDrag (const MouseEvent& e)
- 这几行就是实现一些窗口常用的事件处理函数，需要关闭窗口所以，所以要相应closeButtonPressed这个函数，要移动和拖拽窗口所以mouseDown和mouseDrag需要的。 
- 而paint函数就是绘制函数，这个更需要了，因为我们需要把图片绘制在窗体上，而该函数就是干这事的，至于什么时候调用，怎么调用这事框架关心的，暂时可以不管。
- juce_UseDebuggingNewOperator
- 这一行的意思是允许调new操作符，如果编写过mfc程序，这个就好理解，你经常会在mfc文件中看到，#define _DEBUG_NEW_ NEW 类似的语句，这行就是这个功能。作用就是便于检查内存泄露或者其他调试功能。
- ComponentDragger dragger;
- 负责窗体拖拽
- Image* m_testWindowBK;
- 保存背景图片，在程序启动的时候加载一次背景图片，不要放在paint函数中加载哈，因为磁盘IO影响性能：）

第五步 实现文件





**[cpp]**[view plain](http://blog.csdn.net/wangfangjie/article/details/6019742#)[copy](http://blog.csdn.net/wangfangjie/article/details/6019742#)



- #include "MainWindow.h"  
- MainAppWindow::MainAppWindow()  
-     : Component (JUCEApplication::getInstance()->getApplicationName())  
- {  
-         centreWithSize (500, 400);  
-         setVisible (true);  
-     addToDesktop( ComponentPeer::windowIsTemporary  | ComponentPeer::windowIsResizable , NULL );  
-     m_testWindowBK = ImageFileFormat::loadFrom( File( "f://Program//juce//1.png") );  
- }  
- MainAppWindow::~MainAppWindow()  
- {  
- delete m_testWindowBK ;  
-    m_testWindowBK = NULL;  
- }  
- void MainAppWindow::closeButtonPressed()  
- {  
-     JUCEApplication::getInstance()->systemRequestedQuit();  
- }  
- void MainAppWindow::paint( Graphics& g )  
- {  
- if (isOpaque())  
-         g.fillAll (Colours::white);  
- else  
-         g.fillAll (Colours::blue.withAlpha (0.0f));  
- 
-     g.drawImage( m_testWindowBK, 0, 0, 256,256, 0,0, 256,256 );  
- 
- }  




解释如下：
- centreWithSize (500, 400);
- 是窗口出于桌面的正中，并且设置其大小为500*400
- setVisible (true);
- 是窗口可见
- addToDesktop( ComponentPeer::windowIsTemporary  | ComponentPeer::windowIsResizable , NULL );
- 将窗口添加到桌面，这里很有意思，因为MainAppWIndow窗口是直接继承Component类，该类并不知道其父窗口是谁，所以你必须强行将他加到桌面系统的窗口队列中。如果没有这句，虽然进程在但是看不到窗口。
- m_testWindowBK = ImageFileFormat::loadFrom( File( "f://Program//juce//1.png") );
- 加载文件，ImageFileFormat是个封装数据流或是文件，或是流到Image的类，直接这样就可以加载文件，是不是很类似Gdi+中的Image::FromImage。呵呵。

绘制部分是最重要的，看代码：


- if (isOpaque())
- 判断该窗口是否透明，真是不透明，假是透明。怪异。
-    g.fillAll (Colours::white);
-    如果不透明，就直接填充白色
- else
-    g.fillAll (Colours::blue.withAlpha (0.0f));
-    如果透明，就填充颜色蓝色，并且将它透明度设为0.0f，1.0是不透明哈。目的就是让窗口本身透明。哈哈，至于什么色无所谓了。主要是alpha通的为0
- g.drawImage( m_testWindowBK, 0, 0, 256,256, 0,0, 256,256 );
- 这是吧刚才的图片画上去，和Gdi+中的一致。

编译运行：看结果：

![](http://hi.csdn.net/attachment/201011/18/0_1290077446jjE3.gif)

是不是很帅：） 再看看景甜妹妹的效果：

![](http://hi.csdn.net/attachment/201011/18/0_1290077584Fg2P.gif)

绿色的我的桌面背景哈。



当然可以用在MFC中通过UpdateLayerAttribute函数也可以达到这个效果。但是我觉得有两个缺点：

第一：如果你绘制了png图片，再绘制别的非png元素，该函数会失效

第二：也可以设置遮罩色混合，不过会出现毛边，这是个很恶心的问题。



Juce做的很帅，这只是它小功能之一，还有很多。后续我会放上demo。

回家吃饭了，有空再继续。。。。



文中有些自己的理解，如果不对的地方，请给我提出来，但是不能说脏话哈，理解，理解。。。。









