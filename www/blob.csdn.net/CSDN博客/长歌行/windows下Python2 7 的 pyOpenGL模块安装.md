# windows下Python2.7 的 pyOpenGL模块安装 - 长歌行 - CSDN博客





2012年12月31日 14:28:34[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2938
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









综合整理的，内容都不知道转手多少次了。






下载Python
下载opengl在python中的插件库pyopengl







[http://blog.csdn.net/tongwcs/article/details/6313544](http://blog.csdn.net/tongwcs/article/details/6313544)


**安装步骤**：



STEP 1：安装Python   目录为xxx/Python2.7

STEP 2: 下载其安装pyOpenGL 模块（[http://pypi.python.org/pypi/PyOpenGL](http://pypi.python.org/pypi/PyOpenGL)），在Window下，可以选择

[PyOpenGL-3.0.1.win32.exe](http://pypi.python.org/packages/any/P/PyOpenGL/PyOpenGL-3.0.1.win32.exe#md5=513cc194af65af4c5a640cf9a1bd8462) ([md5](http://pypi.python.org/pypi?:action=show_md5&digest=513cc194af65af4c5a640cf9a1bd8462))安装包。在安装过程中，会要求选择Python的目录，选择xxx/Python2.7即可。







如果此时运行包含OpenGL.Tk模块的程序，会显示


TclError: can't find package Togl之类的错误。于是有


STEP 3：在联网状态下，运行xxx/Python27/Lib/site-packages/OpenGL中的togl.py



和pyopengl一起下载的demo里面也有很多示例代码




可以创建一个示例test.py

from OpenGL.GL import *

from OpenGL.GLU import *

from OpenGL.GLUT import *


def display():

    glClearColor(0,0,0,0)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glMaterialfv(GL_FRONT, GL_AMBIENT,

    [0.1745, 0.0, 0.1, 0.0])

    glMaterialfv(GL_FRONT, GL_DIFFUSE,

    [0.1, 0.0, 0.6, 0.0])

    glMaterialfv(GL_FRONT, GL_SPECULAR,

    [0.7, 0.6, 0.8, 0.0])

    glMaterialf(GL_FRONT, GL_SHININESS, 80)

    glutSolidTeapot(0.5)

    glutSwapBuffers()


glutInit("hello")

glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH)

glutInitWindowSize(400,400)

glutCreateWindow("hello ");


glutDisplayFunc(display)

glutMainLoop()   








**在python shell中 file->open**











**推荐**


其它的关于pyOpenGL的问题可以关注一下网站：

[http://pyopengl.sourceforge.net/](http://pyopengl.sourceforge.net/)

[http://pyopengl.sourceforge.net/documentation/installation.html](http://pyopengl.sourceforge.net/documentation/installation.html)



**鸣谢**


Deitel的Python编程书籍

[http://pyopengl.sourceforge.net/](http://pyopengl.sourceforge.net/)上的相关文章

Python技术交流群中的Ｑｕａｎ┃的群友








Python的IDE

我先给一个初步的表格吧，大家如果有什么意见，或有补充，欢迎提出。有些我没有用过，先不写了。
以下是我使用过的python IDE:
(更新与2010年1月20日)

┌─────┬────┬────┬──┬────┬──┬─────┬─────┐ 
│IDE name│自动补全│智能感知│调试│语法检查│开源│特别注意  │  推荐度    │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│IDLE        │ 手动    │ 有(很差)│用库│  无      │开源│               │★★         │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│PythonWin│手动　 │有        │用库│  无      │开源│Win Only  │★★★     │
├─────┼────┼────┼──┼────┼──┼─────┼─────┤
│SPE         │无        │有        │WPDB│存盘时│开源│                │★★★★  │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│Ulipad     │有         │有        │WPDB│自动   │开源│               │★★★★★│ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│Eric         │有         │有        │类VC│自动     │开源│                │★★★★★│  
├─────┼────┼────┼──┼────┼──┼─────┼─────┤  
│BOA        │手动　　│手动      │类VC│无       │开源│中文支持差│★★★      │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│WingIDE  │有         │有        │类VC│手动    │共享│中文要设置│★★★★   │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│Komodo  │无         │有         │类VC│手动    │共享│相当耗资源│★★★★   │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│VIM+插件 │有　　　│无      │无   │无          │开源│             │★★★    │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│emacs+插件│有　  │无        │无  │无           │开源│               │★★★    │ 
├─────┼────┼────┼──┼────┼──┼─────┼─────┤ 
│eclipse+  │有　      │有        │类VC│自动    │开源│比较耗资源│★★★★★│ 
│pydev插件│            │           │      │            │      │                │              │ 
├─────┴─┬──┼────┼──┼────┼──┼─────┼─────┤ 
│VS.Net 2003   │无  │有(很差)│类VC│无      │共享│兼容性很差│★(基本不 │ 
│+VisualPython │    │            │     │            │      │已停止维护│能用)       │ 
└───────┴──┴────┴──┴────┴──┴─────┴─────┘

除了PythonWin, VisualPython只支持Windows，其它都至少支持Win/Linux/Mac。
各项含义：
自动补全：变量/函数名打到一半时，提示可能的完整的变量/函数名。
智能感知：在库/类/对象后打"."后，提示可能的函数或变量。
调试：分四档，从好用到不好用分别为“类VC”(调试器操作方式与VC/eclipse相似)，“WPDB”(使用WinPdb作为调试器)，“用库”(要配合专门的python调试库，即要改代码来配合调试)，最惨的当然是“无”啦。
语法检查：从好用到不好用分别为“自动”(写完一行查一行)，“存盘时”(存盘时自动检查，也可以在菜单里手动选择检查)，“手动”(在菜单里选择检查)，“无”(没有语法检查功能)
开源：分为开源，共享(提供免费试用，然后需要付费)，收费三种。目前还没有“收费”这一类。
推荐度：五星为最推荐，一星为最不推荐。推荐度为作者主观评价，不代表其他人意见。

各IDE简介及下载地址(注意本文最后修改时间是2010年1月)：
IDLE:
装了python就会有这个，大家肯定都用过了，功能还凑合，调试器的使用方法和大家熟悉的eclipse/Visual Studio很不一样，需要学习和适应。各项表现都一般。推荐度：★★
下载：装了python就有了，不用专门下载。
下载python请去[http://www.python.org/ftp/python/](http://www.python.org/ftp/python/)

PythonWin:
内置Win32 extension，PythonWin成为了win32的python程序开发者必备的工具。虽然它只能运行在Win下，但其实也是开源的。功能上可以认为它是加上了自动补全和智能感知功能的IDLE，虽然和以其它一些复杂的IDE相比有些差距，但却是不错的轻量级Python IDE。推荐度★★★
下载：[http://starship.python.net/crew/mhammond/win32/Downloads.html](http://starship.python.net/crew/mhammond/win32/Downloads.html)

SPE:
全名Stani's Python Editor。相当不错的IDE，语法高亮、代码折叠、智能感知、自动语法检查等功能一应俱全，集成wxGlade。可惜没有自动补全功能。开源，可以用 svn下载到最新的源代码，依赖wxPython。但久未更新(最后一次更新是在2008年2月)，逆水行舟，不进则退，功能上比其它IDE已经没有什么优势了。推荐度★★★★
下载：总有人说下不到SPE，去这里看看：
[http://developer.berlios.de/project/showfiles.php?group_id=4161](http://developer.berlios.de/project/showfiles.php?group_id=4161)
SVN方式下载：
[http://pythonide.blogspot.com/2007/02/how-to-download-latest-spe-from_26.html](http://pythonide.blogspot.com/2007/02/how-to-download-latest-spe-from_26.html)

Ulipad:
前身是NewEdit，和SPE相比，多了自动补全功能，因而比SPE更加方便，不过没有把界面设计器wxGlade集成进来。开源，可以用svn下载到最新的源代码，依赖wxPython。轻便小巧而功能强大，非常适合初学者。推荐度★★★★★。
下载：[http://code.google.com/p/ulipad/downloads/list](http://code.google.com/p/ulipad/downloads/list)

Eric:
Eric 升级到4后，各方面有了很强的提升，全方位超过其它开源IDE。使用PyQt4作为图形库，界面美观大方，并与QtDesigner结合，使得开发GUI 程序变得非常方便，比下面将提到到BOA还要好用。最大的亮点莫过于它的调试器，支持断点设置、单步调试和变量值查看。一句话，有了Eric4，就不用再去捣腾商业的IDE了。推荐度★★★★★，个人强烈推荐。
Eric4在Windows下的安装有些要注意的地方，参见：
[http://hi.baidu.com/runningon/blog/item/091dd009c4c80187d1581b05.html](http://hi.baidu.com/runningon/blog/item/091dd009c4c80187d1581b05.html)

Boa Constructor:
比起SPE和Ulipad，BOA的编辑功能相当单薄，自动补全与智能感知都要手动，而且没有自动语法检查，但调试器比较好用。最大的亮点是界面设计器相当好用，比wxGlade要好用得多。硬伤是对中文支持不好。依赖wxPython。这个IDE也是久未更新了，最后一次更新是在2007年7月，没有什么特别的理由的话就别用它了。推荐度★★★。
下载：
[http://sourceforge.net/projects/boa-constructor/files/](http://sourceforge.net/projects/boa-constructor/files/)

WingIDE:
很不错的商业软件，调试器是类VC/eclipse的，相当好用，而且还支持project组织。但默认的设置是不支持中文的，要设置一下字体。不开源，而且破解不好找。既然已经有了好用的开源软件了，又何必再用盗版的呢？推荐度★★★★
下载(30天试用)：[http://www.wingware.com/downloads](http://www.wingware.com/downloads)

Komodo:
由 ActiveState公司制作，该公司的ActivePython和ActivePerl可是相当有名。Komodo和WingIDE一样也是很不错的商业软件，可以说WingIDE有的大部分优点Komodo也有，非常可惜没有自动补全。还支持宏录制(类似MS Office的宏录制)，不过这也只是个噱头。不爽之处在于相当耗资源，我AMD
 64位双核+1G内存+5400转的硬盘的本本，启动它时硬盘闪了足足一分钟。推荐度★★★★
下载(21天试用)：[http://www.activestate.com/komodo/](http://www.activestate.com/komodo/)

VIM/emacs + 插件:
Linux我也玩了好几年了，平心而论这两个东东不适合初学(的开发)者。现在Linux已经比较好用了，不会vim/emacs也不是什么大不了的事了，特别是后者。推荐度★★★。
下载：(偶母鸡啦，自个google吧，囧rz)

eclipse + pydev:
依靠强大的eclipse，pydev显得格外耀眼。强大的调试功能和舒服的编辑环境让pydev赢得了许多人的青睐。不过eclipse本来就是耗资源大户，pydev在这一点上毫无办法，但现在的电脑都很快了，跑eclipse根本就是小case，不用担心太多。除了强大的调试功能，依靠 eclipse，pydev的代码浏览能力是其它所有IDE所不能比的，比如按住CTRL键再点击一个类就能自动跳到该类的定义，这对于阅读代码非常有用。现在pydev
 extension也开源了，所以一些(我不知道的)高级的功能也被融入到pydev里了。推荐度★★★★★
下载：[http://pydev.org/download.html](http://pydev.org/download.html)
注意pydev可以在eclipse里在线安装，请阅读上面那个网页的右边的Quick Install.

VisualStudio.Net 2003 + VisualPython:
已经停止维护了，烂就一个字，多说无益，基本不能用。推荐度★
下载：(不用了吧)



总结：

1. 如果你是新手，请用ulipad，轻便小巧强大。
2. 如果你是熟手，请用eclipse+pydev，或(如果对调试功能要求不高)ulipad
3. 如果你是vi/emacs狂人，那不用我多说了吧……
4. 如果你要用QT4写GUI程序，请用Eric。一方面因为Eric已经相当不错，另一方面——其实也没其它比较好的选择了。
5. 如果你要用其它图形库写GUI程序，那么用哪个IDE都差不多，所以请参照1和2。




一个资源网站

pygame

[http://eyehere.net/2011/python-pygame-novice-professional-index/](http://eyehere.net/2011/python-pygame-novice-professional-index/)







