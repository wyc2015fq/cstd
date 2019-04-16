# [Office] WPS Excel通过添加宏实现多张表格合并 - 杨秀璋的专栏 - CSDN博客





2018年03月06日 14:20:15[Eastmount](https://me.csdn.net/Eastmount)阅读数：7983








在我们使用Excel时，通常会遇到让各个地区、各个学院或下属单位收集数据的情况，有时还会每月、每周，甚至是每日调度数据，而当我们得到了这些Excel文件表格之后，很多同志会进行人工手动汇总，这大大降低了办公的效率。本文主要介绍WPS Excel宏，讲述两种自动汇总表格数据的方法，通过VB代码实现。







## 一、合并当前文件夹下所有文件表格




假设存在如下贵州省九个市州的数据，如下图所示：



![](https://img-blog.csdn.net/20180306132208736)




打开每个表格的格式都一样，安顺市显示如下图所示。注意，如果是向下属单位收集数据，通常需要固定好格式，方便汇总生成透视图形，形成最终的报告。



![](https://img-blog.csdn.net/20180306132422143)





第一步 创建一个新的xls文件，比如“汇总.xls”。

第二步 在“开发工具”中选择“VB编辑器”，如下图所示。



![](https://img-blog.csdn.net/20180306132723489)





第三步 在弹出的Microsoft Visual Basic界面中，选择“Project”中的“Sheet1”。




![](https://img-blog.csdn.net/2018030613301868)





输入如下代码并点击运行按钮。

```
Sub 合并当前目录下所有工作簿的全部工作表()
Dim MyPath, MyName, AWbName
Dim Wb As workbook, WbN As String
Dim G As Long
Dim Num As Long
Dim BOX As String
Application.ScreenUpdating = False
MyPath = ActiveWorkbook.Path
MyName = Dir(MyPath & "\" & "*.xls")
AWbName = ActiveWorkbook.Name
Num = 0
Do While MyName <> ""
If MyName <> AWbName Then
Set Wb = Workbooks.Open(MyPath & "\" & MyName)
Num = Num + 1
With Workbooks(1).ActiveSheet
.Cells(.Range("B65536").End(xlUp).Row + 2, 1) = Left(MyName, Len(MyName) - 4)
For G = 1 To Sheets.Count
Wb.Sheets(G).UsedRange.Copy .Cells(.Range("B65536").End(xlUp).Row + 1, 1)
Next
WbN = WbN & Chr(13) & Wb.Name
Wb.Close False
End With
End If
MyName = Dir
Loop
Range("B1").Select
Application.ScreenUpdating = True
MsgBox "共合并了" & Num & "个工作薄下的全部工作表。如下：" & Chr(13) & WbN, vbInformation, "提示"
End Sub
```

第四步 汇总数据如下图所示，同时汇总完成之时会提示对话框。



![](https://img-blog.csdn.net/20180306140156640)



同时删除多余的表头，然后简单修订如下图所示：



![](https://img-blog.csdn.net/20180306140253846)

最终输出表格如下所示：


![](https://img-blog.csdn.net/20180306140321822)



![](https://img-blog.csdn.net/20180306140349245)




注意，在汇总过程中如果出现最后一行为备注事宜，建议删除只保留统计的数据；同时如果其他Sheet有数值，建议也删除，否则它也会被复制至汇总表。




第五步 选中所有数据，然后在“数据”栏中点击“数据透视表”。



![](https://img-blog.csdn.net/20180306140521208)


第六步 选择的字段进行透视汇总，比如统计各市州所包含的县市区。




![](https://img-blog.csdn.net/20180306140636670)



同时，可以汇总各式各样的数据，如下图所示：



![](https://img-blog.csdn.net/20180306140807214)






## 二、合并当前文件各个子Sheet数据




下面讲解第二种方法，对当前Excel文件中的各个Sheet进行汇总，步骤和上述方法一样，重要的是这段VB宏代码。假设存在黔东南州、安顺市、毕节市，如下图所示：



![](https://img-blog.csdn.net/2018030614095688)


代码如下：


```
Sub 合并当前工作簿下的所有市级分表工作表()
Application.ScreenUpdating = False
For j = 4 To Sheets.Count
If Sheets(j).Name <> ActiveSheet.Name
Then
X = Range("A65536").End(xlUp).Row + 1
Sheets(j).UsedRange.Copy Cells(X, 1)
End If
Next
Range("B1").Select
Application.ScreenUpdating = True
MsgBox "当前工作簿下的全部市级分表工作表已经合并完毕！", vbInformation, "提示"
End Sub
```


最后希望文章对您有所帮助，如果存在不足之处，请海涵~
(By:Eastmount 2017-03-06 下午3点  [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )











