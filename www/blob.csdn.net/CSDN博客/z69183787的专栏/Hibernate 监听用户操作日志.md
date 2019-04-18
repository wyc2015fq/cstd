# Hibernate 监听用户操作日志 - z69183787的专栏 - CSDN博客
2015年02月03日 20:10:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2842
我们前段时间有个.net项目需要用j2ee改造，有个需求是要对所有的数据库操作(CRUD)都要做历史记录，要记录操作内容，操作的用户和操作时间。这样的需求称为审计日志 Audit log。
项目采用Spring构建，持久层技术采用的是 JPA规范 + Hibernate实现的方案
要实现审计日志的需求，我试用了多种技术方案，最终确定了采用Hibernate 3的新特性事件驱动架构来解决：
**技术选型：**
- 
最土的，在所有的Dao方法中显示的编写日志记录代码
该项目以前是用.net这么干的，这种做法重复工作量太大，维护性差，并且也没实现字段级变更的记录，根本不予考虑。
- 
数据库触发器 - 与数据库耦合
与数据库耦合，违背了使用hibernate的初衷，也不予考虑
- 
原生的Hibernate Interceptor
优点：可以在hibernate对象操作的时候获取最为详细的运行期信息，字段名，原始值，修改后值等等。
缺点：在JPA  API的封装下很难获取到hibernate的session，不能进行持久化操作。
- 
JPA callback / event-listener
优点：JPA规范，最为优雅简单
缺点：功能太弱，不能满足需求
- 
很自然地，干这种事AOP似乎比较合适
优点：灵活，在spring容器中，可以访问所有spring bean
缺点：不能获取详细的运行期信息（字段名，原始值，等等），无法感知hibernate的事务执行，即使dao事务rollback，仍然会插入一条操作历史记录，破坏了“操作”和“历史”的一致性。
- **采用Hibernate 3的新特性 Event-listener**
可以解决以上所有问题
能够取得运行期详细信息，除了能记录粗粒度的实体的保存删除操作外，还能精确追踪对实体字段修改、实体关联/级联关系的变更，能记录更新前的值、更新后的值，可以生成详细日志。
灵活解耦，跨数据库，不影响原有代码。
    Hibernate3 新特性事件处理框架是hibernate 2拦截器的一个补充或者替代，由拦截器被动拦截操作事件变成事件源的主动驱动，这是一个进步。[Hibernate
 事件框架官方文档.](http://www.hibernate.org/hib_docs/v3/reference/en/html/events.html#objectstate-events)
    Hibernate3中定义了很多的事件，涵盖了持久化过程中不同的生命周期。简单说Session的一个方法（load, flush...）分别对应一个事件，当该方法被调用时，就会触发一个相应的事件，这个事件会被我们预先定义的事件监听器收到，再进行相应的处理。这种方式来做审计日志是再适合不过。
    但也有个缺点就是这样的Event-listener是脱离主容器（比如Spring IoC环境）单独实例化的，无法访问主容器的资源（比如要取得当前登录的用户信息就会比较麻烦）。这个暂时还没解决。
在这里我们选取PostInsertEventListener（插入后事件）,PostUpdateEventListener（更新后事件）,PostDeleteEventListener（删除后事件）接口作为CRUD方法的监听接口。hibernate3中事件是分为pre和post，表示该发生事件前、后。这里我们全部用Post，因为PostEvent只有在数据实际改变后才会触发，假如CRUD事务因为异常回滚，则不会触发事件。
首先定义一个mark接口Historiazable，实现该接口的entity类表明是需要做审计日志的。
然后编写我们自定义的EventListener类，实现上述的事件接口。
在事件接口实现方法里在根据不同的事件编写审计日志的代码。
Java代码  ![收藏代码](http://jeffreyhsu.iteye.com/images/icon_star.png)
- **public****class** HistoryListener **implements** PostInsertEventListener,  
-         PostUpdateEventListener, PostDeleteEventListener {  
- 
- @Override
- **public****void** onPostInsert(PostInsertEvent event) {  
- **if** (event.getEntity() **instanceof** Historizable) {  
- //  保存 插入日志
-         }  
-     }  
- 
- @Override
- **public****void** onPostUpdate(PostUpdateEvent event) {  
- **if** (event.getEntity() **instanceof** Historizable) {  
- // 保存 修改日志
-         }  
-     }  
- 
- @Override
- **public****void** onPostDelete(PostDeleteEvent event) {  
- **if** (event.getEntity() **instanceof** Historizable) {  
- // 保存 删除日志
-         }  
-     }  
- }  
配置EventListener
编辑hibernate.cfg.xml，配置监听器
Xml代码  ![收藏代码](http://jeffreyhsu.iteye.com/images/icon_star.png)
- **<****session-factory****>**
- **<****listener**type="post-insert"class="net.jeffrey.hibernate.history.HistoryListener"**/>**
- **<****listener**type="post-update"class="net.jeffrey.hibernate.history.HistoryListener"**/>**
- **<****listener**type="post-delete"class="net.jeffrey.hibernate.history.HistoryListener"**/>**
- **</****session-factory****>**
配置持久化单元
在persistence.xml中加入
 <property name="hibernate.ejb.cfgfile" value="hibernate.cfg.xml"/>
这样JPA环境启动后，就会正确装载初始化自定义的事件监听器。
源代码：
详细的代码请下载源代码，有完整的注释和单元测试用例，一看就明白了。
为了简便起见，没有建立spring或者ejb3项目，使用的是POJO方式，但都是一样的。
在源码根目录里的data.sql是为单元测试准备的基础数据，请在测试前导入到数据库中
源码是Netbeans项目，需要添加Hibernate的依赖和mysql驱动
在http://www.hibernate.org/6.html 下载最新版的Hibernate Core, Hibernate Annotations和
Hibernate EntityManager。解压缩后将lib里所有的jar添加到项目中（注意不要把ant相关jar添加进去，可能会导致netbeans无法正常构建项目）。
