# SpringMVC注解@initbinder解决类型转换问题 - z69183787的专栏 - CSDN博客
2016年08月20日 14:22:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2081
在使用SpringMVC的时候，经常会遇到表单中的日期字符串和JavaBean的Date类型的转换，而SpringMVC默认不支持这个格式的转换，所以需要手动配置，自定义数据的绑定才能解决这个问题。
在需要日期转换的Controller中使用SpringMVC的注解`@initbinder`和Spring自带的`WebDateBinder`类来操作。
WebDataBinder是用来绑定请求参数到指定的属性编辑器.由于前台传到controller里的值是String类型的，当往Model里Set这个值的时候，如果set的这个属性是个对象，Spring就会去找到对应的editor进行转换，然后再SET进去。
代码如下：
```cpp
@InitBinder  
public void initBinder(WebDataBinder binder) {  
    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");  
    dateFormat.setLenient(false);  
    binder.registerCustomEditor(Date.class, new CustomDateEditor(dateFormat, true));  
}
```
需要在SpringMVC的配置文件加上
```
<!-- 解析器注册 -->  
<bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">  
    <property name="messageConverters">  
        <list>  
            <ref bean="stringHttpMessageConverter"/>  
        </list>  
    </property>  
</bean>  
<!-- String类型解析器，允许直接返回String类型的消息 -->  
<bean id="stringHttpMessageConverter" class="org.springframework.http.converter.StringHttpMessageConverter"/>
```
换种写法
```
<mvc:annotation-driven>
    <mvc:message-converters>
        <bean class="org.springframework.http.converter.StringHttpMessageConverter">
            <constructor-arg value="UTF-8"/>
        </bean>
    </mvc:message-converters>
</mvc:annotation-driven>
```
拓展：
spring mvc在绑定表单之前，都会先注册这些编辑器，Spring自己提供了大量的实现类，诸如CustomDateEditor ，CustomBooleanEditor，CustomNumberEditor等许多，基本上够用。
使用时候调用WebDataBinder的registerCustomEditor方法
*registerCustomEditor*源码：
```cpp
public void registerCustomEditor(Class<?> requiredType, PropertyEditor propertyEditor) {
    getPropertyEditorRegistry().registerCustomEditor(requiredType, propertyEditor);
}
```
第一个参数requiredType是需要转化的类型。
第二个参数PropertyEditor是属性编辑器，它是个接口，以上提到的如CustomDateEditor等都是继承了实现了这个接口的PropertyEditorSupport类。
我们也可以不使用他们自带的这些编辑器类。
我们可以自己构造：
```
import org.springframework.beans.propertyeditors.PropertiesEditor;
public class DoubleEditor extends PropertyEditorSupport {
    @Override
    public void setAsText(String text) throws IllegalArgumentException {
        if (text == null || text.equals("")) {
            text = "0";
        }
        setValue(Double.parseDouble(text));
    }
    @Override
    public String getAsText() {
        return getValue().toString();
    }
}
```
