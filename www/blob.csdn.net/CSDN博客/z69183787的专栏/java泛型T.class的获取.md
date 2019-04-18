# java泛型T.class的获取 - z69183787的专栏 - CSDN博客
2014年12月25日 10:24:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：20626
 很早之前写过[利用泛型和反射机制抽象DAO](http://blog.csdn.net/ykdsg/archive/2008/12/28/3628734.aspx) ，对其中获取子类泛型的class一直不是很理解。关键的地方是HibernateBaseDao的构造方法中的
**[java]**[view
 plain](http://blog.csdn.net/ykdsg/article/details/5472591#)[copy](http://blog.csdn.net/ykdsg/article/details/5472591#)
- Type genType = getClass().getGenericSuperclass();   
- Type[] params = ((ParameterizedType) genType).getActualTypeArguments();   
- entityClass =  (Class)params[0];   
但是这个相对子类才会有用，而且那篇文章的HibernateBaseDao还不是abstract，如果一不小心实例化的话就会报异常。感觉java中通过反射取泛型的class还是挺复杂的，不过一旦取到的话就可以做很多事情了。
**改进的例子和测试：**
1.先定义接口（这里写的比较简单就一个方法，具体可以再增加）
**[java]**[view
 plain](http://blog.csdn.net/ykdsg/article/details/5472591#)[copy](http://blog.csdn.net/ykdsg/article/details/5472591#)
- publicinterface BaseDao<T> {  
-     T get(String id);  
- 
- }  
2.定义抽象类实现接口
**[java]**[view
 plain](http://blog.csdn.net/ykdsg/article/details/5472591#)[copy](http://blog.csdn.net/ykdsg/article/details/5472591#)
- import java.lang.reflect.ParameterizedType;  
- import java.lang.reflect.Type;  
- 
- publicabstractclass HibernateBaseDao<T> implements BaseDao<T> {  
- private Class<T> entityClass;  
- 
- /**
-      * 这个通常也是hibernate的取得子类class的方法
-      * 
-      * @author "yangk"
-      * @date 2010-4-11 下午01:51:28
-      */
- public HibernateBaseDao() {  
-         Type genType = getClass().getGenericSuperclass();  
-         Type[] params = ((ParameterizedType) genType).getActualTypeArguments();  
-         entityClass = (Class) params[0];  
-     }  
- 
- @Override
- public T get(String id) {  
- try {  
- return entityClass.newInstance();  
-         } catch (InstantiationException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         } catch (IllegalAccessException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
- returnnull;  
-     }  
- 
- }  
子类的构造函数会调用父类的构造函数，所以当子类实例化的时候，父类的entityClass 已经得到了T.class。
3.定义一个entity
**[java]**[view
 plain](http://blog.csdn.net/ykdsg/article/details/5472591#)[copy](http://blog.csdn.net/ykdsg/article/details/5472591#)
- publicclass Entity {  
- 
- private String name;  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- }  
4.定义entity的DAO
**[c-sharp]**[view
 plain](http://blog.csdn.net/ykdsg/article/details/5472591#)[copy](http://blog.csdn.net/ykdsg/article/details/5472591#)
- publicclass EntityDao extends HibernateBaseDao<Entity> {  
- 
- publicvoid otherOperation() {  
- 
-     }  
- 
- }  
可以在里面定义父类没有的，针对具体子类特殊的方法。
5.测试
**[java]**[view
 plain](http://blog.csdn.net/ykdsg/article/details/5472591#)[copy](http://blog.csdn.net/ykdsg/article/details/5472591#)
- importstatic org.junit.Assert.assertNotNull;  
- 
- import org.junit.After;  
- import org.junit.AfterClass;  
- import org.junit.Before;  
- import org.junit.BeforeClass;  
- import org.junit.Test;  
- 
- publicclass JunitTest {  
- @BeforeClass
- publicstaticvoid setUpClass() throws Exception {  
-     }  
- 
- @AfterClass
- publicstaticvoid tearDownClass() throws Exception {  
-     }  
- 
- @Before
- publicvoid setUp() {  
-     }  
- 
- @After
- publicvoid tearDown() {  
-     }  
- 
- /**
-      * Test of getEClass method, of class tmp.
-      */
- @Test
- publicvoid testNewClass() {  
-         EntityDao testDao = new EntityDao();  
-         Entity e = testDao.get(null);  
-         assertNotNull(e);  
-     }  
- 
- }  
运行，可以看到测试顺利通过。
注意：上面子类DAO的写法public class EntityDao extends HibernateBaseDao<Entity>
一定要在父类后面带上泛型，负责编译就会出错。
 附：这是泛型擦拭法使得Generic无法获取自己的Generic Type类型。实际上BadClass<String>()实例化以后Class里面就不包括T的信息了，对于Class而言T已经被擦拭为Object，而真正的T参数被转到使用T的方法（或者变量声明或者其它使用T的地方）里面（如果没有那就没有存根），所以无法反射到T的具体类别，也就无法得到T.class。而getGenericSuperclass()是Generic继承的特例，对于这种情况子类会保存父类的Generic参数类型，返回一个ParameterizedType，这时可以获取到父类的T.class了，这也正是子类确定应该继承什么T的方法。
