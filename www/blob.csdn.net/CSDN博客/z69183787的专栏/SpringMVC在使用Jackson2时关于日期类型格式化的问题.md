# SpringMVC在使用Jackson2时关于日期类型格式化的问题 - z69183787的专栏 - CSDN博客
2017年11月28日 14:24:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2982
*本例程序使用Jackson2.9.0，jackson1.x的处理方式稍稍有些不同。
在基于Spring&SpringMVC的Web项目中，我们常使用Jackson(1.x/2.x)来增加程序对Json格式的数据的支持。
因此，在实际应用中有个常见的需求：日期的格式化。
假设，User对象有个Date类型的属性birthday：
```
class User implements Serializable {
    
    private Date birthday;
    //...
}
```
程序支持如下api请求，而我们希望在返回Json格式的User资料时，对Date类型的birthday进行一下格式化。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
@Controller
class UserAction {
    
    @RequestMapping("/user/find/{id}")
    public @ResponseBody User getUserById(@PathVariable("id") int id) {
        return userService.getUserById(id);
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
实现上述需求大体有两种常用的方式：
1.使用@JsonFormat注解
该方法只需在关键字段加上@JsonFormat注解即可，如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
class User implements Serializable {
    
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss", timezone = "GMT+8")
    private Date birthday;
    //...
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
参数解释：pattern - 格式，timezone - 时区
2.设置MappingJackson2HttpMessageConverter的objectMapper
该方法主要对json数据转换时用到的HttpMessageConverter进行一些设置，进一步讲就是objectMapper在对日期类型数据序列化时设置成统一的pattern，配置如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<mvc:annotation-driven>
    <mvc:message-converters>
        <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
            <property name="objectMapper">
                <bean class="com.fasterxml.jackson.databind.ObjectMapper">
                    <property name="dateFormat">
                        <bean class="java.text.SimpleDateFormat">
                            <constructor-arg type="java.lang.String" value="yyyy-MM-dd HH:mm:ss" />
                        </bean>
                    </property>
                </bean>
            </property>
        </bean>
    </mvc:message-converters>
</mvc:annotation-driven>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
或者：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<bean id="objectMapper" class="org.springframework.http.converter.json.Jackson2ObjectMapperFactoryBean">
    <property name="simpleDateFormat" value="yyyy-MM-dd HH:mm:ss" />
</bean>
<mvc:annotation-driven>
    <mvc:message-converters>
        <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
            <property name="objectMapper" ref="objectMapper" />
        </bean>
    </mvc:message-converters>
</mvc:annotation-driven>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
方法1使用起来方便灵活，但如果有大量需要统一设置的字段属性，那么推荐使用方法2。或者两种方法混合使用，作用优先级：方法1 > 方法2。
