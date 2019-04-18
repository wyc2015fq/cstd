# springMvc接受日期类型参数处理 - z69183787的专栏 - CSDN博客
2016年10月10日 18:48:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6676
这个问题，也即是springMvc如何进行参数类型的转换
以把client传过来一个String类型，转换为日期类型为例：
1.controller
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/**
     * 接收日期类型参数
     *     注意:
     *         springmvc 在接收日期类型参数时,如不做特殊处理 会出现400语法格式错误
     *  解决办法
     *      1.全局日期处理
     * 
     */
    
    @RequestMapping("/test")
    public String test(Date birthday){
        System.out.println(birthday);
        return "index";
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
2.自定义类型转换规则
SpringMvc提供了Converter接口，它支持从一个Object转换为另一个Object
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/**
 * 全局日期处理类
 * Convert<T,S>
 *         泛型T:代表客户端提交的参数 String
 *         泛型S:通过convert转换的类型
*/
public class DateConvert implements Converter<String, Date> {
    @Override
    public Date convert(String stringDate) {
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd");
        try {
            return simpleDateFormat.parse(stringDate);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return null;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
3.注册自定义的类型转换类
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
　　<!-- 第三步：注册处理器映射器/处理器适配器 ，添加conversion-service属性-->
    <mvc:annotation-driven conversion-service="conversionService"/>
   
    <!-- 第二步： 创建convertion-Service ，并注入dateConvert-->
    <bean id="conversionService" class="org.springframework.format.support.FormattingConversionServiceFactoryBean">
        <property name="converters">
            <set>
                <ref bean="dateConvert"/>
            </set>
        </property>
    </bean>
    <!-- 第一步:  创建自定义日期转换规则 -->   
    <bean id="dateConvert" class="zpark.convert.DateConvert"/>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4.地址栏访问
http://localhost:9999/date/test2?birthday=1990-01-02
参考：http://blog.csdn.net/renhui999/article/details/9837897
ps:测试了很久才成功，失败的次数太多就不记录下来了，以上只是记录了测试正确的步骤
