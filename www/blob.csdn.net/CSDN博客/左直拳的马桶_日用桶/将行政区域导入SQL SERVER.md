# 将行政区域导入SQL SERVER - 左直拳的马桶_日用桶 - CSDN博客
2013年07月11日 12:29:34[左直拳](https://me.csdn.net/leftfist)阅读数：2633
步骤如下：
**一、到国家统计局网站，找到县及县以上行政区划页面。**
我找到的是这个：[http://www.stats.gov.cn/tjbz/xzqhdm/t20130118_402867249.htm](http://www.stats.gov.cn/tjbz/xzqhdm/t20130118_402867249.htm)
然后将页面上的代码直接复制粘贴到记事本，保存为 e:\temp\region.txt。
（注意复制时，可能是受内存限制，不一定能一次性复制完全所有内容，注意确认一下。）
复制粘贴的结果，让我傻眼了：
![](https://img-blog.csdn.net/20130711115806500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
居然是邮政编码与名称各自一行的。手动改成一行？写个程序来纠正？蚊子都睡了。
**二、硬着头皮用SQL SERVER Management Studio导进去。**
导入的时候注意一点，就是要在目标表里设置一个标识列。如图：
![](https://img-blog.csdn.net/20130711121020593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**三、生成区域表**
在这里导进去的目标表，只是一个临时用的中间表：region_tmp，我真正要的，是表region。
中间表region_tmp的结构：
region_tmp
======================
id           int  identity(1,1)
cn          varchar(50)
导进去以后，数据如下：
![](https://img-blog.csdn.net/20130711121621578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而我想要的区域表region
region
=========================
id              int identity(1,1) 
code        char(6)
name       nvarchar(50)
parentId   int
所以现在需要将数据再从region_tmp ==> region。
在导入的时候，之所以要设置一个标识列，就是用来区分邮政编码和地名。在region_tmp，奇数行都是编码，偶数行则是地名，因此可以通过以下语句将编码和地名都写进region的同一行：
```
INSERT INTO [dbo].[Region]
           ([code]
           ,[name]
           ,[parentId])
SELECT a.cn,b.cn,0
	FROM region_tmp a,region_tmp b
	WHERE b.id=a.id+1
	and (a.id % 2) = 1
```
**四、修改区域表**
现在要修改字段region.parentId。就是每个地名记录要有一个父ID，在使用的时候好排序。比如广州的父ID是广东省，天河区的父ID是广州。
```
--设置市级地名的父ID
UPDATE [dbo].[region]
	SET parentid=b.id
	FROM [region],[region] b
	WHERE LEFT([region].code,2)=LEFT(b.code,2)
		AND RIGHT([region].code,4)<>'0000' AND RIGHT([region].code,2)='00'
		AND RIGHT(b.code,4)='0000';
		
--设置县级地名的父ID
UPDATE [dbo].[region]
	SET parentid=b.id
	FROM [region],[region] b
	WHERE LEFT([region].code,4)=LEFT(b.code,4)
		AND RIGHT([region].code,2)!='00'
		AND RIGHT(b.code,2)='00';
```
有些记录不符合使用习惯，修改后删掉
```
UPDATE [dbo].[region] SET parentId=c.Id
	FROM [dbo].[region],[dbo].[region] b,[dbo].[region] c
	WHERE [dbo].[region].parentId=b.id AND b.parentId=c.id
	AND b.name IN('市辖区','县') ;
	
DELETE FROM [dbo].[region] WHERE name IN('市辖区','县') ;
```

