# R语言︱用excel VBA把xlsx批量转化为csv格式 - 素质云笔记/Recorder... - CSDN博客





2016年04月08日 23:56:55[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6011
所属专栏：[R的数据操作与清洗](https://blog.csdn.net/column/details/13587.html)












笔者寄语：批量读取目前看到有以下几种方法：xlsx包、RODBC包、批量转化成csv后读入。本章来自博客：http://www.cnblogs.com/weibaar/p/4506144.html







在上面的尝试已经发现，xlsx本身就是这个复杂问题的最根本原因。与之相反，R对csv等文本格式支持的很好，而且有**fread**这个神器，要处理一定量级的数据，还是得把xlsx转化为csv格式。




以此为思路，在参考了两个资料后，我成功改写了一段VBA，可以选中需要的xlsx，然后在其目录下新建csv文件夹，把xlsx批量转化为csv格式。




代码如下：

```
Sub getCSV()
'这是网上看到的xlsx批量转化，而改写的一个xlsx批量转化csv格式
'1)批量转化csv参考：http://club.excelhome.net/thread-1036776-2-1.html
'2)创建文件夹参考：http://jingyan.baidu.com/article/f54ae2fcdc79bc1e92b8491f.html
'这里设置屏幕不动，警告忽略
Application.DisplayAlerts = False
Application.ScreenUpdating = False
Dim data As Workbook
'这里用GetOpenFilename弹出一个多选窗口，选中我们要转化成csv的xlsx文件，
file = Application.GetOpenFilename(MultiSelect:=True)
'用LBound和UBound
For i = LBound(file) To UBound(file)
    Workbooks.Open Filename:=file(i)
    Set data = ActiveWorkbook
    Path = data.Path
    '这里设置要保存在目录下面的csv文件夹里,之后可以自己调
    '参考了里面的第一种方法
    On Error Resume Next
    VBA.MkDir (Path & "\csv")
    With data
        .SaveAs Path & "\csv\" & Replace(data.Name, ".xlsx", ".csv"), xlCSV
        .Close True
      End With
Next i
'弹出对话框表示转化已完成，这时去相应地方的csv里查看即可
MsgBox "已转换了" & (i-1) & "个文档"
Application.ScreenUpdating = True
Application.DisplayAlerts = True
End Sub
```





操作很简单：

把代码复制进excel的vba编辑器里，然后运行getcsv这个宏，会跳出一个窗口，要求选择你要转化的xlsx文件。（可多选）




选中以后，等一段时间，再回到xlsx文件下，会多一个csv文件夹，里面就是我们要导入R的文本文件了。




这个方法的好处是：

1、操作简单，直接依托于excel的VBA操作，不用配置java环境，之后沟通成本/换电脑成本小 



2、特别适用于有一定数据量，但是数据格式整齐的文件，譬如从某数据端读入的数据。用**fread**还可以控制读取的行(skip=NNN)，代码写入整洁方便。就算有一些异行数据，也可以事先用VBA进行操作，简单方便。


——————————————————————————————————————————————

**读入R数据库可以用lappy来读取：**






```
#lapply读取法
filenames <- list.files("C:/Users/a.csv", pattern = ".csv",full.names = TRUE)  #变成list格式
#没有full.names = TRUE，都会出现cannot open file: No such file or directory

name=function(x) {
  read.csv(x,header=T)
  }
datalist <- lapply(filenames,name)  #filenames执行name函数
```





——————————————————————————————————————————————


**其中需要强调一点WPS excel怎么加载宏**

1、WPS excel本身是没有宏功能的，需要自己加载；

2、加载一个.exe，安装好了，即可。




就需要下载一个“wps的vba模块”，这个模块下载链接：

链接:http://pan.baidu.com/s/1bnvoigJ



![](https://img-blog.csdn.net/20160408234842539)





加载好这一个就行了，

![](https://img-blog.csdn.net/20160408234940028)





然后就是简单输入，就可以调用宏了。

![](https://img-blog.csdn.net/20160408235105325)


参考：http://office.jb51.net/wps/4961.html









