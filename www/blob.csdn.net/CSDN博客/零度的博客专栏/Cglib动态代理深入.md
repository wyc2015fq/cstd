# Cglib动态代理深入 - 零度的博客专栏 - CSDN博客
2016年05月26日 10:38:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：633
**1、CGLIB简介**
CGLIB是一个强大的高性能的代码生成包 .它广泛的被许多AOP的框架使用,例如Spring AOP .诸如 EasyMock和JMock 等通过模仿对象来测试java代码的包都使用CGLIB.他们都通过使用CGLIB来为那些没有实现接口的类创建代理, 流行ORM框架Hibernate 亦使用CGLIB来实现延迟加载和单端映射(新版本Hibernate已直接依赖更底层的ASM), CGLIB底层通过一个小而快的字节码处理框架ASM ,来转换字节码并生成新的类.ASM是低級的字节码生成工具,使用ASM已经近乎接近使用Java
 bytecode编程,而使用CGLIB更像是对ASM进行的高级化封装. 
**2、CGLIB作用 **
       JDK内置的动态代理用起来非常简单, **但它有一个限制,就是使用动态代理的类必须实现一个或多个接口 **.注意必须是接口,如果想代理没有实现接口的类,JDK内置的动态代理就显得非常无力,此时CGLIB就显出了它的魅力,而且CGLIB因為使用直接生成字节码的方式,效率更高 
**3、Cglib创建代理对象**
**目标对象类：**
```
package zmx.dynamic.proxy.test;
public class RayTest {
	
	public String execute(){
		System.out.println("execute it ...");
		return "execute";
	}
	
	public String action(){
		System.out.println("action it ...");
		return "action";
	}
}
```
创建代理对象测试：
```
package zmx.dynamic.proxy.test;
import java.lang.reflect.Method;
import net.sf.cglib.proxy.Callback;
import net.sf.cglib.proxy.CallbackFilter;
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;
import org.junit.Assert;
import org.junit.Test;
public class CglibMainTest {
	// 第一种写法,使用Enhancer静态create方法
	@Test
	public void testCGLIBProxy01() {
	    /*
	     * create有多种重载方式,此重载第一个参数为超类(本类)的类型,
	     * 若然需要动态代理的类没有实现接口,就需要填写第一个参数,
	     * 第二个参数为Class类型的数组,当动态代理的类有实现接口,
	     * 可以选择性填写第二个参数,否则为null
	     */
		RayTest test = (RayTest) Enhancer.create(RayTest.class, null,
				new MethodInterceptor() {
					@Override
					public Object intercept(Object arg0, Method arg1,
							Object[] arg2, MethodProxy arg3) throws Throwable {
						// 真实主题
						RayTest target = new RayTest();
						// 动态代理增加的logic
						String before = "before ";
						// 使用MethodProxy调用效率更高
						Object str = arg3.invoke(target, arg2);
						String after = " after";
						return before + str + after;
					}
				});
		System.out.println(test.execute());
		Assert.assertEquals("before execute after", test.execute());
	}
	
	// 第二种写法,创建Enhancer对象
	  @Test
	  public void testCGLIBProxy2() {
	    // 创建Enhancer类
	    Enhancer enhancer = new Enhancer();
	    // 当类没有实现接口而又需要动态代理,使用setSuperclass
	    enhancer.setSuperclass(RayTest.class);
	    // 当类实现了接口需要动态代理,使用setInterface
	    enhancer.setInterfaces(new Class[] {});
	    enhancer.setCallback(new MethodInterceptor() {
	      // 真实主题
	      RayTest test = new RayTest();
	      @Override
	      public Object intercept(Object arg0, Method method, Object[] args,
	          MethodProxy arg3) throws Throwable {
	        // 动态代理增加的logic
	        String before = "before ";
	        // 使用MethodProxy调用效率更高
	        Object str = arg3.invoke(test, args);
	        String after = " after";
	        return before + str + after;
	      }
	    });
	    System.out.println( ((RayTest)enhancer.create()).execute());
	    Assert.assertEquals("before execute after", ((RayTest)enhancer.create()).execute());
	  }
}
```
       通过上面的代码可以看出. 真正处理代理业务逻辑也就是和JDK动态代理中InvocationHandler中invoke方法类似，使用的是enhancer.setCallback传递Callback接口实现来进行的。 上面例子中我们使用 **MethodInterceptor **,MethodInterceptor能满足几乎所有拦截的业务需求。但一些已经普遍存在的动态代理使用方式,例如使用动态代理实现延迟加载,CGLIB为我们提供了更方便和简化的实现 。
