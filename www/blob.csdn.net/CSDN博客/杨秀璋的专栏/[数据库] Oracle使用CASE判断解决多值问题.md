
# [数据库] Oracle使用CASE判断解决多值问题 - 杨秀璋的专栏 - CSDN博客

2016年07月20日 20:15:31[Eastmount](https://me.csdn.net/Eastmount)阅读数：8487所属专栏：[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)



这是最近在使用Oracle数据库时的一个问题，个人认为是一个非常经典的问题。假设现在有一张专业表，包括如下信息：
![](https://img-blog.csdn.net/20160720144710778)
其中表为：ZY_TAB（ZY_NAME，ZY_CODE，ZY_TYPE，ZY_TIME）。专业表中ZY_NAME表示专业名称，ZY_CODE表示专业代码，ZY_TYPE表示专业类型（包含国家特色专业和省示范专业），ZY_TIME表示设置国家特色专业或省级示范专业的时间。
例如软件工程，2004年设为省示范专业，2012年升级为国家特色专业。现在需要输出如下图所示的表格：
![](https://img-blog.csdn.net/20160720194346998)

如果使用Java后台处理，通常会将整个ZY_TAB内容读取，然后依次对专业的ZY_TYPE（优势专业）进行判断，判断主要包括三类：仅为“国家特色专业”，仅为“省示范专业”，同时为“国家特色专业”和“省示范专业”。
但是总感觉有些别扭，如果只用SQL语句进行解决，怎么处理呢？

**方法一：decode函数**
**相关知识**
首先想到的方法是使用decode函数判断。
decode(类型, '类型1', '值1', '类型2', '值2', '其它')
它的优势是可以输出自定义的值，例如：
decode(sex, 'Man', '男', 'Woman', '女', '无')
先存在下表：PERSON（NAME，SEX）表示人的姓名和性别。
![](https://img-blog.csdn.net/20160720150157500)

```python
select NAME, DECODE(SEX, 'Man', '男', 'Woman', '女', '无') as SEX
from PERSON;
```
![](https://img-blog.csdn.net/20160720150419927)
通常使用 decode(b, 0, 0, a/b) 防止a/b中分母b为0报错，它的具体含义是如果分母b为0，则SQL返回0，否则返回a/b，相当于执行除法操作。
再如重命名为自己喜欢的名称，如null自定义命名：nvl(SEX,'定义为空')，它等价于 decode(SEX, NULL, '定义为空', SEX)。
**具体操作**
这里想使用：
decode(count(ZY_TYPE), '1', '省示范专业', '2', '省示范、国家特色', '无')
```python
select ZY_NAME, 
    DECODE(count(ZY_NAME), '1', '省示范专业', '2', '省示范、国家特色专业', '无') as TYPE
from ZY_TAB GROUP BY ZY_NAME;
```
输出如下图所示结果，其**错误**是当count(ZY_NAME)=1时，“国家特色专业”也变成了“省示范专业”，所以仅仅通过判断出现的次数方法不太好。
![](https://img-blog.csdn.net/20160720151723193)

**方法二：使用case语句**
**相关知识**
例如需要判断职员的工资小于或等于2000元时，返回消息“过低”，大于或等于4000时返回消息“过高”，其余返回“正常”。
![](https://img-blog.csdn.net/20160720152857444)

这种需求通常会遇到，此时需要使用CASE WHEN来判断转换，代码如下所示：
```python
select NAME, SEX, SAL, 
    CASE WHEN SAL<=2000 THEN '过低'
         WHEN SAL>=4000 THEN '过高'
         ELSE '正常'
     END AS STATE
from PERSON
order by NAME;
```
输出如下图所示：
![](https://img-blog.csdn.net/20160720153310393)
再举个例子，下面SQL代码是统计各个学位的职工人数：
```python
SELECT COUNT(*) AS 总人数,  
    COUNT(CASE WHEN HIGHEST_DEGREE='博士'THEN 1 END) AS 博士人数,   
    COUNT(CASE WHEN HIGHEST_DEGREE='硕士'THEN 1 END) AS 硕士人数,   
    COUNT(*)-COUNT(CASE WHEN HIGHEST_DEGREE='博士'THEN 1 END)-COUNT(CASE WHEN HIGHEST_DEGREE='硕士'THEN 1 END) AS 其他学历  
FROM TEACHER;
```
COUNT(CASE WHEN HIGHEST_DEGREE='博士' THEN 1 END) AS NUM2
表示当最高学历HIGHEST_DEGREE字段为'博士'时，统计数量加1。
当然如果需要计算学院各个班级的总人口，可以采用使用下面的SQL：
COUNT(CASE WHEN DW_NAME='软件学院' THEN NUM_STU END) AS NUM2
也可以使用提到的CASE防止除法计算分母为0，ZS总数、SHSJ社会实践人数。即：
round((case when ZS!=0 then SHSJ/ZS else 0 end),3) as bl
**具体操作**
此时需要使用CASE WHEN来判断，这里使用了两次CASE WHEN，第一次是判断是“国家特色专业”或“省示范专业”，第二次是判断该专业出现的次数，如果出现两次则表示两个类型都存在。
原始数据如下图所示：
![](https://img-blog.csdn.net/20160720144710778)

SQL代码如下所示：
```python
select ZY_NAME, 
CASE WHEN COUNT(CASE WHEN ZY_TYPE='省示范专业' OR ZY_TYPE='国家特色专业' THEN 1 END)='2' 
     THEN '省示范专业; 国家特色专业' 
     WHEN COUNT(CASE WHEN ZY_TYPE='国家特色专业' THEN 1 END)='1' 
     THEN '国家特色专业'  
     WHEN COUNT(CASE WHEN ZY_TYPE='省示范专业' THEN 1 END)='1' 
     THEN '省示范专业'  
ELSE NULL END
AS TYPE 
from ZY_TAB group by ZY_NAME;
```
使用group by防止专业名称重复，输出结果如下图所示：
![](https://img-blog.csdn.net/20160720194828334)
如果需要增加专业的其他信息，如“专业代码”，如下代码所示：
```python
select ZY_NAME, ZY_CODE, 
CASE WHEN COUNT(CASE WHEN ZY_TYPE='省示范专业' OR ZY_TYPE='国家特色专业' THEN 1 END)='2' 
     THEN '省示范专业; 国家特色专业' 
     WHEN COUNT(CASE WHEN ZY_TYPE='国家特色专业' THEN 1 END)='1' 
     THEN '国家特色专业'  
     WHEN COUNT(CASE WHEN ZY_TYPE='省示范专业' THEN 1 END)='1' 
     THEN '省示范专业'  
ELSE NULL END
AS TYPE 
from ZY_TAB group by ZY_NAME, ZY_CODE;
```
group by 分组中增加ZY_CODE，输出如下图所示：
![](https://img-blog.csdn.net/20160720194346998)

**进阶篇**
假设现在存在一张专业表ZY，仅仅保存专业名称和专业代码，需要通过连接两张表ZY（专业表）和ZY_TAB（优势专业表）来统计各专业的优势专业信息，怎么处理呢？
ZY（NAME，CODE，INFO，PLACE）对应（专业名称，专业代码，信息，位置）。
![](https://img-blog.csdn.net/20160720194723426)

因为通常数据库设计中，都会设定专业表，再通过外键来关联其他的专业信息，包括优势专业、专业老师情况、专业学生情况、教学情况等等，所以通常需要通过ZY_CODE专业代码来进行关联。
```python
select NAME, CODE, 
	(select
		CASE WHEN COUNT(CASE WHEN ZY_TYPE='省示范专业' OR ZY_TYPE='国家特色专业' THEN 1 END)='2' 
     	THEN '省示范专业; 国家特色专业' 
     	WHEN COUNT(CASE WHEN ZY_TYPE='国家特色专业' THEN 1 END)='1' 
     	THEN '国家特色专业'  
     	WHEN COUNT(CASE WHEN ZY_TYPE='省示范专业' THEN 1 END)='1' 
     	THEN '省示范专业'  
	ELSE NULL END 
		from ZY_TAB 
		where ZY_TAB.ZY_CODE=ZY.CODE
	) AS TYPE, INFO, PLACE
from ZY;
```
输出如下图所示：
![](https://img-blog.csdn.net/20160720200704214)
最后希望文章对你有所帮助，主要讲述了使用DECODE函数和CASE判断多值问题，当然如果多个类型也是可以判断并多指输出的，但建议通常判断该两个类型，要么输出A，要么输出B，要么输出A和B。
还是那句话，数据库相关的知识，只有当你真正遇到这个需求的时候对你帮助才会非常大，否则你也可以把它当成简单基础知识回顾。
（By:Eastmount 2016-7-20 晚上8点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


