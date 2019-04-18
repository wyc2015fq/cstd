# VC++ 操作Word（使用微软office控件） - 深之JohnChen的专栏 - CSDN博客

2019年03月04日 23:25:33[byxdaz](https://me.csdn.net/byxdaz)阅读数：1605


        使用VC编程来操纵Office。你可以实现诸如：Word文件打印、传送数据到Word文档、发送E-MAIL、自动产生表格、Excel数据统计、圆饼图，直方图显示、自动报表生成、播放幻灯、doc，txt，HTML，rtf文件转换、中文简繁体转换、拼音或笔画排序......只要是Office能够实现的功能，都可以在你写的程序中调用。仔细阅读下面的说明，并下载源文件进行参考，你就可以一步一步地掌握这个技术。祝朋友们学习快乐。

**MSDN搜：microsoft.office.interop.word**

Also：https://msdn.microsoft.com/zh-cn/library/Microsoft.Office.Interop.Word(v=office.11).aspx

|使用VC编程来操纵Office。你可以实现诸如：Word文件打印、传送数据到Word文档、发送E-MAIL、自动产生表格、Excel数据统计、圆饼图，直方图显示、自动报表生成、播放幻灯、doc，txt，HTML，rtf文件转换、中文简繁体转换、拼音或笔画排序......只要是Office能够实现的功能，都可以在你写的程序中调用。仔细阅读下面的说明，并下载源文件进行参考，你就可以一步一步地掌握这个技术。祝朋友们学习快乐。一、概念Microsoft 的 Office 产品中，都提供了OLE Automation 自动化程序的接口。**如果你使用VB，VBA 和 Script 脚本调用 Office 功能的话，其实比使用 VC 调用要简单的多。**比如在 WORD 中，调出菜单“**工具(T)\宏(M)\录制新宏(R)”**，这时候它开始记录你在 WORD 中任何菜单和键盘的操作，把你的操作过程保存起来，以便再次重复调用。而保存这些操作的记录，其实就是使用了 VBA 程序（Visual Basic for Application）。而我们下面要实现的功能，也同样要参考 VBA 的方法。二、结构层次为了更有逻辑，更有层次地操作 Office，Microsoft 把应用(Application)按逻辑功能划分为如下的树形结构Application(WORD 为例，只列出一部分）　　Documents（所有的文档）        Document（一个文档）            ......　　Templates（所有模板）        Template（一个模板）            ......　　Windows（所有窗口）        Window        Selection        View　　Selection（编辑对象）        Font        Style        Range        ......只有了解了逻辑层次，我们才能正确的操纵 Office。举例来讲，如果给出一个VBScript语句是：application.ActiveDocument.SaveAs "c:\abc.doc"那么，我们就知道了，这个操作的过程是：第一步，取得Application；第二步，从Application中取得ActiveDocument；第三步，调用 Document 的函数 SaveAs，参数是一个字符串型的文件名。三、基本步骤（1）创建（或打开已有的）一个 MFC 的程序工程（2）Ctrl+W 执行 ClassWizard（本文按照 VC6 操作，例子程序也是在VC6 下编写测试的）（3）Add Class...\From a type Library... 在 Office 目录中，找到你想使用的类型库。（我使用的是 Office2000，其Word 的类型库文件，保存在 C:\Program Files\Microsoft Office\Office\MSWORD9.OLB）根据你 Office 的版本，可以使用下表列出的类型库文件|Office 版本和类型|类型库文件|Office 版本和类型|类型库文件||----|----|----|----||Access 97|Msacc8.olb|PowerPoint 2000|Msppt9.olb||Jet Database|3.5 DAO350.dll|Word 2000|Msword9.olb||Binder 97|Msbdr8.olb|Access 2002|Msacc.olb||Excel 97|Excel8.olb|Excel 2002|Excel.exe||Graph 97|Graph8.olb|Graph 2002|Graph.exe||Office 97|Mso97.dll|Office 2002|MSO.dll||Outlook 97|Msoutl97.olb|Outlook 2002|MSOutl.olb||PowerPoint 97|Msppt8.olb|PowerPoint 2002|MSPpt.olb||Word 97|Msword8.olb|Word 2002|MSWord.olb||Access 2000|Msacc9.olb|Office Access 2003|Msacc.olb||Jet Database 3.51|DAO360.dll|Office Excel 2003|Excel.exe||Binder 2000|Msbdr9.olb|Graph 2003|Graph.exe||Excel 2000|Excel9.olb|Office 2003|MSO.dll||Graph 2000|Graph9.olb|Office Outlook 2003|MSOutl.olb||Office 2000|Mso9.dll|Office PowerPoint 2003|MSPpt.olb||Outlook 2000|Msoutl9.olb|Office Word 2003|MSWord.olb|（4）选择类型库文件后，在弹出的对话窗中继续选择要添加的类。具体选择什么类，要看你将来在程序中打算调用什么功能。当然，你也可以不用考虑这么多，用鼠标和Shift键配合，全部选择也可以。（5）初始化COM。方法一，找到App的InitInstance()函数，在其中添加 AfxOleInit()函数的调用；方法二，在需要调用COM功能的地方 CoInitialize(NULL)，调用完毕后 CoUninitialize()。（6）在你需要调用 Office 功能函数的 cpp 文件中#include   // 为了方便操作 VARIANT 类型变量，使用 CComVariant 模板类#include "头文件.h"   // 具体的头文件名，是由装载类型库的文件名决定的。(鼠标双点包装类的文件，就可以看到)// 比如使用 msword9.olb类型库，那么头文件是 msword9.h（7）好了，现在开始写程序吧。另外要说明的是，步骤3和4，其实也可以使用 #import 方式引入类型库。四、实现技巧在书写调用 Office 函数的过程中，最困难的是确定函数的参数，一般情况下，参数都是 VARIANT 类型的变量指针。那么到底具体我们应该怎么写那？推荐两个方法，其一是阅读有关 VBA 的书籍；其二，是使用 Office 中自带的“宏”功能。强烈推荐大家使用第二个方法，把你要完成的功能，在 Office 的操作环境中，用宏录制下来，然后观察分析录制后的函数和参数，就可以在 VC 中使用了。举一个例子：`1.``ActiveDocument.SaveAs FileName:=``"Hello.doc"``, FileFormat:=wdFormatDocument _``2.``, LockComments:=False, Password:=``""``, AddToRecentFiles:=True, _``3.``WritePassword:=``""``, ReadOnlyRecommended:=False, EmbedTrueTypeFonts:=False, _``4.``SaveNativePictureFormat:=False, SaveFormsData:=False, SaveAsAOCELetter:= _``5.``False`以上是在 Word 中录制的一个保存文件的宏，而在 VC 中对应的函数原型为`1.``void``_Document::SaveAs(VARIANT* FileName, VARIANT* FileFormat, VARIANT* LockComments,``2.``VARIANT* Password, VARIANT* AddToRecentFiles, VARIANT* WritePassword,``3.``VARIANT* ReadOnlyRecommended, VARIANT* EmbedTrueTypeFonts, VARIANT* SaveNativePictureFormat,``4.``VARIANT* SaveFormsData, VARIANT* SaveAsAOCELetter)`分析对照后，我们就能看出，参数 FileName 是字符串 VARIANT(VT_BSTR),参数 LockComments 是布尔VARIANT(VT_BOOL),等等。参数 FileFormat := wdFormatDocument 是什么类型那？其实这是一个表示保存的时候指定文件类型的常量，而且显然是 DWORD 类型VARIANT(VT_I4)。那么常量的数值又是多少那？很简单，写一个宏，调用函数 MsgBox 显示一下不就都知道啦？！五、步步为营特别提示一：编译执行前，一定要先关闭 KV 实时监视病毒的功能（KV 的程序会干扰我们的调用，瑞星的则没关系）。特别提示二：在例子程序中，为了表现程序的关键部分，没有或很少使用了条件判断。为了实现你程序的健壮性，请自己加上条件判断和异常处理。Step1：如何启动和关闭 WORD，及 VARIANT 的最基本的使用方法Step2：和 Step1 同样功能，用 CComVariant 改进了 VARIANT 的使用方式Step3：在 Step2 的基础上，新建一个 WORD 文档，并从程序中传送一些字符到 WORDStep4：在 Step3 的基础上，保存 WORD 文档Step5：一个小应用举例，把输入的汉字按照“笔画”排序Step6：一个小应用举例，盗窃正在使用的 WORD 文档以上这6个小程序中，都有详细的注释。大家阅读后慢慢体会并实验，你就可以自由地操纵任何一个 Office 啦。六、参考：Microsoft Office Development with Visual Studio|Office 版本和类型|类型库文件|Office 版本和类型|类型库文件|Access 97|Msacc8.olb|PowerPoint 2000|Msppt9.olb|Jet Database|3.5 DAO350.dll|Word 2000|Msword9.olb|Binder 97|Msbdr8.olb|Access 2002|Msacc.olb|Excel 97|Excel8.olb|Excel 2002|Excel.exe|Graph 97|Graph8.olb|Graph 2002|Graph.exe|Office 97|Mso97.dll|Office 2002|MSO.dll|Outlook 97|Msoutl97.olb|Outlook 2002|MSOutl.olb|PowerPoint 97|Msppt8.olb|PowerPoint 2002|MSPpt.olb|Word 97|Msword8.olb|Word 2002|MSWord.olb|Access 2000|Msacc9.olb|Office Access 2003|Msacc.olb|Jet Database 3.51|DAO360.dll|Office Excel 2003|Excel.exe|Binder 2000|Msbdr9.olb|Graph 2003|Graph.exe|Excel 2000|Excel9.olb|Office 2003|MSO.dll|Graph 2000|Graph9.olb|Office Outlook 2003|MSOutl.olb|Office 2000|Mso9.dll|Office PowerPoint 2003|MSPpt.olb|Outlook 2000|Msoutl9.olb|Office Word 2003|MSWord.olb|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|Office 版本和类型|类型库文件|Office 版本和类型|类型库文件| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Access 97|Msacc8.olb|PowerPoint 2000|Msppt9.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Jet Database|3.5 DAO350.dll|Word 2000|Msword9.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Binder 97|Msbdr8.olb|Access 2002|Msacc.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Excel 97|Excel8.olb|Excel 2002|Excel.exe| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Graph 97|Graph8.olb|Graph 2002|Graph.exe| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Office 97|Mso97.dll|Office 2002|MSO.dll| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Outlook 97|Msoutl97.olb|Outlook 2002|MSOutl.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|PowerPoint 97|Msppt8.olb|PowerPoint 2002|MSPpt.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Word 97|Msword8.olb|Word 2002|MSWord.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Access 2000|Msacc9.olb|Office Access 2003|Msacc.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Jet Database 3.51|DAO360.dll|Office Excel 2003|Excel.exe| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Binder 2000|Msbdr9.olb|Graph 2003|Graph.exe| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Excel 2000|Excel9.olb|Office 2003|MSO.dll| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Graph 2000|Graph9.olb|Office Outlook 2003|MSOutl.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Office 2000|Mso9.dll|Office PowerPoint 2003|MSPpt.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|Outlook 2000|Msoutl9.olb|Office Word 2003|MSWord.olb| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |

（4）选择类型库文件后，在弹出的对话窗中继续选择要添加的类。具体选择什么类，要看你将来在程序中打算调用什么功能。当然，你也可以不用考虑这么多，用鼠标和Shift键配合，全部选择也可以。

（5）初始化COM。方法一，找到App的InitInstance()函数，在其中添加 AfxOleInit()函数的调用；方法二，在需要调用COM功能的地方 CoInitialize(NULL)，调用完毕后 CoUninitialize()。

（6）在你需要调用 Office 功能函数的 cpp 文件中

#include   // 为了方便操作 VARIANT 类型变量，使用 CComVariant 模板类

#include "头文件.h"   // 具体的头文件名，是由装载类型库的文件名决定的。(鼠标双点包装类的文件，就可以看到)

// 比如使用 msword9.olb类型库，那么头文件是 msword9.h

（7）好了，现在开始写程序吧。另外要说明的是，步骤3和4，其实也可以使用 #import 方式引入类型库。

四、实现技巧

在书写调用 Office 函数的过程中，最困难的是确定函数的参数，一般情况下，参数都是 VARIANT 类型的变量指针。那么到底具体我们应该怎么写那？推荐两个方法，其一是阅读有关 VBA 的书籍；其二，是使用 Office 中自带的“宏”功能。强烈推荐大家使用第二个方法，把你要完成的功能，在 Office 的操作环境中，用宏录制下来，然后观察分析录制后的函数和参数，就可以在 VC 中使用了。举一个例子：

`1.``ActiveDocument.SaveAs FileName:=``"Hello.doc"``, FileFormat:=wdFormatDocument _`

`2.``, LockComments:=False, Password:=``""``, AddToRecentFiles:=True, _`

`3.``WritePassword:=``""``, ReadOnlyRecommended:=False, EmbedTrueTypeFonts:=False, _`

`4.``SaveNativePictureFormat:=False, SaveFormsData:=False, SaveAsAOCELetter:= _`

`5.``False`

以上是在 Word 中录制的一个保存文件的宏，而在 VC 中对应的函数原型为

`1.``void``_Document::SaveAs(VARIANT* FileName, VARIANT* FileFormat, VARIANT* LockComments,`

`2.``VARIANT* Password, VARIANT* AddToRecentFiles, VARIANT* WritePassword,`

`3.``VARIANT* ReadOnlyRecommended, VARIANT* EmbedTrueTypeFonts, VARIANT* SaveNativePictureFormat,`

`4.``VARIANT* SaveFormsData, VARIANT* SaveAsAOCELetter)`

分析对照后，我们就能看出，参数 FileName 是字符串 VARIANT(VT_BSTR),参数 LockComments 是布尔VARIANT(VT_BOOL),等等。参数 FileFormat := wdFormatDocument 是什么类型那？其实这是一个表示保存的时候指定文件类型的常量，而且显然是 DWORD 类型VARIANT(VT_I4)。那么常量的数值又是多少那？很简单，写一个宏，调用函数 MsgBox 显示一下不就都知道啦？！

五、步步为营

特别提示一：编译执行前，一定要先关闭 KV 实时监视病毒的功能（KV 的程序会干扰我们的调用，瑞星的则没关系）。

特别提示二：在例子程序中，为了表现程序的关键部分，没有或很少使用了条件判断。为了实现你程序的健壮性，请自己加上条件判断和异常处理。

Step1：如何启动和关闭 WORD，及 VARIANT 的最基本的使用方法

Step2：和 Step1 同样功能，用 CComVariant 改进了 VARIANT 的使用方式

Step3：在 Step2 的基础上，新建一个 WORD 文档，并从程序中传送一些字符到 WORD

Step4：在 Step3 的基础上，保存 WORD 文档

Step5：一个小应用举例，把输入的汉字按照“笔画”排序

Step6：一个小应用举例，盗窃正在使用的 WORD 文档

以上这6个小程序中，都有详细的注释。大家阅读后慢慢体会并实验，你就可以自由地操纵任何一个 Office 啦。

六、参考：Microsoft Office Development with Visual Studio　

参考资料：

[http://msdn.microsoft.com/zh-cn/library/microsoft.office.interop.word(v=office.11).aspx](http://msdn.microsoft.com/zh-cn/library/microsoft.office.interop.word(v=office.11).aspx)

[http://bbs.csdn.net/topics/290025616](http://bbs.csdn.net/topics/290025616)

[http://blog.sciencenet.cn/blog-43777-318939.html](http://blog.sciencenet.cn/blog-43777-318939.html)

[http://www.cnitblog.com/lifw/articles/vcpp_officeword.html](http://www.cnitblog.com/lifw/articles/vcpp_officeword.html)

[http://www.cnblogs.com/Raikkonen/archive/2010/05/19/1738980.html](http://www.cnblogs.com/Raikkonen/archive/2010/05/19/1738980.html)

[http://blog.csdn.net/zhaoshilei29/article/details/5946333](http://blog.csdn.net/zhaoshilei29/article/details/5946333)

====================================================================

1.实现技巧

写Word文档，首先要从Word应用程序获取文档的类的对象，然后设置文档中接收文字位置，最后将文字写入Word文档。

注：Microsoft Office 2003类型库msword.olb常用的_Application，_Document，Documents，Selection四个类

按照添加类的步骤,将_Application应用程序类和Documents类和Selection类加进应用程序。

在View菜单中，点击ClassWizard，然后进入Automation标签中点击Add Class，选择From A Type Library。找到Microsoft Office 2003类型库msword.olb，选择_Application，_Document，_Documents，Selection四个类，如有其他需要(例如画表也可选择其他的类)。

Documents类是文档对象集类，是所有Document对象的集合。使用Documents对象集合的Add方法可以新建一篇空白文档并将返回的Document 对象添至Documents对象集合之中。

Add方法的原型声明如下：
`LPDISPATCH  Add( VARIANT* Template,  VARIANT* NewTemplate,  VARIANT* DocumentType,  VARIANT* Visible)`
参数Template，指定新文档使用的模板名，如果忽略此参数则使用Normal模板。

 

参数NewTemplate，如果此属性设置为 True 则将文档作为模板打开。默认值为 False。

 

参数DocumentType其值可取下列 WdNewDocumentType常量之一。

 

WdNewBlankDocument 创建一个空白文档，默认值。

 

WdNewEmailMessage 新建一个电子邮件信息。

 

wdNewWebPage 新建一个Web页。

 

参数Visible，如果此参数为True，Microsoft Word将在可见窗口打开文档。如果此参数为False，Word仍会打开此文档但文档窗口的Visible属性变为False，默认值为True。

Selection类对象用于选定文档的文字，然后对选定部分进行操作，如设置文字的格式或键入文字。Selection 对象代表窗体中的选定内容，每个窗体中只能有一个Selection 对象而且只能激活一个 Selection 对象。Selection 对象代表的选定内容既可以是文档中的一个区域也可以仅仅是一个插入点。通过Selection对象的TypeText和GetText可对Word文档进行读写操作。

TypeText的函数作用为写Word文档，其函数的声明如下：
`void TypeText(LPCTSTR Text);`
GetText的函数的作用是读文档，其函数的声明如下：
`CString GetText();`

 1. VC6的操作：

     在第（4）步后，VC6会自动生成"msword.h"和"msword.cpp"两个文件，同时在类视图中会生成你选择的类（譬如_Application _Document等等）；

     此时你只要加入#include "msword.h"，就完成了准备工作。

2. VC2005\2008

     VC2005不会生成"msword.h"和"msword.cpp"这两个文件，[取而代之](https://www.baidu.com/s?wd=%E5%8F%96%E8%80%8C%E4%BB%A3%E4%B9%8B&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的是生成"CApplication.h" "CDocument0.h"等一系列头文件；

     但如果你以为加入了#include "CApplication.h" ，便可以完成准备工作的话，那就大错特错了，此时编译会出现很多错误；

     该怎么办呢？

     把VC6中生成的"msword.h"和"msword.cpp"两个文件拷贝到VC2005工程文件夹目录中，并加入到工程中（项目-添加现有项）；

     然后加上以下代码就一切OK了！

===================================================================

作者：[http://blog.csdn.net/zhaoshilei29/article/details/5946333](http://blog.csdn.net/zhaoshilei29/article/details/5946333)

－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

首先，把我参考的两个地址放上来，博主写到很好，我这里只是把学到的东西做记录自己备查而已。

用Visual C++操纵MS Word：

[http://www.cnblogs.com/scq2099yt/archive/2008/01/07/1028697.html](http://www.cnblogs.com/scq2099yt/archive/2008/01/07/1028697.html)

MFC/VC++调用word进行报表制作：

[http://www.cnblogs.com/scq2099yt/archive/2008/01/07/1028717.html](http://www.cnblogs.com/scq2099yt/archive/2008/01/07/1028717.html)

其次，说说重点：

重点就是怎么把VBA_Sub MyReporter.bas里的VBA代码转换成WORD.OLE代码，上面的两篇文章写的很好，照着一步一步做就成了。

下面，一步一步开始吧：

![](http://write.blog.csdn.net/postedit/8500474)

1. 首先，要打开Word的宏记录功能。

备注：在Word中，使用VBA为脚本的宏，可以使文件具有一定逻辑处理功能。例如自动生成及处理数据等。

![](http://write.blog.csdn.net/postedit/8500474)

2. 然后创建一个新宏

![](http://write.blog.csdn.net/postedit/8500474)

3. 记录一个宏，在记录好以后，可以对宏进行简单的编辑，想调试看运行结果时，点击“运行”按钮。这一步很重要，在把这个VBA脚本翻译成C++可执行的代码前，这里的逻辑就是最后生成Word文档的逻辑，因此，要仔细调试，把其中没有必要的步骤去除，尽量精简生成文档的步骤。

4. 在最终获得一个这样的脚本后，就可以准备开始C++的编程了，这里，贴出我完成的脚本（为了不公开项目里的数据，这里我稍做改动，把数据相关的内容全部替换了）

Sub MyReporter()

    Documents.Add DocumentType:=wdNewBlankDocument ' //生成一个空文档

    Selection.TypeText Text:="某某报表" ' //输入第1行内容

    Selection.TypeParagraph ' //换行

    Selection.TypeText Text:="行1：" ' //输入第2行内容

    Selection.TypeParagraph ' //换行

    Selection.TypeText Text:="行2：" ' //输入第3行内容

    Selection.TypeParagraph ' //换行

    Selection.TypeText Text:="行3：" ' //输入第4行内容

    Selection.TypeParagraph ' //换行

    Selection.TypeText Text:="行4：" ' //输入第5行内容

    Selection.TypeParagraph ' //换行

    Selection.TypeText Text:="行5" ' //输入第6行内容

    Selection.TypeParagraph ' //换行

    ' //添加一个6行4列的表格，使用默认样式 

    ActiveDocument.Tables.Add Range:=Selection.Range, NumRows:=6, NumColumns:= _

        4, DefaultTableBehavior:=wdWord9TableBehavior, AutoFitBehavior:= _

        wdAutoFitFixed

    ' //添加表的内容并设置表的格式

    ' //输入表内第1行内容 

    Selection.TypeText Text:="列1" ' //输入第1列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=1 ' //向右移动鼠标到下一列

    Selection.TypeText Text:="列2" ' //输入第2列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=1 ' //向右移动鼠标到下一列

    Selection.TypeText Text:="列3" ' //输入第3列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=1 ' //向右移动鼠标到下一列 

    Selection.TypeText Text:="列4" ' //输入第4列内容

    ' //输入表内第2行内容 

    Selection.MoveDown Unit:=wdLine, Count:=1 ' //向下移动鼠标到下一行 

    Selection.MoveLeft Unit:=wdCharacter, Count:=3 ' //向左移动鼠标到第1列

    Selection.TypeText Text:="列1" ' //输入第1列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=3 '// 向右移动鼠标到第4列

    Selection.TypeText Text:="列4" ' //输入第4列内容

    ' //输入表内第3行内容 

    Selection.MoveDown Unit:=wdLine, Count:=1 ' //向下移动鼠标到下一行

    Selection.MoveLeft Unit:=wdCharacter, Count:=3 ' //向左移动鼠标到第1列

    Selection.TypeText Text:="列1" ' //输入第1列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=1 ' // 向右移动鼠标到第2列

    Selection.TypeText Text:="列2" '//输入第2列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=1 ' // 向右移动鼠标到第3列

    Selection.TypeText Text:="列3" '//输入第3列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=1  ' // 向右移动鼠标到第4列

    Selection.TypeText Text:="列4" '//输入第4列内容

    ' //输入表内第4行内容 

    Selection.MoveDown Unit:=wdLine, Count:=1 ' //向下移动鼠标到下一行

    Selection.MoveLeft Unit:=wdCharacter, Count:=3 ' //向左移动鼠标到第1列

    Selection.TypeText Text:="列1" ' //输入第1列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=3 ' //向右移动鼠标到第4列

    Selection.TypeText Text:="列4" ' //输入第4列内容

    ' //输入表内第5行内容 

    Selection.MoveDown Unit:=wdLine, Count:=1 ' //向下移动鼠标到下一行

    Selection.MoveLeft Unit:=wdCharacter, Count:=3 ' //向左移动鼠标到第1列

    Selection.TypeText Text:="列1" ' //输入第1列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=3 ' //向右移动鼠标到第4列

    Selection.TypeText Text:="列4" ' //输入第4列内容

    ' //输入表内第6行内容 

    Selection.MoveDown Unit:=wdLine, Count:=1 ' //向下移动鼠标到下一行

    Selection.MoveLeft Unit:=wdCharacter, Count:=3 ' //向左移动鼠标到第1列

    Selection.TypeText Text:="列1" ' //输入第1列内容

    Selection.MoveRight Unit:=wdCharacter, Count:=3 ' //向右移动鼠标到第4列

    Selection.TypeText Text:="列4" ' //输入第4列内容

    ' //设置表的格式 

    Selection.MoveUp Unit:=wdLine, Count:=5 ' //向上移动鼠标到表的第1行（目前鼠标在第1行第4列最后一个字符后）

    Selection.MoveLeft Unit:=wdCharacter, Count:=15 ' //向左移动鼠标到第1列的第1个字符前

    Selection.MoveDown Unit:=wdLine, Count:=5, Extend:=wdExtend ' //向下选取5行，共选取6行（执行这句后，第1列全部选中）

    Selection.MoveRight Unit:=wdCharacter, Count:=3, Extend:=wdExtend ' //向右选取3列，共选取4列（执行这句后，整个表被选中）

    Selection.ParagraphFormat.Alignment = wdAlignParagraphCenter ' //设置对齐方式为“居中”

    ' //开始设置表以前内容的格式 

    Selection.MoveUp Unit:=wdLine, Count:=1 ' //向上移动鼠标到上一行

    Selection.ParagraphFormat.Alignment = wdAlignParagraphCenter '//设置此行为“居中”格式 

    Selection.MoveUp Unit:=wdLine, Count:=5 '// 向上移动鼠标到第1行

    Selection.ParagraphFormat.Alignment = wdAlignParagraphCenter '//设置标题行为“居中”格式

    Selection.EndKey Unit:=wdLine ' //将鼠标定位到第1行末

    Selection.HomeKey Unit:=wdLine, Extend:=wdExtend ' //将鼠标定位到第1行首，同时选取整行

    Selection.Font.Bold = wdToggle ' //设置标题行为“粗体”    

    Selection.Font.Size = 15 ' //设置标题行字号为15（小三号） 

    Selection.MoveDown Unit:=wdLine, Count:=1 ' //向下移动鼠标到正文第1行

    Selection.HomeKey Unit:=wdLine ' //将鼠标定位到正文第1行首，（第2行行首） 

    Selection.MoveDown Unit:=wdLine, Count:=11, Extend:=wdExtend ' //向下选取除标题行外的全部内容

    Selection.Font.Size = 12 ' //设置字号为12（小四号）

    Selection.WholeStory '// 选取全部内容

    Selection.Font.Name = "宋体" '// 设置全部内容为“宋体”

    ' //保存文件为[《某某报表.doc》](https://www.baidu.com/s?wd=%E3%80%8A%E6%9F%90%E6%9F%90%E6%8A%A5%E8%A1%A8.doc%E3%80%8B&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)到默认位置，WORD的默认路径是“我的文档”

    ActiveDocument.SaveAs FileName:="某某报表.doc", FileFormat:= _

        wdFormatDocument, LockComments:=False, Password:="", AddToRecentFiles:= _

        True, WritePassword:="", ReadOnlyRecommended:=False, EmbedTrueTypeFonts:= _

        False, SaveNativePictureFormat:=False, SaveFormsData:=False, _

        SaveAsAOCELetter:=False

    ' 退出程序，关闭Word

    Application.Quit

End Sub

5. 这就是最后一步了，就是把上面的VBA编程C++可执行的代码。当然，在生成这个工程的时候，一定要添加外部的库，这里，**在MFC中操作WORD2003的库使用的是 MSWORD.OLB ，一般情况下，可以在OFFICE所在的文件夹下**，通过搜索获得。对于如何添加这个外部链接库，这里不做记录了，会MFC的人，这个是基本功。那么在生成MFC工程后，**把上面的VBA代码，一条一条翻译成C++的代码就行了，以下是翻译的结果：**

（把以下代码放到一个事件中，比如对一个Button的单击响应事件中，就可以运行了）

 //****待用的操作WORD的 常量

 const int wdCharacter = 1;

 const int wdLine = 5;

 const int wdAlignParagraphCenter = 1;

 const int wdExtend = 1;

 const int wdToggle = 9999998;

 const int wdFormatDocument = 0;

 const int wdWord9TableBehavior = 1;

  const int wdAutoFitFixed = 0;

 const int wdNULL = 0;

 const int wdNewBlankDocument = 0;

 const int wdWindowStateMinimize = 2;

 //****待用的操作WORD的 变量 

 _Application wordApp;

    Documents wordDocs;

    _Document wordDoc;

    Selection wordSelection;

 Range wordRange;

 Tables wordTables;

 Table wordTable;

 Cell wordCell;

 Cells wordCells;

 _Font wordFont;

 Shading wordShading;

 Paragraphs wordParagraphs;

 //****创建word application实例,失败的话返回false

 if (!wordApp.CreateDispatch(_T("Word.Application"))) 

 {

  AfxMessageBox("Word CreateDispatch Failed!");  //显示错误信息

  return FALSE; //返回false

 }     

 wordDocs=wordApp.GetDocuments();//获得 documents 实例 

 //****添加新的空白文档（调用COM）

 //（参考自：[http://www.cnblogs.com/scq2099yt/archive/2008/01/07/1028717.html](http://www.cnblogs.com/scq2099yt/archive/2008/01/07/1028717.html)）

 CComVariant tpl(_T("")),Visble,DocType(0),NewTemplate(false);

 wordDoc=wordDocs.Add(&tpl,&NewTemplate,&DocType,&Visble);

 wordSelection=wordApp.GetSelection(); //获得 wordSelection 实例

 //****添加文字 

 wordSelection.TypeText("某某报表"); //输出1行

 wordSelection.TypeParagraph(); //换行

 wordSelection.TypeText("行1："); //输出2行

 wordSelection.TypeParagraph(); //换行

 wordSelection.TypeText("行2：");  //输出3行

 wordSelection.TypeParagraph(); //换行

 wordSelection.TypeText("行3："); //输出4行 

 wordSelection.TypeParagraph(); //换行

 wordSelection.TypeText("行4："); //输出5行

 wordSelection.TypeParagraph(); //换行

 wordSelection.TypeText("行5"); //输出6行

 wordSelection.TypeParagraph(); //换行

 //****插入表格 

 wordTables=wordDoc.GetTables(); //获得表格

 wordRange = wordSelection.GetRange();

 //添加一个6行4列的表格，使用默认样式

 wordTables.Add(wordRange, 

  6, // 行数

  4, // 列数

  COleVariant((short)wdWord9TableBehavior),

  COleVariant((short)wdAutoFitFixed)); 

 /*** 注意 COleVariant((short)XXX) 这个方法可以把int型转换为 VARIANT* 类型 ***/

 //添加表的内容并设置表的格式

 //输入表内第1行内容 

 wordSelection.TypeText("列1"); //输入第1列内容

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)1), 

  COleVariant((short)wdNULL)); //向右移动鼠标到下一列

 wordSelection.TypeText("列2"); //输入第2列内容

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)1), 

  COleVariant((short)wdNULL)); //向右移动鼠标到下一列

 wordSelection.TypeText("列3"); //输入第3列内容

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向右移动鼠标到下一列

 wordSelection.TypeText("列4"); //输入第4列内容 

 //输入表内第2行内容 

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向下移动鼠标到下一行

 wordSelection.MoveLeft(COleVariant((short)wdCharacter), 

  COleVariant((short)3),

  COleVariant((short)wdNULL)); //向左移动鼠标到第1列

    wordSelection.TypeText("列1"); //输入第1列内容    

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)3), 

  COleVariant((short)wdNULL)); //向右移动鼠标到第4列

 wordSelection.TypeText("列4"); //输入第4列内容

 //输入表内第3行内容 

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向下移动鼠标到下一行

 wordSelection.MoveLeft(COleVariant((short)wdCharacter), 

  COleVariant((short)3),

  COleVariant((short)wdNULL)); //向左移动鼠标到第1列 

    wordSelection.TypeText("列1"); //输入第1列内容    

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)1), 

  COleVariant((short)wdNULL)); //向右移动鼠标到第2列

 wordSelection.TypeText("列2"); //输入第2列内容

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)1), 

  COleVariant((short)wdNULL)); //向右移动鼠标到第3列

 wordSelection.TypeText("列3"); //输入第3列内容

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)1), 

  COleVariant((short)wdNULL)); //向右移动鼠标到第4列

 wordSelection.TypeText("列4"); //输入第4列内容

 //输入表内第4行内容 

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向下移动鼠标到下一行

 wordSelection.MoveLeft(COleVariant((short)wdCharacter), 

  COleVariant((short)3),

  COleVariant((short)wdNULL)); //向左移动鼠标到第1列

    wordSelection.TypeText("列1"); //输入第1列内容    

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)3), 

  COleVariant((short)wdNULL)); //向右移动鼠标到第4列    

 wordSelection.TypeText("列4"); //输入第4列内容

 //输入表内第5行内容 

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向下移动鼠标到下一行

 wordSelection.MoveLeft(COleVariant((short)wdCharacter), 

  COleVariant((short)3),

  COleVariant((short)wdNULL)); //向左移动鼠标到第1列

    wordSelection.TypeText("列1"); //输入第1列内容    

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)3), 

  COleVariant((short)wdNULL)); //向右移动鼠标到第4列    

 wordSelection.TypeText("列4"); //输入第4列内容

 //输入表内第6行内容 

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向下移动鼠标到下一行

 wordSelection.MoveLeft(COleVariant((short)wdCharacter), 

  COleVariant((short)3),

  COleVariant((short)wdNULL)); //向左移动鼠标到第1列

    wordSelection.TypeText("列1"); //输入第1列内容    

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)3), 

  COleVariant((short)wdNULL)); //向右移动鼠标到第4列

 wordSelection.TypeText("列4"); //输入第4列内容

 //设置表的格式

 wordSelection.MoveUp(COleVariant((short)wdLine), 

  COleVariant((short)5),

  COleVariant((short)wdNULL)); //向上移动鼠标到表的第1行（目前鼠标在第一行第4列最后一个字符后）

 wordSelection.MoveLeft(COleVariant((short)wdCharacter), 

  COleVariant((short)15),

  COleVariant((short)wdNULL)); //向左移动鼠标到第1列的第1个字符前 

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)5),

  COleVariant((short)wdExtend)); //向下选取5行，共选取6行（执行这句后，第1列全部选中）

 wordSelection.MoveRight(COleVariant((short)wdCharacter), 

  COleVariant((short)3), 

  COleVariant((short)wdExtend)); //向右选取3列，共选取4列（执行这句后，整个表被选中）

 wordParagraphs = wordSelection.GetParagraphFormat(); //获得对齐方式实例

 wordParagraphs.SetAlignment(wdAlignParagraphCenter); //设置表中的全部内容为“居中”

 //开始设置表以前内容的格式 

 wordSelection.MoveUp(COleVariant((short)wdLine), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向上移动鼠标到上一行

    wordParagraphs = wordSelection.GetParagraphFormat(); //获得对齐方式实例 

 wordParagraphs.SetAlignment(wdAlignParagraphCenter); //设置此行为“居中”格式 

    wordSelection.MoveUp(COleVariant((short)wdLine), 

  COleVariant((short)5),

  COleVariant((short)wdNULL)); // 向上移动鼠标到第1行

 wordParagraphs = wordSelection.GetParagraphFormat(); //获得对齐方式实例

 wordParagraphs.SetAlignment(wdAlignParagraphCenter); //设置标题行为“居中”

 wordSelection.EndKey(COleVariant((short)wdLine),

  COleVariant((short)wdNULL)); //将鼠标定位到第1行末

 wordSelection.HomeKey(COleVariant((short)wdLine),

  COleVariant((short)wdExtend)); //将鼠标定位到第1行末

 wordFont = wordSelection.GetFont(); //获得字体实例

 wordFont.SetBold(wdToggle); //设置标题行为“粗体”     

 wordFont.SetSize(15.0); //设置标题行字号为15（小三号）

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)1),

  COleVariant((short)wdNULL)); //向下移动鼠标到正文第1行

 wordSelection.HomeKey(COleVariant((short)wdLine),

  COleVariant((short)wdNULL)); //将鼠标定位到正文第1行首，（第2行行首） 

 wordSelection.MoveDown(COleVariant((short)wdLine), 

  COleVariant((short)11),

  COleVariant((short)wdExtend)); //向下选取除标题行外的全部内容

 wordFont = wordSelection.GetFont(); //获得字体实例 

 wordFont.SetSize(12.0); //设置标题行字号为12（小四号）

 wordSelection.WholeStory(); // 选取全部内容

 wordFont = wordSelection.GetFont(); //获得字体实例

 wordFont.SetName("宋体"); //设置全部内容为“宋体”

 //保存文件为《某某报表.doc》到默认位置，和此程序在一起 

 CString fileName = "//某某报表.doc";

 CString fullFileName = GGetAppPath() + fileName;

 wordDoc.SaveAs(COleVariant(fullFileName), //FileName

  COleVariant((short)wdFormatDocument), //FileFormat

  COleVariant((short)FALSE), //LockComments

  COleVariant(""), //Password

  COleVariant((short)FALSE), //AddToRecentFiles

  COleVariant(""), //WritePassword

  COleVariant((short)FALSE), //ReadOnlyRecommended 

  COleVariant((short)FALSE), //EmbedTrueTypeFonts 

  COleVariant((short)FALSE), //SaveNativePictureFormat 

  COleVariant((short)FALSE), //SaveFormsData 

  COleVariant((short)FALSE), //SaveAsAOCELetter

  COleVariant((short)wdNULL), //Encoding 

  COleVariant((short)FALSE), //InsertLineBreaks

  COleVariant((short)FALSE), //AllowSubstutitions,

  COleVariant((short)wdNULL), //LineEnding

  COleVariant((short)wdNULL) //AddBiDiMarks

  );

 //退出程序，关闭Word

 wordApp.Quit(COleVariant((short)wdNULL),

  COleVariant((short)wdNULL),

  COleVariant((short)wdNULL));

 return TRUE; // 生成成功，返回TRUE

