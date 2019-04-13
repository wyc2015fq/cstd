
# Exporting points in catia into .txt file or to Excel sheet - cocoonyang的专栏 - CSDN博客


2016年12月25日 09:45:44[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：374个人分类：[CATIA																](https://blog.csdn.net/cocoonyang/article/category/6640468)


Exporting points in catia into .txt file or to Excel sheet
https://grabcad.com/questions/how-to-export-points-in-catia-into-txt-file-or-to-excel-sheet


```python
Dim objGEXCELapp As Object 
Dim objGEXCELwkBks As Object 
Dim objGEXCELwkBk As Object 
Dim objGEXCELwkShs As Object 
Dim objGEXCELSh As Object 
Dim fs, f, f1, fc, s 
Dim coords(2) As Integer 
Dim PartDocument1
Sub CATMain()
CATIA.ActiveDocument.Selection.Search "CATGmoSearch.Point,all"
StartEXCEL
ExportPoint
'objGEXCELSh.Application.ActiveWorkbook.SaveAs (ExcelFolder & Left(CATIA.ActiveDocument.Name,Len(CATIA.ActiveDocument.Name)-8) & ".xls") 
'objGEXCELSh.Application.ActiveWorkbook.Close
End Sub
'****************************************************************************** 
Sub StartEXCEL() 
'****************************************************************************** 
Err.Clear 
On Error Resume Next 
Set objGEXCELapp = GetObject (,"EXCEL.Application") 
If Err.Number <> 0 Then 
Err.Clear 
Set objGEXCELapp = CreateObject ("EXCEL.Application") 
End If
objGEXCELapp.Application.Visible = TRUE 
Set objGEXCELwkBks = objGEXCELapp.Application.WorkBooks 
Set objGEXCELwkBk = objGEXCELwkBks.Add 
Set objGEXCELwkShs = objGEXCELwkBk.Worksheets(1) 
Set objGEXCELSh = objGEXCELwkBk.Sheets (1) 
objGEXCELSh.Cells (1,"A") = "Name" 
objGEXCELSh.Cells (1,"B") = "X" 
objGEXCELSh.Cells (1,"C") = "Y" 
objGEXCELSh.Cells (1,"D") = "Z"
End Sub
'****************************************************************************** 
Sub ExportPoint() 
'****************************************************************************** 
For i = 1 To CATIA.ActiveDocument.Selection.Count 
Set selection = CATIA.ActiveDocument.Selection 
Set element = selection.Item(i) 
Set point = element.value
'Write PointData to Excel Sheet 
point.GetCoordinates(coords)
objGEXCELSh.Cells (i+1,"A") = point.name 
objGEXCELSh.Cells (i+1,"B") = coords(0) 
objGEXCELSh.Cells (i+1,"C") = coords(1) 
objGEXCELSh.Cells (i+1,"D") = coords(2)
Next
End Sub
```


