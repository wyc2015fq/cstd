
# Firefox下载含空格的文件 - 追求卓越,做到专业 - CSDN博客


2013年10月31日 13:43:22[Waldenz](https://me.csdn.net/enter89)阅读数：2813


当下载含空格的文件时，Firefox会显示不完整的文件名。

```python
Dim filename as String = "File Name.jpg"      
        Response.Clear()
        Response.ClearHeaders()
        Response.ClearContent()
        Response.ContentType = "application/octet-stream"
        Response.AddHeader("content-disposition", "attachment; filename=" & filename )
        Response.AddHeader("Content-Length", bs.Length.ToString)
        Response.BinaryWrite(bs)
```
如果filename是包含空格的字符串，则下载时会出现问题。如下图
![](https://img-blog.csdn.net/20131031135006093)
解决方法：在文件名两边加双引号，所以替换以上代码中的一句：VB:
```python
Response.AddHeader("content-disposition", "attachment; filename=""" & filename & """")
```
C\#:

```python
Response.AddHeader("content-disposition", "attachment; filename=\"" + filename + "\"");
```
代码修改过后的下载窗口：
![](https://img-blog.csdn.net/20131031135305468)



