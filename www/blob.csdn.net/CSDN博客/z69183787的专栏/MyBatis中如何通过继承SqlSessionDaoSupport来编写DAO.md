# MyBatis中如何通过继承SqlSessionDaoSupport来编写DAO - z69183787的专栏 - CSDN博客
2017年10月31日 14:22:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：367
（ 本文示例完整源代码与数据库脚本下载地址： [http://down.51cto.com/data/1970833](http://down.51cto.com/data/1970833)）
在 MyBatis 中，当我们编写好访问数据库的映射器接口后， MapperScannerConfigurer 就能自动成批地帮助我们根据这些接口生成 DAO 对象，然后我们再使用 Spring 把这些 DAO 对象注入到业务逻辑层的对象（ Service 类的对象）。因此，在这种情况下的 DAO 层，我们几乎不用编写代码，而且也没有地方编写，因为只有接口。这固然方便，不过如果我们需要在 DAO 层写一些代码的话，这种方式就无能为力了。此时， MyBatis-Spring 提供给我们的 SqlSessionDaoSupport类就派上了用场。今天，就 以访问学生表为例，通过继承 SqlSessionDaoSupport ，来写一个 StudentDao 。
首先来认识一个 SqlSessionDaoSupport 类。类 org.mybatis.spring.support.SqlSessionDaoSupport 继承了类 org.springframework.dao.support.DaoSupport ，它是一个抽象类，本身就是作为DAO 的基类来使用的。它需要一个 SqlSessionTemplate 或一个 SqlSessionFactory ，若两者都设置了，则 SqlSessionFactory 会被忽略（实际上它接收了 SqlSessionFactory 后也会利用 SqlSessionFactory 创建一个 SqlSessionTemplate ）。这样，我们在子类中就能通过调用 SqlSessionDaoSupport 类的 getSqlSession() 方法来获取这个 SqlSessionTemplate 对象。而 SqlSessionTemplate 类实现了 SqlSession 接口，因此，有了 SqlSessionTemplate 对象，访问数据库就不在话下了。所以，我们需要 Spring 给 SqlSessionDaoSupport 类的子类的对象（多个 DAO 对象）注入一个 SqlSessionFactory 或一个 SqlSessionTemplate 。好消息是， SqlSessionTemplate 是线程安全的，因此，给多个 DAO 对象注入同一个 SqlSessionTemplate 是没有问题的，本例也将注入SqlSessionFactory 。
但坏消息是，自 mybatis-spring-1.2.0 以来， SqlSessionDaoSupport 的 setSqlSessionTemplate 和 setSqlSessionFactory 两个方法上的 @Autowired 注解被删除，这就意味着继承于 SqlSessionDaoSupport 的 DAO 类，它们的对象不能被自动注入 SqlSessionFactory 或 SqlSessionTemplate 对象。如果在 Spring 的配置文件中一个一个地配置的话，显然太麻烦。比较好的解决办法是在我们的 DAO 类中覆盖这两个方法之一，并加上 @Autowired 注解。那么如果在每个 DAO 类中都这么做的话，显然很低效。更优雅的做法是，写一个继承于 SqlSessionDaoSupport 的 BaseDao，在 BaseDao 中完成这个工作，然后其他的 DAO 类再都从 BaseDao 继承。 BaseDao 代码如下：
package com.abc.dao.base;
import org.mybatis.spring.SqlSessionTemplate;
import org.mybatis.spring.support.SqlSessionDaoSupport;
import org.springframework.beans.factory.annotation.Autowired;
public classBaseDaoextendsSqlSessionDaoSupport{
 
 @Autowired
 public void setSqlSessionTemplate(SqlSessionTemplate sqlSessionTemplate)
    {
     super.setSqlSessionTemplate(sqlSessionTemplate);
    }
 
}
接着的问题就是 StudentDao 该怎么写。获取了 SqlSessionTemplate 之后，有两种方式访问数据库，一种是通过 SqlSessionTemplate 的相关方法执行 SQL 映射文件中的 SQL 语句，一种是先通过 SqlSessionTemplate 获取映射器对象（在这里就是 StudentMapper 接口类型的对象），然后再调用这个映射器对象的数据库访问方法。鉴于第二种方法更方便（第一种方法需要写冗长的SQL 语句全名，全是字符串，无代码提示，第二种方法可以利用 IDE 的代码提示功能）、及具有更好的类型安全性，本示例就采用后者。
先看 StudentMapper 接口，代码如下：
```java
package com.abc.mapper;
import com.abc.domain.Student;
public interface StudentMapper {
  
 //根据id查找学生
 public Student getById(int id);
 
 //添加一名学生
 public int add(Student student);
 
 //修改学生
 public int update(Student student);
 
 //删除学生
 public int delete(int id);
 
}
```
至于 StudentDao ，应该先定义一个私有的 StudentMapper 类型的变量 studentMapper ，并在合适的时机初始化 studentMapper ，然后就简单了，在各方法中调用 studentMapper 相应的方法即可。不过，难就难在这个合适的时机不好找。如果定义后直接初始化，如下所示：
private StudentMapper studentMapper
            = this.getSqlSession().getMapper(StudentMapper.class);
或者在构造方法中使用类似的代码进行初始化，你都会得到空指针异常。原因是这些代码在运行时， SqlSessionTemplate 对象还没有被注入（如果你选择注入 SqlSessionFactory ，结果是一样的）。
似乎进入了死胡同。
能不能写一个初始化方法，在这个方法中对 studentMapper 进行初始化，然后在 SqlSessionTemplate 被注入后，这个方法被自动调用呢？
首先想到的是 afterPropertiesSet() 方法。接口 org.springframework.beans.factory.InitializingBean 只声明了一个 afterPropertiesSet() 方法，顾名思义，此方法在 Spring 为 bean 注入完所有的依赖关系后会被 Spring 自动调用，如果 StudentDao 实现了此接口，就可以在 afterPropertiesSet() 方法中对 studentMapper 进行初始化。
很完美是吗？
不幸的是， org.springframework.dao.support.DaoSupport 类（ SqlSessionDaoSupport 类的父类）已经实现了此接口，为 afterPropertiesSet() 方法提供了实现，并且把此方法定义成了 final 类型的。也就是说，我们在子类中不能覆盖此方法。所以， afterPropertiesSet() 方法不能为我们所用。
还有一种方法，就是先在 StudentDao 中写好初始化方法，然后在 Spring 中使用 xml 配置 bean 的时候，指定 init-method 属性的值为此方法。则此方法也会在 Spring 注入完所有的依赖关系后，被 Spring 调用。不过这种方法需要使用 xml 配置每一个 DAO bean ，不如使用注解方便，故这种方法也被排除。
还有没有其他办法呢？
有的，我们还可以使用 Spring 的 bean 后处理器。同样顾名思义， bean 后处理器中的方法是在 Spring 注入完依赖关系之后被调用的，所以很适合我们目前的要求。现在，我们就用 bean 后处理器来解决我们的问题。先上 StudentDao 的代码如下（注意对 studentMapper 进行初始化的方法是 init 方法）：
package com.abc.dao;
import org.springframework.stereotype.Repository;
import com.abc.dao.base.BaseDao;
import com.abc.domain.Student;
import com.abc.mapper.StudentMapper;
@Repository
publicclass StudentDao extends BaseDao{
    
 private StudentMapper studentMapper;
 
 public Student getById(int id)
 {
  returnthis.studentMapper.getById(id);
 }
 
 publicvoid deleteById(int id)
 {
  intcount = this.studentMapper.delete(id);
  System.out.println("删除了" + count + "行数据。");
 }
 
 publicvoid update(Student student)
 {
  intcount = this.studentMapper.update(student);
  System.out.println("修改了" + count + "行数据。");
 }
 
 publicvoid add(Student student) {
  // TODO Auto-generated method stubintcount = this.studentMapper.add(student);
  System.out.println("添加了" + count + "行数据。");
 }
 //对studentMapper进行初始化的方法publicvoid init()
 {
  System.out.println("初始化studentMapper...");
  this.studentMapper 
       = this.getSqlSession().getMapper(StudentMapper.class);
 }
 
}
在 StudentDao 中的各数据库访问方法中，我们就可以根据自己的需要来编写相应的代码了。
然后是编写 bean 后处理器，这要通过实现接口 org.springframework.beans.factory.config.BeanPostProcessor 实现，这个接口声明了如下两个方法：
Object   [postProcessBeforeInitialization](http://www.tuicool.com/articles/7BJzyau#)( Object bean, String beanName)
Object   [postProcessAfterInitialization](http://www.tuicool.com/articles/7BJzyau#)( Object bean, String beanName)
两个方法的第一个参数都是待处理的 bean ，第二个参数都是待处理的 bean 的名字， Spring会调用这两个方法对容器中的每个 bean 进行处理，具体的运行顺序是：
1 、注入依赖关系；
2 、调用 postProcessBeforeInitialization 方法；
3 、调用 afterPropertiesSet 方法；
4 、调用 init-method 方法（如前所述，在使用 xml 配置 bean 的时候，可使用 init-method 属性指定 bean 的初始化方法）；
5 、调用 postProcessAfterInitialization 方法。
由于第一步就已经完成了依赖关系注入，因此我们在 postProcessBeforeInitialization 或 postProcessAfterInitialization 这两个方法中调用 DAO （这里只有 StudentDao ，但实际上应该有很多 DAO 类，每个 DAO 类都应该有自己的 init 方法）的 init 方法都可以，这里我们在 postProcessBeforeInitialization 方法中调用。不过这里还有一个问题需要解决，就是在 postProcessBeforeInitialization 方法中，参数 bean 的类型是 Object 类型，我们最多只能把它强制转换为 BaseDao 类型（因为具体的 DAO 类型有很多），但即使如此，也不能通过参数 bean 调用 DAO 的 init 方法，因为 init 方法不是在 BaseDao 中声明的。而如果每种 DAO 类型都分别判断一遍再做相应的强制类型转换，则显然很低效，而且每增加一种 DAO 类型，就得添加相应的类型判断、强制类型转换的代码。对于这个问题，有两个解决方法，一是用反射，二是用多态，而用多态显然更优雅。具体做法是改造 BaseDao ，在 BaseDao 中声明一个抽象的 init 方法，显然此时 BaseDao 类也应该声明为抽象类，然后在各子类中实现此 init 方法。这样，我们只需要把参数 bean 强制转换为 BaseDao 类型，就可以通过参数 bean 调用 init 方法了。而根据多态的原理，实际调用的是具体的子类（如 StudentDao 类）中实现的 init 方法。这样，我们的问题就完美解决了。经多态改造后的 BaseDao 代码如下：
package com.abc.dao.base;
import org.mybatis.spring.SqlSessionTemplate;
import org.mybatis.spring.support.SqlSessionDaoSupport;
import org.springframework.beans.factory.annotation.Autowired;
public abstractclassBaseDaoextendsSqlSessionDaoSupport{
 
 @Autowired
 public void setSqlSessionTemplate(SqlSessionTemplate sqlSessionTemplate)
    {
     super.setSqlSessionTemplate(sqlSessionTemplate);
    }
 
 //抽象方法
 public abstract void init(); 
 
}
StudentDao的代码不用改动，而bean后处理器的代码如下：
package com.abc.post;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import com.abc.dao.base.BaseDao;
public classDaoPostProcessorimplementsBeanPostProcessor{
 
 @Override
 public Object postProcessAfterInitialization(Object bean, String beanName)
   throws BeansException {
  // TODO Auto-generated method stub//只处理BaseDao的子类的对象if(bean.getClass().getSuperclass()==BaseDao.class)
  {
   BaseDao dao = (BaseDao)bean;
   dao.init();
  }
  //返回原bean实例return bean;
 }
 
 @Override
 public Object postProcessBeforeInitialization(Object bean, String beanName)
   throws BeansException {
  // TODO Auto-generated method stub//直接返回原bean实例，不做任何处理return bean;
 }
}
最后一步就是要在 Spring 的配置文件中配置这个 bean 后处理器，跟配置普通的 bean 一样。而且如果你不需要获取这个 bean 后处理器的话，你甚至可以不给它指定 id 属性的值。配置代码如下：
<bean class="com.abc.post.DaoPostProcessor"/>
如果 Spring 容器是 BeanFactory ，则还需手动注册此后处理器；而如果 Spring 容器是 ApplicationContext ，则无需手动注册，我们这里采用后者。执行类的代码如下（StudentDao类的对象被注入到了StudentService对象，这里是请求了 StudentService 对象并调用了相关的方法，具体请参考 StudentService 、 StudentDao 的代码，以及Spring中context:component-scan的相关配置。本示例完整源代码与数据库脚本下载地址： [http://down.51cto.com/data/1970833](http://down.51cto.com/data/1970833) ）：
package com.demo;
import org.springframework.context.ApplicationContext;
import com.abc.service.StudentService;
import com.abc.domain.Student;
import org.springframework.context.support.ClassPathXmlApplicationContext;
publicclassTestDaoSupport{
 
 privatestaticApplicationContext ctx;
 static {
  // 在类路径下寻找spring主配置文件，启动spring容器
  ctx = new ClassPathXmlApplicationContext(
    "classpath:/applicationContext.xml");
 }
 
 publicstatic void main(String[] args) {
  // 从Spring容器中请求服务组件StudentService studentService = 
    (StudentService)ctx.getBean("studentService");
  
  Student student = studentService.getById(13);
  System.out.println(student.getName());
  
 }
}
运行结果如下：
![wKiom1StWFqxOy3DAAmWi-j3zM8927.jpg](http://img2.tuicool.com/vqAj6r.jpg!web)
感谢你耐心看到这里，如果我现在告诉你，其实用不着费这么大劲写后处理器，有更简便的方法，你会不会很生气？ o( ∩ _ ∩ )o
其实，在 StudentDao 的 init 方法上，加上 @PostConstruct 注解（需要引入 javax.annotation.PostConstruct ）就可以了。用 @PostConstruct 标注 init 方法后， init 方法就会成为初始化方法，而在 Spring 完成依赖注入后被 Spring 调用。也就是说，此注解与前面提到的 init-method 属性的用途类似，读者可自行尝试一下。
