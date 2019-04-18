# spring+mybatis返回json日期处理（日期全局处理相差8个小时解决方法） - z69183787的专栏 - CSDN博客
2016年08月12日 17:08:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1914
个人分类：[SpringMvc																[Json转换-Jackson](https://blog.csdn.net/z69183787/article/category/2750399)](https://blog.csdn.net/z69183787/article/category/2176803)
```
<mvc:annotation-driven>
        <mvc:message-converters>
            <bean
                class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
                <property name="objectMapper">
                    <bean class="com.fasterxml.jackson.databind.ObjectMapper">
                        <!-- 处理responseBody 里面日期类型 -->
                        <property name="dateFormat">
                            <bean class="java.text.SimpleDateFormat">
                                <constructor-arg type="java.lang.String" value="yyyy-MM-dd HH:mm:ss" />
                            </bean>
                        </property>
                        <!-- 时区指定 -->
                        <property name="timeZone" value="GMT+8" />
                        <!-- 为null字段时不显示 -->
                        <property name="serializationInclusion">
                            <value type="com.fasterxml.jackson.annotation.JsonInclude.Include">NON_NULL</value>
                        </property>
                    </bean>
                </property>
            </bean>
        </mvc:message-converters>
    </mvc:annotation-driven>
```
