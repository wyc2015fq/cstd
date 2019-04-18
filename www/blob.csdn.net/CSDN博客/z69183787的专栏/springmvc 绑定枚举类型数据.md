# springmvc 绑定枚举类型数据 - z69183787的专栏 - CSDN博客
2017年09月20日 15:38:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2160
在开发的过程中，会涉及到大量的枚举类型数据，我们都知道，Springmvc本身能自动转换很多的数据类型，也支持你自定义转换类型，非常灵活。
Enum类型的变量，在form表单里，传过来的value是个String类型值，那么我们需要让它自动匹配到对应的Enum类上去。
配置如下：
spring配置：
<!-- 枚举类型自动绑定 -->
    <bean id="conversionService"
        class="org.springframework.context.support.ConversionServiceFactoryBean">
        <property name="converters">
            <set>
                <bean class="com.ziwow.pay.ws.convert.StringToEnumConverterFactory" />
            </set>
        </property>
    </bean>
另外这里加上
<mvc:annotation-driven  conversion-service="conversionService"/>
StringToEnumConverterFactory是一个通用的转换类，如果你要实现自己定制的转换类，也可以自己去实现，按照规范，实现ConverterFactory即可。
代码如下
**[java]**[view
 plain](http://blog.csdn.net/china_bobo/article/details/43016415#)[copy](http://blog.csdn.net/china_bobo/article/details/43016415#)
- @SuppressWarnings("unchecked")  
- finalclass StringToEnumConverterFactory implements
-         ConverterFactory<String, Enum> {  
- 
- public <T extends Enum> Converter<String, T> getConverter(  
-             Class<T> targetType) {  
- returnnew StringToEnum(targetType);  
-     }  
- 
- privateclass StringToEnum<T extends Enum> implements Converter<String, T> {  
- 
- privatefinal Class<T> enumType;  
- 
- public StringToEnum(Class<T> enumType) {  
- this.enumType = enumType;  
-         }  
- 
- public T convert(String source) {  
- if (source.length() == 0) {  
- returnnull;  
-             }  
- return (T) Enum.valueOf(this.enumType, source.trim());  
-         }  
-     }  
- 
- }  
比如我有个Enum类，是GenderEnum类
**[java]**[view
 plain](http://blog.csdn.net/china_bobo/article/details/43016415#)[copy](http://blog.csdn.net/china_bobo/article/details/43016415#)
- publicenum GenderEnum{  
- BOY(1,"boy"),GIRL(2,"girl")  
- ......  
- }  
那我的表单里，gender对应的value，只要是BOY或者GIRL，它就能自动匹配绑定好了，如果你要根据id的值来绑定，那就修改上面的StringToEnumConverterFactory类，修改它的convert方法即可。
参考：http://haohaoxuexi.iteye.com/blog/1860732
