# VC++6.0如何设置使其支持Unicode编程 - ljx0305的专栏 - CSDN博客
2010年06月23日 16:18:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：4015标签：[编程																[vc++																[语言																[windows																[mfc																[dialog](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[MFC																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403913)
VC++ 6.0支持Unicode编程，但默认的是ANSI，所以开发人员只需要稍微改变一下编写代码的习惯便可以轻松编写支持UNICODE的应用程序。使用VC++ 6.0进行Unicode编程主要做以下几项工作： 
1、为工程添加UNICODE和_UNICODE预处理选项。 
　　具体步骤：打开[工程]->[设置…]对话框，在C/C++标签对话框的“预处理程序定义”中去除_MBCS，加上_UNICODE,UNICODE。（注意中间用逗号隔开）.
　　在没有定义UNICODE和_UNICODE前，所有函数和类型都默认使用ANSI的版本；在定义了UNICODE和_UNICODE之后，所有的MFC类和Windows API都变成了宽字节版本了。
2、设置程序入口点
　　因为MFC应用程序有针对Unicode专用的程序入口点，我们要设置entry point。否则就会出现连接错误。
　　设置entry point的方法是：打开[工程]->[设置…]对话框，在Link页的Output类别的Entry Point里填上wWinMainCRTStartup。
3、使用ANSI/Unicode通用数据类型 
　　微软提供了一些ANSI和Unicode兼容的通用数据类型，我们最常用的数据类型有_T ，TCHAR，LPTSTR,LPCTSTR。
顺便说一下，LPCTSTR和const TCHAR*是完全等同的。其中L表示long指针，这是为了兼容Windows 3.1等16位操作系统遗留下来的，在Win32 中以及其它的32位操作系统中，long指针和near指针及far修饰符都是为了兼容的作用，没有实际意义。P（pointer）表示这是一个指针；C （const）表示是一个常量；T(_T宏)表示兼容ANSI和Unicode，STR（string）表示这个变量是一个字符串。综上可以看出， LPCTSTR表示一个指向常固定地址的可以根据一些宏定义改变语义的字符串。比如： 
TCHAR* szText=_T(“Hello!”);TCHAR szText[]=_T(“I Love You”);LPCTSTR lpszText=_T(“大家好！”);使用函数中的参数最好也要有变化，比如：MessageBox(_T(“你好”)); 
　　其实，在上面的语句中，即使您不加_T宏，MessageBox函数也会自动把“你好”字符串进行强制转换。但我还是推荐您使用_T宏，以表示您有Unicode编码意识。 
4、修改字符串运算问题
　　一些字符串操作函数需要获取字符串的字符数(sizeof(szBuffer)/sizeof(TCHAR))，而另一些函数可能需要获取字符串的字节数sizeof(szBuffer)。您应该注意该问题并仔细分析字符串操作函数，以确定能够得到正确的结果。
ANSI操作函数以str开头，如strcpy()，strcat()，strlen()；
Unicode操作函数以wcs开头，如wcscpy，wcscpy()，wcslen()；
ANSI/Unicode操作函数以_tcs开头 _tcscpy(C运行期库)；
ANSI/Unicode操作函数以lstr开头 lstrcpy(Windows函数)；
考虑ANSI和Unicode的兼容，我们需要使用以_tcs开头或lstr开头的通用字符串操作函数。
举个Unicode编程的例子
第一步：
　　打开VC++6.0，新建基于对话框的工程Unicode，主对话框IDD_UNICODE_DIALOG中加入一个按钮控件，双击该控件并添加该控件的响应函数：
void CUnicodeDlg::OnButtonUnicode() { TCHAR* str1=_T("ANSI和UNICODE编码试验"); m_disp=str1; UpdateData(FALSE);}　　添加静态文本框IDC_DISP，使用ClassWizard给该控件添加CString类型变量m_disp。使用默认ANSI编码环境编译该工程，生成Unicode.exe。 
第二步：修改系统内码
　　打开“控制面板”，单击“日期、时间、语言和区域设置”选项，在“日期、时间、语言和区域设置”窗口中继续单击“ 区域和语言选项”选项，弹出“区域和语言选项”对话框。在该对话框中，单击“高级”标签，将“非Unicode的程序的语言”选项改为“日语”，单击“应用”按钮，
弹出的对话框单击“是”，重新启动计算机使设置生效。
运行Unicode.exe程序并单击“Button1”按钮，看，静态文本框出现了乱码。
第三步：
　　改为Unicode编码环境编译该工程，生成Unicode.exe。再次运行Unicode.exe程序并单击“Unicode”按钮。看到Unicode编码的优势了吧。
本文来自: IT知道网([http://www.itwis.com](http://www.itwis.com)) 详细出处参考：[http://www.itwis.com/html/c/ctool/20091127/7024.html](http://www.itwis.com/html/c/ctool/20091127/7024.html)
