# nhibernate一表对应多个实体类问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年09月28日 14:17:54[左直拳](https://me.csdn.net/leftfist)阅读数：241
应用了NHIBERNATE的web页面忽然报错：未明确定义列。
将NHIBERNATE生成的SQL语句拿出来直接运行，发现里面有两个字段的名字一样，类似：
```
select row_.* from 
( select meta0_.ID as ID6_, meta0_.NAME as NAME6_, 
  meta0_.PARENTID as PARENTID6_,meta0_.ParentID as ParentID6_  --这两个字段名字一样！！！
  from work.Meta meta0_ where meta0_.CATEGORY=21 order by meta0_.ID asc 
) row_ 
where rownum <=20
```
为何出现有名字一样的情况？原因在于实体类里，有一个指向自身的属性：
实体类
```
[Serializable]
    public partial class Meta
    {
		public virtual int ID { get; set; }
        public virtual string NAME { get; set; }
        public virtual int PARENTID { get; set; }
        public virtual Meta Parent { get; set; }//这是一个树形的结构，父对象parent也是同一类型。换言之，这个属性指向了自己
    }
```
映射
```
public partial class MetaMap : ClassMap<Meta>
    {
        public MetaMap()
        {
            Table("Meta");
            Id(s => s.ID);
            Map(s => s.NAME);
            Map(s => s.PARENTID);
            References(s => s.Parent).Column("ParentID")
                .LazyLoad()
                .NotFound.Ignore();
        }
    }
```
针对这种情况，有两种处理办法。
**办法一：**
在映射里，将parentId的映射取消，如果要访问这个字段，就要用Parent.ID来代替。问题是，并非所有记录都有父对象，最顶层的节点是没有父对象的。
```
public partial class MetaMap : ClassMap<Meta>
    {
        public MetaMap()
        {
            Table("Meta");
            Id(s => s.ID);
            Map(s => s.NAME);
            //Map(s => s.PARENTID);//取消这个映射，要访问此属性，用Parent.ID来代替
            References(s => s.Parent).Column("ParentID")
                .LazyLoad()
                .NotFound.Ignore();
        }
    }
```
**办法二：**
声明一个新的实体类MetaPro，避免指向自身
```
实体类一分为二
    [Serializable]
    public partial class Meta
    {
		public virtual int ID { get; set; }
        public virtual string NAME { get; set; }
        public virtual int PARENTID { get; set; }
    }
    
    [Serializable]
    public partial class MetaPro
    {
		public virtual int ID { get; set; }
        public virtual string NAME { get; set; }
        public virtual int PARENTID { get; set; }
        public virtual Meta Parent { get; set; }//指向的是Meta，而不是自己
    }
```
相应的映射
```
public partial class MetaMap : ClassMap<Meta>
    {
        public MetaMap()
        {
            Table("Meta");
            Id(s => s.ID);
            Map(s => s.NAME);
            Map(s => s.PARENTID);
        }
    }
    
   public partial class MetaProMap : ClassMap<MetaPro>
    {
        public MetaProMap()
        {
            Table("Meta");
            Id(s => s.ID);
            Map(s => s.NAME);
            Map(s => s.PARENTID);
            References(s => s.Parent).Column("ParentID")
                .LazyLoad()
                .NotFound.Ignore();
        }
    }
```
注意实体类之间不能继承。原本很自然想到，实体类应该这样定义：
```
//根本行不通
    [Serializable]
    public partial class Meta
    {
		public virtual int ID { get; set; }
        public virtual string NAME { get; set; }
        public virtual int PARENTID { get; set; }
    }
    
    [Serializable]
    public partial class MetaPro ： Meta
    {
        public virtual Meta Parent { get; set; }//指向的是Meta，而不是自己
    }
```
但出现的结果却是，原本使用meta的地方，系统全部自动用到了MetaPro。为何如此，令人困惑，这个要看nhibernate自己的解释机制。
