# 使用Spring的HibernateDaoSupport时的getSession()3个方法的区别 - z69183787的专栏 - CSDN博客
2013年01月29日 20:27:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3188
**在Hibernate中的两种session打开方式**
**this.getsession实际上是调用了父类中的方法获得session。使用spring管理hibernate的SessionFactory的时候，这个方法会从session池中拿出一session.这样做有可能有问题，就是超session池连接数的时候，spring无法自动的关闭session。 不推荐使用**
**this.getHibernateTemplate().getSessionFactory().getCurrentSession()从spring管理的sessionFactory中创建一个绑定线程的session.spring会根据该线程的执行情况来自动判断是关闭session还是延迟关闭。这样做可以避免手动的管理实务，同时一个线程最多开启和关闭一次session又可以提高程序的性能。 极力推荐使用这种方法**
**this.getHibernateTemplate().getSessionFactory().OpenSession。这种方法从spring管理的sessionFactory中创建一个session，此session不是线程绑定的。当执行完一个实务的时候自动关闭session.这种方法不用手动管理实务，但是同一个线程多次的开启和关闭session,浪费系统资源和影响执行效率，正常情况下还是不要用了。**
**本人自己测试下来发现**
```java
/**
 * 
 */
package test;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Scope;
import org.springframework.orm.hibernate3.HibernateTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import test.archive.model.TodoItem;
/** 
 * @ClassName: SampleService 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2012-12-12 上午10:10:38 
 *  
 */
@Transactional(propagation=Propagation.REQUIRED,rollbackFor=Exception.class)
@Service("sampleService")
@Scope("prototype")
public class SampleService implements ISample{
	
	private HibernateTemplate h;
	
	@SuppressWarnings("unchecked")
	public void test() {
		//List list = this.h.find("from TodoItem t where 1=1");
		TodoItem t = new TodoItem();
		t.setApp("zszszs");
		//this.h.getSessionFactory().getCurrentSession().save(t);
		this.h.save(t);
		System.out.println("----------------------------------------");
		//System.out.println(list.size());
		
		/**
		 * TodoItem t = new TodoItem();
		t.setApp("zszszs");
		Session s = this.h.getSessionFactory().openSession();
		Transaction tt = s.beginTransaction();
		s.save(t);
		tt.commit();
		s.flush();
		s.close();
		System.out.println("----------------------------------------");
		 * */
	}
	public HibernateTemplate getH() {
		return h;
	}
	@Autowired(required=false)
	public void setH(@Qualifier(value="hibernateTemplate")HibernateTemplate h) {
		this.h = h;
	}
	
	
}
```
**this.h.save(t);**
****可自动关闭session，与是否声明事务代理无关。无需显示提交事务。****
**this.h.getSessionFactory().getCurrentSession().save(t);**
**需声明事务代理（原因见[http://blog.csdn.net/z69183787/article/details/8553400](http://blog.csdn.net/z69183787/article/details/8553400)），否则无法获得session，无需显示提交事务。**
****this.h.getSessionFactory().openSession().save(t);****
**无法自动关闭session，需显示关闭 或 声明事务代理才能够关闭session，**
**而且做add与update操作需要显示提交事务。**
