# LINQ to SQL公共基类 - weixin_33985507的博客 - CSDN博客
2009年02月16日 09:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
**备注：本文的源代码例子，使用的数据库为SQL Server 2005下的Northwind示范数据库，同时为相关表建立了TimeStamp列。**
  LINQ 是Visual Studio 2008中提供的一系列新特性，用以扩展C#或者Visual Basic语言，提供了强有力的查询能力。作为LINQ的组成部分，LINQ to SQL提供了将关系数据作为对象处理的运行时架构。从某种程度上说，它相当于是微软提供的类似于NHibernate和Castle之类的ORM工具或框 架。当我们需要对数据库进行访问时，LINQ to SQL常常会成为我们的首选。
  在LINQ to SQL中，关系数据库数据模型中的所有变量都是强类型的，它提供了编译时验证以及智能感知等优点。我们可以使用查询表达式（包括查询语法和方法语法）从数据库中获取数据。
  然而，强类型并不利于对数据操作进行抽象，因此，开发人员就不得不为每个实体对象定义特定的类，从而导致大量的重复代码。如果我们可以实现一个共同的基 类，封装公共的数据操作，例如Select、Where、Add、Update和Delete，这对于开发N层应用程序而言，是非常有用的。
  所幸，利用泛型可以帮助我们实现这一目标。方法是调用DataContext的GetTable<T>()方法。例如，我们可以实现Where方法，通过传递一个Lambda表达式找到我们希望获得的结果：
public IList<TEntity> Where(Func<TEntity, bool> predicate)
{
    InitDataContext();
return m_context.GetTable<TEntity>().Where(predicate).ToList<TEntity>();
}
这很简单，我们甚至可以利用动态查询，暴露一些方法去接收条件表达式:
publicstaticclass DynamicQueryable
{
publicstatic IQueryable<T> Where<T>(this IQueryable<T> source, string predicate, paramsobject[] values)
    {
return (IQueryable<T>)Where((IQueryable)source, predicate, values);
    }
publicstatic IQueryable Where(this IQueryable source, string predicate, paramsobject[] values)
    {
if (source ==null) thrownew ArgumentNullException("source");
if (predicate ==null) thrownew ArgumentNullException("predicate");
        LambdaExpression lambda = DynamicExpression.ParseLambda(source.ElementType, typeof(bool), predicate, values);
return source.Provider.CreateQuery(
            Expression.Call(
typeof(Queryable), "Where",               
new Type[] { source.ElementType },
                source.Expression, Expression.Quote(lambda)));
    }
}
public IList<TEntity> Where(string predicate, paramsobject[] values)
{
    InitDataContext();
return m_context.GetTable<TEntity>().Where(predicate, values).ToList<TEntity>();
}
   当然，对于抽象一个基类而言，查询并没有问题，因为我们在调用这些方法时，是不需要实体属性的，也不必关心Lambda表达式的组成。
  关键的问题是如何更新或删除数据表记录。在更新或删除它之前，我们必须获得要操作的实体。而查询的关键字通常是记录的ID。此外，在Data Context中的Object Identity和Change Tracking也需要对象的ID来跟踪它的变化。ID通常是自增长字段或者Guid字段，并作为表的主键。这样我们就可以根据它获得实体：
publicvoid Update(Employee employee)
{
    LinqSampleDataContext context =new LinqSampleDataContext();
    Employee emp =this.Where(e => e.EmployeeID == employee. EmployeeID);
    emp.FirstName ="First Name";
    emp.LastName ="Last Name";
    context.SubmitChanges();            
}
  显然，如果使用泛型例如TEntity，我们并不知道实体的属性。没有关键字，我们又该如何关联已经存在的记录的变化呢？LINQ引入了Attach方法，通过它可以将修改的对象附加到Data Context中。Attach方法具有三个重载版本，如下所示：
Attach(Object entity): 以未修改状态将实体附加到DataContext中；
Attach(Object entity, bool asModified): 以修改或未修改状态将集合中的所有实体附加到DataContext中；
Attach(Object entity, Object orginal): 通过指定实体及其原始状态，以修改或未修改状态将实体附加到 DataContext。.
Attach方法通常用来将反序列化的实体与DataContext的一个新实体关联。但我们也可以将一个DataContext中的实体关联到另外一个DataContext中。在修改或删除操作中，这一方法是非常有用的。
例如，我们根据另外一个DataContext中已修改的实体更新记录：
publicvoid Update(TEntity changedEntity)
{
    InitDataContext();
try
    {
        m_context.GetTable<TEntity>().Attach(changedEntity, true);
        m_context.SubmitChanges();
    }
catch (ChangeConflictException)
    {
        m_context.ChangeConflicts.ResolveAll(RefreshMode.KeepCurrentValues);
        m_context.SubmitChanges();
    }       
}
   这样的实现看起来很美，却远远不够。我们不能附加一个修改过的实体，除非该实体所对应的表中具有TimeStamp列（或者主键字段的IsVersion 为true）。因此，我们需要在数据库中添加列，使其类型为Timestamp，或者在LINQ to SQL设计器中将ID属性的IsVersion属性设置为true。我的建议是为所有数据表创建一个TimeStamp列，它可以提高系统的性能，因为在 处理并发的时候，系统只需要检查主键和TimeStamp是否有变化，而不需要检查所有的字段。
  事实上，我们也可以通过泛型传递原始的实体值，这种方法就不需要为每个表添加TimeStamp列，方法是使用Action<T> 委托，代码段如下所示：
