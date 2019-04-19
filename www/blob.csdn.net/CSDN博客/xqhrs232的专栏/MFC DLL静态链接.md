# MFC DLL静态链接 - xqhrs232的专栏 - CSDN博客
2014年01月04日 21:36:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：831
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/password318/article/details/6906270](http://blog.csdn.net/password318/article/details/6906270)
相关文章
1、VC定义的三种类型DLL有什么区别----[http://wenku.baidu.com/link?url=a6puBAWF3wLPwvf73T17SkT1EveH3O8hvg0Rw_6PArhKvqEhYg3IVUQNy9kRZO8EpbeTdxB9MzaJX7L7Zb-c0VfGwIRKLgXsOEMvdVFhUJG](http://wenku.baidu.com/link?url=a6puBAWF3wLPwvf73T17SkT1EveH3O8hvg0Rw_6PArhKvqEhYg3IVUQNy9kRZO8EpbeTdxB9MzaJX7L7Zb-c0VfGwIRKLgXsOEMvdVFhUJG)
2、MFC DLL 的三种类型----[http://hi.baidu.com/fyging/item/6474f846f8f341e6bdf45103](http://hi.baidu.com/fyging/item/6474f846f8f341e6bdf45103)
在使用向导创建基于MFC的DLL时，有如下三种类型
![](http://hi.csdn.net/attachment/201110/26/0_1319596870acXh.gif)
**MFC扩展DLL**
使用MFC扩展DLL，产生的DLL只能与MFC客户程序一起工作。他可以导出C++类给客户端。导出的函数可以使用c++/MFC数据类型作为参数或返回值，导出一个类时，客户端能创建类对象或者派生这个类。同时，在DLL中也可以使用DLL和MFC。
VC的MFC类库也保存在一个DLL中，MFC扩展DLL动态链接到MFC代码库的DLL，客户程序也因此需要动态链接到MFC代码库的DLL。由于MFC代码库的DLL存在多个版本，客户程序和扩展DLL都必须使用相同版本的MFC代码DLL。为了让MFC扩展DLL能很好的工作，扩展DLL和客户程序需要动态链接到MFC代码DLL，而这个DLL必须在客户程序运行的计算机上存在。
**规则DLL：**
MFC扩展DLL只能和MFC客户程序一起工作，采用规则DLL，可以使用更广泛的DLL，它不受MFC的限制。缺点是不能发送指针或MFC派生类和对象的引用，即规则DLL和客户程序的接口（函数签名部分）不能使用MFC。但在DLL和客户程序内部，还是可以使用MFC。
在使用规则DLL时，有动态链接和静态链接之分。
如果是动态链接，表示规则DLL需要的MFC代码没有构建到DLL中，这类似于扩展DLL，需要 保证在运行DLL的计算机上必须要有含有MFC代码库的DLL。
如果是静态链接，规则DLL中已经包含了需要的MFC代码，因此他得体积将比较大，但他可以在没有包含MFC代码库的计算机上正常运行。
示例：
一、创建DLL
1、创建项目 DLLDemo，选择MFC DLL模板，选择“使用共享MFC DLL”的规则DLL选项，单击完成
2、在头文件中，添加如下外部函数声明
**[cpp]**[view
 plain](http://blog.csdn.net/password318/article/details/6906270#)[copy](http://blog.csdn.net/password318/article/details/6906270#)
- // CDLLDemoApp
- // 有关此类实现的信息，请参阅 DLLDemo.cpp
- //
- extern"C"void PASCAL EXPORT HintBox();  
- class CDLLDemoApp : public CWinApp  
- {  
- public:  
-     CDLLDemoApp();  
- 
- // 重写
- public:  
- virtualBOOL InitInstance();  
- 
-     DECLARE_MESSAGE_MAP()  
- //  virtual void AddToRecentFileList(LPCTSTR lpszPathName);
- };  
3、在实现文件中，对此函数进行实现
**[cpp]**[view
 plain](http://blog.csdn.net/password318/article/details/6906270#)[copy](http://blog.csdn.net/password318/article/details/6906270#)
- CDLLDemoApp theApp;  
- 
- 
- // CDLLDemoApp 初始化
- 
- BOOL CDLLDemoApp::InitInstance()  
- {  
-     CWinApp::InitInstance();  
- 
- return TRUE;  
- }  
- 
- 
- extern"C"void PASCAL EXPORT HintBox(){  
-     AfxMessageBox(L"您的工作时间太长了，请注意休息!");  
- }  
4、在DLLDemo.def文件中添加导出信息
**[cpp]**[view
 plain](http://blog.csdn.net/password318/article/details/6906270#)[copy](http://blog.csdn.net/password318/article/details/6906270#)
- ; DLLDemo.def : 声明 DLL 的模块参数。  
- 
- LIBRARY  
- 
- EXPORTS  
-     ; 此处可以是显式导出  
-     HintBox  
二、调用DLL
1、新建一个项目CallDLLDemo，选择基于FormView的单文档框架，在Form上添加一个按钮Button，添加按钮的事件处理程序
2、在事件处理程序中添加如下代码
**[cpp]**[view
 plain](http://blog.csdn.net/password318/article/details/6906270#)[copy](http://blog.csdn.net/password318/article/details/6906270#)
- // CCallDLLDemoView 消息处理程序
- 
- 
- void CCallDLLDemoView::OnBnClickedButton1()  
- {  
- // TODO: 在此添加控件通知处理程序代码
-     HintBox();  
- }  
提示出错，说明未能识别HintBox函数，接下去采用静态链接的方式完成HintBox调用。
3、为项目添加链接静态库的目录
![](http://hi.csdn.net/attachment/201110/26/0_131959877718v8.gif)
4、在CallDLLDemoView.cpp的顶部，添加如下信息
**[cpp]**[view
 plain](http://blog.csdn.net/password318/article/details/6906270#)[copy](http://blog.csdn.net/password318/article/details/6906270#)
- #ifndef SHARED_HANDLERS
- #include "CallDLLDemo.h"
- #endif
- 
- #include "CallDLLDemoDoc.h"
- #include "CallDLLDemoView.h"
- #include "..\DLLDemo\DLLDemo.h"
- 
- #pragma comment(lib,"DLLDemo.lib")
倒数第二行需要添加HintBox的头文件，倒数第一行，引入DLLDemo.lib静态库
三、采用动态加载DLL来实现调用
如上完成第一步后，只需添加如下代码，即可实现同样效果
**[cpp]**[view
 plain](http://blog.csdn.net/password318/article/details/6906270#)[copy](http://blog.csdn.net/password318/article/details/6906270#)
- typedefint(* Func)();  
- ……  
- void CCallDLLDemo2View::OnBnClickedButton1()  
- {  
- // TODO: 在此添加控件通知处理程序代码
-     Func _func;  
- HINSTANCE hInstLibrary = LoadLibrary(L"DLLDemo.dll");  
- if (hInstLibrary==NULL)  
-     {  
-         FreeLibrary(hInstLibrary);  
-     }  
-     _func = (Func)GetProcAddress(hInstLibrary,"HintBox");  
- if (_func!=NULL)  
-     {  
-         _func();  
-     }  
-     FreeLibrary(hInstLibrary);  
- 
- 
- }  
- 
