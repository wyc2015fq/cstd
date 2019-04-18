# Hibernate事件系统及监听器 - z69183787的专栏 - CSDN博客
2015年02月03日 19:47:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1273
**1.Hibernate事件系统(Event System)及监听器**
•如果需要响应持久层的某些特殊事件，你也可以使用Hibernate的事件框架。 该事件系统可以用来替代拦截器，也可以作为拦截器的补充来使用。
•基本上，Session接口的每个方法都有相对应的事件。比如 LoadEvent（对应session.load()方法），FlushEvent（对应session.flush()方法），等等（可以查阅XML配置文件 的DTD以及**org.hibernate.event**包来获得所有已定义的事件的列表）。
•当某个方 法被调用时，Hibernate Session会生成一个相应的事件并激活所有配置好的事件监听器（观察者模式）。被监听的方法所做的其实仅仅是激活监听器，“实际”的工作是由监听器完成的。你可以自由地选择实现一个自己定制的监听器：比如，实现并注册用来处理处理LoadEvent的**LoadEventListener**接口， 来处理所有的调用Session的load()方法的请求。
•监听器在运行时被实例化为**单例(singleton)对象**，也就是说，**所有同类型的事件的处理共享同一个监听器实例**，因此监听器不应该保存任何与请求相关的状态。
**2.定制监听器**
**•实现用户自定义的监听器有如下三种方法：**
（1）实现对应的监听器接口：实现接口必须实现接口中的所有方法，关键是必须实现Hibernate对应的持久化操作，即数据库访问，这意味着程序员完全取代了Hibernate的底层操作。（此时Hibernate提供的功能就失效了，就没有意义了）
（2）继承事件适配器：可以选择型的实现需要关注的方法，但依然试图取代Hibernate完成数据库的访问。（此时Hibernate提供的功能就失效了，就没有意义了）
**（3）继承事件系统默认的事件监听器：扩展特定方法**
不推荐使用第一种方法和第二种方法。因为Hibernate的持久化操作也是通过这些监听器实现的，如果用户取代了这些监听器，则应该自己实现所有的持久化操作，这意味着：用户放弃了Hibernate的持久化操作，而改为自己完成Hibernate的核心操作，使用Hibernate就没有多大意义了。
通常推荐采用第三种方式实现自己的事件监听器。Hibernate默认的事件监听器都被声明成non-final，从而方便用户继承，Hibernate默认的事件监听器定义在
**org.hibernate.event.def**包中。
**例：使用第三种方式实现自定义的监听delete操作的监听器：**
**[java]**[view
 plain](http://blog.csdn.net/jialinqiang/article/details/8739326#)[copy](http://blog.csdn.net/jialinqiang/article/details/8739326#)
- package listener;  
- 
- import org.hibernate.HibernateException;  
- import org.hibernate.event.DeleteEvent;  
- import org.hibernate.event.def.DefaultDeleteEventListener;  
- 
- import bean.Student;  
- 
- @SuppressWarnings("serial")  
- publicclass MyDeleteListener extends DefaultDeleteEventListener{  
- 
- @Override
- publicvoid onDelete(DeleteEvent arg0) throws HibernateException {  
- 
-         System.out.println("Delete EntityName:"+arg0.getEntityName());  
-         Object object=arg0.getObject();  
- if(object instanceof Student)  
-         {  
-             Student s=(Student)object;  
-             System.out.println("name:"+s.getName());  
-             System.out.println("age:"+s.getAge());  
-             System.out.println("id:"+s.getId());  
-         }  
- //注意一定要调用父类的对应的方法
- super.onDelete(arg0);  
-     }  
- }  
注意：因为覆盖了父类（即Hibernate默认的事件监听器）的方法，别忘了在方法中调用父类的对应的方法，否则Hibernate的功能就会失效，那样的话就和前两种方式相同了。
**3.注册监听器**
•用户定制的监听器可以通过编程使用Configuration对象 来注册，也可以在Hibernate的XML格式的配置文件中进行声明。
**(1)使用Configuration对象注册自定义的监听器：**
Configuration的相关方法有：
 //注册一个监听器(type指定事件类型如"delete")
 void setListener(String type, String listener) 
 void setListener(String type, Object listener) 
 //注册多个监听器
 void setListeners(String type, Object[] listeners)         
 void setListeners(String type, String[] listenerClasses)  
例：
**[java]**[view
 plain](http://blog.csdn.net/jialinqiang/article/details/8739326#)[copy](http://blog.csdn.net/jialinqiang/article/details/8739326#)
- Configuration c=new Configuration();  
- c.setListener("delete", new MyDeleteListener());//或c.setListener("delete","listener.MyDeleteListener");
关于type参数的取值可到EventListeners.java源文件中查找，如果觉得比较麻烦，可以采取以下方式：
**[java]**[view
 plain](http://blog.csdn.net/jialinqiang/article/details/8739326#)[copy](http://blog.csdn.net/jialinqiang/article/details/8739326#)
- Configuration c=new Configuration();  
- DeleteEventListener[] stack={new MyDeleteListener()};//自定义的监听器要有Hibernate默认监听器的功能，否则要在数组中加入delete事件默认的监听器
- c.getEventListeners().setDeleteEventListeners(stack);  
注意：如果我们自定义的监听器没有实现Hibernate原来的持久化功能（如使用前两种方式自定义的拦截器或在第三种方式中没有调用父类的方法super.onDelete(arg0)，那么就必须使用setLiseteners()方法或在DeleteEventListener[]类型的监听器栈数组中加入元素：Hibernate的delete事件的默认监听器DefaultDeleteEventListener。同样，对于其它事件的监听也是如此。
**(2)使用Hibernate的主配置文件(hibernate.cfg.xml)注册自定义的监听器：**
——使用<event>元素注册：
**[html]**[view
 plain](http://blog.csdn.net/jialinqiang/article/details/8739326#)[copy](http://blog.csdn.net/jialinqiang/article/details/8739326#)
- <eventtype="delete">
- <listenerclass="listener.MyDeleteListener"/>
- <!--如果自定义的监听器没有实现Hibernate默认监听器的功能，就必须加上默认的监听器-->
- <listenerclass="org.hibernate.event.def.DefaultDeleteEventListener"/>
- </event>
——直接使用<listener>元素注册：
**[html]**[view
 plain](http://blog.csdn.net/jialinqiang/article/details/8739326#)[copy](http://blog.csdn.net/jialinqiang/article/details/8739326#)
- <listenerclass="listener.MyDeleteListener"type="delete"></listener>
注意：直接使用<listener>元素注册监听器，对于同一个type指定的事件类型只能指定一个监听器，如果指定了多个，只有最后一个有效，例：
**[html]**[view
 plain](http://blog.csdn.net/jialinqiang/article/details/8739326#)[copy](http://blog.csdn.net/jialinqiang/article/details/8739326#)
- <listenerclass="listener.MyDeleteListener"type="delete"></listener>
- <listenerclass="org.hibernate.event.def.DefaultDeleteEventListener"type="delete"/>
则只有默认的监听器有效，我们自定义的监听器就没有使用上了，所以对应于一个事件若有多个监听器，应使用<event>元素注册而不能直接使用<listener>元素注册。
对于type事件类型的可取值，通过MyEclipse的帮助功能，可以看到，其取值有：
![](https://img-my.csdn.net/uploads/201303/30/1364630113_9058.PNG)
转载请注明出处：http://blog.csdn.net/jialinqiang/article/details/8739326