publicvoid Update(TEntity originalEntity, Action<TEntity> update)
{
    InitDataContext();
try
    {
        m_context.GetTable<TEntity>().Attach(originalEntity);  
        update(originalEntity);
        m_context.SubmitChanges();
    }
catch (ChangeConflictException)
    {
        m_context.ChangeConflicts.ResolveAll(RefreshMode.KeepCurrentValues);
        m_context.SubmitChanges();
    }       
}
现在，我们可以通过传递Lambda表达式来调用该方法：
[TestMethod()]
publicvoid UpdateWithAction()
{
    LinqSampleDataContext context =new LinqSampleDataContext ();
    EmployeeAccessor accessor =new EmployeeAccessor();
    Employee employee = context.Employees.Single(e => e.EmployeeID ==1);
    accessor.Update(employee, t => { t.FirstName ="First"; t.LastName ="Last"; });
}
遗憾的是，这样的测试用例有时却无法通过，会抛出NotSupportedException异常，异常的信息如下：An attempt has been made to Attach or Add an entity that is not new, perhaps having been loaded from another DataContext.  This is not supported.
原因何在? 究竟发生了什么? 真正的原因是我们所处理的实体与其他实体之间存在关联。如图1所示：
![](http://blog.51cto.com/attachment/201003/103803823.jpg)
图1  具有关系的实体对象
如果移除Employee表中的所有关系，然后重新生成数据模型，测试就能够通过了。
如何解决这个问题呢？显然，显式地去除表的关联并非解决这一问题的最佳方法。这样会影响整个数据模型。对此，Steve Michelotti提出了一个解决方案，也就是利用partial类，为每个数据实体提供一个Detach方法，用以移除实体间的关系：
publicpartialclass Contact
{
publicvoid Detach()
    {
foreach (Address address inthis.Addresses)
        {
            address.Detach();
        }
    }
}
publicpartialclass Address
{
publicvoid Detach()
    {
this._AddressType =default(EntityRef<AddressType>);
this._State =default(EntityRef<State>);
    }
}
  好办法，但还不是最好。首先，采用这种方法，我们需要为每个数据实体都定义Detach方法，这过于繁琐。其次，我们不能用这种方法对该逻辑进行抽象。由于在基类中，我们并不知道TEntity的具体类型。此时，我们应该使用反射技术，以下是我的实现：
privatevoid Detach(TEntity entity)
{
foreach (FieldInfo fi in entity.GetType().GetFields(BindingFlags.NonPublic |BindingFlags.Instance))
    {
if (fi.FieldType.ToString().Contains("EntityRef"))
        {
            var value = fi.GetValue(entity);
if (value !=null)
            {
                fi.SetValue(entity, null);
            }
        }
if (fi.FieldType.ToString().Contains("EntitySet"))
        {
            var value = fi.GetValue(entity);
if (value !=null)
            {
                MethodInfo mi = value.GetType().GetMethod("Clear");
if (mi !=null)
                {
                    mi.Invoke(value, null);
                }
                fi.SetValue(entity, value);
            }
        }
    }
}
针 对EntityRef<T> 字段，可以通过调用FieldInfo的SetValue方法将值赋为null，以此来移除关联。但是，对于EntitySet则不能用同样的方法，因为 该字段为集合。如果设置为null，会抛出异常。因此我通过反射调用了该字段的Clear方法，清除集合中的所有元素。最后，我实现的Update方法如下所示：
publicvoid Update(TEntity originalEntity, Action<TEntity> update, bool hasRelationship)
{
    InitDataContext();
try
    {
if (hasRelationship) 
        {
//Remove the relationship between the entitis
             Detach(originalEntity);
        }
        m_context.GetTable<TEntity>().Attach(originalEntity); 
        update(originalEntity);
        m_context.SubmitChanges();
    }
catch (ChangeConflictException)
    {
        m_context.ChangeConflicts.ResolveAll(RefreshMode.KeepCurrentValues);
        m_context.SubmitChanges();
    }       
}
删除方法是相似的，除了不需要调用Attach的第二个版本 (Attach(object entity, bool asModified))。代码片断如下：
publicvoid Delete(TEntity entity, bool hasRelationship)
{
    InitDataContext();
try
    {
if (hasRelationship)
        {
//Remove the relationship between the entities;
            Detach(entity);
        }
        m_context.GetTable<TEntity>().Attach(entity);
        m_context.GetTable<TEntity>().DeleteOnSubmit(entity);
        m_context.SubmitChanges();
    }
catch (ChangeConflictException)
    {
        m_context.ChangeConflicts.ResolveAll(RefreshMode.KeepCurrentValues);
        m_context.SubmitChanges();
    }
}
publicvoid Delete(IList<TEntity> entities, bool hasRelationship)
{
    InitDataContext();
try
    {
if (hasRelationship)
        {
//Remove the relationship
foreach (TEntity entity in entities)
            {
                Detach(entity);
            }
        }
        m_context.GetTable<TEntity>().AttachAll(entities);
        m_context.GetTable<TEntity>().DeleteAllOnSubmit(entities);
        m_context.SubmitChanges();
    }
catch (ChangeConflictException)
    {
        m_context.ChangeConflicts.ResolveAll(RefreshMode.KeepCurrentValues);
        m_context.SubmitChanges();
    }            
}
  虽然移除了实体间的关系，但却不用担心最终结果的正确性。Attach方法只负责将实体与DataContext的一个新实例进行关联，并跟踪实体的变 化。在提交修改时，DataContext会检查映射的数据库中实际的值，然后再根据传递进来的实体更新或删除记录。特别的，我们需要在数据库中设置级联 的Action，如图2所示：
![](http://blog.51cto.com/attachment/201003/103816713.jpg)
图2：设置删除规则
  如果没有action，在删除实体时，可能会抛出System.Data.SqlClient.SqlException异常，信息如下：
The DELETE statement conflicted with the REFERENCE constraint "FK_Orders_Employees". The conflict occurred in database "Northwind", table "dbo.Orders", column 'EmployeeID'.
  或许，你已经注意到在每个方法中都调用了InitDataContext方法，实现如下：
private TContext m_context =null;
private TContext CreateContext()
{
return Activator.CreateInstance<TContext>() as TContext;
}
privatevoid InitDataContext()
{
    m_context = CreateContext();
}
  为何需要为每个方法创建一个DataContext实例？原因在于DataContext的缓存机制。如果创建了一个DataContext的新实例，然 后通过它查询数据库的数据，再修改其值；假设使用同一个实例执行下一次查询，DataContext就会返回存储在内部缓存中的数据，而不是重新映射数据 表中的列。更多信息，请参考Linq in Action.
  因此，最佳实践应该是为每个操作创建一个新的DataContext实例。不用担心性能的问题，DataContext属于轻量级的资源。
  再来看看并发的问题。默认的选项是乐观并发（Optimistic Concurrency）。一旦保存了值， DataContext会检查之前的值是否更改。如果发生冲突，DataContext需要知道是否：自动重写之前的修改，或者保存之前的修改，或者以某 种方式合并修改。
  关于并发的问题并非本文讨论的问题。我们无法确定哪一种方法最好，或者最差，这需要视业务逻辑而定。通常，我会以last submit win的策略处理并发。因此，我封装了SubmitChanges方法，并将其定义为虚方法。若有必要，子类可以重写该方法：
publicclass AccessorBase<TEntity, TContext>
where TEntity : class, new()
where TContext : DataContext, new()
{
private TContext m_context =null;
protectedvirtualbool SubmitChanges(TContext context)
    {
try
        {
            context.SubmitChanges(ConflictMode.ContinueOnConflict);
returntrue;
        }
catch (ChangeConflictException)
        {
            context.ChangeConflicts.ResolveAll(RefreshMode.KeepCurrentValues);
            context.SubmitChanges();
returntrue;
        }
catch (Exception ex)
        {
            LogService.Error("Submit Changes error.", ex);
returnfalse;
        }
finally
        {
            context.Dispose();
        }
    }
publicbool Update(TEntity originalEntity, Action<TEntity> update, bool hasRelationship)
    {
        InitDataContext();
try
{
if (hasRelationship)
            {
//Remove the relationship between the entitis
                Detach(originalEntity);
            }
            m_context.GetTable<TEntity>().Attach(originalEntity);
            update(originalEntity);
            SubmitChanges(m_context);
      }
catch (InvalidCastException ex)
      {
          LogService.Error("Update Entity error.", ex);
returnfalse;
      }
catch (NotSupportedException ex)
      {
          LogService.Error("Update Entity error.", ex);
returnfalse;
      }
catch (Exception ex)
      {
          LogService.Error("Update Entity error.", ex);
returnfalse;
      }    
    }
}
现在，我们拥有了一个通用的公共类，实体对象的访问类可以继承它。例如：
publicclass EmployeeAccessor:AccessorBase<Employee,NorthwindDataContext>
{
}
你不需要实现任何方法，就能够很方便地对实体对象进行操作：
[TestMethod()]
publicvoid UpdateEmployee()
{
    EmployeeAccessor accessor =new EmployeeAccessor();
    IList<Employee> entities = accessor.Where(e => e.EmployeeID ==1);
if (entities !=null&& entities.Count >0)
    {
        entities[0].FirstName ="Bruce";
        entities[0].LastName ="Zhang";
        accessor.Update(entities[0],true,true);
    }
}
你甚至可以直接让Employee实体类继承基类：
publicpartialclass Employee:AccessorBase<Employee,NorthwindDataContext>
{
}
这种方法类似于充血模式，正如Martin Fowler在Anemic Domain Model一文中提到的那样。
