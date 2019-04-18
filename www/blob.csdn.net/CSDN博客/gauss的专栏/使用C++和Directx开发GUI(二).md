# 使用C++和Directx开发GUI(二) - gauss的专栏 - CSDN博客
2011年12月05日 12:21:06[gauss](https://me.csdn.net/mathlmx)阅读数：294
欢迎您继续阅读"使用C++和Directx开发GUI"的第二部分.这里是[第一部分](http://dev.gameres.com/Program/Abstract/UseDXGUI1.htm).接着我们的主题(讲解在我未来的游戏如何使用GUI(图形用户界面)),本文将解释窗体的许多神秘之处.我们将关注窗体树如何工作,为我们使用GUI制订计划,以及创建窗体类的细节,包括绘制,消息机制,坐标系统和其他所有的麻烦事儿.
 在此我们将着重使用C++.如果你对纯虚函数,dynamic_cast'ing等等已经生疏了,那么赶快翻翻C++书再继续吧.
不开玩笑了,让我们开始.
在涉及代码之前,明确我们的目标是很重要的.
在我们的游戏已完成的GUI里,我们将使用一个树来跟踪显示在屏幕上的每个窗体.窗体树是个简单的N节点树.树的根部是视窗桌面(windows desktop).桌面窗体(Desktop window)的子窗体通常是应用程序的主窗体;主窗体的子窗体是对话框,对话框的子窗体是独立的对话控件(按钮,文本框等).重要的区别在于--窗体的外观并不取决于它在树中的位置.例如,许多游戏把按钮直接放在他们的桌面窗体上,就如同对话框一样. 是的,按钮也是窗体.意识到这一点是很重要的.一个按钮只是一个有着有趣外观的窗体.实际上,所有的GUI控件都是有着不同外观的简单窗体.这体现了C++的能力.如果我们创建一个继承的窗体类,给它几条虚函数,我们就能通过重载基类的函数轻易地创建我们的控件.如此应用多态性简直称得上优雅;实际上,许多C++书将它作为范例(在第三部分我将详述此点).
 这是我们的基本设计,下面让我们想想应用方法.
**计划**
当我应用我的GUI时,我做了如下几步:
1.首先我写了些基本的窗体管理代码.这些代码负责窗体树,增加/删除窗体,显示/隐藏窗体,把它们移动到Z坐标的顶端(即在最前显示),等等.我通过在窗体应处的位置绘制矩形完成了窗体的绘制过程,然后根据窗体的Z坐标在左上角绘制一个数字. 如果你购买或编写一个优秀可靠的指针阵列的模版类,那你的生活将会变得非常轻松.STL(标准模版库Standard Template Library)得到许多C++版本的支持,它有很多好的模板性的指针阵列类,但是如果你想使用你自己的模板类,在你应用于你的窗体管理之前要进行完整彻底的测试.现在你要注意的问题是由错误的阵列类所引起的不易察觉的内存泄漏或空指针引用.
2.一旦我有了基础的窗体管理函数,我花了一些时间思考我的坐标系统.写了一些坐标管理函数.
3.下一步,我处理窗体绘制代码.我继承一个"奇异窗体"类,并显示它如何使用一套九个精灵程序绘制自身的--其中四个精灵程序绘制角落,四个绘边,一个绘制背景. 使用这九个窗体精灵程序,使创建既有独特的艺术外观又可动态改变大小(ala StarDock's WindowBlinds)的窗体成为可能.这样做的基础是你需要有一个相当智能的绘图库,一个能处理封存精灵程序,弹性精灵程序以及集中精灵程序的库,并且它是一个非常复杂的窗体生成程序(一些艺术家可以用以创建他们的窗体的代码),这使这种方法可以实际的实现.当然,你也要注意窗体绘制速度.
4.一旦普通窗体的绘制代码完成,我开始实现控制部分.代码控制是简单的,但还是需要非常彻底的测试.我由简单的控制:静态,图标等开始像在前面解释的那样来回反复我的工作.
5.最后,完成我的控制部分后,我开始编写一个简单的资源编辑器,一个允许用户可视的放置控件,布局对话框的程序.这个资源编辑器用了我整整一个月的时间,但我强烈建议这样做(而不是用文本文件去决定位置)--图形化对话框的建立非常容易,并且这也是一个好的练习:在完善中我在我的控制部分的代码中没有发现几个bug,在实际的程序中被证明是很难解决的.
我被编写一个可以转换MSVC++的资源(.RC)文件为我的GUI可使用的资源文件的程序的这个想法困扰了好久.最后,我发现这样一个程序远比它的价值麻烦.我写这个GUI的目的就是要摆脱Windows的限制,为了正真的做到这一点,我要由自己的编辑器,使用我自己的资源文件格式,按自己的形式做事情.我决定用MFC由底层实现一个所见即所得(WYSIWYG)的资源编辑器.我的需求,我决定;你的需求也许不同.如果某人想要写一个转化器,我将很乐于听到这样的消息. 现在到哪了?这篇文章剩下的部分将探究开始的两步.这一系列的第三部分将进入令人麻木的控制代码细节.第四部分将讨论一点资源编辑器的实现和序列化窗体.
 因此...让我们来开始第一步:基本的窗体管理代码.
**实现**
我们开始.这是为我们基本窗体类定义的好的开始:
class gui_window
{
public:
    gui_window(); // boring
    ~gui_window(); // boring
    virtual void init(void); // boring
    gui_window *getparent(void) { return(m_pParent); }
/////////////
// section I: window management controls
/////////////
    int addwindow(gui_window *w);
    int removewindow(gui_window *w);
    void show(void) { m_bIsShown = true; }
    void hide(void) { m_bIsShown = false; }
    bool isshown(void) { return(m_bIsShown); }
    void bringtotop(void);
    bool isactive(void);
/////////////
// Section II: coordinates
/////////////
    void setpos(coord x1, coord y1); // boring
    void setsize(coord width, coord height); // boring
    void screentoclient(coord &x, coord &y);
    int virtxtopixels(coord virtx); // convert GUI units to actual pixels
    int virtytopixels(coord virty); // ditto
    virtual gui_window *findchildatcoord(coord x, coord y, int flags = 0);
/////////////
// Section III: Drawing Code
/////////////
    // renders this window + all children recursively
    int renderall(coord x, coord y, int drawme = 1);
    gui_wincolor &getcurrentcolorset(void)
    { return(isactive() ? m_activecolors : m_inactivecolors); }
/////////////
// Messaging stuff to be discussed in later Parts
/////////////
    int calcall(void);
    virtual int wm_paint(coord x, coord y);
    virtual int wm_rendermouse(coord x, coord y);
    virtual int wm_lbuttondown(coord x, coord y);
    virtual int wm_lbuttonup(coord x, coord y);
    virtual int wm_ldrag(coord x, coord y);
    virtual int wm_lclick(coord x, coord y);
    virtual int wm_keydown(int key);
    virtual int wm_command(gui_window *win, int cmd, int param) { return(0); };
    virtual int wm_cansize(coord x, coord y);
    virtual int wm_size(coord x, coord y, int cansize);
    virtual int wm_sizechanged(void) { return(0); }
    virtual int wm_update(int msdelta) { return(0); }
protected:
    virtual void copy(gui_window &r); // deep copies one window to another
    gui_window *m_pParent;
    uti_pointerarray m_subwins;
    uti_rectangle m_position;
    // active and inactive colorsets
    gui_wincolor m_activecolor;
    gui_wincolor m_inactivecolor;
    // window caption
    uti_string m_caption;
};
当你细读我们讨论的函数,你将会发现递归到处可见.比如,我们的程序将通过调用一个源窗体的方法renderall()来绘制整个GUI系统,这个方法又将回调它的子窗体的renderall()方法,这些子窗体的renderall()方法还要调它们的子窗体的renderall()方法,以此类推.大部分的函数都遵循这种递归模式. 整个GUI系统有一个全局的静态变量--源窗体.出于安全性的考虑,我把它封装在一个全局的函数GetDesktop()中.
现在,我们开始,我们来完成一些函数,由窗体管理代码开始,如何?
**窗体管理**
/****************************************************************************
addwindow: adds a window to this window's subwin array
****************************************************************************/
int gui_window::addwindow(gui_window *w)
{
    if (!w) return(-1);
    // only add it if it isn't already in our window list.
    if (m_subwins.find(w) == -1) m_subwins.add(w);
    w->setparent(this);
    return(0);
}
/****************************************************************************
removewindow: removes a window from this window's subwin array
****************************************************************************/
int gui_window::removewindow(gui_window *w)
{
    w->setparent(NULL);
    return(m_subwins.findandremove(w));
}
/****************************************************************************
bringtotop: bring this window to the top of the z-order. the top of the
z-order is the HIGHEST index in the subwin array.
****************************************************************************/
void gui_window::bringtotop(void)
{
    if (m_parent) {
        // we gotta save the old parent so we know who to add back to
        gui_window *p = m_parent;
        p->removewindow(this);
        p->addwindow(this);
    }
}
/****************************************************************************
isactive: returns true if this window is the active one (the one with input focus).
****************************************************************************/
bool gui_window::isactive(void)
{
    if (!m_parent) return(1);
    if (!m_parent->isactive()) return(0);
    return(this == m_parent->m_subwins.getat(m_parent->m_subwins.getsize()-1));
}
这一系列函数是处理我所说的窗体管理:新建窗体,删除窗体,显示/隐藏窗体,改变它们Z坐标.所有的这些都是完全的列阵操作:在这里你的列阵类得到测试. 在增加/删除窗体函数中唯一感兴趣的问题是:"谁来对窗体指针负责?"在C++中,这总是一个问自己得很好的问题.Addwindow和removewindow都要获得窗体类的指针.这就意味这创建一个新的窗体你的代码新建一个指针并通过addwindow()把指针传到父(桌面)窗体.那么,谁来负责删除你新建的指针呢?
我的回答是"GUI不拥有窗体指针;游戏本身负责增加指针".这与C++的笨拙规则"谁创建谁删除"是一致的.
我选择的可行的方法是"父窗体为它的所有子窗体指针负责".这就意味着为了防治内存泄漏,每个窗体必须在它的(虚拟)析构函数(记住,有继承类)中搜寻它的子窗体列阵并且删除所有的包括在其中的窗体.
如果你决定实现一个拥有指针系统的GUI,注意一个重要的原则--所有的窗体必须动态的分配.这样的系统崩溃最快的方法是把一个变量的地址传到堆栈中,如调用"addwindow(&mywindow)",其中mywindow被定义为堆栈中的局部变量.系统将好好工作直到mywindow超出它的有效区,或其父窗体的析构函数被调用,此时系统将试图删除给地址,这样系统即崩溃.所以说"对待指针一定要特别的小心".
这就是为什么我的GUI不拥有窗体指针的主要原因.如果你在你的GUI中处理大量复杂的窗体指针(也就是说,比如你要处理属性表),你将更想要这样一个系统,它不必跟踪每一个指针比且删除只意味着"这个指针现在为我所控制:只从你的列阵中移走它但并不删除它".这样只要你能保证在指针超出有效区前removewindow(),你也可以使用(小心)在堆栈中的局部变量地址.
继续?显示和隐藏窗体通过一个布尔型变量来完成.Showwindow()和hindewindow()只是简单的设置或清除这个变量:窗体绘制程序和消息处理程序在它们处理任何之前先检查这个"窗体可见"标志位.非常简单吧!
Z坐标顺序也是相当的简单.不熟悉这种说法,可把z坐标顺序比为窗体"堆栈"一个重叠一个.一开始,你也许想像DirectDraw处理覆盖那样实现z坐标顺序,你也许决定给每个窗体一个整数来描述它在z坐标的绝对位置,也就是说,可能0表示屏幕的顶端,则-1000代表最后.我想了一下这种Z坐标顺序实现方法,但我不赞成--Z坐标绝对位置不是我所关心的;我更关心的是他们的相对位置.也就是说,我不需要准确的知道一个窗体在另一个的多后,我只要简单的知道这个给定的窗体在另一个的后面还是前面.
所以,我决定实现Z坐标顺序如下:在列阵中有最大的索引值,m_subwins,的窗体在"最前".拥有[size-1]的窗体紧跟其后,紧接着是[size-2],依次类推.位置为[0]的窗体将在最底.用这种方法Z坐标顺序实现变得非常容易.而且,一举两得,我将把最前的窗体视为活动窗体,或更技术的说法,它将被视为拥有输入焦点的窗体.尽管我的GUI使用的这种"始终最前"窗体是有限制的(比如,在Windows NT中的任务管理器不管输入焦点始终在所有的窗体之前),我觉得这样有利于使代码尽可能的简单.
当然,我用数列表示Z坐标顺序在我移动窗体到最前时处理数列付出了一些小的代价.比如,我要在50个窗体中将第二个窗体移到最前;我将为了移动二号窗体而移动48个窗体.但信运的是,移动窗体到Z坐标最前不是最耗时的函数,即使是,也有很多好的快的方法可以处理,比如链表即可.
看看我在bringtotop()函数中的小技巧.因为我知道窗体不拥有指针,我就删除这个窗体又马上创建一个,非常有效率的将它重定位在数列最前.我这样做是因为我的指针类,uti_pointerarray,已经被编写好了一旦删除一个元素,所有的更高的元素将向后移动.
这就是窗体管理了.现在,进入有趣的坐标系统?
**坐标系统**
/****************************************************************************
virtual coordinate system to graphics card resolution converters
****************************************************************************/
const double GUI_SCALEX = 10000.0;
const double GUI_SCALEY = 10000.0;
int gui_window::virtxtopixels(int virtx)
{
    int width = (m_parent) ? m_parent->getpos().getwidth() : getscreendims().getwidth();
    return((int)((double)virtx*(double)width/GUI_SCALEX));
}
int gui_window::virtytopixels(int virty)
{
    int height = (m_parent) ? m_parent->getpos().getheight() : getscreendims().getheight();
    return((int)((double)virty*(double)height/GUI_SCALEY));
}
/****************************************************************************
findchildatcoord: returns the top-most child window at coord (x,y);
recursive.
****************************************************************************/
gui_window *gui_window::findchildatcoord(coord x, coord y, int flags)
{
    for (int q = m_subwins.getsize()-1; q >= 0; q--)
    {
        gui_window *ww = (gui_window *)m_subwins.getat(q);
        if (ww)
        {
            gui_window *found = ww->findchildatcoord(x-m_position.getx1(), y-m_position.gety1(), flags);
            if (found) return(found);
        }
    }
    // check to see if this window itself is at the coord - this breaks the recursion
    if (!getinvisible() && m_position.ispointin(x,y))
        return(this);
    return(NULL);
}
我的GUI最大的优势是独立的解决方案,我称之为"弹性对话框".基本上,我希望我的窗体和对话框根据它们运行系统的屏幕设置决定它们的大小.对系统的更高的要求是,我希望窗体,控件等在640 x 480的屏幕上扩张或缩小.同时我也希望不管它们父窗体的大小,它们都可以适合.
这就意味着我需要实现一个像微软窗体一样的虚拟坐标系统.我以一个任意的数据定义我的虚拟坐标系统--或者说,"从现在起,我将不管窗体的实际尺寸假设每一个窗体都是10000 x 10000个单元",然后我的GUI将在这套坐标下工作.对于桌面,坐标将对应显示器的物理尺寸.
我通过以下四个函数实现我的想法:virtxtopixels(),virtytopixels(), pixelstovirtx(), 和pixelstovirty(). (注意:在代码中之列出了两个;我估计你已理解这个想法了).这些函数负责把虚拟的10000 x 10000单元坐标要么转换为父窗体的真实尺寸要么转换为显示器的物理坐标.显然,显示窗体的函数将倚重它们.
函数screentoclient()负责取得屏幕的绝对位置并将它转换为相对的虚拟坐标.相对的坐标从窗体的左上角开始,这和3D空间的想法是相同的.相对坐标对对话框是必不可少的.
在GUI系统中所有的坐标都是相对于其他的某物的．唯一的一个例外就是桌面窗体，它的坐标是绝对的．相对的方法可以保证当父窗体移动时它的子窗体也跟着移动，而且可以保证当用户拖动对话框到不同位置时其结构是一致的．同时，因为我们整个虚拟坐标系统都是相对的，当用户拉伸或缩小一个对话框时其中的所有控件都会随之变化，自动的尽量适合新的尺寸．对我们这些曾在win32中试过相同特性的人来说，这是个令人惊异的特点．
最后，函数findchildatcoord()取得(虚拟)坐标确定哪个(如果有)子窗体在当前坐标--非常有用,比如,当鼠标单击时,我们需要知道哪个窗体处理鼠标单击事件.这个函数通过反向搜寻子窗体列阵(记住,最前的窗体在列真的最后面),看那个点在哪个窗体的矩形中.标志参数提供了更多的条件去判断点击是否发生;比如,当我们开始实现控制时,我们会意识到不让标示和光标控件响应单击是有用的,取而带之应给在它们下面的窗体一个机会响应--如果一个标示放在一个按钮上面,即使用户单击标示仍表示单击按钮.标志参数控制着这些特例.
现在,我们已经有了坐标,我们可以开始绘制我们的窗体了?
**绘制窗体**
递归是一柄双刃剑.它使得绘制窗体的代码很容易跟踪，但是它也会造成重复绘制像素，而这将严重的影响性能。(这就是说，例如你有一个存放50个相同大小相同位置的窗体，程序会一直跑完50个循环，每个像素都会被走上50遍)。这是个臭名昭著的问题。肯定有裁剪算法针对这种情况，实际上，这是个我需要花些时间的领域。在我自己的程序-Quaternion's GUI 在非游戏屏幕过程(列标题和关闭等等)中一般是激活状态的，要放在对GUI而言最精确的位置是很蠢的想法，因为根本就没有任何其他的动作在进行。
但是，我在对它进行修补。现在我试图在我的绘制方法中利用DirectDrawClipper对象。到现在为止，初始的代码看起来很有希望。下面是它的工作方式：桌面窗口“清除”裁剪对象。然后每个窗口绘制它的子窗口，先画顶端的，在画底端的。当每个窗口绘制完毕后，把它的屏幕矩形加入到裁剪器，有效地从它之下的窗口中“排除”这个区域(这假设所有的窗口都是100%不透光的).这有助于确保起码每个像素将被只绘制一次；当然，程序还是被所有的GUI渲染所需要的计算和调用搞的乱糟糟的，(并且裁剪器可能已经满负载工作了),但是起码程序不会绘制多余的像素.裁剪器对象运行的快慢与否使得这是否值得还不明了。
我也在尝试其他的几个主意-也许利用3D显卡的内建Z缓冲,或者某种复杂的矩形创建器(dirty rectangle setup).如果你有什么意见，请告诉我;或者自己尝试并告诉我你的发现。
我剪掉了大量的窗体绘制代码，因为这些代码是这对我的情况的(它调用了我自定的精灵类).一旦你知晓你要绘制窗体的确切的屏幕维数(screen dimensions)时,实际的绘制代码就能够直接被利用。基本上，我的绘制代码用了9个精灵-角落4个，边缘4个，背景1个-并用这些精灵绘制窗体.
色彩集需要一点儿解释.我决定每个窗口有两套独特的色彩集;一套当窗口激活时使用,一套不激活时使用.在绘制代码开始之前，调用getappropriatecolorset(),这个函数根据窗口的激活状态返回正确的色彩集.具有针对激活和非激活状态的不同色彩的窗口是GUI设计的基本规则;它也比较容易使用.
现在我们的窗口已经画完了，开始看看消息吧。
**窗口消息**
这一节是执行GUI的核心。窗口消息是当用户执行特定操作(点击鼠标，移动鼠标，击键等等)时发送给窗口的事件.某些消息(例如WM_KEYDOWN)是发给激活窗口的，一些(WM_MOUSEMOVE)是发给鼠标移动其上的窗口,还有一些(WM_UPDATE)总是发给桌面的.
微软的Windows有个消息队列.我的GUI则没有-当calcall()计算出需要给窗口送消息时,它在此停下并且发送消息-它为窗口调用适当的WM_XXXX()虚函数.我发现这种方法对于简单的GUI是合适的.除非你有很好的理由，不要使用一个太复杂的消息队列，在其中存储和使用线程获取和发送消息.对大多说的游戏GUI而言，它并不值得.
此外，注意WM_XXXX()都是虚函数.这将使C++的多态性为我们服务.需要改变某些形式的窗口(或者控件,比如按钮),处理鼠标左键刚刚被按下的事件？很简单,从基类派生出一个类并重载它的wm_lbuttondown()方法.系统会在恰当的时候自动调用派生类的方法;这体现了C++的力量.
就我自己的意愿，我不能太深入calcall()的细节,这个函数得到所有的输入设备并发出消息.它做很多事,并有很多对我的GUI而言特定的行为.例如,你或许想让你的GUI像X-Window一样运行,在鼠标活动范围之内的窗口总是处于激活状态的窗口.或者,你想要使得激活窗口成为系统模态窗口(指不可能发生其他的事直到用户关闭它),就像许多基于苹果平台(Mac)的程序那样.你会想要在窗口内的任何位置点击来关闭窗口,而不是仅仅在标题栏,像winamp那样.calcall()的执行结果根据你想要GUI完成什么样的功能而会有很大的不同.
我会给你一个提示,虽然-calcall()函数不是没有状态的,实际上,你的calcall()函数可能会变成一个很复杂的状态机(state machine).关于这一点的例子是拖放物体.为了恰当的计算普通的"鼠标键释放"事件和相似的但完全不同的"用户在拖动的物体刚刚放下"事件之间的不同,calcall()必须有一个状态参数.如果你对有限状态机已经生疏了,那么在你执行calcall()之前复习复习将会使你不那么头痛.
在窗口头文件中包括的wm_xxxx()函数是我感觉代表了一个GUI要计算和发送的信息的最小集合.你的需要可能会不同,你也不必拘泥于微软视窗的消息集合;如果自定的消息对你很合适,那么就自己做一个.
**窗口消息**
在文章的第一部分我提到了一个叫做CApplication::RenderGUI()的函数,它是在计算之后绘制我们的GUI的主函数:
void CApplication::RenderGUI(void)
{
    // get position and button status of mouse cursor
    // calculate mouse cursor's effects on windows / send messages
    // render all windows
    // render mouse
    // flip to screen
}
最后,让我们开始加入一些PDL(页面描述语言).
void CApplication::RenderGUI(void)
{
    // get position and button status of mouse cursor
    m_mouse.Refresh();
    // calculate mouse cursor's effects on windows / send messages
    GetDesktop()->calcall();
    // render all windows
    GetDesktop()->renderall();
    // render mouse
    m_mouse.Render();
    // flip to screen
    GetBackBuffer()->Flip();
}
查看这些代码将会使你看到程序是如何开始在一起工作的.
在下一章,第三部分中,我们会处理对话框控件.按钮,文本框和进度条.
