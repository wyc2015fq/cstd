# Fluently NHibernate 插入CLOB字段 - 左直拳的马桶_日用桶 - CSDN博客
2016年07月19日 19:19:36[左直拳](https://me.csdn.net/leftfist)阅读数：2531
`ORA-01461: can bind a LONG value only for insert into a LONG column`
插入oracle某表时报的错。
查来查去，是插入的某个字段值超长。怎么会超长呢？一个CLOB类型的字段，号称有4G容量的。
表设计： 
![这里写图片描述](https://img-blog.csdn.net/20160719190653669)
我用的是Fluently NHibernate，for .net。实体类及映射如下：
```
public class CatchResultContent
    {
        public virtual int CatchResultContentId { get; set; }
        public virtual int CatchResultId { get; set; }
        public virtual string Content { get; set; }
    }
    public CatchResultContentMap()
    {
        Table("CatchResultContent"); 
        Id(s => s.CatchResultContentId).GeneratedBy.Sequence("SEQ_CatchResultContent"); 
        Map(s => s.CatchResultId);
        Map(s => s.Content);
    }
```
真正到数据库执行的是如下语句：
`INSERT INTO CatchResultContent (CatchResultId, Content, CatchResultContentId) VALUES (?,?,?);`
可能是这条SQL语句中，Content作为字符串输入，整条SQL超过4000，所以超长了？不清楚。
后来是酱紫搞定的。关键在映射这里：
```
public CatchResultContentMap()
    {
。。。
        Map(s => s.Content).CustomSqlType("Clob").CustomType("StringClob");
    }
```
有老外给出的资料说是这样：
`Map(s => s.Content).CustomSqlType("Clob");`
其实还不够的。
======================================= 
以上答案，似乎只能在 FluentNHibernate 1.3 + NHibernate4.0 里受支持，换成FluentNHibernate2.0或以上，就无效了，本人还不知道如何解决。
