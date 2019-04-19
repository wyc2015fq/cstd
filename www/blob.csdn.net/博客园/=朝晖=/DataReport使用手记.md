# DataReport使用手记 - =朝晖= - 博客园
# [DataReport使用手记](https://www.cnblogs.com/dhcn/p/7116163.html)
06年的一篇blog，转过来：
   前几天，帮同事改一个VB的课业程序，具体任务就是在程序中添加报表功能，由于考虑到部署环境的问题，所以没有采用我以前惯用的Excel实现，而采用了同事提出的VB自带的DataReport实现，查到的资料大多数都是在DataReport数据源上连接DataEnvironment，MSDN也有这方面的一篇文章，可惜英文版和中文版一样的模糊，移到程序中也无法运行，后来根据实验中的错误提示在网上查到一个提问题的帖子，在他的帖子中倒是找到了正确的方法，原来直接把数据源链接为RecordSet就可以了，DataMember可以不用赋值，DataField赋值为字段名称，DataReport的空间索引必须以控件名称索引即可，下面是源代码。我在details Section按顺序建立了8个TextBox和Label接受字段值和字段名。
Set DataReport.DataSource = rs_data2
    If rs_data2.RecordCount = 0 Then
        Exit Sub
    End If
    Dim i As Integer
    Dim j As String
   rs_data2.MoveFirst
   Do While Not rs_data2.EOF
      For i = 0 To 7
        With DataReport
            j = i + 1
            .Sections("Section1").Controls("Label" + j).Caption = rs_data2.Fields(i).Name + ":"
            .Sections("Section1").Height = 60
            If Len(rs_data2.Fields(0)) <> 0 Then
                .Sections("Section1").Controls("Text" + j).DataField = rs_data2.Fields(i).Name
            Else
                .Sections("Section1").Controls("Text" + j).DataField = ""
            End If
        End With
     Next i
     rs_data2.MoveNext
   Loop
   DataReport.Show

