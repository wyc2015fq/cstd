# notepad++ 查找引用(Find Reference)（适用于c c++及各类脚本比如lua、python等） - DoubleLi - 博客园






在程序开发过程中，程序员经常用到的一个功能就是查找引用（Find Reference)，Visual Studio里面的对应功能是“查找所有引用”(Find All References)。

    我在使用notepad++写代码的时候一开始一直因为找不到类似的功能而苦恼。只好每次使用“在文件中查找”(Find in files)来找到所有引用。

![](http://img.blog.csdn.net/20150704155117218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    但是这样每次查找都需要遍历目标目录的整个文件系统，文件多的话会耗费很长时间。

    notepad++有插件可以实现查找引用的功能。相比于搜索整个文件系统，这种插件只需要执行一次生成关键词索引，后面就只需要在关键词索引里面查找就行了。速度很快，出结果是瞬间的。而且后面即使你修改了文件也不需要重新生成关键词索引[数据库](http://lib.csdn.net/base/mysql)，因为在保存该文件的同时就会自动更新关键词索引数据库。

    这个插件就是NppGTags。NppGTags的默认解析器是只针对C, C++, Yacc, [Java](http://lib.csdn.net/base/java), PHP4和汇编的源文件的，但是稍微改一下配置就可以支持lua、[Python](http://lib.csdn.net/base/python)或者其他类型的语言了。

    在NppGTags目录下面（取决于你的notepad++安装目录，我的是C:\Program Files (x86)\Notepad++\plugins\NppGTags）有一个文件gtags.conf有一个文件gtags.conf

![](http://img.blog.csdn.net/20150704164115987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    这里面指定了在使用非默认的解析器时对不同的语言用不同的解析器文件（dll）。往上翻发现在使用build-in的解析器的时候指定了不同的语言对应的文件后缀名！于是我猜想这里添加.lua就能支持lua文件的关键词索引数据库创建。如下所示：

![](http://img.blog.csdn.net/20150704164448224?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   接下来（确保在setting里面切回默认解析器，而不是pygments或ctags。如果没有改过setting那么就肯定是默认解析器，不需要做切换。），然后点击插件->NppGtags->Create Database创建索引数据库，然后选中关键词后就可以查找引用（点击插件->NppGtags->Find Reference）了，结果如下图所示（查找结果在最下面的窗口，这里是查找对”rect“的引用）：

![](http://img.blog.csdn.net/20150704165316720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



    配合快捷键使用就很方便了。我一般对Find Reference设置快捷键ctrl+shift+G，选中关键词后按快捷键就能得到查找引用的结果了。从查找结果中双击可以进入源文件对应位置进行查看修改。

    将光标定位在单词上（不需要选中）也能进行该单词的查找引用，如果想要对某个关键字查找引用，但不知道该关键字出现在哪个文件里，只需要将光标定位在任意非单词处（比如空行处，或者一个括号结束的地方等等），再进行查找引用（快捷键或者菜单进入），就会出现查找引用的窗口，这里还会有关键字代码提示功能。

![](http://img.blog.csdn.net/20150706140428058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



    同理，其他语言只需要将对应源文件的后缀名按同样的方法加入gtags.conf就可以使用查找引用了（比如只需要在gtags.conf相应位置加入.py就能支持[python](http://lib.csdn.net/base/python)文件的查找引用了）。怎么样，还是很酷的吧 :D



----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    下面是找到如何处理非c c++文件（我这里是尝试lua文件）之前的一些尝试，仅作记录：

    这个插件的官方声明里是这样说的：GTags built-in code parser supports C, C++, Yacc, Java, PHP4 and Assembly. Other languages are supported through Pygments + CTags parser.就是说默认的（内建的）语法解析器只支持C C++ Yacc [Java](http://lib.csdn.net/base/java)PHP4和汇编语言，而可选的Pygments和CTags parser这两个解析器是可以支持其他语言的。我用notepad++是开发lua脚本的，所以看来只能选择后两种解析器了。可是CTags parser实际上只能用到Find Definitions而Find Reference（查找引用）这个功能没有用。Pygments这个一直不能成功使用，提示"gtags: cannot open shared object 'pygments-parser.dll'.

    发现lua不能被支持挺沮丧的。不过后来试了一下将.lua后缀名改成.c后竟然能用！于是想了一下将整个工程文件下的.lua重命名为.c或.hpp来支持lua的查找引用。我也实际写了个脚本这么做了，可以这样查找所有的引用，但是运行的时候还是需要.lua。如果复制一份并修改复制的.lua为.c或.hpp，则无法在完美支持查找引用的同时进行修改，因为修改了以后需要将相应的修改应用到修改了后缀名的文件上，并且查找引用里面的点击跳转无法跳转到.lua文件，只能跳转到改了后缀名的文件里面，这样无法及时对相应lua文件进行修改。

    最后在查找pygments不能使用（因为网上有人说跳出提示说pygments-parser.dll打不开的原因是指定的路径不对）的过程中发现修改gtags.conf文件可以达到我们想要的目的。:)

    另外一个同类型的插件在plugin manager里面叫“Gtags Search"，安装好后叫做"gtagfornplus"。但是经过试用，我发现在当前最新版本(6.7.9.2)的notepad++中无法使用。使用方法参见：[http://sourceforge.net/p/gtagfornplus/code/HEAD/tree/](http://sourceforge.net/p/gtagfornplus/code/HEAD/tree/)









