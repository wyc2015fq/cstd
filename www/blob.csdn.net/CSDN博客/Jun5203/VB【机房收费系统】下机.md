# VB【机房收费系统】下机 - Jun5203 - CSDN博客
2018年09月01日 20:42:39[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：157
个人分类：[【机房收费系统】](https://blog.csdn.net/Ellen5203/article/category/7907465)
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
####前言
 不知上篇的上机大家懂了没？反正小编又要发干货，欢迎大家“抢购”，下机其实和上机大同小异，唯一的区别就是你得计算消费者消费了多长时间，花了多少钱，最后还剩多少钱。这和我们平时去网吧的套路差不多，只不过现在你变成了程序的缔造者，加油啊！
![这里写图片描述](https://img-blog.csdn.net/20180901203837965?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
####代码分享
```
Private Sub cmdOffline_Click()
    txtSQL = "select * from student_Info where cardno ='" & Trim(txtCardID.Text) & "'"
    Set mrc = ExecuteSQL(txtSQL, MsgText)
    
    If mrc.EOF = True Then
        MsgBox "该卡号不存在，请先注册！", vbOKOnly + vbExclamation, "警告"
        txtCardID.SetFocus
        txtCardID = ""
        txtType.Text = ""
        txtStudentID.Text = ""
        txtName.Text = ""
        txtMajor.Text = ""
        txtSex.Text = ""
        txtOnlineDate.Text = ""
        txtOnlineTime.Text = ""
        txtOfflineDate.Text = ""
        txtOfflineTime.Text = ""
        txtConsumeTime.Text = ""
        txtConsumeMoney.Text = ""
        txtRemainingSum.Text = ""
        
    Exit Sub
    Else
    
    txtSQL = "select * from OnLine_Info where cardno='" & Trim(txtCardID.Text) & "'"
    Set mrc1 = ExecuteSQL(txtSQL, MsgText)
    
    If mrc1.EOF = True Then
        MsgBox "该卡号没有上机，不能进行下机处理", vbOKOnly + vbExclamation, "警告"
        txtCardID.SetFocus
        txtCardID = ""
        txtType.Text = ""
        txtStudentID.Text = ""
        txtName.Text = ""
        txtMajor.Text = ""
        txtSex.Text = ""
        txtOnlineDate.Text = ""
        txtOnlineTime.Text = ""
        txtOfflineDate.Text = ""
        txtOfflineTime.Text = ""
        txtConsumeTime.Text = ""
        txtConsumeMoney.Text = ""
        txtRemainingSum.Text = ""
    Exit Sub
    Else
        
        txtCardID.Text = mrc1.Fields(0)
        txtType.Text = mrc1.Fields(1)
        txtStudentID.Text = mrc1.Fields(2)
        txtName.Text = mrc1.Fields(3)
        txtMajor.Text = mrc1.Fields(4)
        txtSex.Text = mrc1.Fields(5)
        txtOnlineDate.Text = mrc1.Fields(6)
        txtOnlineTime.Text = mrc1.Fields(7)
        txtOfflineDate.Text = Date
        txtOfflineTime.Text = Time
        
        'DateDiff函数，将日期和时间都转换为分钟数
        consumetime = Abs(Val(DateDiff("n", Trim(mrc1!Date), Now)))
        txtConsumeTime.Text = consumetime
        mrc1.Update
      End If
  End If
    txtSQL = "select * from BasicData_Info"
    Set mrc2 = ExecuteSQL(txtSQL, MsgText)
    
    '判断消费时间是否小于准备时间
    If consumetime < Trim(mrc2.Fields(4)) Then
        txtConsumeMoney = "0"
        txtRemainingSum.Text = mrc.Fields(7)
    Else
    
    '判断消费时间是否小于至少上机时间
    If consumetime < Val(mrc2.Fields(3)) Then
        txtConsumeMoney.Text = "2"
        txtRemainingSum.Text = Val(mrc.Fields(7)) - Val(txtConsumeMoney.Text)
    Else
    
    If Trim(mrc1.Fields(1)) = "临时用户" Then
        txtConsumeMoney.Text = Round((mrc2.Fields(1) / 60 * consumetime))
    Else
        txtConsumeMoney.Text = Round((mrc2.Fields(0) / 60 * consumetime))
        End If
    End If
End If
        txtRemainingSum.Text = Val(mrc.Fields(7)) - Val(txtConsumeMoney.Text)
        mrc.Fields(7) = Val(txtRemainingSum.Text)
        mrc.Fields(11) = "未结账"
        mrc.Update
        
    txtSQL = "delete from OnLine_Info where cardno='" & Trim(txtCardID.Text) & "'"
    Set mrc1 = ExecuteSQL(txtSQL, MsgText)
        
    txtSQL = "select * from Line_Info where cardno='" & Trim(txtCardID.Text) & "'"
    Set mrc3 = ExecuteSQL(txtSQL, MsgText)
    
    mrc3.AddNew
    mrc3.Fields(1) = Trim(txtCardID.Text)
    mrc3.Fields(2) = Trim(txtStudentID.Text)
    mrc3.Fields(3) = Trim(txtName.Text)
    mrc3.Fields(4) = Trim(txtMajor.Text)
    mrc3.Fields(5) = Trim(txtSex.Text)
    mrc3.Fields(6) = Trim(txtOnlineDate.Text)
    mrc3.Fields(7) = Trim(txtOnlineTime.Text)
    mrc3.Fields(8) = Trim(txtOfflineDate.Text)
    mrc3.Fields(9) = Trim(txtOfflineTime.Text)
    mrc3.Fields(10) = Trim(txtConsumeTime.Text)
    mrc3.Fields(11) = Trim(txtConsumeMoney.Text)
    mrc3.Fields(12) = Trim(txtRemainingSum.Text)
    mrc3.Fields(13) = "正常下机"
    mrc3.Update
    mrc3.MoveNext
    mrc3.Close
        MsgBox "下机成功！", vbOKOnly + vbInformation, "提示"
        mrc.MoveNext
End Sub
```
####小结
 嘻嘻嘻，代码还算整齐哈，不管你认为是不是，反正我觉得是这样的，不知从什么时候开始有了强迫症，虽然敲的时候麻烦一点，但是给人的感觉很不一样，建议你们也要把代码码正了呦~我也得多加规范！
