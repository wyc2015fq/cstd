# MEF实现设计上的“松耦合”（2） - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4760740.html)
前言：前篇 [C#进阶系列——MEF实现设计上的“松耦合”（一）](http://blog.jobbole.com/99221/)介绍了下MEF的基础用法，让我们对MEF有了一个抽象的认识。当然MEF的用法可能不限于此，比如MEF的目录服务、目录筛选、重组部件等高级应用在这里就不做过多讲解，因为博主觉得这些用法只有在某些特定的环境下面才会用到，着实不太普遍，感觉没有钻下去的必要。如果你有兴趣也可以去了解下。这篇打算将MEF和仓储模式结合起来谈谈MEF在项目中的使用。
1、仓储模式：也叫Repository模式。Repository是一个独立的层，介于领域层与数据映射层（数据访问层）之间。它的存在让领域层感觉不到数据访问层的存在，它提供一个类似集合的接口提供给领域层进行领域对象的访问。Repository是仓库管理员，领域层需要什么东西只需告诉仓库管理员，由仓库管理员把东西拿给它，并不需要知道东西实际放在哪。Repository模式一般是用来封装数据访问层的，这也就是为什么很多地方看到说的什么“数据仓储”，大概就是这个意思。Repository模式并不是本文的重点，这里就不再展开，后面会单独分享这块。
关于仓储模式有以下几点需要注意：
（1）Repository模式是架构模式，在设计架构时，才有参考价值；
（2）Repository模式使用的意义：一是隔离业务逻辑层和底层数据访问层，保证数据出入口的唯一性；二是Repository模式针对聚合根设计的，而并不是针对表和实体设计的，换句话说，使用Repository是为了实现内聚，前端只负责向Repository请求数据即可，而不用关心数据的具体来源；
（3）Repository模式实际用途：更换、升级ORM引擎，不影响业务逻辑；
上面这些东西写得有点官方。博主的理解是，仓储模式就是对数据访问层（或者叫数据映射层）做了一层包装，每一次前端需要查询什么数据或者提交什么数据的时候，都是通过仓储对象Repository去操作的，前端基本上感觉不到数据访问层的存在。这样说你有没有好理解一点呢？没有？好吧，我们来看Demo。
2、MEF在仓储模式上面的应用：由于框架使用的是EF，所以这里也用EF结合仓储模式进行讲解。为了省略Repository模式的复杂结构，我们仅仅通过仓储的Save方法来说明。
IRepository接口以及实现代码：

C#
```
public interface IRepository<T>
     where T : BaseEntity
    {
        T Save(T entitiy);
    }
```


C#
```
[Export(typeof(IRepository<BaseEntity>))]
    public abstract class Repository<T> : IRepository<T> where T : BaseEntity
    {
        //工作单元
        [Import]
        protected IUnitOfWork context { set; get; }
        private IDbSet<T> _entities;
        //注册MEF
        public Repository()
        {
            Register.regisgter().ComposeParts(this);
        }
        public virtual T Save(T entitiy)
        {
            if (entitiy == null) throw new ArgumentException("entitiy nul");
            context.Save<T>(entitiy);
            return entitiy;
        }
    }
    public static class Register
    {
        public static CompositionContainer regisgter()
        {
            var catalog = new AssemblyCatalog(Assembly.GetExecutingAssembly());
            var container = new CompositionContainer(catalog);
            return container;
        }
    }
```
BaseEntity是一个EF实体的公共基类，定义EF实体必须要遵循的约束。
IUnitOfWork工作单元接口以及实现

C#
```
public interface IUnitOfWork : IDisposable
    {
        int Commit();
        void Rollback();
        void Save<T>(T entity) where T : BaseEntity;
    }
```


C#
```
/// <summary>
        /// 单元操作实现
        /// </summary>
　　　　 [Export(typeof(IUnitOfWork))]
        public abstract class UnitOfWorkContextBase : IUnitOfWork
        {
            [ImportMany]
            protected abstract IEnumerable<DbContext> Contexts { get; }
            protected abstract DbContext Cur_context { get; set; }
            public UnitOfWorkContextBase()
            {
　　　　　　　　　 Register.regisgter().ComposeParts(this);
                if (Contexts.Count() <= 0)
                {
                    throw new Exception();
                }
                Cur_context = Contexts.FirstOrDefault();
            }
            /// <summary>
            ///     获取 当前单元操作是否已被提交
            /// </summary>
            public bool IsCommitted { get; private set; }
            /// <summary>
            ///     提交当前单元操作的结果
            /// </summary>
            /// <returns></returns>
            public int Commit()
            {
                if (IsCommitted)
                {
                    return 0;
                }
                try
                {
                    int result = Cur_context.SaveChanges();
                    IsCommitted = true;
                    return result;
                }
                catch (DbUpdateException e)
                {
                    if (e.InnerException != null && e.InnerException.InnerException is SqlException)
                    {
                        SqlException sqlEx = e.InnerException.InnerException as SqlException;
                        string msg = DataHelper.GetSqlExceptionMessage(sqlEx.Number);
                        throw PublicHelper.ThrowDataAccessException("提交数据更新时发生异常：" + msg, sqlEx);
                    }
                    throw;
                }
            }
            /// <summary>
            /// 把当前单元操作回滚成未提交状态
            /// </summary>
            public void Rollback()
            {
                IsCommitted = false;
            }
            public void Dispose()
            {
                if (!IsCommitted)
                {
                    Commit();
                }
                Cur_context.Dispose();
            }
            public void Save<T>(T entity) where T : T
            {
                Cur_context.SaveChanges();
            }
        }
```
既然这里使用了ImportMany，那么肯定有一个地方需要Export。我们使用EF新建一个edmx文件，在生成的上下文对象上面加上Export

C#
```
[Export(typeof(DbContext))]
    public partial class Entities : DbContext
    {
        public Entities()
            : base("name=Entities")
        {
        }
    
        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            throw new UnintentionalCodeFirstException();
        }
    
        public DbSet<TB_USERS> TB_USERS { get; set; }
    }
```
这里为什么要使用ImportMany？前面说了，仓储的好处之一在于对数据访问层做封装，使得前端不比关心数据的具体来源。当我们再建一个数据库连接的edmx时，我们只需要修改仓储里面的Cur_context 这个对象的赋值即可，由于其他地方都是针对Cur_context这一个上下文对象做的操作，所以基本都不需要做很大的变化。绕了这么大一圈，其实博主只是想说明Import和ImportMany和仓储模式结合使用的好处，至于仓储模式的适用性问题不是本文的重点。
