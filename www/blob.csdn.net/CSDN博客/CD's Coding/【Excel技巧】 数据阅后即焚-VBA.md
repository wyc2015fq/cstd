# 【Excel技巧】 数据阅后即焚-VBA - CD's Coding - CSDN博客





2013年11月29日 20:30:26[糖果天王](https://me.csdn.net/okcd00)阅读数：1997







打开VBA编辑器在ThisWorkbook插入下面的代码
可以实现，当前日期超过11月13日的时候，自动清空A1：M15的内容
Private Sub Workbook_Open()
If CDate(Date) > CDate("2012-11-13") Then
sheet1.range("a1:M15")= ""
End If
End Sub


