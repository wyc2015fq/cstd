# Linq的分页 - 左直拳的马桶_日用桶 - CSDN博客
2015年02月06日 15:37:33[左直拳](https://me.csdn.net/leftfist)阅读数：2928
真有趣。
C#里面的List对象、set对象，都可以直接使用Linq（这是因为，它们都实现了接口IEnumable？），比如说：Where()、OrderBy()什么的。假如有点SQL基础的人，一看这些方法，会觉得很亲切。
在ORM、EF之前，我们操作数据库，主要是在DAL里写SQL，然后调用各种SqlHelper。现在好了，数据库与开发代码分离，上帝的归上帝，凯撒的还给凯撒，代码里面只操作各种对象了，直接的select * from 叉叉 where 叉叉 order by 叉叉没有了，代之以：
lisObj.Where(m=>m叉叉).OrderBy(m=>m.Id)
呵呵，不过还别说，linq用来就是爽。我开始习惯，并忍不住越来越喜欢了。
now，现在使用Entity Framwork，各种对象都直接使用linq，那么分页该怎么处理呢？
答案是将Skip()和Take()两个方法结合使用：
Skip((pageIndex - 1) * pageSize)：忽略当前页之前的记录
Take(pageSize)：读取指定数量的记录，也就是当前页的记录
完整示例：
```
IList<Test> list = this.DbContext.Set<Test>()
    .Where(m => m.ProjectID == projectId && m.Assigner == assigner)
    .OrderByDescending(m => m.ID)
    .Skip((pageIndex - 1) * pageSize)
    .Take(pageSize)
    .ToList();
```
