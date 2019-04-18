# Hibernate拦截器(Interceptor)与事件监听器(Listener) - z69183787的专栏 - CSDN博客
2015年02月03日 15:44:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1092
Hibernate拦截器(Interceptor)与事件监听器(Listener)
　　拦截器（Intercept）：与Struts2的拦截器机制基本一样，都是一个操作穿过一层层拦截器，每穿过一个拦截器就会触发相应拦截器的事件做预处理或善后处理。
监听器（Listener）：其实功能与拦截器是相似的，但它实现原理不同，它是为每一个事件注册一个或多个监听器，一旦事件发生，则事件源通知所有监听该事件的监听器，然后监听器处理通知（观察者模式）。
拦截器
　　Hibernate为我们提供了实现拦截器的接口org.hibernate.Interceptor，它里面提供了许多拦截事件。通常不需要实现这个接口，因为我们实现自己的拦截器不可能每一个事件都是必须的。所以Hibernate为我们提供了org.hibernate.Interceptor接口的一个空实现类org.hibernate.EmptyIntercept，通常情况下我们只需继承这个空实现类，Override需要的事件方法即可。
拦截器的工作原理简易示意图：
![](http://pic002.cnblogs.com/images/2012/369936/2012012314330446.png)
设置拦截器后，相应的操作都会先穿过一层层相应的拦截器，让拦截器执行预处理或善后处理。
拦截器使用实例：
创建拦截器：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class AutoUpdateTimeInterceptor extends EmptyInterceptor
{
private static final long serialVersionUID = -8597658125309889388L;
/*
     * entity - POJO对象
     * id - POJO对象的主键
     * state - POJO对象的每一个属性所组成的集合(除了ID)
     * propertyNames - POJO对象的每一个属性名字组成的集合(除了ID)
     * types - POJO对象的每一个属性类型所对应的Hibernate类型组成的集合(除了ID)
*/
    @Override
public boolean onSave(Object entity, Serializable id, Object[] state,
            String[] propertyNames, Type[] types)
    {    
if (entity instanceof People)
        {
for (int index=0;index<propertyNames.length;index++)
            {
/*找到名为"checkinTime"的属性*/
if ("checkinTime".equals(propertyNames[index]))
                {
/*使用拦截器将People对象的"checkinTime"属性赋上值*/
                    state[index] = new Timestamp(new Date().getTime());
return true;
                }
            }
        }
return false;
    }
} 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
场景类
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class Client
{
public static void main(String[] args)
    {
/*为Session添加拦截器*/
        Session session = HibernateUtil.getSessionFactory().openSession(new AutoUpdateTimeInterceptor());
        Transaction tx = null;
try
        {
            tx = session.beginTransaction();
            People people = new People();
            people.setName("zhangsan");
            session.save(people);
            tx.commit();
        }
catch (Exception e)
        {
if(tx!=null)
            {
                tx.rollback();
            }
            e.printStackTrace();
        }
finally
        {
            session.close();
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
场景类中并没有显示的设置了people对象的"checkinTime"的属性值，启动该场景类代码，现在来查看数据库信息：
![](http://pic002.cnblogs.com/images/2012/369936/2012012315274248.png)
　　可以看到checkin_time这列属性依然被赋值了，说明该赋值操作是拦截器帮助我们完成的。使用拦截器的时候需要注意拦截器的返回值，我以前一直以为拦截器的返回值会控制一个操作是否可以继续，通过实验发现，即使返回false操作也会继续执行的，只是返回false的话，拦截器的所有设置都是无效的，不会反应到数据库中。
返回false：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class AutoUpdateTimeInterceptor extends EmptyInterceptor
{
private static final long serialVersionUID = -8597658125309889388L;
/*
     * entity - POJO对象
     * id - POJO对象的主键
     * state - POJO对象的每一个属性所组成的集合(除了ID)
     * propertyNames - POJO对象的每一个属性名字组成的集合(除了ID)
     * types - POJO对象的每一个属性类型所对应的Hibernate类型组成的集合(除了ID)
*/
    @Override
public boolean onSave(Object entity, Serializable id, Object[] state,
            String[] propertyNames, Type[] types)
    {    
if (entity instanceof People)
        {
for (int index=0;index<propertyNames.length;index++)
            {
/*找到名为"checkinTime"的属性*/
if ("checkinTime".equals(propertyNames[index]))
                {
/*使用拦截器将People对象的"checkinTime"属性赋上值*/
                    state[index] = new Timestamp(new Date().getTime());
//                  return true;
                }
            }
        }
return false;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
查看插入的数据：
![](http://pic002.cnblogs.com/images/2012/369936/2012012315345532.png)
可以看到数据依然保存到数据库中了，但拦截器的操作并没有反映到数据库中，拦截器的操作是无效的。
但是比较奇怪的是Console输出的SQL语句：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
Hibernate: 
insert 
into
        people
        (name, checkin_time, id) 
values
        (?, ?, ?)
Hibernate: 
update
        people 
set
        name=?,
        checkin_time=? 
where
        id=?
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
居然多了一条Update语句，我使用了p6spy显示了这两条SQL语句的绑定值：
```
**1327304507491**|**0**|**0**|statement|insert into people (name, checkin_time, id) values (?, ?, ?)|insert into people (name, checkin_time, id) values ('zhangsan', '2012-01-23 15:41:47.45', '402881e53509837f0135098380370001')
**1327304507491**|**0**|**0**|statement|update people set name=?, checkin_time=? where id=?|update people set name='zhangsan', checkin_time='' where id='402881e53509837f0135098380370001'
```
　　可以看到，拦截器的操作会直接反映到数据库中的，但是如果返回值是false的话，Hibernate会产生一条Update SQL语句将拦截器的操作结果取消了。
　　最后，Hibernate的拦截器有两种设置方式，一种是使用sessionFactory.openSession(Interceptor interceptor)，这样的拦截器只会针对该session有效，又叫做局部拦截器。另一种是使用Configuration的setInterceptor(Interceptor interceptor)方法设置，这样的拦截器对每一个session都有效，又称之为全局拦截器。
事件监听器
　　基本上，Session接口的每个方法都有相对应的事件。比如 LoadEvent，FlushEvent，等等（查阅XML配置文件的DTD，以及org.hibernate.event包来获得所有已定义的事件的列表）。当某个方法被调用时，Hibernate Session会生成一个相对应的事件并激活所有配置好的事件监听器。系统预设的监听器实现的处理过程就是被监听的方法要做的（被监听的方法所做的其实仅仅是激活监听器，
 “实际”的工作是由监听器完成的）。不过，你可以自由地选择实现一个自己定制的监听器（比如，实现并注册用来处理处理LoadEvent的LoadEventListener接口）， 来负责处理所有的调用Session的load()方法的请求。
　　在定义自己的事件监听器时，其实不需要实现XXXListener接口，Hibernate为了方便我们定义事件监听器，已经为每个事件监听器接口实提供了一个默认的实现。在org.hibernate.event.def包下面可以找到Hibernate为我们提供的默认实现，我们只需要继承这些默认实现，在其基础上添加我们自定义的功能即可。
事件监听器的简单示意图：
![](http://pic002.cnblogs.com/images/2012/369936/2012012316020581.png)
当某个方法被调用时，Hibernate Session会生成一个相对应的事件并激活所有配置好的事件监听器。
事件监听器使用：
创建事件监听器：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class SaveOrUpdateListener extends DefaultSaveOrUpdateEventListener
{
private static final long serialVersionUID = 7496518453770213930L;
/*监听保存或更新事件*/
    @Override
public void onSaveOrUpdate(SaveOrUpdateEvent event)
    {
        People people = null;
if(event.getObject() instanceof People)
        {
            people = (People)event.getObject();
        }
        people.setCheckinTime(new Timestamp(new Date().getTime()));
        System.out.println("invoke!");
/*一定要调用父类提供的功能，不然就和继承接口一样了*/
super.onSaveOrUpdate(event);
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
通过hibernate.cfg.xml配置文件将事件监听器配置到Hibernate中：
第一种配置方式： 
```
<event type="save-update">
<listener class="com.suxiaolei.hibernate.listener.SaveOrUpdateListener"/>
</event>
```
第二种配置方式：
```
<listener class="com.suxiaolei.hibernate.listener.SaveOrUpdateListener" type="save-update"/>
```
　　两种配置方式产生的效果都是一样的。只是一个以"事件"为主，一个以"监听器"为主。type是指定监听事件的类型，class指定监听器的实现类，一个事件可以有多个监听器。type有许多取值，下表列出了所有type的值:
![](http://pic002.cnblogs.com/images/2012/369936/2012012320254341.png)
上面列表每一个选项对应着一个特定的事件。
场景类：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class Client
{
public static void main(String[] args)
    {
        Session session = HibernateUtil.getSessionFactory().openSession();
        Transaction tx = null;
try
        {
            tx = session.beginTransaction();
            People people = new People();
            people.setName("lisi");
            session.saveOrUpdate(people);
            tx.commit();
        }
catch (Exception e)
        {
if(tx!=null)
            {
                tx.rollback();
            }
            e.printStackTrace();
        }
finally
        {
            session.close();
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
people对象依然没有设置checkinTime属性，运行程序后，查看数据库：
![](http://pic002.cnblogs.com/images/2012/369936/2012012320312436.png)
可以看到，checkin_time字段的依然赋值了，说明我们配置的事件监听器生效了。
使用事件监听器我发现需要注意父类行为的顺序，例如：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public void onSaveOrUpdate(SaveOrUpdateEvent event)
{
        People people = null;
if(event.getObject() instanceof People)
        {
            people = (People)event.getObject();
        }
        people.setCheckinTime(new Timestamp(new Date().getTime()));
        System.out.println("invoke!");
/*一定要调用父类提供的功能，不然就和继承接口一样了*/
super.onSaveOrUpdate(event);
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public void onSaveOrUpdate(SaveOrUpdateEvent event)
{
/*一定要调用父类提供的功能，不然就和继承接口一样了*/
super.onSaveOrUpdate(event);
        People people = null;
if(event.getObject() instanceof People)
        {
            people = (People)event.getObject();
        }
        people.setCheckinTime(new Timestamp(new Date().getTime()));
        System.out.println("invoke!");
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　例如上面的顺序，虽然最后产生的效果一致，但是第二种顺序会多产生一条update语句，有可能会产生性能问题，所以在使用事件监听器的时候需要多加注意。
