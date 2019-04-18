# VC++中的ON_COMMAND_RANGE宏和ON_UPDATE_COMMAND_UI_RANGE宏 - L_Andy的专栏 - CSDN博客

2013年10月22日 09:42:05[卡哥](https://me.csdn.net/L_Andy)阅读数：1515


                
VC++中的ON_COMMAND_RANGE宏和ON_COMMAND等宏一样，是用来声明消息处理函数的，与

ON_COMMAND不同的是，此宏可用来定义一组消息的处理函数。

两个宏的用法是：

ON_COMMAND(id,memberFxn)

ON_COMMAND_RANGE(id1,id2,memberFxn)

看起来其中memberFxn似乎没有什么差别，但是在学习使用中，发现ON_COMMAND_RANGE宏中的memberFxn常常会被定义成带参数的处理函数，而ON_COMMAND则是不带参数的处理函数。这就让我奇怪了，于是查阅MSDN的解释，循着提供的线索一直查到了《Visual C++ Programmer’s Guide》中的《Handlers for Message-Map Ranges》一章，于是在《Declaring the Handler Function》一节中找到了答案，引用原文如下：

Handler functions for single commands normally take no parameters. With the exception of update handler functions, handler functions for message-map ranges require an extra parameter, nID, of type UINT. This parameter is the first parameter. The extra parameter
 accommodates the extra command ID needed to specify which command the user actually chose.  

为了帮助和我一样不太懂英文的朋友，顶着头皮翻译了下：

单个命令（消息）的处理函数通常不带参数。但是更新处理函数、针对消息映射范围的处理函数需要一个额外的参数，一个UINT类型的nID。此参数是第一个参数，这个额外的参数收集了用来指定用户真正选择命令的命令ID。

因此完全可以一个带参数的消息处理函数来接收指定的命令ID，不过这里要提醒朋友一点的是，并不是非得只有一个参数，可以使用多个参数的消息处理函数，如function(WPARAM wParam, LPARAM lParam)，但是真正能够使用的参数是第一个参数wParam，它是用户选择的命令ID。 

对于更新处理函数，也存在相对应于ON_COMMAND_RANGE的ON_UPDATE_COMMAND_UI_RANGE宏，它的作用也是处理连续的范围(contiguous range)，但是它与ON_COMMAND_RANGE有一点点区别。通过上面可知，一般范围处理函数(ON_COMMAND_RANGE)是通过在处理函数后面加一个参数，当消息进行处理时，将会把该命令ID传至该参数。而ON_UPDATE_COMMAND_UI_RANGE则使用的是pCmdUI，一个CCmdUI的指针类型，它内部就包含了一个数据成员(m_nID)，用来指向命令ID，具体请参考MSDN中的内容。上述内容在MSDN中的《Visual
 C++ Programmer’s Guide》中的《Handlers for Message-Map Ranges》一章《Example for a Range of Command IDs》一节中可以找到原文：

Update handler functions for single commands normally take a single parameter, pCmdUI, of type CCmdUI*. Unlike handler functions, update handler functions for message-map ranges do not require an extra parameter, nID, of type UINT. The command ID, which is
 needed to specify which command the user actually chose, is found in the CCmdUI object.

意思大概也是：

单个命令的更新处理函数通常只带一个参数，即一个CCmdUI指针类型的pCmdUI。不像普通的处理函数一样，针对消息映射范围的更新处理函数不需要一个额外的UINT参数nID，这个用来指定用户真正选择的命令ID，能够在CCmdUI对象中找到。


