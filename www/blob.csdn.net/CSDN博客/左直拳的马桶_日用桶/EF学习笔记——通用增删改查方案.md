# EF学习笔记——通用增删改查方案 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月10日 13:33:16[左直拳](https://me.csdn.net/leftfist)阅读数：24194
我刚接触EF未久，还不知道它有什么强大之处，但看上去，EF提供了一般的增删改查功能。以往用过一些ORM方法，尽管有代码生成器，但代码量总的来说比较多。这次采用EF，我就想，能不能尽量写出一些通用的方法，以后添加表、实体类，只需做很少的改动，就能拥有基本的增删改查功能呢？
方案如下：
1、分为几个部分：BLL、DAL、Model、Interface。其中，EF生成的代码放在Model。因为不想由BLL直接操作Model，因此还是增加一个DAL，由它来对EF的方法作进一步的封装，供BLL调用。至于接口，则是出于如下考虑：增删改查，BLL和DAL都需要实现，因此它们最好实现同一接口；最重要的，因为使用了接口，在BLL中，方便调用DAL对象。
这几个部分分别介绍如下：
1、接口Interface
```
public interface IEntity
    {
        long _ID { get;}
    }
```
```
public interface ICommon<T> where T : class,IEntity
    {
        T Add(T model);
        T Update(T model);
        void Delete(T model);
        //按主键删除,keyValues是主键值
        void Delete(params object[] keyValues);
        //keyValues是主键值
        T Find(params object[] keyValues);
        List<T> FindAll();
    }
```
2、DAL
通用的增删改查代码：
```
public abstract class BaseCommon<T> : Interface.ICommon<T> where T : class,Interface.IEntity
    {
        DbContext db;
        public BaseCommon(DbContext context)
        {
            this.db = context;
        }
        public DbContext Context
        {
            get
            {
                return db;
            }
        }
        #region ICommon<T>
        public T Add(T model)
        {
            db.Set<T>().Add(model);
            db.SaveChanges();
            return model;
        }
        public T Update(T model)
        {
            if (db.Entry<T>(model).State == EntityState.Modified)
            {
                db.SaveChanges();
            }
            else if (db.Entry<T>(model).State == EntityState.Detached)
            {
                try
                {
                    db.Set<T>().Attach(model);
                    db.Entry<T>(model).State = EntityState.Modified;
                }
                catch (InvalidOperationException)
                {
                    T old = Find(model._ID);
                    db.Entry(old).CurrentValues.SetValues(model);
                }
                db.SaveChanges();
            }
            return model;
        }
        public void Delete(T model)
        {
            db.Set<T>().Remove(model);
            db.SaveChanges();
        }
        public void Delete(params object[] keyValues)
        {
            T model = Find(keyValues);
            if (model != null)
            {
                db.Set<T>().Remove(model);
                db.SaveChanges();
            }
        }
        public T Find(params object[] keyValues)
        {
            return db.Set<T>().Find(keyValues);
        }
        public List<T> FindAll()
        {
            return db.Set<T>().ToList();
        }
        #endregion
    }
```
这里面，已经封装好了一般的增删改查方法。而对应数据库每个表对象的那些类，只需继承这个BaseCommon类，即可拥有增删改查功能。如：
`public partial class TableA : BaseCommon<Model.TableA> { }`
这样，TableA对象会自然拥有增删改查功能。假如需要扩充它的功能，我们可以再写一个TableA的分部类。
以后，新增一个表TableB，在DAL这里，一般情况下只需新增一句：
`public partial class TableB : BaseCommon<Model.TableB> { }`
是不是很方便？
3、BLL
BLL是供UI层，或者上一层调用的，因此，它每个对象，也应该有增删改查的基本功能。当然，BLL无须直接实现，是通过调用DAL来实现：
```
public abstract class Common<TDAL, TModel> : Interface.ICommon<TModel>
        where TDAL : class ,new()
        where TModel : class ,Interface.IEntity 
    {
        protected TDAL dal;
        private Interface.ICommon<TModel> common;
        public Common()
        {
            dal = new TDAL();
            common = (Interface.ICommon<TModel>)dal;
        }
        #region ICommon<TModel>
        public TModel Add(TModel model)
        {
            return common.Add(model);
        }
        public TModel Update(TModel model)
        {
            return common.Update(model);
        }
        public void Delete(TModel model)
        {
            common.Delete(model);
        }
        public void Delete(params object[] keyValues)
        {
            common.Delete(keyValues);
        }
        public TModel Find(params object[] keyValues)
        {
            return common.Find(keyValues);
        }
        public List<TModel> FindAll()
        {
            return common.FindAll();
        }
        #endregion
    }
```
与DAL类似，TableA对象在这里是这样的：
`public partial class TableA : Common<DAL.TableA, Model.TableA> { }`
同理，如果以后新增了表TalbeB，在这里也只是新增一句而已。
这里还有一个好处，就是对外界调用而言，根本不需要知道泛型，就是一个 TableA table = new TalbeA();就可以了。
