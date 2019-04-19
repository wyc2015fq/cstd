# 利用Python和win32编程范例——按需定制一个按键精灵 - 宇宙浪子的专栏 - CSDN博客
2015年04月09日 11:35:10[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：5100

转自：作者
[橘子一方](http://www.orangecube.net/articles/author/9173860)[http://www.orangecube.net/articles/python-win32-example.html](http://www.orangecube.net/articles/python-win32-example.html)
# 利用Python和win32编程范例——按需定制一个按键精灵
本文假设各位看官对python是足够熟悉的，但却不太了解win32编程。
嘛。。其实我也没学过win32编程，脸请各位看官随意招呼。
**需求**：最近因为做课题，需要用面孔建模软件FaceGen打开大量的.fg文件（几千），并另存为图片，以备后续操作。
**分析**：要想直接从文件解析出面孔的图片有一定的难度，原因在于.fg文件的存储格式很奇葩，300byte就能存下一张脸的全部信息（基于PCA的面孔特征抽取），必须模拟操作。这样一来，事情也就变得简单机械，无非把“载入.fg文件并将其另存为图片”，重复上千次。鉴于一系列原因，不是很想用按键精灵之类的东西。何况利用pywin32可以很方便的利用win32的一些接口，何必再去模拟操作。
## Part 0： 开始之前？
首先，[来这里](http://sourceforge.net/projects/pywin32/)安装一个Pywin32吧，Pywin32是一个Python库，为python提供访问Windows API的扩展，提供了齐全的windows常量、接口、线程以及COM机制等等。安装后自带一个pythonwin的IDE。或者也可以[来这边](http://www.activestate.com/activepython/downloads)下载一个ActivePython，整合了pywin32和一些其他的库以及一大堆的支持文档，[他们的文档](http://docs.activestate.com/)查起来是非常方便的。当然了，WIN32的一些相关函数在MSDN上也能直接找到。
其次，为了方面查找目标窗口的句柄，可以下载一个微软自家的Spy++，这玩意儿满大街都是。有了它，还能很方便的查看窗体的消息。
> 
**句柄**是一个32位整数，在windows中标记对象用，类似一个dict中的key，详情参看[这篇文章](http://blog.csdn.net/shark_boss/article/details/3546919)。
&
**消息**是windows应用的重要部分，用来告诉窗体“发生了什么”，比如给一个按钮发送BN_CLICKED这么个消息，按钮就知道“哦，我被点了”，才能执行相应的下一步操作。本文将大量使用消息机制。详情参看[这篇文章](http://blog.csdn.net/liulianglin/article/details/14449577)。
## Part 1： 查找窗体句柄
貌似在win32编程的世界里，包括窗口到文本框的所有控件就是窗体，所有的窗体都有独立的句柄。要操作任意一个窗体，你都需要找到这个窗体的句柄，这里，我们就可以用到FindWindow函数和FindWindowEx函数。在pywin32中，他们都属于win32gui的模块。
- FindWindow(lpClassName=None, lpWindowName=None):
- 描述：自顶层窗口（也就是桌面）开始搜索条件匹配的窗体，并返回这个窗体的句柄。不搜索子窗口、不区分大小写。找不到就返回0 
- 参数：
- lpClassName：字符型，是窗体的类名，这个可以在Spy++里找到。 
- lpWindowName：字符型，是窗口名，也就是标题栏上你能看见的那个标题。 
- 说明：这个函数我们仅能用来找主窗口。 
- FindWindowEx(hwndParent=0, hwndChildAfter=0, lpszClass=None, lpszWindow=None);
- 描述：搜索类名和窗体名匹配的窗体，并返回这个窗体的句柄。不区分大小写，找不到就返回0。 
- 参数：
- hwndParent：若不为0，则搜索句柄为hwndParent窗体的子窗体。 
- hwndChildAfter：若不为0，则按照z-index的顺序从hwndChildAfter向后开始搜索子窗体，否则从第一个子窗体开始搜索。 
- lpClassName：字符型，是窗体的类名，这个可以在Spy++里找到。 
- lpWindowName：字符型，是窗口名，也就是标题栏上你能看见的那个标题。 
- 说明：找到了主窗口以后就靠它来定位子窗体啦。 
有了这两个函数，我们就可以写出可以定义到任意一个窗体句柄的函数啦：
``
``
这样在后续的调用中，我们就能使用我们定义的finde_subHandle来方便地找到某个特定的Edit窗体控件，比如要定位000A848打开对话框的这个Edit控件，
![图1](http://bcs.duapp.com/seele-private-bucket/blog/python-win32-f1.png)
直接这样调用即可：
``
另外，python中找回来的句柄都是十进制整型，Spy++里显示的都是十六进制整型，这个要注意下，调试的时候用十六进制输出句柄，如下：
``
## Part 2：菜单操作
有了句柄，我们就可以操作FaceGen了！嗯，要先打开文件，File→Open，然后再File→Save Image（很悲剧，Save Image没有快捷键，所以不得不进行菜单操作）。现在我们有了FindWindow和FindWindowEx，要怎么操作菜单呢？
哦，抱歉，靠他俩还做不到。
窗口的菜单就像窗口的标题栏一样，是窗口自身的一部分，不是其他窗体控件，也就没有办法用FindWindow和FindWindowEx返回句柄。所以要对菜单进行操作的话，我们需要新的函数，也就是GetMenu，GetSubMenu和GetMenuItemID，它们也都属于win32gui模块。结合下图来说：
![FacegenMenu](http://bcs.duapp.com/seele-private-bucket/blog/python-win32-FaceGenMenu.png)
- GetMenu(hwnd)
- 描述：获取窗口的菜单句柄。 
- 参数：
- hwnd：整型，需要获取菜单的窗口的句柄。 
- 说明：获取的是插图中黄色的部分。 
- GetSubMenu(hMenu, nPos)
- 描述：获取菜单的下拉菜单或者子菜单。 
- 参数：
- hMenu：整型，菜单的句柄，从GetMenu获得。 
- nPos：整型，下拉菜单或子菜单的的索引，从0算起。 
- 说明：这个可以获取插图中蓝色的部分；如描述所述，这个不仅可以获取本例中的下拉菜单，还可以获取子菜单。 
- GetMenuItemID(hMenu, nPos)
- 描述：获取菜单中特定项目的标识符。 
- 参数：
- hMenu：整型，包含所需菜单项的菜单句柄，从GetSubMenu获得。 
- nPos：整型，菜单项的索引，从0算起。 
- 说明：这个获取的就是红色区域中的项目啦，**注意**，分隔符是被编入索引的，所以Open的索引是2而非1，而Exit的索引是9而非6。
找到这个菜单项的标识符，我们就可以通过*消息机制*告诉应用程序：我们要执行这个菜单项的命令！这需要我们要给应用程序发个消息，让它执行所需菜单项的命令。假设之前获取的Open的标识符是open_ID，那么只需要这样：
``
就会有一个打开文件的对话框出现啦。
解释一下：
- PostMessage(hWnd, Msg, wParam, lParam)
- 描述：在消息队列中加入为指定的窗体加入一条消息，并马上返回，不等待线程对消息的处理。 
- 参数：
- hWnd：整型，接收消息的窗体句柄 
- Msg：整型，要发送的消息，这些消息都是windows预先定义好的，可以参见[系统定义消息（System-Defined Messages）](http://msdn.microsoft.com/en-us/library/windows/desktop/ms644927(v=vs.85).aspx#system_defined)
- wParam：整型，消息的wParam参数 
- lParam：整型，消息的lParam参数 
- 说明：简单说，就是给指定程序发一个消息，这些消息都用整型编好号，作为windows的常量可以查询的。在这里，我们用的就是win32con这个库里定义的WM_COMMAND这个消息，具体的wParam和lParam是根据消息的不同而不同的。具体请根据MSDN查阅。
### 关于wParam的low word和high word：
![图2](http://bcs.duapp.com/seele-private-bucket/blog/python-win32-hi-lo.png)
> 
查阅MSDN的消息时，会发现有的wParam定义了low word和high word，这是什么呢？wParam的定义是32位整型，high word就是他的31至16位，low word是它的15至0位，如图。有时，一个消息只需要不超过两个参数，那wParam就可以当一个参数用。万一参数多了，wParam就给拆成了两个int16来使用。这种时候在python里记得用16进制把整形表示出来就比较清爽啦。
更新一下我们定义的类，把菜单添加进去：
``
然后定义一个菜单操作的方法：
``
在这里顺便返回了弹出来的对话框的句柄和确定按钮的句柄，后面操作会用到。
## Part 3：控件操作A
通过菜单的目录操作，我们打开了打开文件对话框。为了简单起见，我们可以直接在文件名处填入要打开文件的绝对路径。怎么填呢？
首先还是定位到文本框控件
``
find_subHandle()是在Part 1中定义的函数，可以按照列表的信息查找Mhandle的子窗体。列表中的元组提供窗体的类名和排位号（z-index）。列表索引编号较小的为父窗体。
接着我们依然是利用win32的消息机制，给这个文本框控件送去一个消息：
``
在这里，我们用了SendMessage而不是PostMessage，其区别就在于我们可以通过SendMessage取得消息的返回信息。因为对于我们要设置文本框信息的WM_SETTEXT信息来说，设置成功将返回True。
- SendMessage(hWnd, Msg, wParam, lParam)
- 描述：在消息队列中加入为指定的窗体加入一条消息，直到窗体处理完信息才返回。 
- 参数：
- hWnd：整型，接收消息的窗体句柄 
- Msg：整型，要发送的消息，这些消息都是windows预先定义好的，可以参见[系统定义消息（System-Defined Messages）](http://msdn.microsoft.com/en-us/library/windows/desktop/ms644927(v=vs.85).aspx#system_defined)
- wParam：整型，消息的wParam参数 
- lParam：整型，消息的lParam参数 
- 说明：wParam和IParam根据具体的消息不同而有不同的定义，详情参阅Part 2. 
- WM_SETTEXT 消息
- 描述：设置窗体的文本 
- 参数：
- wParam：未使用 
- lParam：一个指针，指向以null结尾的字符串。窗体文本将被设置为该字符串。 
- 返回值：
- 如果成功设置，则返回1（MSDN原文是返回True） 
- 说明：
- 上面的定义是直接从MSDN上翻译过来的，在Python的语境里面没有指针，你只需要把变量名作为lParam传入就好了。 
- 另外，请注意编码，**包含中文请用gbk编码，否则乱码**。 
再利用一个WM_COMMAND消息来点击确定按钮：
``
- WM_COMMAND 消息
- 描述：当用户选择了菜单（或按钮等控件的）命令，或控件发送通知到父窗口，或加速键击（accelerator keystroke is translated）时发送。
- 参数：根据情景不同而不同，在这里属于用户命令，参数配置如下
- wParam：HIWORD为0（未使用），LOWORD为控件的ID 
- lParam：0（未使用） 
- 返回值：如果窗体处理了消息，应返回0 
至此，利用以上的win32API，便可完成打开fg文件的操作：
``
顺便，如果要获取目标文本框的内容呢，可以使用WM_GETTEXT，如下：
- WM_GETTEXT消息：
- 描述：将窗体的文本内容复制到指定的buffer对象中 
- 参数：
- wParam：要复制字符的最大长度，包括截尾的空字节 
- lParam：用来保存字符串的buffer的指针 
- 返回值：返回复制字符的数量，不包括截尾的空字节 
利用win32gui.PyMakeBuffer(len, addr)可以造一个buffer对象，类似python3中的bytearray，lParam的返回值。而利用WM_GETTEXTLENGTH可以获取不含截尾空字节的文本长度的长度，可以用来设置Buffer的长度。完整的示例如下：
``
## Part 4：控件操作B
至于另存为图片，情况要稍微复杂一点，因为另存为图片的默认选项是BMP，特别不巧，我使用的FaceGen版本保存为BMP有BUG，不能成功保存，所以我们除了定位保存文件的路径以外，还需要对文件类型的下拉组合框(ComboBox进)行操作：
我们假设我们找到了组合框的句柄为CB_handle，我们可以用CB_SETCURSEL消息来更改当前的选项：
- CB_SETCURSEL 消息
- 描述： 
- 参数：
- wParam：以0起始的待选选项的索引；如果该值为-1，将从组合框列表中删除当前选项，并使当前选项为空 
- lParam：未使用。 
- 返回值：
- 更改选择成功将返回所设置选项的索引号。 
只要给组合框发一个CB_SETCURSEL消息，你就会发现下拉列表的选项已经改变了。
这时点保存，你就会发现，这保存的跟之前的一样啊！根本没有变！
问题在哪里？
我们用鼠标或者键盘操作一下，是没有问题的，一旦更保存类型，保存窗口里的预览也会随之变化。所以，除了CB_SETCURSEL以外，一定还缺了点儿什么。
调用Spy++的消息机制查看手动操作，我们的下拉组合框除了渲染和点击，好像没有什么特别值得注意的。
那再看看父窗体呢？好像有点儿不太一样的东西：
- CBN_SELENDOK 通知（notification code）
- 描述：当用户选择了有效的列表项时发送，提示父窗体处理用户的选择。父窗体通过WM_COMMAND消息接收这个通知。 
- 参数：（作为WM_COMMAND的参数）
- wParam：LOWORD为组合框的ID. HIWORD为CBN_SELENDOK的值。 
- lParam：组合框的句柄。 
- CBN_SELCHANGE 通知（notification code）
- 描述：当用户更改了列表项的选择时发送，不论用户是通过鼠标选择或是通过方向键选择都会发送此通知。父窗体通过WM_COMMAND消息接收这个通知。 
- 参数：（作为WM_COMMAND的参数）
- wParam：LOWORD为组合框的ID. HIWORD为CBN_SELCHANGE的值。 
- lParam：组合框的句柄。 
- 说明：他们是WM_COMMAND消息wParam的high word（wParam的16-31位，详情参见Part 2）的常数之一，在Python中可以用位移操作将其移动到高位上（a<<16），再用加法加上低位的内容。
继续查MSDN的资料，我们发现，对于一个有效的选择，一定会发送这两个通知，发送完CBN_SELENDOK以后马上发送CBN_SELCHANGE。而且，使用CB_SETCURSEL消息时，CBN_SELCHANGE通知是**不会**被送达的！
问题就在这里，加上这两个消息之后，就能正常操作下拉菜单了。
``
