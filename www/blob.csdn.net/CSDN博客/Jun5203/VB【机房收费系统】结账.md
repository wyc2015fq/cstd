# VB【机房收费系统】结账 - Jun5203 - CSDN博客
2018年09月02日 16:06:19[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：145
个人分类：[【机房收费系统】](https://blog.csdn.net/Ellen5203/article/category/7907465)
所属专栏：[机房收费系统](https://blog.csdn.net/column/details/26589.html)
##前言
 说起结账大家肯定都不陌生，因为我们在做生意的时候一定要算算自己挣没挣到钱，要是不挣钱这个生意还做的话迟早就得倒闭。就拿我们的机房收费系统来说吧，结账就是一个很好的功能，而且是每天结账，一方面可以知道今天挣了多少钱，另一方面可以与昨天相比看自己赚钱还是赔钱，同理周结。
 还没做结账之前，师哥问我结账是给谁结的？怎么结的？我当时也是特别懵逼，不知道嘟囔些什么，后来看了很多巨人的博客才明白，原来结账的窗体中单次是给单个操作员结账，而结账按钮是结的总账，也就是说所有的操作员的帐，换句话来说就是给老板结的，因为操作员是给老板打工的嘛，钱又进不了他们的口袋。下面来看看我是怎么结的帐吧！
![这里写图片描述](https://img-blog.csdn.net/20180902160012137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##部分代码展示
给操作员结账
```
Private Sub cboUserID_Click()
    txtSQL = "select * from User_Info where userID='" & cboUserID.Text & "'"
    Set mrc7 = ExecuteSQL(txtSQL, MsgText)
    
    lblUserName.Caption = mrc7.Fields(3)
    mrc7.Close
    
    Call viewdata  '调用过程
End Sub
Private Sub viewdata()
    If cboUserID.Text = "" Then
        MsgBox "请选择操作员用户名", vbOKOnly + vbExclamation, "警告"
        Exit Sub
    End If
 
    txtSQL = "select * from student_Info where UserID='" & cboUserID.Text & "' and date='" & Date & "'"
    Set mrc = ExecuteSQL(txtSQL, MsgText)
    
    With MSHFlexGrid1
        .CellAlignment = 4
        .Rows = 1
        .ColWidth(2) = 1900
        .TextMatrix(0, 0) = "学号"
        .TextMatrix(0, 1) = "卡号"
        .TextMatrix(0, 2) = "日期"
        .TextMatrix(0, 3) = "时间"
        
    Do While Not mrc.EOF
        .Rows = .Rows + 1
        .CellAlignment = 4
        .TextMatrix(.Rows - 1, 0) = mrc.Fields(1)
        .TextMatrix(.Rows - 1, 1) = mrc.Fields(0)
        .TextMatrix(.Rows - 1, 2) = mrc.Fields(12)
        .TextMatrix(.Rows - 1, 3) = mrc.Fields(13)
        mrc.MoveNext
        Loop
    End With
    
    txtSQL = "select * from Recharge_Info where UserID='" & cboUserID.Text & "' and date='" & Date & "'"
    Set mrc5 = ExecuteSQL(txtSQL, MsgText)
    
    RechargeCash = 0
   
    With MSHFlexGrid2
        .CellAlignment = 4
        .Rows = 1
        .ColWidth(3) = 1900
        .TextMatrix(0, 0) = "学号"
        .TextMatrix(0, 1) = "卡号"
        .TextMatrix(0, 2) = "充值金额"
        .TextMatrix(0, 3) = "日期"
        .TextMatrix(0, 4) = "时间"
        .TextMatrix(0, 5) = "状态"
        
    Do While Not mrc5.EOF
        .Rows = .Rows + 1
        .CellAlignment = 4
        .TextMatrix(.Rows - 1, 0) = mrc5.Fields(1) & ""
        .TextMatrix(.Rows - 1, 1) = mrc5.Fields(2)
        .TextMatrix(.Rows - 1, 2) = mrc5.Fields(3)
        .TextMatrix(.Rows - 1, 3) = mrc5.Fields(4)
        .TextMatrix(.Rows - 1, 4) = mrc5.Fields(5)
        .TextMatrix(.Rows - 1, 5) = mrc5.Fields(7)
        mrc5.MoveNext
    Loop
    End With
    
    txtSQL = "select * from CancelCard_Info where UserID='" & Trim(cboUserID.Text) & "' and date='" & Date & "'"
    Set mrc4 = ExecuteSQL(txtSQL, MsgText)
    
    CancelCash = 0
    
    With MSHFlexGrid3
        .CellAlignment = 4
        .Rows = 1
        .ColWidth(2) = 1900
        .TextMatrix(0, 0) = "学号"
        .TextMatrix(0, 1) = "卡号"
        .TextMatrix(0, 2) = "日期"
        .TextMatrix(0, 3) = "时间"
        .TextMatrix(0, 4) = "退卡金额"
        .TextMatrix(0, 5) = "状态"
        
    Do While Not mrc4.EOF
        .Rows = .Rows + 1
        .CellAlignment = 4
        .TextMatrix(.Rows - 1, 0) = mrc4.Fields(0)
        .TextMatrix(.Rows - 1, 1) = mrc4.Fields(1)
        .TextMatrix(.Rows - 1, 2) = mrc4.Fields(3)
        .TextMatrix(.Rows - 1, 3) = mrc4.Fields(4)
        .TextMatrix(.Rows - 1, 4) = mrc4.Fields(2)
        .TextMatrix(.Rows - 1, 5) = mrc4.Fields(6)
        mrc4.MoveNext
        Loop
    End With
    
    txtSQL = "select * from student_Info where UserID='" & Trim(cboUserID.Text) & "' and type='临时用户'and date='" & Date & "'"
    Set mrc6 = ExecuteSQL(txtSQL, MsgText)
    
    With MSHFlexGrid4
        .CellAlignment = 4
        .Rows = 1
        .ColWidth(2) = 1900
        .TextMatrix(0, 0) = "学号"
        .TextMatrix(0, 1) = "卡号"
        .TextMatrix(0, 2) = "日期"
        .TextMatrix(0, 3) = "时间"
        
    Do While Not mrc6.EOF
        .Rows = .Rows + 1
        .CellAlignment = 4
        .TextMatrix(.Rows - 1, 0) = mrc6.Fields(1)
        .TextMatrix(.Rows - 1, 1) = mrc6.Fields(0)
        .TextMatrix(.Rows - 1, 2) = mrc6.Fields(12)
        .TextMatrix(.Rows - 1, 3) = mrc6.Fields(13)
        mrc6.MoveNext
        Loop
    End With
    
    txtSellCard.Text = mrc.RecordCount
    
    txtExitCard.Text = mrc4.RecordCount
    txtSQL = "select sum(addmoney) from ReCharge_Info where UserID='" & Trim(cboUserID.Text) & "' and date='" & Date & "'"
    Set mrc5 = ExecuteSQL(txtSQL, MsgText)
    
    If IsNull(mrc5.Fields(0)) Then       '无记录
        txtRecharge.Text = "0"
    Else
        txtRecharge.Text = mrc5.Fields(0)
    End If
    txtSQL = "select * from student_Info where UserID='" & Trim(cboUserID.Text) & "'and type='" & "临时用户" & "'and date='" & Date & "'"
    Set mrc = ExecuteSQL(txtSQL, MsgText)
    
    j = 0
    For i = 1 To mrc.RecordCount
        j = j + Val(mrc.Fields(7))
        txtTemporary.Text = Val(j)
        mrc.MoveNext
    Next i
    
    txtSQL = "select sum(CancelCash) from CancelCard_Info where UserID='" & Trim(cboUserID.Text) & "'and date='" & Date & "'"
    Set mrc4 = ExecuteSQL(txtSQL, MsgText)
    
    If IsNull(mrc4.Fields(0)) Then       '无记录
        txtExitMoney.Text = "0"
    Else
        txtExitMoney.Text = mrc4.Fields(0)
    End If
  
    txtAllCard.Text = Val(txtSellCard.Text) - Val(txtExitCard.Text)
    
    txtMoney.Text = Val(txtRecharge.Text) - Val(txtExitMoney.Text)
    
End Sub
```
给老板结账
```
Private Sub cmdOK_Click()
      
    txtSQL = "select sum(addmoney) from Recharge_Info where date='" & Date - 1 & "'"
    Set mrc5 = ExecuteSQL(txtSQL, MsgText)
    If IsNull(mrc5.Fields(0)) = True Then
        LastRecharge = "0"
    Else
        LastRecharge = mrc5.Fields(0)
        
    txtSQL = "select sum(CancelCash) from CancelCard_Info where Date='" & Date - 1 & "'"
    Set mrc4 = ExecuteSQL(txtSQL, MsgText)
    If IsNull(mrc4.Fields(0)) Then
        LastExitCash = "0"
    Else
        LastExitCash = mrc4.Fields(0)
        
        RemainCash = LastRecharge - LastExitCash
        End If
    End If
    
    txtSQL = "select sum(addmoney) from Recharge_Info where date  = '" & Date & "'" '
    Set mrc5 = ExecuteSQL(txtSQL, MsgText)
    If IsNull(mrc5.Fields(0)) = True Then
        RechargeCash1 = "0"
    Else
        RechargeCash1 = mrc5.Fields(0)
    End If
    
    txtSQL = "select sum(consume) from Line_Info where offdate='" & Date & "'"
    Set mrc3 = ExecuteSQL(txtSQL, MsgText)
    If IsNull(mrc3.Fields(0)) = True Then
        ConsumeCash = "0"
    Else
        ConsumeCash = mrc3.Fields(0)
    End If
    txtSQL = "select sum(CancelCash) from CancelCard_Info where Date='" & Date & "'"
    Set mrc4 = ExecuteSQL(txtSQL, MsgText)
    If IsNull(mrc4.Fields(0)) Then
        CancelCash1 = "0"
    Else
        CancelCash1 = mrc4.Fields(0)
    End If
  
    txtSQL = "delete CheckDay_Info where date='" & Date & "'"
    Set mrc9 = ExecuteSQL(txtSQL, MsgText)
    
    txtSQL = "select * from CheckDay_Info"
    Set mrc9 = ExecuteSQL(txtSQL, MsgText)
    
        mrc9!RemainCash = Val(RemainCash)
        mrc9!RechargeCash = Val(RechargeCash1)
        mrc9!ConsumeCash = Val(ConsumeCash)
        mrc9!CancelCash = Val(CancelCash1)
        mrc9!AllCash = Val(RechargeCash1) - Val(CancelCash1)
        mrc9!Date = Date
        mrc9.Update
        mrc9.Close
            MsgBox "结账成功", vbOKOnly + vbInformation, "提示"
        
    txtSQL = "select * from ReCharge_Info where date='" & Date & "' and status = '" & "未结账" & "'"
    Set mrc5 = ExecuteSQL(txtSQL, MsgText)
    
    Do While Not mrc5.EOF
        mrc5!Status = "结账"
        mrc5.Update
        mrc5.MoveNext
    Loop
    mrc5.Close
    
    txtSQL = "select * from CancelCard_Info where date='" & Date & "' and status = '" & "未结账" & "'"
    Set mrc4 = ExecuteSQL(txtSQL, MsgText)
    
    Do While Not mrc4.EOF
        mrc4!Status = "结账"
        mrc4.Update
        mrc4.MoveNext
    Loop
    mrc4.Close
    txtSQL = "delete CheckWeek_Info"
    Set mrc10 = ExecuteSQL(txtSQL, MsgText)
    txtSQL = "Insert into CheckWeek_Info select * from CheckDay_Info"
    Set mrc9 = ExecuteSQL(txtSQL, MsgText)
    
End Sub
```
##小结
 结账这个东西，看你怎么理解，每个人有每个人的想法，只要逻辑没有问题，最后能算出钱来就很OK，加油吧！
