# Spring jackson message converter解析多余属性出错 - z69183787的专栏 - CSDN博客
2016年08月24日 17:16:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2395
个人分类：[SpringMvc																[Json转换-Jackson](https://blog.csdn.net/z69183787/article/category/2750399)](https://blog.csdn.net/z69183787/article/category/2176803)
问题：
实体类2个属性，传递的json三个属性，报404错误，Bad request.
配置如下:
```
<mvc:annotation-driven>
        <mvc:message-converters register-defaults="true">
            <bean
                class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
                <property name="objectMapper">
                     <bean class="com.fasterxml.jackson.databind.ObjectMapper">
                        <property name="serializationInclusion">
                            <value type="com.fasterxml.jackson.annotation.JsonInclude.Include">NON_NULL</value>
                        </property>
                    </bean>
                </property>
            </bean>
        </mvc:message-converters>
    </mvc:annotation-driven>
```
网上找的解决:
1.自定义mapper 配置到spring.xml中没卵用 
public class JacksonConvert extends ObjectMapper {
```
private static final long serialVersionUID = 1L;
public JacksonConvert() {
    super();
    this.setSerializationInclusion(Include.NON_NULL);
    this.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);//没生效
}
```
}
2.配置xml属性配置一个DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES，没弄好到底怎么配置的。失败
### 最终解决:在你需要转换的bean上注解
@JsonIgnoreProperties(ignoreUnknown
 = true)
