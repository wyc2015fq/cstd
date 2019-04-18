# 终于搞懂了，预编译头文件(precompiled header) - ljx0305的专栏 - CSDN博客
2010年05月11日 17:08:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：577标签：[header																[mfc																[编译器																[file																[components																[internet](https://so.csdn.net/so/search/s.do?q=internet&t=blog)](https://so.csdn.net/so/search/s.do?q=components&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)

来源：[VCer.net](http://vcer.net/) 文章地址：[http://vcer.net/1047209431368.html](http://vcer.net/1047209431368.html)
预编译头文件
今天在改一个很大的程序，慢慢看，慢慢改。突然发现一个.c文件，里面什么也没有， 
就几个头文件，我一看，我靠，这不是把简单的问题搞复杂了吗，随手删掉那个c文件。 
结果不能编译了，我靠： 
fatal error C1083: Cannot open precompiled header file: /'Debug/v13_3.pch/': 
No such file or directory 
怎么rebuild all都不行。 
上网查了一下，才搞懂了： 
－－－－－－－－－－－－－－－－总结－－－－－－ 
如果工程很大，头文件很多，而有几个头文件又是经常要用的，那么 
1。把这些头文件全部写到一个头文件里面去，比如写到preh.h 
2。写一个preh.c，里面只一句话：#include "preh.h" 
3。对于preh.c，在project setting里面设置creat precompiled headers，对于其他 
.c文件，设置use precompiled header file 
// 
哈哈 
我试了一下，效果很明显，不用precompiled header，编译一次我可以去上个厕所，用 
precompiled header，编译的时候，我可以站起来伸个懒腰，活动活动就差不多啦 
－－－－－－－－－转载的文章－－－－－－－－－－ 
预编译头的概念： 
所谓的预编译头就是把一个工程中的那一部分代码,预先编译好放在一个文件里(通常是 
以.pch为扩展名的)，这个文件就称为预编译头文件这些预先编译好的代码可以是任何的 
C/C++代码--------甚至是inline的函数，但是必须是稳定的，在工程开发的过程中不会 
被经常改变。如果这些代码被修改，则需要重新编译生成预编译头文件。注意生成预编 
译头文件是很耗时间的。同时你得注意预编译头文件通常很大，通常有6-7M大。注意及 
时清理那些没有用的预编译头文件。 
也许你会问：现在的编译器都有Time stamp的功能，编译器在编译整个工程的时候，它 
只会编译那些经过修改的文件，而不会去编译那些从上次编译过，到现在没有被修改过 
的文件。那么为什么还要预编译头文件呢？答案在这里，我们知道编译器是以文件为单 
位编译的，一个文件经过修改后，会重新编译整个文件，当然在这个文件里包含的所有 
头文件中的东西（.eg Macro, Preprocesser ）都要重新处理一遍。VC的预编译头文件 
保存的正是这部分信息。以避免每次都要重新处理这些头文件。 
预编译头的作用： 
根据上文介绍，预编译头文件的作用当然就是提高便宜速度了，有了它你没有必要每次 
都编译那些不需要经常改变的代码。编译性能当然就提高了。 
预编译头的使用： 
要使用预编译头，我们必须指定一个头文件，这个头文件包含我们不会经常改变的 
代码和其他的头文件，然后我们用这个头文件来生成一个预编译头文件（.pch文件） 
想必大家都知道 StdAfx.h这个文件。很多人都认为这是VC提供的一个“系统级别”的 
，编译器带的一个头文件。其实不是的，这个文件可以是任何名字的。我们来考察一个 
典型的由AppWizard生成的MFC Dialog Based　程序的预编译头文件。（因为AppWizard 
会为我们指定好如何使用预编译头文件，默认的是StdAfx.h，这是VC起的名字）。我们 
会发现这个头文件里包含了以下的头文件： 
#include <afxwin.h> // MFC core and standard components 
#include <afxext.h> // MFC extensions 
#include <afxdisp.h> // MFC Automation classes 
#include <afxdtctl.h> // MFC support for Internet Explorer 4 
Common Controls 
#include <afxcmn.h> 
这些正是使用MFC的必须包含的头文件，当然我们不太可能在我们的工程中修改这些头文 
件的，所以说他们是稳定的。 
那么我们如何指定它来生成预编译头文件。我们知道一个头文件是不能编译的。所以我 
们还需要一个cpp文件来生成.pch 文件。这个文件默认的就是StdAfx.cpp。在这个文件 
里只有一句代码就是：#include “Stdafx.h”。原因是理所当然的，我们仅仅是要它能 
够编译而已?D?D?D也就是说，要的只是它的.cpp的扩展名。我们可以用/Yc编译开关来指 
定StdAfx.cpp来生成一个.pch文件，通过/Fp编译开关来指定生成的pch文件的名字。打 
开project ->Setting->C/C++ 对话框。把Category指向Precompiled Header。在左边的 
树形视图里选择整个工程　 
Project Options(右下角的那个白的地方)可以看到 /Fp “debug/PCH.pch”，这就是指 
定生成的.pch文件的名字，默认的通常是　<工程名>.pch（我的示例工程名就是PCH）。 
然后，在左边的树形视图里选择StdAfx.cpp.//这时只能选一个cpp文件！ 
这时原来的Project Option变成了 Source File Option（原来是工程，现在是一个文件 
，当然变了）。在这里我们可以看到 /Yc开关，/Yc的作用就是指定这个文件来创建一个 
Pch文件。/Yc后面的文件名是那个包含了稳定代码的头文件，一个工程里只能有一个文 
件的可以有YC开关。VC就根据这个选项把 StdAfx.cpp编译成一个Obj文件和一个PCH文件 
。 
然后我们再选择一个其它的文件来看看，//其他cpp文件 
在这里，Precomplier 选择了 Use ⋯⋯⋯一项，头文件是我们指定创建PCH 文件的stda 
fx.h 
文件。事实上，这里是使用工程里的设置，（如图1）/Yu”stdafx.h”。 
这样，我们就设置好了预编译头文件。也就是说，我们可以使用预编译头功能了。以 
下是注意事项： 
1):如果使用了/Yu，就是说使用了预编译，我们在每个.cpp文件的最开头，我强调一遍 
是最开头，包含 你指定产生pch文件的.h文件（默认是stdafx.h）不然就会有问题。如 
果你没有包含这个文件，就告诉你Unexpected file end. 如果你不是在最开头包含的， 
你自己试以下就知道了，绝对有很惊人的效果⋯.. 
fatal error C1010: unexpected end of file while looking for precompiled 
header directive 
Generating Code... 
2）如果你把pch文件不小心丢了，编译的时候就会产生很多的不正常的行为。根据以上 
的分析，你只要让编译器生成一个pch文件。也就是说把 stdafx.cpp（即指定/Yc的那个 
cpp文件）从新编译一遍。当然你可以傻傻的 Rebuild All。简单一点就是选择那个cpp 
文件，按一下Ctrl + F7就可以了。不然可是很浪费时间的哦。 
// 
呵呵，如果你居然耐着性子看到了这里，那么再回到帖子最开始看看我的总结吧！ 

