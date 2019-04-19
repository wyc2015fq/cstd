# fluent nhibernate映射的数值类型问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年08月04日 20:41:00[左直拳](https://me.csdn.net/leftfist)阅读数：397
fluent nhibernate中，数值类型设置不当，就可能会引发一些意想不到错误。
一、引发映射错误 
比如，oracle数据库中，字段ID类型是number，结果用codesmith生成代码，实体类中自动写成decimal
`public virtual Decimal ID { get;set;}`
结果在映射中这样写，运行过程中会引发错误：
`Id(s => s.ID).GeneratedBy.Sequence("SEQ_RPYEAR");`
究其原因，decimal类型可能会被解释成含小数的实数，因此系统认为不适合使用sequence，因而报错。将实体类中的decimal改为int，错误得以修正。
二、数值类型不符导致关联失败 
还是那个decimal问题。
```
[Serializable]
    public class RpYear  
    {
        public virtual int ID{get; set;}
        public virtual decimal RPCATEGORYID{get; set;}
        public virtual RpCategory RpCategory { get; set; }
    }
    [Serializable]
    public class RpCategory
    {
        public virtual int? ID { get; set; }
    }
```
有映射
```
public RpYearMap()
        {
            Table("Rpyear");
            Id(s => s.ID).GeneratedBy.Sequence("SEQ_RPYEAR");
            Map(s => s.RPCATEGORYID);
            References<RpCategory>(s => s.RpCategory).Column("RPCATEGORYID").Not.Insert().Not.Update();
        }
```
结果实体类 RpYear 的属性RpYear.RpCategory永远是空的。究其原因，是因为RpYear.RPCATEGORYID 是decimal类型，而RpCategory.ID是int，对应不上。同理将RpYear.RPCATEGORYID改为int，问题得以修正。
