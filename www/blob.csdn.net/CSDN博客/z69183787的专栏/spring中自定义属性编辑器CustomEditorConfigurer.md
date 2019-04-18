# spring中自定义属性编辑器CustomEditorConfigurer - z69183787的专栏 - CSDN博客
2016年10月10日 19:23:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：685
什么是属性编辑器，作用？ 
* 自定义属性编辑器，spring配置文件中的字符串转换成相应的对象进行注入 
spring已经有内置的属性编辑器，我们可以根据需求自己定义属性编辑器 
* 如何定义属性编辑器？ 
* 继承PropertyEditorSupport类，覆写setAsText()方法，参见：UtilDatePropertyEditor.java 
* 将属性编辑器注册到spring中，参见：applicationContext.xml 
比如： 
有一个类里面有一个Date属性 
- publicclass Bean1 {  
- private Date dateValue;  
- publicvoid setDateValue(Date dateValue) {  
- this.dateValue = dateValue;  
-     }  
- }  
applicationContext.xml配置文件如下： 
- <!--将bean1中的Date赋值2008-08-15，spring会认为2008-08-15是String，无法转换成Date，会报错！-->  
- <bean id="bean1"class="com.bjsxt.spring.Bean1">  
-       <property name="dateValue">  
-          <value>2008-08-15</value>  
-     </property>  
- </bean>  
- <!-- 于是定义属性编辑器 -->        
- <bean id="customEditorConfigurer"class="org.springframework.beans.factory.config.CustomEditorConfigurer">  
-     <property name="customEditors">  
-         <map>  
-             <entry key="java.util.Date">  
-                 <bean class="com.bjsxt.spring.UtilDatePropertyEditor">  
-                                         <!--干脆把format也注入，灵活处理格式-->  
-                     <property name="format" value="yyyy-MM-dd"/>  
-                 </bean>  
-             </entry>  
-         </map>  
-     </property>  
- </bean>     
UtilDatePropertyEditor.java 如下，必须继承java.beans.PropertyEditorSupport类，覆写setAsText()方法 
- import java.beans.PropertyEditorSupport;  
- import java.text.ParseException;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- 
- /**
-  * java.util.Date属性编辑器 
-  * @author Administrator
-  *
-  */
- publicclass UtilDatePropertyEditor extends PropertyEditorSupport {  
- 
- private String format="yyyy-MM-dd";  
- 
- @Override
- publicvoid setAsText(String text) throws IllegalArgumentException {  
-         System.out.println("UtilDatePropertyEditor.saveAsText() -- text=" + text);  
- 
-         SimpleDateFormat sdf = new SimpleDateFormat(format);  
- try {  
-             Date d = sdf.parse(text);  
- this.setValue(d);  
-         } catch (ParseException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- publicvoid setFormat(String format) {  
- this.format = format;  
-     }  
- 
- }  
这样就可以完成正确解析了，注意customEditors是Spring的类CustomEditorConfigurer提供的属性，是一个Map,里面存放的都是自定义的编辑器（customEditors），比如这里存放的是UtilDatePropertyEditor日期编辑器，看CustomEditorConfigurer源码就知道了。 
测试一下： 
- import org.springframework.beans.factory.BeanFactory;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- 
- import junit.framework.TestCase;  
- 
- publicclass InjectionTest extends TestCase {  
- 
- private BeanFactory factory;  
- 
- @Override
- protectedvoid setUp() throws Exception {  
-         factory = new ClassPathXmlApplicationContext("applicationContext.xml");   
-     }  
- 
- publicvoid testInjection1() {  
-         Bean1 bean1 = (Bean1)factory.getBean("bean1");  
-         System.out.println("bean1.dateValue=" + bean1.getDateValue());  
-     }  
- 
- }  
能打印出日期就OK了。
