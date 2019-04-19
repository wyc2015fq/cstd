# WPF初体验 - 逍遥剑客 - CSDN博客
2010年04月11日 14:34:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2983
前段时间无聊装了个VS2010RC, 然后试着建个了WPF的工程, 体验了一番.
刚开始没觉得这东西比WinForms方便在哪, 只是觉得它是一套跟Win32/MFC/WinForms完全不同的的东西. Win32/MFC/WinForms只要了解了消息循环机制, 理解起来就比较容易, 可WPF冒似没这套东西. 而且, HWND已经不存在于控件上了, 这是一个令人很不习惯的地方. 要知道不论什么UI库, 要想跟DirectX一起使用的话, 必须有一个HWND来指定绘制区域.
那么问题就来了: 怎么用WPF做一个DX的编辑工具?
最早看到过的方式是WPF提供的D3DImage类. 这个类的作用就是把BackBuffer拷贝过来, 然后再用WPF那一套东西来显示. 这样的好处就是可以跟WPF的排版/Alpha融合什么的无缝结合, 但是, 由于中间多了一个拷贝, 会影响一部分效率. 再者, D3D9在XP和Vista上的处理方式不一样. 还有一个更麻烦的事: WPF是基于D3D9的, 这就意味着GL/DX10/DX11就不能直接利用这种方式来整合.
所以呢, 还是要通过HWND的传统方式来. 因为绘制区域的半透明/变形之类的特性, 对于一个工具来说, 完全没有必要. WPF推出时, 也考虑到了向后兼容. 在WPF中显示Win32控件, 可以使用HwndHost, 而对于WinForms, 可以使用WindowsFormsHost. WinForms本质上也是HWND机制, 所以就不细说了, 有兴趣的可以看我之前写的文章. 这里主要说说HwndHost.
HwndHost是一个抽象类, 我们需要实现它. 使用起来很简单. 在BuildWindowCore里创建一个Win32子窗口, 并把这个窗口的HWND做为DX初始化的参数. 如果需要改变大小, 还需要实现OnRenderSizeChanged. 注意WPF的size是跟DPI设定有关系的, 需要一个缩放值. 如果还需要进行消息处理, 可以实现基类的WndProc. 那么, 还剩下一个问题: Render()在哪调用呢?
这个问题呢, 我也没找到一个完美的方式. 一种思路是在WndProc里, 这样的话, 只有在有WM消息触发时才会渲染, 就像UE3那个查看器一样, 鼠标不动, 它就不更新. 另一种思路是在HostWindow的调用端设置一个Timer, 通过固定间隔的触发来进行渲染. 这种方式可能会引起帧率无法自适应的问题, 但一般情况下是可以接受的. 最完美的方式就是找到WPF的主循环, 问题是WPF有这东西么? 有待研究.
WPF的优势是什么? 控件多样性不如WinForms, 与C++的交互便利性不如MFC, 为什么还要使用它呢? 目前来说, 最吸引人的地方就是DataBinding. 通过DataBinding, 可以直接把控件的某一个属性与对象的属性进行绑定, 不用写一堆的OnButtonClicked之类的事件响应函数, 也不用写一堆界面初始化函数. 另外, WPF把界面的布局放入了XAML里, 让代码变得更干净, 再也不用对着一堆button1, button2什么的骂娘了.
学习资料, 推荐两本电子书(ebookee上有):
Applications = Code + Markup By Charles Petzold.chm
Sams.Windows.Presentation.Foundation.Unleashed.Dec.2006.pdf
