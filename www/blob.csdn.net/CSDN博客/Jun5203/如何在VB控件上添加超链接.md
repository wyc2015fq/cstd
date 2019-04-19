# 如何在VB控件上添加超链接 - Jun5203 - CSDN博客
2018年03月30日 21:05:59[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：371
### 想给自己的小作品添加亮点，当然少不了超链接。那么，如何在VB控件上添加超链接呢？答案，请往下找~
### 第一步：建立一个Label，输入自己想链接的文字
### 第二步：代码
### （1）在通用中输入
### Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
### （2）Private Sub Label1_Click()
###         ShellExecute hWnd, "open", "网址", vbNullString, vbNullString, conSwNormal
###          End Sub
### 第三步：F5运行程序，你就会发现，呀，好神奇！
