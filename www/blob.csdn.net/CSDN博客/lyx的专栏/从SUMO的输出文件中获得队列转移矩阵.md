# 从SUMO的输出文件中获得队列转移矩阵 - lyx的专栏 - CSDN博客





2015年11月30日 18:17:16[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1040








SUMO的功能是很强大，不过可视化和后期期望结果的多样性似乎就不太如人意了。

本次我们利用SUMO的dump仿真输出文件来获取一个队列转移矩阵（lane change rate matrix）。这一矩阵在优化中有着很重要的地位。

## 1.首先来看一下dump文件

在仿真配置文件中的output部分加入下面这样的语句，就会生成dump文件



```
<output> 
   <netstate-dump value="<FILE>" />
</output>
```


其中FILE是你希望的文件名。

文件里面是这样的，具体是什么我们等会儿转换成csv导入python之后再看。



```
<netstate>
   <timestep time="<TIME_STEP>">
      <edge id="<EDGE_ID>">
         <lane id="<LANE_ID>">
            <vehicle id="<VEHICLE_ID>" pos="<VEH_POSITION>" speed="<VEH_SPEED>"/>

            ... more vehicles if any on this lane ...

         </lane>

         ... more lanes if the edge possesses more ...

      </edge>

      ... more edges ....

   </timestep>

... the next timestep ...

</netstate>
```

## 2.dump文件转csv

dump文件是xml格式的，其实也可以用BeautifulSoup和python联合来处理，但是，这样比较繁琐，至少在程序上是这样的。况且，sumo自带的tools里面有xml2csv的程序，可以好好利用一下。



在sumo，tools文件夹下面的xml子文件夹，里面有一个xml2csv.py的python程序，可以在命令行中运行它。

就像这样就可以了。

```python

```


如果dump文件比较大，需要等等待一些时间。特别注意，dump.xml需要和python文件同文件夹下。

## 3.python处理

获取csv，那么之后的处理就方便多了。我们可以轻易的把csv文件导入到python，利用python强大的pandas和numpy模块处理。

导入之后，查看一下，发现dump中含有的信息还是很多的，比如仿真时间、edge编号，lane编号、车辆编号等等，下图只是截取了部分。









```python
import pandas as pd
import numpy as np
data = pd.read_csv('E:/dump.csv',sep = ';')
dataFrame = pd.DataFrame(data)
dataNtNd= dataFrame[['lane_id','vehicle_id']]

dataNtNd = dataClearNt.dropna()

dataClearNt = dataNtNd.drop_duplicates()


dataNtNdSort = dataNtNd.sort(columns='vehicle_id')

dataNtNdSort.to_csv('E:/lc.csv')

lane = dataNtNdSort['lane_id']
lane=lane.drop_duplicates()
lane.to_csv('E:/lane.csv')
```
上面的python代码，从dump文件生成的csv文件中截取了需要的字段，同时做了一些数据清理工作。最后，生成lc.csv文件用于计算队列转移矩阵的值，lane.csv文件用于形成矩阵的行列坐标。。当然啦，这里我们只是生成了两个csv文件，而没有直接生成矩阵。原因是转移矩阵要求在excel中展现，而且之前有写过vba程序，所以这里python只是做一个数据清洗，毕竟几百万条的记录，直接用excel处理，电脑就挂了。

## 4.excelVBA生成矩阵




把生成的数据，按照上图，相同间隔相同空行放置。从左往右前两列为python导出的cl.csv中的数据，要把列名删除。H列就是生成的lane.csv中的数据。

位置正确之后，我们就可以利用excel的VBA自动实现了。

VBA代码如下。



```
Private Sub Workbook_Activate()
    'Dim Dic, Arr 'define the Dic for get the matrix asix,and Arr to process the Dic
    Dim cnt As Long 'the counter for the probability calucatino
    Dim i As Long, r As Long 'i is the counter  for get the matrix axis
   
    Dim cntSum 'the sum of the amount of the turn-line
    
     'generate the matrix
    For i = 2 To 109
        For j = 2 To 109
            Cells(i, 7 + j) = ""
        Next j
    Next i
    
    'clear the matrix content
    For i = 2 To 109
      Cells(1, 7 + i) = Cells(i, 8)
    Next i
    
    'calculate the probability
    For cnt = 2 To r 'calaulate the amount of the avaliable line change
        If Cells(cnt, 2) = Cells(cnt + 1, 2) Then   'the same vehicle
            cntSum = cntSum + 1
        End If
    Next cnt
    
    For cnt = 2 To r 'fill the matrix
        If Cells(cnt, 2) = Cells(cnt + 1, 2) Then
            'MsgBox r
            Set f = Rows(1).Find(Cells(cnt, 1))
            Set c = Columns(8).Find(Cells(cnt + 1, 1))
            
            If Cells(c.Row, f.Column) = "" Then
               Cells(c.Row, f.Column) = 1 / cntSum
            Else
                Cells(c.Row, f.Column) = Cells(c.Row, f.Column).Value + 1 / cntSum
            End If
        End If
    Next cnt
End Sub
```
最后就是一个这样子的转移矩阵










