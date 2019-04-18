# IntroductionInterceptor - z69183787的专栏 - CSDN博客
2015年09月25日 15:38:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：751
對於之前介紹過的[Before
 Advice](http://www.openhome.cc/Gossip/SpringGossip/BeforeAdvice.html)、[After
 Advice](http://www.openhome.cc/Gossip/SpringGossip/AfterAdvice.html)、[Around
 Advice](http://www.openhome.cc/Gossip/SpringGossip/AroundAdvice.html)、[Throw
 Advice](http://www.openhome.cc/Gossip/SpringGossip/ThrowAdvice.html)，從使用者的角度來看，它們「影響了目標物件上某些方法的行為」，例如讓某些方法看來似乎增加了一些記錄的動作。
Introduction是個特別的Advice，從使用者的角度來看，它「影響了目標物件的行為定義，直接增加了目標物件的職責（具體來說就是增加了可 操作的方法）」，例如讓某個已定義好的物件，在不修改該物件之類別檔案的情況下，卻可以增加一些額外的操作方法到物件之上。
就Java程式語言類別設計的觀點來說，動態為物件增加可操作的方法顯得不可思議，事實上在Spring AOP中，您可以透過實作org.springframework.aop.IntroductionInterceptor來實現Introduction。
IntroductionInterceptor繼承了MethodInterceptor與DynamicIntroductionAdvice介面， 其中implementsInterface()方法（繼承自DynamicIntroductionAdvice）如果返回true的話，表示目前的 IntroductionInterceptor實作了給定的介面（也就是要額外增加行為的介面），此時您要使用invoke()呼叫介面上的方法，讓目
 標物件執行額外的行為，您不可能使用MethodInvocation的proceed()方法，因為您要執行的是物件上原來沒有的行為，呼叫 proceed()方法沒有意義。
從文字上來理解Introduction會比較抽象，舉個實際的例子來說，假設您的系統中已經有以下的類別：
- ISome.java
```
package onlyfun.caterpillar;
public interface ISome {
    public void doSome();
}
```
- Some.java
```
package onlyfun.caterpillar;
public class Some implements ISome { 
    public void doSome() {
        System.out.println("原來物件的職責。。。");
    }
}
```
您希望在不修改原始檔案的情況下，為Some類別增加一些可操作的方法，也許您甚至連原始碼檔案都沒有，只有.class檔案，您唯一知道的也許是他們的API說明，在不對它們作出修改的情況下，您希望Some類別可以增加doOther()方法。
在Spring中，您可以藉由實作IntroductionInterceptor介面來完成上面的任務，首先您為doOther()方法建立介面：
- IOther.java
```
package onlyfun.caterpillar; 
public interface IOther {  
    public void doOther();
}
```
接著定義一個OtherIntroduction類別實作IntroductionInterceptor介面，並在實作IntroductionInterceptor介面的同時，也實作IOther介面，例如：
- OtherIntroduction.java
```
package onlyfun.caterpillar;
import org.aopalliance.intercept.MethodInvocation;
import org.springframework.aop.IntroductionInterceptor;
public class OtherIntroduction 
              implements IntroductionInterceptor, IOther {   
    // 是否實作自IOther介面
    public boolean implementsInterface(Class clazz) {
        return clazz.isAssignableFrom(IOther.class);
    }
    
    public Object invoke(MethodInvocation methodInvocation) 
                                      throws Throwable {
        // 如果呼叫的方法來自IOther介面的定義
        if(implementsInterface(
            methodInvocation.getMethod().getDeclaringClass())) {
            // 呼叫執行額外加入（mixin）的行為
            return methodInvocation.getMethod().
                    invoke(this, methodInvocation.getArguments());
        }
        else {
            return methodInvocation.proceed();
        }
    }
    
    public void doOther() {
        System.out.println("增加的職責。。。");
    }
}
```
接著您要在Bean定義檔中將Introduction縫合至Some物件之上，使用org.springframework.aop.support.DefaultIntroductionAdvisor就可以了，例如：
- beans-config.xml
```
<?xml version="1.0" encoding="UTF-8"?> 
<!DOCTYPE beans PUBLIC "-//SPRING/DTD BEAN/EN" 
  "http://www.springframework.org/dtd/spring-beans.dtd"> 
<beans> 
    <bean id="some" 
          class="onlyfun.caterpillar.Some"/> 
    <bean id="otherIntroduction" 
          class="onlyfun.caterpillar.OtherIntroduction"/> 
          
    <bean id="otherAdvisor" 
          class="org.springframework.aop.support.DefaultIntroductionAdvisor"> 
          <constructor-arg index="0">
              <ref bean="otherIntroduction"/>
          </constructor-arg>  
          <constructor-arg index="1">
              <value>onlyfun.caterpillar.IOther</value>
          </constructor-arg>  
    </bean>
   
    <bean id="proxyFactoryBean" 
          class="org.springframework.aop.framework.ProxyFactoryBean"> 
        <property name="proxyInterfaces"> 
            <value>onlyfun.caterpillar.ISome</value> 
        </property> 
        <property name="target"> 
            <ref bean="some"/> 
        </property> 
        <property name="interceptorNames"> 
            <list> 
                <value>otherAdvisor</value> 
            </list> 
        </property> 
    </bean> 
</beans>
```
DefaultIntroductionAdvisor在建構時，需要給它IntroductionInterceptor的實例，以及所要代理額外行為的介面，現在，來撰寫一個簡單的程式測試，從這個程式當中，您可以更進一步了解何謂為物件額外增加行為：
- SpringAOPDemo.java
```
package onlyfun.caterpillar;
import org.springframework.context.ApplicationContext;
import org.springframework.context.
              support.FileSystemXmlApplicationContext;
public class SpringAOPDemo {
    public static void main(String[] args) {
        ApplicationContext context = 
            new FileSystemXmlApplicationContext(
                    "beans-config.xml"); 
    
        ISome some = 
            (ISome) context.getBean("proxyFactoryBean"); 
    
        some.doSome();
    
        // 看來好像some物件動態增加了職責
        ((IOther) some).doOther();
    }
}
```
對於some所參考的物件來說，它原先並不會有doOther()方法可供操作，然而透過Spring AOP的Introduction機制，現在some所參考的物件多了doOther()方法可以操作。
