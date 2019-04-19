# 机房重构——DataGridView数据显示，窗体显示 - Big Smile - CSDN博客
2016年02月07日 17:05:24[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1127
所属专栏：[机房重构系列](https://blog.csdn.net/column/details/14105.html)

在敲机房的时候遇到的第一个问题肯定就是窗体的显示问题了，然而不同的情况有不同的解决方案，但是总体上都是在与MDI窗体的特征在做斗争，在制定 父窗体的情况上有所变化，在容器的选择上有变化，其他的地方是没有变化的。另外在数据显示空间的使用上面是有问题的，所以出此篇博客以叙述
# 一、窗体的显示
        在VB6.0的时候我们就学会利用setparent函数来制定特殊容器来作为窗体的“背景”，其实在VB.NET中也是一个原理，首先来看VB.NET中的函数声明：
`Declare Function SetParent Lib "user32" Alias "SetParent" (ByVal hWndChild As Integer, ByVal hWndNewParent As Integer) As Integer`
特别要注意的就是数据类型的变化，是和VB6.0中不一样，其次就是容器的变化了，在VB.NET中不能用PictureBox来作容器盛放主要控件了，因为他根本不是容器，但是又出现了一个新的容器，就是Panel控件，外表来看就是一个窗体的背景，到是蛮适合MDI父窗体这个灰色的背景的。![](https://img-blog.csdn.net/20160207150828886?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后再这个控件上面画想要的控件和图片，然后以这个Panel为Setparent指定的父容器，就可以显示控件又可以让窗体覆盖在上面了，要不后果是这样的：
![](https://img-blog.csdn.net/20160207151559530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，实现后的图：![](https://img-blog.csdn.net/20160207151635858?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码：
```
Form2.MdiParent = Me
        Form2.Show()
        SetParent(Form2.Handle, Me.Panel1.Handle)
```
# 二、DataGridView的数据显示，选定
        虽然在VB6.0和VB.NET中都有这个DataGridView控件，但是用法却是很不一样。我个人认为VB.NET中的这个控件比VB6.0中的要好很多，可以直接添加数据源，可以自动调整列宽和行高，可以直接指定数据格数，在属性框里面都可以调整，最大的改变就是标题和正式的数据是分隔开的，叫做head。
**(1)数据的显示**
                在这里数据的显示的方法有两种，一种是在控件上直接绑定数据源，在属性DataSource里面可以直接设置，也可以在代码中直接指定，如果用的是泛型(dataTable)的话，可以直接用List容器(直接)指定数据源，显示的数据就是获得的数据表的数据：
```
Dim TeacherList As New List(Of Model.Work_Log_Info)
        DataGridView1.DataSource = TeacherList
```
![](https://img-blog.csdn.net/20160207165709611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
但是如果直接指定数据源的话，对于七层可能有违背的地方，况且这样的表展示出来了之后，也不好看，所以我们需要手动的添加数据，一个格一个格的添加，然后调整高和宽才可以，具体的代码如下：
```
dgvRecharge.Rows.Add(Result.Count)                                 '数据表添加行，行数等于数据行数
For i = 0 To Result.Count - 1
      dgvRecharge.Rows(i).Cells(0).Value() = Result(i).CardNO       '变量写入行上的每个数据，rows代表行，Cell代表第几个格
      dgvRecharge.Rows(i).Cells(1).Value() = Result(i).ChargeMoney
      dgvRecharge.Rows(i).Cells(2).Value() = Result(i).ChargeDate
      dgvRecharge.Rows(i).Cells(3).Value() = Result(i).ChargeTime
      dgvRecharge.Rows(i).Cells(4).Value() = Result(i).SysOperator
      dgvRecharge.Rows(i).Cells(5).Value() = Result(i).Status
      dgvRecharge.Rows(i).Cells(6).Value() = Result(i).IsCheck
Next
```
调整行高列宽：
```
dgvRecharge.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells          '按所有的格调整列宽
dgvRecharge.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells               '按所有的格调整行高
```
**(2)行的选择**
                 首先要设置该表只能按行选择：
![](https://img-blog.csdn.net/20160207165727459?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 
       在控件的属性里面就可以调整，然后显示数据，在数据的显示后就可以看到数据是整行选择的然后就是获得选择行的数据了，选择行的数据可以通过一个属性来获得：
```
int rows = dataGridView1.indexrows    '获得选种行的索引
string str = dataGridView1.rows[rows].cells[num].text    '获取第rows行的索引为num列的值
```
其中这样的选择和数据赋值的方法是差不多的，只要获得了关键值了之后就可以随意的操作了。
# 三、总结
        这次的数据显示和调整列宽，都不是新的知识，只是旧的知识的再利用，有些细微的改变，只要心够细，完全可以自己找出来的，数据的获得和显示可能和之前的有些不同，但是都没有关系，只要去接触，就可以得到想要的。