```
public class TestMain {
  private Enhancer enhancer;
  @Before
  public void init() {
    this.enhancer = new Enhancer();
    enhancer.setSuperclass(RayTest.class);
  }
  @Test
  public void testCGLIBUseSuperClass() {
    // NoOp实现类会使用默认的父类实现,没增加任何logic
    enhancer.setCallback(NoOp.INSTANCE);
    RayTest test = (RayTest) enhancer.create();
    Assert.assertNotNull(test);
  }
  @Test
  public void testCGLIBLazyObject() {
    //实现LazyLoader接口,在创建对象时可以增加业务logic和创建对象的子类等
    enhancer.setCallback(new LazyLoader() {
      @Override
      public Object loadObject() throws Exception {
        final RayTest lazyTest = new RayTest();
        return lazyTest;
      }
    });
    RayTest test = (RayTest) enhancer.create();
    Assert.assertNotNull(test);
  }
}
```
**      由于CGLIB可以不需要实现接口来实现动态代理 。**其原理是通过字节码生成类的一个子类来完成的，**那就有可能出现需要动态代理对象不存在一个无参构造函数 **,那么CGLIB在生成子类并实例化时将会产生错误。此时，可以使用create的重载方式,参数1是构造函数参数类型,参数2是值。
```
public class TestMain {
  /*
   * 我們假设RayTest类不存在默认构造函数,
   * 只提供需要传入 String和Integer作为参数的构造函数
   */
  private Enhancer enhancer;
  @Before
  public void init() {
    this.enhancer = new Enhancer();
    enhancer.setSuperclass(RayTest.class);
  }
  public void testNotExistDefaultConstructor() {
    enhancer.setCallback(NoOp.INSTANCE);
    // 可以使用create的重载方式,参数1是构造函数参数类型,参数2是值
    RayTest test = (RayTest) enhancer.create(new Class[] { String.class,
        Integer.class }, new Object[] { "Hello World", 100 });
    Assert.assertNotNull(test);
  }
}
```
         在我们真实开发当中,在使用动态代理进行方法请求拦截时,可能会需要判断调用的方法然后决定拦截的逻辑, **也就是同一个代理类在调用不同的方法时拦截的逻辑都不相同 **,CGLIB提供了 **CallbackFilter **来帮助我们实现这一功能 。
```
public class TestMain {
  private Enhancer enhancer;
  @Before
  public void init() {
    this.enhancer = new Enhancer();
    enhancer.setSuperclass(RayTest.class);
  }
  @Test
  public void testCGLIBCallbackFilter() {
    // 创建callback1
    Callback callback1 = new MethodInterceptor() {
      // 真是主题类
      RayTest test = new RayTest();
      @Override
      public Object intercept(Object obj, Method method, Object[] args,
          MethodProxy proxy) throws Throwable {
        String before = "callback1 before ";
        Object str = proxy.invoke(test, args);
        String after = " callback1 after";
        return before + str + after;
      }
    };
    // 创建callback2
    Callback callback2 = new MethodInterceptor() {
      // 真是主题类
      RayTest test = new RayTest();
      @Override
      public Object intercept(Object obj, Method method, Object[] args,
          MethodProxy proxy) throws Throwable {
        String before = "callback2 before ";
        Object str = proxy.invoke(test, args);
        String after = " callback2 after";
        return before + str + after;
      }
    };
    // 使用setCallbacks设置多个Callback
    enhancer.setCallbacks(new Callback[] { callback1, callback2 });
    enhancer.setCallbackFilter(new CallbackFilter() {
      static final int EXECUTE_METHOD = 0;
      static final int OTHER_METHOD = 1;
      /*
       * accept需要返回一個int类型,
       * 该int类型为上文中setCallbacks设置的多个
       * Callback处理逻辑的数组的下标,上文中设置了两个Callback,
       * 分别为callback1和callback2
       */
      @Override
      public int accept(Method method) {
        /*
         * Method参数代表代理类的执行方法, 
         * 以下logic为 判断执行方法名称是否为execute,
         * 是则执行callback1,也就是数组下标为0的逻辑, 
         * 否则执行Other逻辑
         */
        if ("execute".equals(method.getName()))
          return EXECUTE_METHOD;
        else
          return OTHER_METHOD;
      }
    });
    RayTest test = (RayTest) enhancer.create();
    String executeResult = test.execute();
    Assert.assertEquals("callback1 before execute callback1 after",
        executeResult);
    String otherResult = test.action();
    Assert.assertEquals("callback2 before action callback2 after",
        otherResult);
  }
}
```
**4、总结**
       在传统使用JDK动态代理时,会受限于必须实现接口而带来不便,若然该类是自己编写的话还可以抽象出多一层接口,若然该类是别人编写好并且已经正在生产使用,那就没有任何改动的理由 ,CGLIB帮助我们解决了没实现接口的继承问题,而且使用简单,提供了更多的功能例如CallbackFilter,这些都是JDK动态代理所没有的,而且效率也要高于JDK动态代理 
