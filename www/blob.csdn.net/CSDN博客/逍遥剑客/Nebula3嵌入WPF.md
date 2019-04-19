# Nebula3嵌入WPF - 逍遥剑客 - CSDN博客
2010年09月24日 20:55:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2201标签：[wpf																[math																[system																[input																[引擎](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=math&t=blog)](https://so.csdn.net/so/search/s.do?q=wpf&t=blog)
个人分类：[引擎研究																[工具开发](https://blog.csdn.net/xoyojank/article/category/1858383)](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
- C++/CLI只能使用MD/MDd的运行库, 所以N3的编译选项需要改一下. 可以在makeslnwin32.tcl里改, 然后重新生成所有的工程文件.
- N3的System命名空间与.Net的冲突. 主要是byteorder.h这个头文件里的Math命名空间, 改成"::Math"或"using namespace Math"都可以. Input之类的同理.
- 编译时有些代码(估计是内联部分)会提示无法编译成托管代码, 只需要把#include用#pragma unmanaged/#pragma managed包起来
- XNAMath的对齐数据会无法编译成托管代码, 会有一个警告, 工程设置里忽略就可以.
- WPF没有所谓的主循环, 所以Appcliation的Run方法就不能用了. 所以UpdateXXX之类的方法需要开放给外部调用.
- 为了不段地更新引擎, Update/Render之类的方法需要有个地方调用. 临时使用的是CompositionTarget.Rendering这个事件, 大约60FPS的样子. ComponentDispatcher.ThreadIdle可能是更好的解决方法, 我还没搞明白怎么用.
- WPF不再使用HWND来做控件, 所以DX要跟WPF结合的话只能使用HwndHost. BuildWindowCore转入的hwndParent做为DisplayDevice的父窗口句柄.
- BuildWindowCore在MSDN里有个说明: "The child window will be created only if the window is owned by the calling thread." 而N3的DisplayDevice是在渲染线程创建的窗口, 所以需要把窗口的创建改到主线程.
- 由于窗口在主线程创建, 那么Win32DisplayDevice::WndPro也需要在HwndHost::WndProc中实现一遍. 要不然所有的输入消息都没法在InputServer中得到处理.
- 键盘消息的处理比较麻烦. 看看是不是应该切换焦点, 还是重写HwndHost的函数来处理.
![](http://hi.csdn.net/attachment/201009/24/0_1285343169pWpv.gif)
