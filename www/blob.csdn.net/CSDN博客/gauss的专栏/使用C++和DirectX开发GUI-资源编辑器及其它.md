# 使用C++和DirectX开发GUI - 资源编辑器及其它 - gauss的专栏 - CSDN博客
2011年12月05日 12:22:16[gauss](https://me.csdn.net/mathlmx)阅读数：233标签：[c++																[application																[游戏																[function																[存储																[磁盘](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
　欢迎回到“使用C++和DX开发GUI”的第四部分。接着我们的主题（我是如何为我未来的游戏开发GUI的 — Quaternion），本文将关注所有的有关游戏GUI的细节问题。
**4.1、保存窗口**
　　窗口序列化（存储和载入窗口）对你的工程而言或许不重要。如果你的游戏GUI很简单，你可以全靠程序在游戏中实现窗口。但如果你的GUI相对复杂，或者随着开发的过程经常会改变，那么你会想写程序以把一个窗口（和所有它的子窗口）存到文件里，然后再装载它。对初学者而言，窗口序列化代码允许你改变游戏的GUI而不用重新编译，而且它对于多人协调工作也是有益的。
　　我的计划是从主对话窗口开始，然后遍历它的所有子窗口，把每一个存到磁盘上。如果我用C语言写程序的话，我告诉自己的第一句话是“好的，如果我必须保存这些窗口，我需要每个窗口有一个字节来告诉我它是什么样的窗口，以使我能正确的载入它。比如1是按钮，2是列表框，3是图表，等等。”
　　这种问题是C++的RTTI（运行期类型识别）关心的。RTTI提供了两个东东，type_info类和typeid()函数，两者允许我查询一个对象的所属类名称，例如gui_window，gui_button等等。我没有使用枚举和ID，而是简单地为每个将要保存的窗口调用typid()，并“写下”窗口的类名称。
　　我注意到使用RTTI的对象识别函数来帮助保存窗口的两个小缺点。第一，RTTI ID是字符串而不是整型，这意味着它们将占用磁盘上的更多空间（按照Pascal方式存储字串将是前4个字节代表字串的长度，接下来是字串本身的数据）。第二，如果你改变一个窗口类的名字，你会破坏已经存好的所有窗口文件。
　　由于这些原因，你可能不会这样使用RTTI。毕竟，并不是有技术就一定要使用它。然而，我发现RTTI对我的代码而言却是救生员。要获得更多的关于RTTI和这两个函数的信息，请在你的联机帮助文档里查找。
　　另外，如果你决定在VC++里使用RTTI，确保你在工程属性的C/C++栏和C++语言选项中打开它。
**4.2、载入窗口**
　　载入窗口比存储他们要难一点儿，这主要是因为你必须新建（new）每个窗口，载入，并当它不再需要的时候删除。
　　这是个递归函数，用PDL表达的话如下所示：
void gui_window:load(int filehandle)
　　{
// read window properties (colorsets, etc.)
　　　　// read total number of children for this window
　　　　// for each child?
　　　　　　// read window ID from disk
　　　　　　// new a gui_window derivative based on that ID
　　　　　　// tell the newly created window to load itself (recurse!)
　　　　// next child
}
　　换句话说，你得像你所想得那样从磁盘载入窗口。第一，要处理基类窗口：读取他的属性。然后，读取基类窗口的所有子窗口的数目。对每个子窗口，读取ID字节，根据ID新建一个窗口，然后让新窗口载入自己（低轨到它）。当所有的字窗口载入完毕时，就结束了。
　　当然，你的文件结构一致也非常重要。确保你的存储代码以你想要载入的顺序存储信息。
**4.3、资源编辑器**
　　要想真正使你的GUI大放光彩，你必须有一个资源编辑器。当然你不需要做个象开发环境提供的资源编辑器那样的华丽和强大，但是你起码得有个基本的程序来完成加入，编辑，删除，排列，免除你为对话框的每个控件计算虚拟坐标位置的麻烦事儿。
　　写一个功能完善的，所见即所得（WYSIWYG）的资源编辑器超出了本文的范围，但是我能给你一些小提示来帮助你完成这个壮举：
- **共享你的代码**。特别地，让你的资源编辑器和你的游戏一起分享同样的渲染代码。这样，你就得到了所见即所得支持，并且免除了开发两套GUI代码的麻烦，我向你保证，调整你的DirectX代码以使它渲染一个GDI表面而不是一个双缓冲系统将比重新开发一整套新的绘制代码简单。记住过一段时间之后你的GUI系统可能会改变，你不会想要经常在两个不同的地方改写代码。
- **不要试图模仿开发环境的外表和感觉**。换句话说，不要费时间模仿开发环境的细节（例如，属性页和预览窗口）。如果你的编辑器相对而言比较难看的话，不要沮丧；的确，小组的效率是和他使用的工具的效能成直接正比关系的，但是同时，小组之外的人是不可能使用你的资源编辑器的，你也不会用它去开发一个完整的GUI程序；你不过只是做几个对话框而已。你不需要环境文本帮助（context
 sensitive help）。你不需要环境文本菜单（context menus），除非你觉得这会简化一个特定的繁复的操作。如果你的资源编辑器不那么漂亮也无所谓，只要它能完成工作就行了。
- **强调数据完整而不是速度**。资源编辑器是个数据整合者，而不是个高性能程序，没有什么比你花了一个小时来设计的东西由于程序错误而丢失而更让人恼火的事儿了。当写你的GUI的时候，保存数据是你的最高目标。花些时间来做自动存储，释放缓冲区（autosaves, undo buffers）等等，别把优化看得那么重要。
**4.4、生成子类**（subclass）
　　那些熟悉Win32处理窗口的人们可能已经知道“subclass”这个术语的含义了。如果不知道的话，当你“subclass”一个窗口的时候，你就“衍生（derive）”一个新的窗口类型，然后把新的窗口类型嵌入到旧窗口要使用的地方。
　　让我做更详细地解释。比如我们需要一个超级列表框。我们已经有个普通的列表框类，但是因为某种原因它不适合；我们的游戏需要超级列表框。所以我们从普通的列表框类中衍生出一个超级列表框类。就是这样。
　　但是我们如何在我们的游戏对话框中放置这个超级列表框呢？由于超级列表框是为我们的程序特制的，我们不能为我们的资源编辑器增加函数来支持它。但同时，我们怎样通知GUI系统为这个特殊的实例（我们的游戏），让所有的列表框都是超级列表框呢？这就是生成子类（subclass）要做的事情。这不是个精确的技术定义，但是表达了足够的信息。
　　这里我要讲述的方法称作“载入过程生成子类（subclassing at load time）”。要理解它，让我们从上一节所述的基本的载入代码开始。我们有个载入函数，它第归地完成创建，载入，增加窗口。这里我们用PDL表述如下：
// read total number of children for this window
　　// for each child...
　　　　// read window ID from disk
　　　　// new a gui_window derivative based on that ID
　　　　// ...
　　// next child
　　要完成生成子类，我让我的窗口载入函数“给程序一个创建这一类型窗口的机会“，像这样：
// read total number of children for this window
　　// for each child...
　　　　// read window ID from disk
　　　　// give application a chance to create a window of this type
　　　　// if the application didn't create a window,
　　　　　　// then new a gui_window derivative based on the ID
　　　　// else, use the application's created window
　　　　// ...
　　// next child
　　我通过一个函数指针给程序这个机会。如果程序需要为一个窗口生成子类，就在函数指针里填上自己函数的地址。当窗口载入的过程中，调用这个函数,传入想要创建的窗口ID。如果程序想要根据ID为一个窗口生成子类，新建一个适当的对象并把新指针返回给窗口。如果程序不需要这个ID，则返回NULL，窗口函数根据返回值创建恰当的默认对象。这种方法允许程序“预先过滤”引入的窗口ID信息，并为特定的窗口类型重载默认函数。太完美了（**译者**：这一段我翻译得实在不能说是完美，相反是简直不知所云，这里把原文贴出来，请您自己斟酌吧）。
Specifically, I give the application this chance by way of a function pointer. If the application needs to subclass a window, it fills in the function pointer with the address of its own function. When the windows are loading, they call
 this application function, passing in the ID of the window they want to create. If the application wants to subclass a window from this ID, it news up the appropriate object and returns the new pointer back to the window. If the app doesn't want to do anything
 special for this ID, it returns NULL, and the window function senses this and news up the appropriate default object. This method allows the app to "pre-filter" the incoming window ID bytes, and to override the default behavior for certain window types. Perfect!
　　用这种方法在创建自定控件的时候给了我很大的自由。我为我的资源编辑器增加代码以使我能为每个存储的窗口改变ID。然后，当我需要自定控间的时候，我只需用资源编辑器改变保存这个窗口ID的字节。在磁盘上保存的是ID和所有为自定控件的其他基类属性。
　　很快吧？还有其他的方法来做同样的事，这是在模仿STL需要创建对象时使用的方法。STL使用特定的“allocator（分配符）”类，这有点儿像“类厂”（factories），它们按照客户告诉他们的需要来创建类。你可以使用这种方法来创建窗口。
　　这种方法的工作原理如下：创建一个类并把它叫做“gui_window_allocator”。写一个虚拟函数，叫做CreateWindowOfType，它接受一个给定的窗口ID并传出一个新的指针给窗口。现在你就得到了一个简单的分配符类，你的窗口载入代码将使用它来新建需要的窗口。
　　现在，当你的程序需要为窗口重载“new”操作符，衍生一个新的、程序相关的gui_window_allocator类，并告诉你的窗口载入代码来使用这个分配符，而不是默认的那个。这种方法就象提供一个函数指针，只用了一点儿C++。
**4.5、加速GUI渲染**
　　还有一个能帮助你加速GUI渲染的小提示。
　　关键的概念是，就象其它绘制函数的优化一样，不要画你不需要的东西。默认方式，GUI花很多时间绘制没有变化的部分。然而，你能通过告诉GUI绘制改变的窗口（windows that are dirty）作些优化。当窗口的外观需要改变的时候，窗口设定它们的dirty标志，在绘制的时候清除它们的dirty标志。
　　由于我们的GUI控件可能是透明的，当一个控件被标记为dirty，它的父窗口必须也被标记为dirty。这样，当它被绘制的时候，背景没有改变，因为父窗口也刚刚被重绘。
**4.6、总结**：**相逢在XGDC**
　　呼，好长的4篇文章，然而我们还是遗漏了很多。
　　我准备在即将来临的Armageddon XGDC（[*eXtreme Game Developer's Conference*](http://www.xgames3d.com/armmain.htm)）上讲述GUI。我将尽力让我的发言有用，现在你已经看了所有的文章，如果你有什么需要扩展的问题，或是我有什么遗漏，请给我写信让我了解。祝各位做游戏愉快。
**作者简介**：
　　Mason McCuskey先生是[Spin Studio](http://www.spin-studios.com/)（一个正在制作名为Quaternion的伟大游戏的制作小组）的领导者.他盼望着你的建议和评论，他的电子邮件是（*[mason@spin-studios.com](mailto:mason@spin-studios.com)*）。
**译者手记**：
　　终于结束了漫长的翻译过程，这是Rick翻译过的最长的也是难度最大的技术文章了。其中一定有不少的错误和遗漏，如果您发现了这些问题，请来信告诉我（*[rick_yinhua@sina.com](mailto:rick_yinhua@sina.com)*）。谢谢您的支持。
