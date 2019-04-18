# MDI命令消息的流浪过程 - ljx0305的专栏 - CSDN博客
2010年03月17日 14:50:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：684标签：[command](https://so.csdn.net/so/search/s.do?q=command&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
以下我举一个具体例子。假设命令消息从Scribble 的【Edit/Clear All】发出，其处理常
式位在CScribbleDoc，下面是这个命令消息的流浪过程：
1. MDI 主窗口（ CMDIFrameWnd） 收到命令消息WM_COMMAND， 其ID 为
ID_EDIT_CLEAR_ALL。
2. MDI 主窗口把命令消息交给目前作用中的MDI 子窗口（CMDIChildWnd）。
3. MDI 子窗口给它自己的子窗口（也就是View）一个机会。
4. View 检查自己的Message Map。
5. View 发现没有任何处理例程可以处理此命令消息，只好把它传给Document。
6. Document 检查自己的Message Map，它发现了一个吻合项：
BEGIN_MESSAGE_MAP(CScribbleDoc, CDocument)
ON_COMMAND(ID_EDIT_CLEAR_ALL, OnEditClearAll)
...
END_MESSAGE_MAP()
于是调用该函数，命令消息的流动路线也告终止。
如果上述的步骤6 仍没有找到处理函数，那么就：
7. Document 把这个命令消息再送到Document Template 对象去。
8. 还是没被处理，于是命令消息回到View。
9. View 没有处理，于是又回给MDI 子窗口本身。
10. 传给CWinApp 对象-- 无主消息的终极归属。
