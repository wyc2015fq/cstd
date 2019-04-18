# asp调用Word打印的代码 - 深之JohnChen的专栏 - CSDN博客

2009年02月24日 19:56:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1289


** asp调用Word打印的代码**

调用Word打印.
运行下列的脚本需要将IE的安全级别设为低，或在中级安全级别下将那个ActiveX控件设为启用，别忘记了。 
<html> 
<head> 
<title>Build Document by Script</TITLE> 
</HEAD> 
<Table id="myData" border=1 align=center> 
<Tr align=center> 
<Td>产品名称</Td> 
<Td>产品描述</Td> 
<Td>产品单价</Td> 
<Td>产品等级</Td> 
</Tr> 
<Tr align=center> 
<Td>产品一</Td> 
<Td>This is a test</Td> 
<Td align=right>300.50</Td> 
<Td>一级</Td> 
</Tr> 
<Tr align=center> 
<Td>产品二</Td> 
<Td>This is a test</Td> 
<Td align=right>300.50</Td> 
<Td>一级</Td> 
</Tr> 
<Tr align=center> 
<Td>产品三</Td> 
<Td>This is a test</Td> 
<Td align=right>300.50</Td> 
<Td>一级</Td> 
</Tr> 
</Tabld> 
<input type=button onclick="vbscript:buildDoc '',4" value="build"> 
<BODY> 
</BODY> 
</HTML> 
<script language="vbscript"> 
Sub buildDoc(theTemplate,intTableRows) 
Dim Table1 
set Table1 = document.all.myData 
row = Table1.rows.length 
Set objWordDoc = CreateObject("Word.Document") 
ObjWordDoc.Application.Visible=True 
Dim theArray(4,4) 
'Redim Preserve theArray(4,row) 
colnum = Table1.rows(1).cells.length 
for i=0 to row-1 
for j=0 to colnum-1 
theArray(j+1,i+1) = Table1.rows(i).cells(j).innerHTML 
next 
next 
intNumrows = 4 
objWordDoc.Application.ActiveDocument.Paragraphs.Add.Range.InsertBefore("测试的表格") 
objWordDoc.Application.ActiveDocument.Paragraphs.Add.Range.InsertBefore("") 
objWordDoc.Application.ActiveDocument.Paragraphs.Add.Range.InsertBefore("") 
Set rngPara = objWordDoc.Application.ActiveDocument.Paragraphs(1).Range 
With rngPara 
.Bold = True 
.ParagraphFormat.Alignment = 1 
.Font.Name = "Arial" 
.Font.Size = 12 
End With 
Set rngCurrent = objWordDoc.Application.ActiveDocument.Paragraphs(3).Range 
Set tabCurrent = ObjWordDoc.Application.ActiveDocument.Tables.Add(rngCurrent,intNumrows,4) 
for i = 1 to colnum 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(1).Cells(i).Range.InsertAfter theArray(i,1) 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(1).Cells(i).Range.ParagraphFormat.alignment=1 
next 
tabRow = 2 
For j = 2 to intNumrows 
'ObjWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Borders.Enable=False 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(1).Range.InsertAfter theArray(1,j) 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(1).Range.ParagraphFormat.alignment=1 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(2).Range.InsertAfter theArray(2,j) 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(2).Range.ParagraphFormat.alignment=1 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(3).Range.InsertAfter FormatCurrency(theArray(3,j)) 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(3).Range.ParagraphFormat.alignment=2 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(4).Range.InsertAfter theArray(4,j) 
'objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(4).Range.InsertAfter Chr(10) 
objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(4).Range.ParagraphFormat.alignment=1 
tabRow = tabRow + 1 
Next 
' objWordDoc.Application.ActiveDocument.Tables(1).Rows(tabRow).Cells(4).Range.ParagraphFormat.alignment=1 
'objWordDoc.Application.ActiveDocument.Paragraph.Add.Range.InsertAfter(" ") 
'objWordDoc.Application.ActiveDocument.Paragraph.Add.Range.InsertAfter(" ") 
'objWordDoc.Application.ActiveDocument.Paragraph.Add.Range.InsertAfter("Regards,") 
'objWordDoc.Application.ActiveDocument.Paragraph.Add.Range.InsertAfter(" ") 
'objWordDoc.Application.ActiveDocument.Paragraph.Add.Range.InsertAfter("Daryl B. Morticum") 
'objWordDoc.Application.ActiveDocument.Paragraph.Add.Range.InsertAfter("Sales Associate") 
'objWordDoc.Application.ActiveDocument.Envelope.Insert Address 
' objWordDoc.Application.ActiveDocument.Paragraphs.Add.Range.InsertAfter("由廖嘉设计") 
objWordDoc.Application.ActiveDocument.SaveAs "tempSample.doc", 0,False,"",True,"",False,False,False, False,False 
'objWordDoc.Application.printout() 
End Sub 
</script>

