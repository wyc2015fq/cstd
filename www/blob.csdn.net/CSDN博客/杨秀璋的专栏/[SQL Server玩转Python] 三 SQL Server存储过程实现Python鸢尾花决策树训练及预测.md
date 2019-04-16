# [SQL Server玩转Python] 三.SQL Server存储过程实现Python鸢尾花决策树训练及预测 - 杨秀璋的专栏 - CSDN博客





2018年11月14日 16:30:45[Eastmount](https://me.csdn.net/Eastmount)阅读数：278标签：[SQL Server																[Python																[数据分析																[存储过程																[鸢尾花](https://so.csdn.net/so/search/s.do?q=鸢尾花&t=blog)
个人分类：[SQL Server+Python																[Python数据挖掘课程																[机器学习](https://blog.csdn.net/Eastmount/article/category/2812423)
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)](https://blog.csdn.net/Eastmount/article/category/6423551)




在开发项目过程中，更多的是通过Python访问SQL Server数据库接口，进行数据挖掘的操作；而SQL Server2016版本之后，嵌入了强大的R、Python、Machine Learning等功能，尤其是Python代码置于存储过程中，可以实现一些数据分析功能。

本系列文章主要讲解SQL Server 2017实现Python数据分析的文章，同时对比两者的优劣。前两篇文章主要讲解SQL Server开发Python环境的安装过程，T-SQL实现表的查询及简单的数据分析实验。这篇文章通过存储过程实现Python鸢尾花数据分析，将训练和预测分离进行实验。本文是基础性文章，该方面知识较少，自己也仍在不断学习中，希望对你有所帮助。

前文：

本文主要参考 heidi steen、ilprod 大神的文章，强烈推荐大家学习。地址如下：
[创建、 定型和 SQL Server 中使用存储过程中使用 Python 模型](https://docs.microsoft.com/zh-cn/sql/advanced-analytics/tutorials/train-score-using-python-in-tsql?view=sql-server-2017)
[鸢尾花演示数据的 SQL Server 中的 Python 和 R 教程](https://docs.microsoft.com/zh-cn/sql/advanced-analytics/tutorials/demo-data-iris-in-sql?view=sql-server-2017)
PS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。

# 一. T-SQL创建存储过程

Python脚本代码嵌入到存储过程中将有效地提升数据分析的效率，通常包括两个存储过程，一个用于数据的训练，另一个用于数据的预测。

**1.创建数据库及表**

创建数据库名叫鸢尾花dbiris，并使用该数据库。

```
--创建数据库
CREATE DATABASE dbiris
GO
USE dbiris
GO
```

创建一张表为iris_data用于存储数据，该数据将通过sklearn包导入。其中DROP…IF语句，通常用来避免创建表的时候，已经存在重复的表，故先删除再进行创建。

```
DROP TABLE IF EXISTS iris_data;
GO
CREATE TABLE iris_data (
  id INT NOT NULL IDENTITY PRIMARY KEY
  , "Sepal.Length" FLOAT NOT NULL, "Sepal.Width" FLOAT NOT NULL
  , "Petal.Length" FLOAT NOT NULL, "Petal.Width" FLOAT NOT NULL
  , "Species" VARCHAR(100) NOT NULL, "SpeciesId" INT NOT NULL
);
```

创建第二张表用于存储训练的模型。若要保存在 SQL Server 中的 Python （或 R） 模型，它们必须序列化和存储的列中的类型varbinary （max）。

```
--创建表 存储训练模型
DROP TABLE IF EXISTS iris_models;
GO
CREATE TABLE iris_models (
  model_name VARCHAR(50) NOT NULL DEFAULT('default model') PRIMARY KEY,
  model VARBINARY(MAX) NOT NULL
);
GO
```

对应创建如下图所示：
![](https://img-blog.csdnimg.cn/20181114153500211.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
除了模型内容中，通常情况下，你将还添加其他有用的元数据，例如模型的名称、 日期进行训练，源算法和参数，源数据列等。 现在我们将简单地说，并使用只是模型名称。

**2.SQL Server执行Python脚本过程**

首先，如果只想简单的通过调用 sp_execute_external_script 获取sklearn库中的鸢尾花数据，代码如下：

```python
--简单脚本显示鸢尾花数据
EXEC sp_execute_external_script @language = N'Python', 
@script = N'
from sklearn import datasets
iris = datasets.load_iris()
iris_data = pandas.DataFrame(iris.data)
iris_data["Species"] = pandas.Categorical.from_codes(iris.target, iris.target_names)
iris_data["SpeciesId"] = iris.target
',
@input_data_1 = N'', 
@output_data_1_name = N'iris_data'
WITH RESULT SETS (("Sepal.Length" float not null, "Sepal.Width" float not null,
    "Petal.Length" float not null, "Petal.Width" float not null, 
	"Species" varchar(100) not null, "SpeciesId" int not null));
```

对应输出的结果如下所示：
![](https://img-blog.csdnimg.cn/20181114154225498.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
Python执行的代码如下所示：
![](https://img-blog.csdnimg.cn/20181114154548885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.创建存储过程获取相关的数据**

创建存储过程如下所示：

```
--创建存储过程 get_iris_dataset
CREATE PROCEDURE get_iris_dataset
AS
BEGIN
EXEC sp_execute_external_script 
@language = N'Python', 
@script = N'
from sklearn import datasets
iris = datasets.load_iris()
iris_data = pandas.DataFrame(iris.data)
iris_data["Species"] = pandas.Categorical.from_codes(iris.target, iris.target_names)
iris_data["SpeciesId"] = iris.target
', 
@input_data_1 = N'', 
@output_data_1_name = N'iris_data'
WITH RESULT SETS (("Sepal.Length" float not null, "Sepal.Width" float not null, "Petal.Length" float not null, "Petal.Width" float not null, "Species" varchar(100) not null, "SpeciesId" int not null));
END;
GO
```

其中，输出值为Python脚本中定义的iris_data变量，将其值用 WITH RESULT SETS 赋值至对应的字段中。

**4.执行存储过程插入数据**

运行存储过程并向指定的表 iris_data 中写入数据。

```
--运行存储过程插入数据至表iris_data中
INSERT INTO iris_data ("Sepal.Length", "Sepal.Width", "Petal.Length", "Petal.Width", "Species", "SpeciesId")
EXEC dbo.get_iris_dataset;
```

查询及显示的结果如下所示：
![](https://img-blog.csdnimg.cn/20181114155335745.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 二. T-SQL存储过程实现鸢尾花训练及预测

上面的存储过程主要是将数据插入至表格中，接下来的两个存储过程将实现数据的训练及预测。

**1.创建决策树训练模型存储过程**

代码需要调用pickle库实现序列化模型，将从表iris_data中将0-4列数据进行训练。SQL语句对应获取五个值，分别为：花萼长度、花萼宽度、花瓣长度、花瓣宽度（训练数据），花所属分类（训练类标）。

select “Sepal.Length”, “Sepal.Width”, “Petal.Length”, “Petal.Width”, “SpeciesId” from iris_data;

参考：[[Python数据挖掘课程] 四.决策树DTC数据分析及鸢尾数据集分析](https://blog.csdn.net/Eastmount/article/details/52820400)![](https://img-blog.csdnimg.cn/2018111416025635.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
完整代码如下：

```python
--创建鸢尾花训练的存储过程
CREATE PROCEDURE generate_iris_model (@trained_model varbinary(max) OUTPUT)
AS
BEGIN
EXEC sp_execute_external_script @language = N'Python',
@script = N'
import pickle
from sklearn.tree import DecisionTreeClassifier
DTC = DecisionTreeClassifier()
trained_model = pickle.dumps(DTC.fit(iris_data[[0,1,2,3]], iris_data[[4]]))
'
, @input_data_1 = N'select "Sepal.Length", "Sepal.Width", "Petal.Length", "Petal.Width", "SpeciesId" from iris_data'
, @input_data_1_name = N'iris_data'
, @params = N'@trained_model varbinary(max) OUTPUT'
, @trained_model = @trained_model OUTPUT;
END;
GO
```

输出结果如下所示：
![](https://img-blog.csdnimg.cn/20181114160644877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.执行存储过程将模型插入数据库iris_models表中**

（参考heidi steen、ilprod 大神的文章）

在此步骤中，执行该存储过程以运行嵌入的代码，创建作为输出的序列化训练模型。 序列化为字节流和存储在数据库表中的 varbinary （max） 列中以供重复使用 SQL Server 中的存储模型。 一旦创建、 训练、 序列化，并保存到数据库模型，它可以调用其他过程或通过预测 T-SQL评分工作负荷中的函数。

```
--执行存储过程创建模型
DECLARE @model varbinary(max);
DECLARE @new_model_name varchar(50)
SET @new_model_name = 'Decision Tree Classifier'
SELECT @new_model_name 
EXEC generate_iris_model @model OUTPUT;
DELETE iris_models WHERE model_name = @new_model_name;
INSERT INTO iris_models (model_name, model) values(@new_model_name, @model);
GO
```

此脚本先删除现有的同名 (“Decision Tree Classifier”) 模型，以容纳创建的新模型。模型存储在名为的表iris_models中，运行结果如下所示：
![](https://img-blog.csdnimg.cn/20181114161414461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
使用 select * from iris_models 可以看到模型的名称及对应的模型。
![](https://img-blog.csdnimg.cn/20181114161740836.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.创建预测的存储过程**

运行以下代码以创建存储的过程执行评分。 在运行时，此过程将加载二进制模型，使用列[1,2,3,4]作为输入，列[0,5,6]作为输出。

```python
--创建决策树预测的存储过程
CREATE PROCEDURE predict_species (@model varchar(100))
AS
BEGIN
DECLARE @nb_model varbinary(max) = (SELECT model FROM iris_models WHERE model_name = @model);
EXEC sp_execute_external_script 
@language = N'Python', 
@script = N'
import pickle
irismodel = pickle.loads(nb_model)
species_pred = irismodel.predict(iris_data[[1,2,3,4]])
iris_data["PredictedSpecies"] = species_pred
OutputDataSet = iris_data[[0,5,6]] 
print(OutputDataSet)
'
, @input_data_1 = N'select id, "Sepal.Length", "Sepal.Width", 
                  "Petal.Length", "Petal.Width", "SpeciesId" from iris_data'
, @input_data_1_name = N'iris_data'
, @params = N'@nb_model varbinary(max)'
, @nb_model = @nb_model
WITH RESULT SETS ( ("id" int, "SpeciesId" int, "SpeciesId.Predicted" int));
END;
GO
```

代码中，@input_data_1 作为输入数据，为select查询；@input_data_1_name为对应数据的名称，可以供Python脚本调用；参数 nb_model 为输入的模型，对应的为存储过程输入的变量。

DECLARE @nb_model varbinary(max) = (SELECT model FROM iris_models WHERE model_name = @model);

输出结果包括 id（数据编号）、SpeciesId（真实类标）、SpeciesId.Predicted（预测类标）。

```python
--执行存储过程
EXEC predict_species 'Decision Tree Classifier';
GO
```

运行结果如下图所示：
![](https://img-blog.csdnimg.cn/20181114162716388.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
运行存储的过程时，它将返回 Python data.frame。 T-SQL 的这行指定返回的结果的架构： WITH RESULT SETS ( (“id” int, “SpeciesId” int, “SpeciesId.Predicted” int));。 可以将结果插入到一个新表，或返回到应用程序。

写到这里，“SQL Server存储过程实现Python鸢尾花决策树训练及预测” 已经讲解完毕，它的优势是将训练和预测分开，并嵌入存储过程中，同时可以撰写多个训练模型，进行分别调用。

希望文章对大家有所帮助，如果有错误或不足之处，还请原谅。最近经历的事情太多，有喜有悲，关闭了朋友圈，希望通过不断学习和写文章来忘记烦劳，将忧郁转换为动力，每周学习都记录下来。

（By：Eastmount 2018-11-14 下午5点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)](https://blog.csdn.net/Eastmount/article/category/8335156)](https://so.csdn.net/so/search/s.do?q=存储过程&t=blog)](https://so.csdn.net/so/search/s.do?q=数据分析&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)](https://so.csdn.net/so/search/s.do?q=SQL Server&t=blog)




