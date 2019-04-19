# nhibernate的关系 - 左直拳的马桶_日用桶 - CSDN博客
2016年11月06日 16:39:26[左直拳](https://me.csdn.net/leftfist)阅读数：640标签：[nhibernate](https://so.csdn.net/so/search/s.do?q=nhibernate&t=blog)
个人分类：[.NET																[Java																[oracle																[nhibernate](https://blog.csdn.net/leftfist/article/category/6387144)](https://blog.csdn.net/leftfist/article/category/5734215)](https://blog.csdn.net/leftfist/article/category/2412493)](https://blog.csdn.net/leftfist/article/category/94497)
用nhibernate，觉得比较难把握的是其中表间的关系。
我用的是Fluently Nhibernate，直接用代码，而不是XML来书写代码与数据表的映射。其中表间关系有3种：
> 
1、References（关联，可用于多对一，一对一）
2、HasMany（一对多）
3、HasOne（一对一）
**1、References（多对一）**
用于从表映射主表。常与主表映射从表的HasMany一起使用，可在同一事务中，让主表的ID和从表关联主表的ID一起生成。
比如说，主表的ID是个自增的字段，或者应用序列号（sequence）的字段。然后从表有个相应的字段关联了主表的ID。很明显，要先获得这个主表ID的值，从表这个关联ID才能赋值。如果是分步骤、分开事务的话，好像太琐碎了点。现在可以这样：
```
/*实体类*/
public class Master
{//主表
	public virtual int ID{ get; set; }
	public virtual IList<Slave> Slaves { get; set; }
}
public class Slave
{//从表
	public virtual int ID { get; set; }
	public virtual int MasterID { get; set; }
	public virtual Master Master { get; set; }
}	
/*映射*/
public class MasterMap : ClassMap<Master>
{//主表
	public MasterMap()
	{
		Table("Master");
		Id(s => s.ID).GeneratedBy.Sequence("SEQ_Master");
		HasMany<Slave>(s => s.Slaves)
			.KeyColumn("MasterID") //指的是关联表的关联字段（自动用本表的主键字去关联对方）
			.Cascade.All()
			.LazyLoad();
	}
}
public class  SlaveMap : ClassMap<Slave>
{//从表
	public SlaveMap()
	{
		Table("Slave");
		Id(s => s.ID).GeneratedBy.Sequence("SEQ_Slave");
		//Map(s => s.MasterID);注意这个属性不要再映射，否则插入时会报数组越界错误，可能是导致字段重复插入
		References<Master>(s => s.Master).Not.LazyLoad().Column("MasterID");//本表的字段，关联主表的关键字
	}
}
/*数据库操作Insert*/
Master master = new Master();
IList<Slave> lisSlave = new List<Slave>()
{
	new Slave(){ Master = master},//你中有我
	new Slave(){ Master = master},
	new Slave(){ Master = master}
};
master.Slaves = lisSlave;//我中有你
masterService.Add(master);//插入到数据库
```
这样子之后，从表里的字段MasterID都已自动赋上了正确的值。不必先插入主表记录，获得其ID，然后再手动赋给从表对象这么麻烦。
PropertyRef
在关系References 和 HasMany里都有这个属性。注意是实体类里的属性名，而不是字段名。
```
property-ref：指定关联类的一个属性，这个属性将会和本外键相对应。如果没有指定，会使用对方关联类的主键（可选）．property-ref属性只应该用来对付老旧的数据库系统，可能出现外键指向对方关联表的是个非主键字段（但是应该是一个惟一关键字）的情况。这是一种十分丑陋的关系模型。比如说，假设Product类有一个惟一的序列号，它并不是主键；
```
**2、HasMany（一对多）**
参考1
**3、HasOne（一对一）**
没啥心得，一般用References就够用了。
示例：
实体类：
```
[Serializable]
	public partial class Task_Dounit  
    {
		public virtual int ID{get; set;}
        public virtual string UNIT_NAME { get; set; }
		public virtual long TASK_ID{get; set;}
		public virtual Task TASK {get; set;}
    }
```
映射：
```
public Task_DounitMap()
        {
            Table($"Task_Dounit");
            Id(s => s.ID).GeneratedBy.Sequence($"SEQ_Task_DoUnit");
            Map(s => s.TASK_ID);
            References(s => s.TASK).Column("TASK_ID")
                .LazyLoad() 			//延迟加载
				.NotFound.Ignore()	//左对齐？
                .Not.Insert()				//不参与新增
                .Not.Update();			//不参与修改
        }
```
这里面不参与新增和修改很重要，因为我们只是想拿到相关对象的信息而已，并不想改变什么。
