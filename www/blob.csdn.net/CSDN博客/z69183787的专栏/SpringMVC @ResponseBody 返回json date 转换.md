# SpringMVC @ResponseBody 返回json date 转换 - z69183787的专栏 - CSDN博客
2016年07月07日 17:26:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2402
配置文件
```
<mvc:annotation-driven>  
    <!-- 处理responseBody 里面日期类型 -->  
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
```
```
@ResponseBody
@RequestMapping("main.json")
```
