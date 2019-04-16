# [SQL Server玩转Python] 一.安装环境及T-SQL调用python脚本 - 杨秀璋的专栏 - CSDN博客





2018年11月11日 23:41:41[Eastmount](https://me.csdn.net/Eastmount)阅读数：1054
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









在开发项目过程中，更多的是通过Python访问SQL Server数据库接口，进行数据挖掘的操作；而SQL Server2016版本之后，嵌入了强大的R、Python、Machine Learning等功能，尤其是Python代码置于存储过程中，可以实现一些便捷数据分析功能。

本系列文章主要讲解SQL Server 2017实现Python数据分析的文章，同时对比两者的优劣。第一篇文章主要讲解SQL Server开发Python环境的安装过程及基本的数据分析代码实现。基础性文章，自己也在不断学习中，希望对你有所帮助。

PS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。

# 一. 安装SQL Server

**(一) 安装SQL Server 2017**

本文安装的软件为：cn_sql_server_2017_developer_x64_dvd_11296175.iso

下载地址：

1.选择“全新 SQL Server 独立安装或向现有安装添加功能”。
![](https://img-blog.csdnimg.cn/20181111222434667.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
2.安装所需功能，注意机器学习服务、R、Python均需要安装。
![](https://img-blog.csdnimg.cn/20181111222750951.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
3.选择默认实例“MSSQLSERVER”。
![](https://img-blog.csdnimg.cn/20181111222919573.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
4.选择混合性模型，包括SQL Server身份验证（sa）和Windows身份验证，同时添加当前用户。
![](https://img-blog.csdnimg.cn/20181111223344245.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
5.安装。
![](https://img-blog.csdnimg.cn/20181111223514392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
安装成功之后，你会发现仍然不能编写SQL Server代码，这是因为还需要安装SQL Server管理工具。


**(二) 安装SQL Server Management Studio**

微软官方下载地址：[https://docs.microsoft.com/en-us/sql/ssms/download-sql-server-management-studio-ssms?view=sql-server-2017](https://docs.microsoft.com/en-us/sql/ssms/download-sql-server-management-studio-ssms?view=sql-server-2017)

作者上传下载地址：

安装过程如下图所示。
![](https://img-blog.csdnimg.cn/2018111122414899.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181111224247788.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
安装成功之后即可使用SQL Server编写代码了。
![](https://img-blog.csdnimg.cn/20181111224259117.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 二. 配置Python和R开发环境

安装成功之后我们新建一个数据库test01，可以发现SQL语句可以编写，但编写Python或R代码仍然会报错。

推荐文章：
[https://docs.microsoft.com/zh-tw/sql/advanced-analytics/tutorials/run-python-using-t-sql?view=sql-server-2017](https://docs.microsoft.com/zh-tw/sql/advanced-analytics/tutorials/run-python-using-t-sql?view=sql-server-2017)

解决方法：
[http://www.cnblogs.com/OpenCoder/p/7090370.html](http://www.cnblogs.com/OpenCoder/p/7090370.html)
[http://www.kodyaz.com/t-sql/enable-external-script-on-sql-server-for-r-python.aspx](http://www.kodyaz.com/t-sql/enable-external-script-on-sql-server-for-r-python.aspx)![](https://img-blog.csdnimg.cn/20181111224854759.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
1.查看Python是否已经启用，查看SQL Server环境下是否有Python可执行文件。如：C:\Program Files\Microsoft SQL Server\MSSQL14.MSSQLSERVER\PYTHON_SERVICES。

注意，Scripts文件夹下可以通过pip安装其他Python库。
![](https://img-blog.csdnimg.cn/20181111225408494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
2.检查是否已启用外部指令，代码如下：

```
--查看外部指令
sp_configure 'external scripts enabled'
--启用外部脚本，设置config_value为1
EXEC sp_configure  'external scripts enabled', 1
RECONFIGURE WITH OVERRIDE
```

如果run_value为1，则机器学习功能已经安装成功并且可供使用。否则设置config_value为1。
![](https://img-blog.csdnimg.cn/20181111225043358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
3.此时如果执行Python代码，可能会提示错误“此 SQL Server 实例已禁用 sp_execute_external_script，请使用 sp_configure 的已启用的外部脚本对其进行启用。” 这表示run_value仍然为0，此时需要开启服务。开启方法如下：
**1) 开启SQL Server实例数据库引擎服务**
**2) 开启SQL Server实例Lanuchpad服务**

注意执行完后要重启数据库引擎服务和SQL Server Lanuchpad服务才会正式生效：![](https://img-blog.csdnimg.cn/20181111230546178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
4.重启电脑和数据库引擎服务。
![](https://img-blog.csdnimg.cn/20181111231650450.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
5.添加最简单的Python和R语言代码，并执行。
**Pyhton代码**

```python
EXEC sp_execute_external_script @language = N'Python', 
@script = N'print(3+4)'
```

运行结果如下：
![](https://img-blog.csdnimg.cn/20181111232305421.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**R代码**

```
exec sp_execute_external_script  @language =N'R',   
 @script=N'OutputDataSet<-InputDataSet',     
 @input_data_1 =N'select 1 as hello'   
 with result sets (([hello] int not null));   
 go
```

运行结果如下：
![](https://img-blog.csdnimg.cn/20181111232403154.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
# 三. T-SQL调用Python数据分析入门

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

推荐这篇文章供大家学习基础知识：[使用 T-SQL 執行 Python](https://docs.microsoft.com/zh-tw/sql/advanced-analytics/tutorials/run-python-using-t-sql?view=sql-server-2017)

**1.程序一 调用包**

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
**2.程序二 回归预测**

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
后续文章将结合存储过程详细讲解SQL Server玩转Python的过程，比如这篇文章。
[https://docs.microsoft.com/zh-cn/sql/advanced-analytics/tutorials/sqldev-py5-train-and-save-a-model-using-t-sql?view=sql-server-2017](https://docs.microsoft.com/zh-cn/sql/advanced-analytics/tutorials/sqldev-py5-train-and-save-a-model-using-t-sql?view=sql-server-2017)

希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。最近经历的事情太多，有喜有悲，关闭了朋友圈，希望通过不断学习和写文章来忘记烦劳，将忧郁转换为动力，每周学习都记录下来。

（By：Eastmount 2018-11-12 晚上12点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)





