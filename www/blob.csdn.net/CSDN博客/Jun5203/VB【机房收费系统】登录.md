# VB【机房收费系统】登录 - Jun5203 - CSDN博客
2018年09月01日 21:16:34[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：84
个人分类：[【机房收费系统】](https://blog.csdn.net/Ellen5203/article/category/7907465)
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
####前言
 在经过和纪老师、师傅、师哥等人的交流后，我的登录窗体终于成功“问世”，而且我在探索的过程中，也发现了不少东西，事实证明，纪老师说的是正确的，若要了解详情，请看上上上上上篇博客，下面我就来分享登录的操作。
![这里写图片描述](https://img-blog.csdn.net/20180901211334478?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
####代码分享
```
Private Sub cmdOK_Click()
UserName = ""
    If Trim(txtUserName.Text = "") Then                                         '判断用户名是否为空
        MsgBox "用户名为空，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtUserName.SetFocus
        txtUserName.Text = ""
        txtPassword.Text = ""
    Else
    If Trim(txtPassword.Text = "") Then                                 '判断密码是否为空
        MsgBox "密码为空，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtPassword.SetFocus
        txtUserName.Text = ""
        txtPassword.Text = ""
    Else
        txtSQL = "select * from User_Info where userID= '" & txtUserName.Text & "'"  '查询指定用户名记录
        Set mrc = ExecuteSQL(txtSQL, MsgText)  '执行查询语句
        
    If mrc.EOF = True Then
        MsgBox "无此用户，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtUserName.SetFocus
        txtUserName.Text = ""
        txtPassword.Text = ""
    Else
    
    If Trim(mrc.Fields(1)) = Trim(txtPassword.Text) Then
        ok = True
        Me.Hide
        UserName = Trim(txtUserName.Text)
    End If
       
    If Trim(mrc.Fields(2)) = "一般用户" Then   '一般用户的权限
    If Trim(mrc.Fields(1)) = Trim(txtPassword.Text) Then
                   
    txtSQL = "select * from OnWork_Info"      '获取OnWork表
    Set mrc1 = ExecuteSQL(txtSQL, MsgText)
                  
        Me.Hide
                    
        UserName = Trim(txtUserName.Text)         '将用户名给username
        ok = True
                
        MDImain.Show
        MDImain.MenuGeneralUser.Enabled = True
        MDImain.MenuOperator.Enabled = False
        MDImain.MenuAdministrator.Enabled = False
                  
        mrc1.AddNew                               '添加记录到onWork表中
        mrc1.Fields(0) = Trim(txtUserName)
        mrc1.Fields(1) = Trim(mrc.Fields(2))
        mrc1.Fields(2) = Trim(Date)
        mrc1.Fields(3) = Trim(Time)
        mrc1.Fields(4) = VBA.Environ("computername")
        mrc1.Update
                  
    txtSQL = "select * from worklog_Info"     '获取worklog_Info表
    Set mrc2 = ExecuteSQL(txtSQL, MsgText)
 
        mrc2.AddNew                               '添加记录到Worklog_Info表中
        mrc2.Fields(1) = Trim(txtUserName)
        mrc2.Fields(2) = Trim(mrc.Fields(2))
        mrc2.Fields(3) = Trim(Date)
        mrc2.Fields(4) = Trim(Time)
        mrc2.Fields(7) = VBA.Environ("computername")
        mrc2.Update
                  
        mrc.Close                                 '关闭数据连接
                  
    Else
        MsgBox "输入密码不正确，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtPassword.SetFocus
        txtPassword.Text = ""
    End If
     
    ElseIf Trim(mrc.Fields(2)) = "操作员" Then '操作员的权限
    If Trim(mrc.Fields(1)) = Trim(txtPassword.Text) Then
                  
    txtSQL = "select * from OnWork_Info"      '获取OnWork表
    Set mrc1 = ExecuteSQL(txtSQL, MsgText)
                  
        Me.Hide
                  
        UserName = Trim(txtUserName.Text)         '将用户名给username
        ok = True
                
        MDImain.Show
        MDImain.MenuGeneralUser.Enabled = True
        MDImain.MenuOperator.Enabled = True
        MDImain.MenuAdministrator.Enabled = False
                  
        mrc1.AddNew                               '添加记录到onWork表中
        mrc1.Fields(0) = Trim(txtUserName)
        mrc1.Fields(1) = Trim(mrc.Fields(2))
        mrc1.Fields(2) = Trim(Date)
        mrc1.Fields(3) = Trim(Time)
        mrc1.Fields(4) = VBA.Environ("computername")
        mrc1.Update
                  
                   
    txtSQL = "select * from worklog_Info"     '获取worklog_Info表
    Set mrc2 = ExecuteSQL(txtSQL, MsgText)
                
        mrc2.AddNew                               '添加记录到Worklog_Info表中
        mrc2.Fields(1) = Trim(txtUserName)
        mrc2.Fields(2) = Trim(mrc.Fields(2))
        mrc2.Fields(3) = Trim(Date)
        mrc2.Fields(4) = Trim(Time)
        mrc2.Fields(7) = Environ("computername")
        mrc2.Update
                  
        mrc.Close                                 '关闭数据连接
                  
    Else
        MsgBox "输入密码不正确，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtPassword.SetFocus
        txtPassword.Text = ""
    End If
                  
    ElseIf Trim(mrc.Fields(2)) = "管理员" Then '管理员的权限
    If Trim(mrc.Fields(1)) = Trim(txtPassword.Text) Then
                  
    txtSQL = "select * from OnWork_Info"      '获取OnWork表
    Set mrc1 = ExecuteSQL(txtSQL, MsgText)
                  
        Me.Hide
                  
        UserName = Trim(txtUserName.Text)         '将用户名给username
        ok = True
                  
        MDImain.Show
        MDImain.MenuGeneralUser.Enabled = True
        MDImain.MenuOperator.Enabled = True
        MDImain.MenuAdministrator.Enabled = True
                  
        mrc1.AddNew                               '添加记录到onWork表中
        mrc1.Fields(0) = Trim(txtUserName)
        mrc1.Fields(1) = Trim(mrc.Fields(2))
        mrc1.Fields(2) = Trim(Date)
        mrc1.Fields(3) = Trim(Time)
        mrc1.Fields(4) = Environ("computername")  '获取计算机名字
        mrc1.Update
                                    
    txtSQL = "select * from worklog_Info"     '获取worklog_Info表
    Set mrc2 = ExecuteSQL(txtSQL, MsgText)
 
        mrc2.AddNew                               '添加记录到Worklog_Info表中
        mrc2.Fields(1) = Trim(txtUserName)
        mrc2.Fields(2) = Trim(mrc.Fields(2))
        mrc2.Fields(3) = Trim(Date)
        mrc2.Fields(4) = Trim(Time)
        mrc2.Fields(7) = Environ("computername")
        mrc2.Update
                  
        mrc.Close                                 '关闭数据连接
                  
    Else
        MsgBox "输入密码不正确，请重新输入！", vbOKOnly + vbExclamation, "警告"
        txtPassword.SetFocus
        txtPassword.Text = ""
    End If
           
    Else
        End                                           '三者都不是，直接退出
    End If
    
    End If
  End If
End If
   
Rem：记载输入密码次数
    miCount = miCount + 1
    If miCount = 3 Then
        '超过允许的登录次数，显示提示信息
        MsgBox "你已经超过允许验证次数！" & vbCr & "应用程序将结束！", vbCritical, "登录验证"
        Me.Hide
    End If
    Exit Sub
End Sub
```
####小结
 其实写到这儿，大家肯定也发现了不少东西，在做机房收费系统的时候，无非就是增、删、改、查，只要我们把思路理清，就按着套路来呗，很简单的，有没有？
