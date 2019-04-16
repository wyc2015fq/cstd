# 在Excel中获得汉字的首字母 - lyx的专栏 - CSDN博客





2016年05月31日 20:12:50[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：756








在EXECL中，按ALT+F11，插入---模块复制下列代码：



Function hztopy(hzpy As String) As String 

Dim hzstring As String, pystring As String 

Dim hzpysum As Integer, hzi As Integer, hzpyhex As Integer 

hzstring = Trim(hzpy) 

hzpysum = Len(Trim(hzstring)) 

pystring = "" 

For hzi = 1 To hzpysum 

hzpyhex = "&H" + Hex(Asc(Mid(hzstring, hzi, 1))) 

Select Case hzpyhex 

Case &HB0A1 To &HB0C4: pystring = pystring + "A" 

Case &HB0C5 To &HB2C0: pystring = pystring + "B" 

Case &HB2C1 To &HB4ED: pystring = pystring + "C" 

Case &HB4EE To &HB6E9: pystring = pystring + "D" 

Case &HB6EA To &HB7A1: pystring = pystring + "E" 

Case &HB7A2 To &HB8C0: pystring = pystring + "F" 

Case &HB8C1 To &HB9FD: pystring = pystring + "G" 

Case &HB9FE To &HBBF6: pystring = pystring + "H" 

Case &HBBF7 To &HBFA5: pystring = pystring + "J" 

Case &HBFA6 To &HC0AB: pystring = pystring + "K" 

Case &HC0AC To &HC2E7: pystring = pystring + "L" 

Case &HC2E8 To &HC4C2: pystring = pystring + "M" 

Case &HC4C3 To &HC5B5: pystring = pystring + "N" 

Case &HC5B6 To &HC5BD: pystring = pystring + "O" 

Case &HC5BE To &HC6D9: pystring = pystring + "P" 

Case &HC6DA To &HC8BA: pystring = pystring + "Q" 

Case &HC8BB To &HC8F5: pystring = pystring + "R" 

Case &HC8F6 To &HCBF9: pystring = pystring + "S" 

Case &HCBFA To &HCDD9: pystring = pystring + "T" 

Case &HEDC5: pystring = pystring + "T" 

Case &HCDDA To &HCEF3: pystring = pystring + "W" 

Case &HCEF4 To &HD1B8: pystring = pystring + "X" 

Case &HD1B9 To &HD4D0: pystring = pystring + "Y" 

Case &HD4D1 To &HD7F9: pystring = pystring + "Z" 

Case Else 

pystring = pystring + Mid(hzstring, hzi, 1) 

End Select 

Next 

hztopy = pystring 

End Function 



=hztopy(A1) 回车执行即可.






