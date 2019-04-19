# If 语句 VS Select Case 语句 - Jun5203 - CSDN博客
2018年01月24日 21:16:17[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：190
在“研磨”百例的过程中，突然发现"If 语句" 和 "Select Case 语句"有异曲同工之妙，于是果断把实例2“蹂躏”一番。
实例：要求用Select Case 语句实现成绩等级优、良、中、差、不及格，90分以上是优、80-90是良、70-79是中、60-69是差、60分以下是不及格。
**Select Case 语句**
Option Explicit
Dim x
Private Sub Form_Load()
 x = Val(InputBox("请输入0-100中的数值:", "成绩等级"))
 Select Case x
 Case 90 To 100
  lblscore.Caption = "优" & Chr(13) & "成绩不错,要保持!"
 Case 80 To 89
  lblscore.Caption = "良" & Chr(13) & "还可以，要继续努力，向优靠近呐!"
 Case 70 To 79
  lblscore.Caption = "中" & Chr(13) & "要努力了，不然就要不及格了!"
 Case 60 To 69
  lblscore.Caption = "差" & Chr(13) & "努力呀，不然就要不及格了!"
 Case 0 To 59
  lblscore.Caption = "不及格" & Chr(13) & "胜不骄，败不馁，只要你肯努力，下次一定会成功的!"
 End Select
End Sub
综上为电子教材的模板~
以下是JunH的另一种解法：
**If 语句**
Option Explicit
Dim x
Private Sub Form_Click()
    x = InputBox("请输入数字")
     If x >= 90 And x <= 100 Then
    Print "优" & Chr(13) & "成绩不错，要保持啊！"
     ElseIf x >= 80 And x <= 89 Then
    Print "良" & Chr(13) & "还可以，要继续努力，向优靠近呐！"
     ElseIf x >= 70 And x <= 79 Then
    Print "中" & Chr(13) & "要努力了，不然就落后啦"
     ElseIf x >= 60 And x <= 69 Then
    Print "差" & Chr(13) & "努力呀，不然就要不及格了！"
     ElseIf x >= 0 And x <= 59 Then
    Print "不及格" & Chr(13) & "胜不骄，败不馁，只要你肯努力，下次一定会成功的！"
 End If
End Sub
需要注意的是If 语句中是窗体单击事件，而Select Case语句中是窗体加载事件，二者的区别就是在If语句中需单击后才可运行。
其实代码写到这儿小程序基本上是可以运行的，但美中不足的是这个标签框中的文字并没有居中显示，我根据Select  Case语句的设置方法改了一下，但是没有用，后来就去找了度娘，毕竟我们要“站在巨人的肩膀上”。度娘告诉我在VB中print如果输出不指定的话，输出一直在屏幕左上角。
解决方法如下：
Private Sub Form_Load()
ScaleMode = 3
CurrentX = 100
CurrentY = 100
Print "X:100,Y:100"
End Sub
但此种方法还是没有达到我预想的结果，虽然位置改变，但仍处于左侧。我猜测应该是解决方法中的数字问题，后续有待考究，或者是有哪位大佬指点一二![](http://www.sinaimg.cn/uc/myshow/blog/misc/gif/E___6717EN00SIGG.gif)，谢谢思密达！
