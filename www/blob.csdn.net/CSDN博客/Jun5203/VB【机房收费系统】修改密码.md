# VB【机房收费系统】修改密码 - Jun5203 - CSDN博客
2018年09月01日 21:04:35[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：56
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
####前言
 在做机房收费系统的修改密码时，我在网上发现了“新大陆”，于是就改变以往的传统风格，进行了一场别开生面的“盛宴”。因为在做学生信息管理系统时，我的登录用户一直传不到修改密码的界面，所以就采取了新的方案，站在巨人的肩膀上，不知这样的界面你喜欢吗？
![这里写图片描述](https://img-blog.csdn.net/20180901205601513?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
温馨提示：“您的登录名”为变色字体
![这里写图片描述](https://img-blog.csdn.net/2018090120591790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
####部分代码分享
```
Private Sub cmdOK_Click()
    Dim txtSQL, MsgText As String
    Dim mrc As ADODB.Recordset
    
    Rem:判断两次新密码是否一致
    '若不一致
    If Trim(txtNewPassword.Text) <> Trim(txtCheckNewPassword.Text) Then
        MsgBox "新密码输入不一致，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtOldPassword.Text = "·输入原密码"
        txtNewPassword.Text = "·设置新密码"
        txtCheckNewPassword.Text = "·确认新密码"
    Else
    
    '若一致，判断旧密码
    txtSQL = "select * from User_Info where userID= '" & UserName & "'"
    Set mrc = ExecuteSQL(txtSQL, MsgText)
    
    If Trim(mrc.Fields(1)) <> txtOldPassword.Text Then
        MsgBox "原密码输入错误，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtOldPassword.Text = "·输入原密码"
        txtNewPassword.Text = "·设置新密码"
        txtCheckNewPassword.Text = "·确认新密码"
    Else
    
    If txtNewPassword.Text = txtOldPassword.Text Then
        MsgBox "新密码不能和原密码一样，请重新输入！", vbOKOnly + vbExclamation, "警告！"
        txtOldPassword.Text = "·输入原密码"
        txtNewPassword.Text = "·设置新密码"
        txtCheckNewPassword.Text = "·确认新密码"
    Exit Sub
    End If
            
        mrc.Fields(1) = txtNewPassword.Text
        mrc.Update
        mrc.Close
        MsgBox "密码修改成功！", vbOKOnly + vbInformation, "提示"
      End If
    End If
    
        txtOldPassword.Text = "·输入原密码"
        txtNewPassword.Text = "·设置新密码"
        txtCheckNewPassword.Text = "·确认新密码"
End Sub
```
 代码展示不全，如果有需要的小伙儿伴可以私聊我呀，很乐意为之效劳。
####小结
 做完修改密码，我感触最深的有两点：一是方法千万遍，看你有没有“思想”；二是该面对总是要面对，至少没有遗憾，有一句话送给大家还有现在的自己，“不论你在什么时候开始，重要的是开始之后就不要停止。”加油吧！
