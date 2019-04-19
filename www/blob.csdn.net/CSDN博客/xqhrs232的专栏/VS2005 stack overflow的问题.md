# VS2005 stack overflow的问题 - xqhrs232的专栏 - CSDN博客
2012年08月07日 17:45:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1252
原文地址::[VS2005 stack overflow的问题](http://blog.csdn.net/virtualdesk/article/details/5636005)
相关网帖
1.请高手指教，在vs2005中如何设置栈的大小？----[http://topic.csdn.net/u/20090427/18/d0daee6c-9fe9-4924-9aff-d55ad9f31577.html](http://topic.csdn.net/u/20090427/18/d0daee6c-9fe9-4924-9aff-d55ad9f31577.html)
Visual Studio 2005中，关于堆溢出(Stack overflow)问题的解决
      程序build没问题，运行时直接跳到intel的chkstk.asm文件中去了，说明你的程序中某个数组太大了，超过了
默认stack的size.解决方法有两个：
1）project property->Configuration Properties->Linker->System->Stack Reserve Size
改成10000000,就可以了，注意是7个0哦
2）将数组改成指针，然后用new或malloc在heap中动态分配。
### To set this linker option in the Visual Studio development environment
- 
Open the project's **Property Pages** dialog box. For details, see Setting Visual C++ Project Properties.
- 
Click the **Linker** folder.
- 
Click the **System** property page.
- 
Modify one of the following properties:
- 
**Stack Commit Size**
- 
**Stack Reserve Size**
//=============================================
备注::
1.我这样设置了，不起作用啊！！！不知道为什么。
