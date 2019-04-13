
# [黑科技] WPS通过VB宏函数实现自编号功能 - 杨秀璋的专栏 - CSDN博客

2017年03月15日 12:52:41[Eastmount](https://me.csdn.net/Eastmount)阅读数：3524标签：[VB																](https://so.csdn.net/so/search/s.do?q=VB&t=blog)[Word																](https://so.csdn.net/so/search/s.do?q=Word&t=blog)[宏函数																](https://so.csdn.net/so/search/s.do?q=宏函数&t=blog)[黑科技																](https://so.csdn.net/so/search/s.do?q=黑科技&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=宏函数&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Word&t=blog)个人分类：[黑科技																](https://blog.csdn.net/Eastmount/article/category/6790540)
[
																								](https://so.csdn.net/so/search/s.do?q=Word&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=VB&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=VB&t=blog)

这篇文章主要是作为李老师《算法设计与分析》助教课程中，与她交流，学到的一些基础知识。它主要是讲述Word通过宏函数设置一些操作，比如在Word全文中替换一些符号；再如对Word上角表进行编号，如果删除中间某个值，运行宏函数自动编号；对Word中所有图片进行大小统一整合；文章竖着排版等操作。Word宏函数主要是基础VB编写的，希望这篇文章对你有所帮助~


## 一. WPS实现宏函数可用
Office中Word的宏函数直接可以使用，而WPS需要安装一个软件后才能使用。打开WPS Word如下图所示，宏是不能使用的。

![](https://img-blog.csdn.net/20170314234511661)

这时需要下载VBA for WPS并安装才能使用。下载地址：
[https://yun.baidu.com/share/link?shareid=2773182689&uk=892671164](https://yun.baidu.com/share/link?shareid=2773182689&uk=892671164)
下载安装如下图所示：

![](https://img-blog.csdn.net/20170314234733428)

安装完后可以设置宏函数，如下图所示：

![](https://img-blog.csdn.net/20170314235520111)
点击"宏"，然后"创建"宏函数，如下图所示，取名为test。

![](https://img-blog.csdn.net/20170315000405188)

创建后如下图所示，可以看到是VB代码进行编写的。
![](https://img-blog.csdn.net/20170315000621515)
WPS需要保存为带宏函数的格式，如下图所示。
![](https://img-blog.csdn.net/20170315000911049)
然后运行宏函数如下图所示，点击"运行"即可，如下图所示。
![](https://img-blog.csdn.net/20170315001327301)
这里，如果想好好研究这个，可以看看VB一些基础书籍，程序就那回事。我简单看了《VB从入门到精通》后，开始写相应功能的。


## 二. 编写宏函数实现自编号
Visual Basic支持一个对象集合，该集合中的对象直接对应于Microsoft Word 97中的元素，并且通过用户界面，用户熟悉这些元素中的绝大多数。
例如，Document 对象代表了一个打开的文档，Bookmark对象代表了一个文档中的书签，而Selection对象则代表了在一个文档窗口窗格中的选定内容。在Word中，每一类元素－文档、表格、段落、书签、域等等－都可以用Visual Basic的对象来表示。要在Word中自动执行任务，可以使用这些对象的方法和属性。
下面简单讲解宏函数实现一些功能。

```python
Sub test()
'
' test Macro
'
    Dim sLineNum3 As String     '行号(文字)
    Dim nLineNum                '行号(数值)
    Dim i As Long
 
    Title = "输入编号信息"
    a1 = "请输入总编号开始号："
    b1 = InputBox(a1, Title)
     
End Sub
```
运行宏函数如下图所示：
![](https://img-blog.csdn.net/20170315101801022)
运行结果如下图所示，弹出界面输入：

![](https://img-blog.csdn.net/20170315103024664)

下面是完整的代码，如下所示：

```python
Sub test()
'
' test Macro
'
    '定义变量
    Dim sLineNum3 As String     '行号(文字)
    Dim nLineNum                '行号(数值)
    Dim sLineNum As String      '行号(文字)
    Dim sLineNum1 As String     '行号(文字)
    Dim sLineNum2 As String     '行号(文字)
    Dim selRge As Range
    Dim i As Long
    Dim x As Long
    Dim y As Long
    Dim k As Long
 
 
    '输入行号对话框
    Title = "输入编号信息"
    a1 = "请输入总编号开始号："
    b1 = InputBox(a1, Title)
    'Val函数将数字字符串换成数值
    y = 200000 + Val(b1) - 1
    
    
    i = 1
    For k = 1 To 20
        sLineNum1 = Str(i + y)            '200001
        sLineNum1 = LTrim(sLineNum1)      '移除字符串最左边的空白字符
        sLineNum1 = Right(sLineNum1, 5)   '生成行号格式"00001"
        
        '行号前加"/"
        'sLineNum = "/" & b2 & sLineNum
        'sLineNum2 = sLineNum1 + sLineNum
        sLineNum2 = sLineNum1
        
   
        '移动光标至当前行首 Selection.HomeKey wdLine
        Selection.HomeKey Unit:=wdLine
        '选择从光标至当前行首的内容 Selection.HomeKey wdLine, wdExtend
        Selection.EndKey Unit:=wdLine, Extend:=wdExtend
    
        sLineNum3 = Selection.Text
        sLineNum3 = Left(sLineNum3, 3)    '从左边获取每行前3个字符
        If sLineNum3 = "%%%" Then         '替换行号
            Selection.Find.Execute FindText:="%%%", ReplaceWith:=sLineNum2
            i = i + 1
        End If
        Selection.MoveDown Unit:=wdLine, Count:=1
 
       Next k
     
End Sub
```
现在Word中内容如下所示：![](https://img-blog.csdn.net/20170315121850406)

然后运行"宏"，运行如下所示：
![](https://img-blog.csdn.net/20170315122030797)

运行结果如下图所示：

![](https://img-blog.csdn.net/20170315124005467)
下面补充一些代码，常见的操作Word宏的 函数。
参考：[http://wangye.org/blog/archives/135/](http://wangye.org/blog/archives/135/)
[http://blog.sina.com.cn/s/blog_5e646c1f0100u24w.html](http://blog.sina.com.cn/s/blog_5e646c1f0100u24w.html)

```python
Sub MoveToCurrentLineStart()
  ' 移动光标至当前行首
  ' Selection.HomeKey wdLine
  Selection.HomeKey unit:=wdLine
End Sub
Sub MoveToCurrentLineEnd()
  ' 移动光标至当前行尾
  ' Selection.EndKey wdLine
  Selection.EndKey unit:=wdLine
End Sub
Sub SelectToCurrentLineStart()
  ' 选择从光标至当前行首的内容
  ' Selection.HomeKey wdLine, wdExtend
  Selection.HomeKey unit:=wdLine, Extend:=wdExtend
End Sub
Sub SelectToCurrentLineEnd()
  ' 选择从光标至当前行尾的内容
  ' Selection.EndKey wdLine, wdExtend
  Selection.EndKey unit:=wdLine, Extend:=wdExtend
End Sub
Sub SelectCurrentLine()
  ' 选择当前行
  ' Selection.HomeKey wdLine
  ' Selection.EndKey wdLine, wdExtend
  Selection.HomeKey unit:=wdLine
  Selection.EndKey unit:=wdLine, Extend:=wdExtend
End Sub
Sub MoveToDocStart()
  ' 移动光标至文档开始
  ' Selection.HomeKey wdStory
  Selection.HomeKey unit:=wdStory
End Sub
Sub MoveToDocEnd()
  ' 移动光标至文档结尾
  ' Selection.EndKey wdStory
  Selection.EndKey unit:=wdStory
End Sub
Sub SelectToDocStart()
  ' 选择从光标至文档开始的内容
  ' Selection.HomeKey wdStory, wdExtend
  Selection.HomeKey unit:=wdStory, Extend:=wdExtend
End Sub
Sub SelectToDocEnd()
  ' 选择从光标至文档结尾的内容
  ' Selection.EndKey wdStory, wdExtend
  Selection.EndKey unit:=wdStory, Extend:=wdExtend
End Sub
Sub SelectDocAll()
  ' 选择文档全部内容（从WholeStory可猜出Story应是当前文档的意思）
  Selection.WholeStory
End Sub
Sub MoveToCurrentParagraphStart()
  ' 移动光标至当前段落的开始
  ' Selection.MoveUp wdParagraph
  Selection.MoveUp unit:=wdParagraph
End Sub
Sub MoveToCurrentParagraphEnd()
  ' 移动光标至当前段落的结尾
  ' Selection.MoveDown wdParagraph
  Selection.MoveDown unit:=wdParagraph
End Sub
Sub SelectToCurrentParagraphStart()
  ' 选择从光标至当前段落开始的内容
  ' Selection.MoveUp wdParagraph, wdExtend
  Selection.MoveUp unit:=wdParagraph, Extend:=wdExtend
End Sub
Sub SelectToCurrentParagraphEnd()
  ' 选择从光标至当前段落结尾的内容
  ' Selection.MoveDown wdParagraph, wdExtend
  Selection.MoveDown unit:=wdParagraph, Extend:=wdExtend
End Sub
Sub SelectCurrentParagraph()
  ' 选择光标所在段落的内容
  ' Selection.MoveUp wdParagraph
  ' Selection.MoveDown wdParagraph, wdExtend
  Selection.MoveUp unit:=wdParagraph
  Selection.MoveDown unit:=wdParagraph, Extend:=wdExtend
End Sub
Sub DisplaySelectionStartAndEnd()
  '显示选择区的开始与结束的位置，注意：文档第1个字符的位置是0
  MsgBox ("第" & Selection.Start & "个字符至第" & Selection.End & "个字符")
End Sub
Sub DeleteCurrentLine()
  ' 删除当前行
  ' Selection.HomeKey wdLine
  ' Selection.EndKey wdLine, wdExtend
  Selection.HomeKey unit:=wdLine
  Selection.EndKey unit:=wdLine, Extend:=wdExtend
  Selection.Delete
End Sub
Sub DeleteCurrentParagraph()
  ' 删除当前段落
  ' Selection.MoveUp wdParagraph
  ' Selection.MoveDown wdParagraph, wdExtend
  Selection.MoveUp unit:=wdParagraph
  Selection.MoveDown unit:=wdParagraph, Extend:=wdExtend
  Selection.Delete
End Sub
```

最后希望文章对你有所帮助，如果文章中存在错误或不足之处，还请海涵~后面肯恩会补充一些实用的功能，娜美人生，精彩人生。
（By:Eastmount 2017-03-15 中午1点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）



