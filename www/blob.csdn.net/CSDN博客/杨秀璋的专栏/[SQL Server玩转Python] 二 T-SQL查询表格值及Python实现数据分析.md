# [SQL Server玩转Python] 二.T-SQL查询表格值及Python实现数据分析 - 杨秀璋的专栏 - CSDN博客





2018年11月13日 23:10:45[Eastmount](https://me.csdn.net/Eastmount)阅读数：1010
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









在开发项目过程中，更多的是通过Python访问SQL Server数据库接口，进行数据挖掘的操作；而SQL Server2016版本之后，嵌入了强大的R、Python、Machine Learning等功能，尤其是Python代码置于存储过程中，可以实现一些便捷数据分析功能。

本系列文章主要讲解SQL Server 2017实现Python数据分析的文章，同时对比两者的优劣。前一篇文章主要讲解SQL Server开发Python环境的安装过程及基本的数据分析代码实现，本文主要讲解T-SQL实现表的查询及简单的数据分析实验。基础性文章，该方面知识也较少，自己也仍在不断学习中，希望对你有所帮助。

推荐官网学习文档：[https://docs.microsoft.com/zh-cn/sql/advanced-analytics/?view=sql-server-2017](https://docs.microsoft.com/zh-cn/sql/advanced-analytics/?view=sql-server-2017)
![](https://img-blog.csdnimg.cn/2018111315172147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
PS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。

# 一. T-SQL调用Python脚本入门知识

脚本语言的基本语法如下，推荐官方文章 [sp_execute_external_script (TRANSACT-SQL)](https://docs.microsoft.com/zh-cn/sql/relational-databases/system-stored-procedures/sp-execute-external-script-transact-sql?view=sql-server-2017) 。

```python
sp_execute_external_script   
    @language = N'language',   
    @script = N'script'  
    [ , @input_data_1 = N'input_data_1' ]   
    [ , @input_data_1_name = N'input_data_1_name' ]  
    [ , @output_data_1_name = N'output_data_1_name' ]  
    [ , @parallel = 0 | 1 ]  
    [ , @params = N'@parameter_name data_type [ OUT | OUTPUT ] [ ,...n ]' ] 
    [ , @parameter1 = 'value1' [ OUT | OUTPUT ] [ ,...n ] ]
```

参数如下：
**@language = N’语言’：** 具体取决于你的 SQL Server 版本，有效的值为 R (SQL Server 2016 及更高版本)、 Python (SQL Server 2017 及更高版本) 和 Java （SQL Server 2019 预览版）。
**@script = u’脚本语言’：**  需要被执行的外部脚本，脚本指定为参数或变量的输入的外部语言脚本， 脚本是nvarchar （max）。
**@input_data_1 = N’input_data_1’：** 外部语言脚本的输入数据。
**@input_data_1_name = N’input_data_1_name’：** 可选输入名，用于表示定义的查询变量名称@input_data_1，默认名inputDataSet。注意：外部脚本变量中的数据类型取决于语言。 对于 R，则输入的变量是数据帧；对于 Python，输入必须为表格。
**@output_data_1_name = N’output_data_1name’：** 输出名，默认名为OutputDataSet。指定的变量名称中包含要返回到的数据的外部脚本 SQL Server 存储过程调用完成后， 外部脚本的变量中的数据类型取决于语言。
**@parrallel = 0 | 1 ：** 启用并行执行脚本@parallel参数为 1，此参数默认值为 0 （不能并行）。 如果@parallel = 1和输出进行流式处理直接向客户端计算机，则WITH RESULT SETS子句是必需的并且必须指定输出架构。
**@params = N’parameter_name data_type [输出] [，…n]’：** 外部脚本中使用的输入的参数声明的列表。
**@parameter1 = ‘value1：’** 有关使用外部脚本的输入参数的值的列表。
下面是T-SQL执行Python脚本代码最基本的情况，如下所示：

```python
execute sp_execute_external_script 
@language = N'Python', 
@script = N'
import math
a = 1
b = 2
c = a*b
print(a,b,c)
d = math.pi/6
print(math.sin(d))
'
```

输出结果如下所示：
![](https://img-blog.csdnimg.cn/20181111233750461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
推荐这篇文章供大家学习基础知识：[使用 T-SQL 執行 Python](https://docs.microsoft.com/zh-tw/sql/advanced-analytics/tutorials/run-python-using-t-sql?view=sql-server-2017)

接在讲述导入sklearn库进行线性回归预测的代码，如下所示：

```python
execute sp_execute_external_script 
@language = N'Python', 
@script = N'
from sklearn import linear_model      
import matplotlib.pyplot as plt     
import numpy as np
#X表示匹萨尺寸 Y表示匹萨价格
X = [[6], [8], [10], [14], [18]]
Y = [[7], [9], [13], [17.5], [18]]
print(X)
print(Y)
#回归训练
clf = linear_model.LinearRegression() 
clf.fit(X, Y)                         
res = clf.predict(np.array([12]).reshape(-1, 1))[0]
print(u"预测一张12英寸匹萨价格：$%.2f" % res)
#预测结果
X2 = [[0], [10], [14], [25]]
Y2 = clf.predict(X2)
'
```

输出结果可以看到线性回归预测的价格。
![](https://img-blog.csdnimg.cn/20181111233915963.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 二. T-SQL含查询的Python脚本

**1.查询显示所有数据库**

下面结合SQL语句查询编写Python脚本，代码如下：其中输入数据为"select 1 as Coll”，输出结果赋值为MyOutput变量，最后输出的表头定义为“ResultValue”。

```python
execute sp_execute_external_script 
@language = N'Python', 
@script = N'
MyOutput = MyInput
',
@input_data_1_name = N'MyInput',
@input_data_1 = N'SELECT 1 as Col1',
@output_data_1_name = N'MyOutput'
WITH RESULT SETS ((ResultValue int))
```

输出结果如下所示：
![](https://img-blog.csdnimg.cn/20181113211239159.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
另一种获取表格的方法如下所示：

```python
--	自定义输入的数据集变量，如：dbname
execute sp_execute_external_script 
@language = N'Python', 
@script = N'
print(dbname)
print(type(dbname))
',@input_data_1 = N'SELECT database_id,name FROM sys.databases'
,@input_data_1_name = N'dbname'
```

输出结果如下所示，包括代码及运行结果（数据库名称）。
![](https://img-blog.csdnimg.cn/20181113211038812.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.创建表格并显示查询的数据**

下面自己创建表，再查询表格中的数据。

```
create database yxz;

use yxz;

--学生表
create table Student(
	Sno varchar(10) primary key,
	Sname varchar(10) not null,
	Sex char(2),
	Sdept varchar(20),
	SBirthday datetime
);

insert into Student(Sno,Sname,Sex,Sdept,SBirthday)
values('S01','王建平','男','自动化','1996-01-12');
insert into Student(Sno,Sname,Sex,Sdept,SBirthday)
values('S02','刘华','女','自动化','1995-07-01'),
	('S03','范林军','女','计算机','1994-06-30');
insert into Student(Sno,Sname,Sex,Sdept,SBirthday)
values('S04','李伟','女','数学','1995-05-01'),
	('S05','黄烟','男','数学','1996-04-01'),
	('S06','何淳','男','数学','1995-06-30');

select * from Student;

SELECT Ages = DATEDIFF(YEAR,[SBirthday],GETDATE())
FROM [yxz].[dbo].[Student];
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20181113224708306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**R脚本**

```
EXEC sp_execute_external_script
    @language = N'R',
	@script = N'res<-quantile(InputDataSet$Ages);
                df <- data.frame(res);',
	@input_data_1 = N'SELECT DATEDIFF(YEAR,[SBirthday],GETDATE()) AS Ages
			    FROM [yxz].[dbo].[Student];',
	@output_data_1_name = N'df'
WITH RESULT SETS (("res" int not null));
```

输出结果如下所示：

```
res
1  22
2  22
3  23
4  23
5  24
```

**Python脚本**

```python
EXEC sp_execute_external_script
@language = N'Python',
@script = N'
print(InputDataSet[["Ages","Sno"]])
',
@input_data_1 = N'SELECT DATEDIFF(YEAR,[SBirthday],GETDATE()) as Ages, Sno
		FROM [yxz].[dbo].[Student];',
@output_data_1_name = N'df'
```

其中，InputDataSet表示输入数据集，需要定义到[[“Ages”, “Sno”]]中。 输出结果如下：
![](https://img-blog.csdnimg.cn/20181113225253402.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
而如果查询输出字段包含了中文，如用户名时，会提示编码utf-8错误。

```python
EXEC sp_execute_external_script
@language = N'Python',
@script = N'
print(InputDataSet["Ages","Sname","Sno"])
',
@input_data_1 = N'SELECT DATEDIFF(YEAR,[SBirthday],GETDATE()) as Ages, Sname, Sno
		FROM [yxz].[dbo].[Student];',
@output_data_1_name = N'df'
```

如下图所示，在Python中可以尝试读入文件指定utf-8编码，而嵌入SQL Server中我还不知道如何解决，尝试并百度也没找到解决方法。

Error in execution.  Check the output for more information.

UnicodeDecodeError: ‘utf-8’ codec can’t decode byte 0xcd in position 0: invalid continuation byte![](https://img-blog.csdnimg.cn/2018111322540142.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三. Python读取文件及聚类分析

接下来讲解Python读取文件并进行聚类分析的代码。输入的数据是glass玻璃数据集，如下所示：
![](https://img-blog.csdnimg.cn/20181113225916844.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
单独的Python代码如下：

```python
# -*- coding: utf-8 -*-
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
from sklearn.cluster import Birch

#获取数据集及降维
glass = pd.read_csv("glass.csv")
pca = PCA(n_components=2)  
newData = pca.fit_transform(glass)  
print newData[:4]  
L1 = [n[0] for n in newData]  
L2 = [n[1] for n in newData]
plt.rc('font', family='SimHei', size=10) #设置字体
plt.rcParams['axes.unicode_minus'] = False #负号

#聚类 类簇数=4
clf = Birch(n_clusters=4)
clf.fit(glass)
pre = clf.predict(glass)
plt.title(u"Birch聚类 n=4")  
plt.scatter(L1,L2,c=pre,marker="s",s=100)  
plt.show()
```

运行结果如下图所示：
![](https://img-blog.csdnimg.cn/20181113230334256.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
那怎么通过SQL Server嵌入Python代码实现简单的聚类分析呢？

首先通过下面代码实现读入数据操作。

```python
@language = N'Python', 
@script = N'
import pandas as pd
xl = pd.ExcelFile("C:/glass.xlsx")
df = xl.parse(xl.sheet_names[0])
print(df)
print(df.dtypes)
'
```

输出表格内容如下图所示：
![](https://img-blog.csdnimg.cn/20181113230544821.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
另一个中文数据表 glass2.xlsx 输出如下所示：
![](https://img-blog.csdnimg.cn/20181113232010612.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
数据分析部分同样增加相关代码即可，如下：

```python
execute sp_execute_external_script 
@language = N'Python', 
@script = N'
import pandas as pd
from sklearn.cluster import Birch
xl = pd.ExcelFile("C:/glass.xlsx")
df = xl.parse(xl.sheet_names[0])
clf = Birch(n_clusters=4)
clf.fit(df)
pre = clf.predict(df)
print(pre)
'
```

输出的预测结果如下所示：
![](https://img-blog.csdnimg.cn/2018111323070996.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
后续会继续补充如何将导入的数据存储至表中，如何显示中文数据，如何编写存储过程进行训练和测试。这系列文章资料比较少，作者也在一步步学习研究中，望读者海涵。

希望文章对大家有所帮助，如果有错误或不足之处，还请原谅。最近经历的事情太多，有喜有悲，关闭了朋友圈，希望通过不断学习和写文章来忘记烦劳，将忧郁转换为动力，每周学习都记录下来。

（By：Eastmount 2018-11-13 晚上12点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)