6. 最后，编译，调试，运行，哈哈，最后，我生成了一个这样的表

=========================================================

使用word进行数据报告的制作可谓非常方便，word具有非常强大的编辑、排版功能。使用word能够制作出内容丰富、样式精美的报告。我们在工作中当然会有报表报告的需求，如果能够在MFC/VC++里面能够调用word进行word格式报告的自动生成，岂不是一件非常惬意的事情。                       

        我在工作当中需要对大量的数据或者各种测试数据进行统计分析，最终的统计分析结果总归要汇总为一份报告，不管是内部使用也好还是外部提供给客户也好，一份内容翔实、格式精美的报告自然必不可少。我对MFC/VC++也不是高手，只是业余爱好自己动动手做些东西。自然，低于VC操作word可谓完全没有经验，在网络上面也查找了很多资料，都是些只言片语，不能真正的领略通过VC使用word的方法。于是自己摸索了几个礼拜，可谓耗时甚长，劳心劳力啊。MS的东西，封装的也太严实了，对于函数部分只有些许的简单介绍，往往看的[云里雾里](https://www.baidu.com/s?wd=%E4%BA%91%E9%87%8C%E9%9B%BE%E9%87%8C&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)。没有实践还是不行啊！体会自己的经历，虽然辛苦但也有收获。不想其他朋友再继续走我走过的路，浪费时间、精力，故成文以共享。

废话少说，进入正题吧。

 第一步，当然我们需要首先导入word需要的库。通过ClassWizard->Add Class选择From a Type Library…定位需要的库文件导入。本例应该使用C:"Program Files"Microsoft Office"OFFICE11"MSWORD.OLB，在这里尽可选择所有的类导入，反正用的时候都有了，呵呵。完成这一步，我们得到两个文件msword.h和msword.cpp，那就加入你的工程吧。

说明:如果需要在VC++/MFC开发程序操作word/excel等office元素,那么需要对必要的类型库进行导入.下面是office系列的类型库参考,导入类型库时候请选择正确的类型库进行导入.                      

第二步，进入具体使用实践阶段。激动人心的时刻到来了，呵呵。不过别忘记把你的H文件#Include进来。下面是我的一个例子，从我具体的工程当中抽取出来供各位参考。有些地方我会简要解释。还等什么，赶紧试验一下代码吧。修改一下就是你的报告了，开心吧。

 BOOLCMDIDemoApp::OnMakeReport()

  {

                                   _ApplicationwordApp;

                                  DocumentswordDocs;

                                  _DocumentwordDoc;

                                  SelectionwordSelection;

                                  RangewordRange;

                                  TableswordTables;

                                  TablewordTable;     

                                  CellwordCell;   

                                  CellswordCells;

                                  //Paragraphs wordParagraphs;

                                  //Paragraph wordParagraph;

                                  _FontwordFont;

                                  ShadingwordShading;

                                  intnRow=0;

                                  COleVariantvTrue((short)TRUE), 

                                  vFalse((short)FALSE), 

                                  vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR); 

                  CComVariantdefaultBehavior(1),AutoFitBehavior(0);

                                  //创建word application实例

                                  if 

                  (!wordApp.CreateDispatch(_T("Word.Application"))) 

                                  {

                                          AfxMessageBox("Word CreateDispatch 

                  Failed!");

                                          returnFALSE;

                                  }     

                                  wordApp.SetVisible(TRUE);//make visible

                                  wordApp.Activate();//激活

                                  wordDocs=wordApp.GetDocuments();//get 

                  documents object

                                  CStringstrDocTemplate;

                                  CStringstrPath;

                                  charszFullPath[_MAX_PATH];

::GetModuleFileName(NULL,szFullPath,_MAX_PATH);

                                  strDocTemplate.Format("%s",szFullPath);

                                  intnPos=strDocTemplate.ReverseFind('""');

                                  strDocTemplate=strDocTemplate.Left(nPos);

                                  strPath=strDocTemplate;

                                  TRACE1("%s"n",strDocTemplate);

                                  //     

                  strDocTemplate+="""Template""Report.doc";

                                  //     

                  wordDoc=wordDocs.Open(COleVariant(strDocTemplate),&vOpt,&vOpt,&vOpt,&vOpt,

                                  //            

                  &vOpt,&vOpt,&vOpt,&vOpt,&vOpt,&vOpt,&vTrue,&vOpt,&vOpt,&vOpt,&vOpt);

                  CComVarianttpl(_T("")),Visble,DocType(0),NewTemplate(false);

                  wordDoc=wordDocs.Add(&tpl,&NewTemplate,&DocType,&Visble);

                                  wordSelection=wordApp.GetSelection();

                  //wordTables.AttachDispatch(wordDoc.GetTables());

                                  wordTables=wordDoc.GetTables();

                                  //wordParagraphs = wordDoc.GetParagraphs();

                                  //wordParagraph=wordParagraphs.GetLast();

                                  //wordRange=wordParagraph.GetRange();

                  //wordSelection.EndKey(COleVariant((short)6),COleVariant((short)0));

                                  //move insertion point to the end of the story

                                  /*

                                  wdUnits:

                                  wdCharacter=1   

                                  wdWord=2

                                  wdSentence=3

                                  wdParagraph=4

                                  wdSection=8

                                  wdStory=6

                                  wdCell=12

                                  wdColumn=9

                                  wdRow=10

                                  wdTable=15       

                                  wdMovementType:

                                  wdMove=0

                                  wdExtend=1       

                                  */

                                  //move to end of story

                  wordSelection.EndOf(COleVariant((short)6),COleVariant((short)0));

                                  //1.1 RxLev Full 

                                  wordSelection.TypeText("1. 统计报告");

                  wordSelection.HomeKey(&CComVariant(5),&CComVariant(1));

                                  //Format the line with selection

                                  wordFont = wordSelection.GetFont();

                                  wordFont.SetBold(9999998);//wdToggle

                  wordSelection.EndOf(&CComVariant(5),&CComVariant(0));

                                  wordSelection.TypeParagraph();    

                                  wordSelection.TypeText("(1.1). 分段统计");

                                  wordSelection.TypeParagraph();

                                  wordFont.SetBold(9999998);//wdToggle

                                  wordRange=wordSelection.GetRange();

                                   //add table

                                  //nRow=m_nRange1+1;

     wordTable=wordTables.Add(wordRange,5/*row*/,4/*column*/,&defaultBehavior,&AutoFitBehavior);

                                  wordRange=wordTable.GetRange();

                  //wordRange.MoveEnd(COleVariant((short)15),COleVariant((short)1));

                                  //wordRange.Select();

                                  //move end of table

                  //wordSelection.EndOf(COleVariant((short)15),COleVariant((short)0));

                                  //insert rows

                  //wordSelection.InsertRowsBelow(COleVariant((short)5));

                                  //选择第一个单元,进而选择第一行进行格式化

                                  wordCell=wordTable.Cell(1,1);

                                  wordCell.Select();

                                  //select the row with current selection

                  wordSelection.EndKey(&CComVariant(10),&CComVariant(1));

                                  //Format the row with selection

                                  //wordFont = wordSelection.GetFont();

                                  wordFont.SetBold(9999998);//wdToggle

                                  wordCells=wordSelection.GetCells();

                                  wordShading = wordCells.GetShading();

                                  wordShading.SetTexture(0);

                  wordShading.SetBackgroundPatternColor(14737632);//15987699 

                  14737632 adColorBlue

                  wordShading.SetForegroundPatternColor(-16777216);//-16777216 

                  wdColorAutomatic

                                  //move to end of table

                  //wordSelection.EndOf(COleVariant((short)15),COleVariant((short)0));

                                  //wordParagraph=wordParagraphs.GetLast();

                                  //wordRange=wordParagraph.GetRange();

                  //wordRange.MoveEnd(COleVariant((short)4),COleVariant((short)1)); 

                                  //wordRange.SetText("Test");

                                  //wordSelection=wordApp.GetSelection();

                  //wordSelection.MoveEnd(COleVariant((short)6),COleVariant((short)1));

                                  wordCell=wordTable.Cell(1,1);

                                  wordCell.Select();

                                  wordSelection.TypeText("统计项目");

                  wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));

                                  wordSelection.TypeText("采样");

                  wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));

                                  wordSelection.TypeText("百分比");

                  wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));

                                  wordSelection.TypeText("累计百分比");

                  wordSelection.EndOf(COleVariant((short)6),COleVariant((short)0));

                                  wordSelection.TypeParagraph();    

                                  wordSelection.TypeText("(1.2). 分段统计");

                  wordSelection.HomeKey(&CComVariant(5),&CComVariant(1));

                                  /Format the line with selection

                                  wordFont = wordSelection.GetFont();

                                  wordFont.SetBold(9999998);//wdToggle

                  wordSelection.EndOf(&CComVariant(5),&CComVariant(0));

                                  wordSelection.TypeParagraph();

                                  wordFont.SetBold(9999998);//wdToggle

                                  wordRange=wordSelection.GetRange();

                                  //add table

                                  //nRow=m_nRange1+1;

                  wordTable=wordTables.Add(wordRange,5/*row*/,4/*column*/,&defaultBehavior,&AutoFitBehavior);

                                  wordRange=wordTable.GetRange();

                                  //选择第一个单元,进而选择第一行进行格式化

                                  wordCell=wordTable.Cell(1,1);

                                  wordCell.Select();

                                  //select the row with current selection

                  wordSelection.EndKey(&CComVariant(10),&CComVariant(1));

                                  //Format the row with selection

                                  wordFont = wordSelection.GetFont();

                                  wordFont.SetBold(9999998);

                                  wordCells=wordSelection.GetCells();

                                  wordShading = wordCells.GetShading();

                                  wordShading.SetTexture(0);

                  wordShading.SetBackgroundPatternColor(14737632);//15987699

                  wordShading.SetForegroundPatternColor(-16777216);

                                  wordCell=wordTable.Cell(1,1);

                                  wordCell.Select();

                                  wordSelection.TypeText("Range");

                  wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));

                                  wordSelection.TypeText("Samples");

                  wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));

                                  wordSelection.TypeText("Percentage");

                  wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));

                                  wordSelection.TypeText("Calculation");

                                  //Save document as report

                                  SYSTEMTIMEsysTime;

                                  GetLocalTime(&sysTime);

                                  CStringstrReport;

                                  strReport.Format("%d年-%d月-%d日 %d-%d-%d 

                  报告",sysTime.wYear,sysTime.wMonth,

                  sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

                                  strReport=strPath+""""+strReport;

                  wordDoc.SaveAs(COleVariant(strReport),COleVariant((short)0),vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt);

                                  //Release com

                                  wordFont.ReleaseDispatch();

                                  wordCells.ReleaseDispatch();

                                  wordShading.ReleaseDispatch();

                                  wordTable.ReleaseDispatch();

                                  wordTables.ReleaseDispatch();

                                  wordRange.ReleaseDispatch();

                                  wordSelection.ReleaseDispatch();

                                  wordDoc.RecheckSmartTags();

                                  wordDocs.ReleaseDispatch();

                                  wordApp.ReleaseDispatch();

                                  returnTRUE;

                          }

                          注释如下：

1.使用之前比忘记先进行COM环境的初始化，最好放在BOOLCMDIDemoApp::InitInstance()里吧。

     //Initialize COM context

      if (FAILED(::CoInitialize(NULL)))

      {

            AfxMessageBox("Initialize COM context failed!");

            returnFALSE;}

2.注意几个函数的用法：

   a) wordSelection.EndOf(COleVariant((short)6),COleVariant((short)0));

   b)wordSelection.EndKey(&CComVariant(10),&CComVariant(1));

   c) wordSelection.HomeKey(&CComVariant(5),&CComVariant(1));

   d) wordTable=wordTables.Add(wordRange,5/*row*/,4/*column*/,&defaultBehavior,&AutoFitBehavior);

   这几个函数可以说是我收获最大的地方了，EndOf/EndKey/HomeKey实现了当前焦点的任意移动，比如到一个文档的末尾或者行尾、表格的操作位置等。举例来说：EndOf  (VARIANT* Unit, VARIANT* Extend) 

   其第二个参数Extend决定了你是单纯的移动光标还是扩展移动选中内容。通过VBA参考，你可以知道wdMove=0 wdExtend=1，这样就可以决定是移动还是扩展选择内容了。当然还有其他很多操作可以摸索，大可以先用word的宏录制，然后编写自己的VC代码，不过确实是个繁琐的过程，很多VBA常量需要查找具体的value替换。通过预先的模版，你可以实现打开一个word模版，然后添加你自己的内容，灵活应用。然后保存为自己的报告。请参阅我的例子自己实现吧。Enjoy it by yourself...... J由于时间关系，我还没有摸索通如何在word报告中插入图表（chart/bar/pie）以及如何插入图片jpg等，这样就可以做出图文并茂的报告了。如果你有经验，欢迎交流共享。

来源：http://www.cnblogs.com/For-her/p/3499778.html

