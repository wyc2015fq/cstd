# SQLServer2005中的CTE递归查询得到一棵树 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [SQLServer2005中的CTE递归查询得到一棵树](https://www.cnblogs.com/xdp-gacl/p/3500193.html)



　　最近研究了一下CTE递归查询，感觉这个CTE递归查询蛮好用的，在网上找到了一个比较好的例子，测试例子如下

```
1 use City;
 2 go
 3 create table Tree
 4 (
 5   ID int identity(1,1) primary key not null,
 6   Name varchar(20) not null,
 7   Parent varchar(20) null
 8 )
 9 go
10 insert Tree values('大学',null)
11 insert Tree values('学院','大学')
12 insert Tree values('计算机学院','学院')
13 insert Tree values('网络工程','计算机学院')
14 insert Tree values('信息管理','计算机学院')
15 insert Tree values('电信学院','学院')
16 insert Tree values('教务处','大学')
17 insert Tree values('材料科','教务处')
18 insert Tree values('招生办','大学')
19 go
20 with CTE as
21 (
22 -->Begin 一个定位点成员
23  select ID, Name,Parent,cast(Name as nvarchar(max)) as TE,0 as Levle from Tree where Parent is null
24 -->End 
25 union all
26 -->Begin一个递归成员
27  select Tree.ID, Tree.Name,Tree.Parent,cast(replicate(' ',len(CTE.TE))+'|_'+Tree.name as nvarchar(MAX)) as TE,Levle+1 as Levle
28         from Tree inner join CTE
29         on Tree.Parent=CTE.Name
30 -->End
31 )
32 select * from CTE order by ID
33 --1.将 CTE 表达式拆分为定位点成员和递归成员。
34 --2.运行定位点成员，创建第一个调用或基准结果集 (T0)。
35 --3.运行递归成员，将 Ti 作为输入(这里只有一条记录)，将 Ti+1 作为输出。
36 --4.重复步骤 3，直到返回空集。
37 --5.返回结果集。这是对 T0 到 Tn 执行 UNION ALL 的结果。
```

上面的SQL语句再次插入一条数据：

insert Tree values('网络1班','网络工程')

     运行结果如下图：

![](http://hi.csdn.net/attachment/201011/10/0_1289396897R7x7.gif)

图1 运行结果

注意点：貌似在递归成员处所选择的字段都必须Tree表的数据，而不能是CTE结果集中的除了Tree中没有而CTE中有的字段在这里才可以引用，比如字段TE。

    首先看下，遍历的第1条记录的SQL语句：

```
1 select ID, Name,Parent,cast(Name as nvarchar(max)) as TE,0 as Levle from Tree where Parent is null
```

获取的结果为：

   Name  Parent   TE    Levle

-------------------------------------

     大学    NULL   大学     0

   递归第2次所获取的结果集合的类SQL语句为：

```
1 select Tree.ID, Tree.Name,Tree.Parent,cast(replicate(' ',len(CTE.TE))+'|_'+Tree.name as nvarchar(MAX)) as TE,Levle+1 as Levle
2 from Tree inner join
3    (select ID, Name,Parent,cast(Name as nvarchar(max)) as TE,0 as Levle from Tree where Parent is null)
4 as CTE
5 on Tree.Parent=CTE.Name
```

上面的CTE子查询的结果就是第一次递归查询的结果集，上面SQL运行结果为：

![](http://hi.csdn.net/attachment/201011/10/0_1289397808S2H0.gif)

同样的，将第二次递归查询的上面三条记录作为第三次查询的‘定位成员’：

  【这里要注意，上面的三条记录是从最后一条开始依次作为第三次递归的输入的，即第一条是ID=9的记录，接下来是7和2，关于第四次递归也类似】

   第三次递归类SQL语句

```
1 select Tree.ID, Tree.Name,Tree.Parent,cast(replicate(' ',len(CTE.TE))+'|_'+Tree.name as nvarchar(MAX)) as TE,Levle+1 as Levle
2 from Tree inner join
3 (第二次递归查询的SQL语句)as CTE
4 on Tree.Parent=CTE.Name
```

结果如下：

![](http://hi.csdn.net/attachment/201011/10/0_1289398104CkJ5.gif)

其实每次递归的类SQL可为如下所示：

```
1 select Tree.ID, Tree.Name,Tree.Parent,cast(replicate(' ',len(CTE.TE))+'|_'+Tree.name as nvarchar(MAX)) as TE,Levle+1 as Levle
2 from Tree inner join
3 (上次递归查询的结果集，仅仅是上次那一次的，而不是以前的总和结果集)
4 as CTE
5 on Tree.Parent=CTE.Name
```

第四次递归一次类推，最后所查询的结果为上面所有递归的union。

 续：在上面的SQ语句查询结果中，ID为10的记录应该要放在ID为4的后面。

 往数据表中再次添加两条记录：

insert Tree values('计科','计算机学院') insert Tree values('我','网络1班') 

再次修改上面的SQL语句：

```
1 with CTE as  
 2 (   
 3 -->Begin 一个定位点成员   
 4  select ID, Name,Parent,cast(Name as nvarchar(max)) as TE,
 5         ROW_NUMBER()over(order by getdate()) as OrderID
 6         --最关键是上面这个字段，要获取排序字段，按字符串来排序。
 7         --其中窗口函数必须要使用order by，但是不能用整型，那就用时间吧
 8         from Tree where Parent is null  
 9 -->End    
10 union all   
11 -->Begin一个递归成员   
12  select Tree.ID, Tree.Name,Tree.Parent,cast(replicate(' ',len(CTE.TE))+'|_'+Tree.name as nvarchar(MAX)) as TE,
13         CTE.OrderID*100+ROW_NUMBER()over(Order by GETDATE()) as OrderID
14         from Tree inner join CTE   
15         on Tree.Parent=CTE.Name   
16 -->End   
17 )   
18 select * from CTE
19 order by LTRIM(OrderID)--最后将这个整型数据转换为字符串型的进行排序
20 
21 --有时候整型可以比大小，字符串也可以，字符串比的大小是一位一位进行字符比较的
22 --整型+字符串==整型，只有字符串+字符串==两个字符串的并和
23 --递归查询中：第二条记录可以引用第一条记录的值
24 --动态加载记录时，同一个等级的记录识别符：RowNumber()over(order by getdate())
25 --延伸：可以动态获取某个部门下的所以子部门。也可以获取该部门上级的所以部门
26 
27 --总结：首先要拼凑出一个整型数据，然后转换为字符串，最后是进行字符串的order，而不是整型数据的order，
```

![](http://hi.csdn.net/attachment/201011/20/0_1290225874b9H7.gif)

图2 运行结果

这样，无论用户插入多少条记录都可以进行按部门，按规律进行查询。









