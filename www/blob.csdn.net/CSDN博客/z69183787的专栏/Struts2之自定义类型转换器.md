# Struts2之自定义类型转换器 - z69183787的专栏 - CSDN博客
2015年05月21日 21:21:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：526
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)

Struts2自定义类型转换器分为局部类型转换器和全局类型转换器
(1)局部类型转换器 
如果页面传来一个参数reg.action?birthday=2010-11-12到后台action，然后属性用date类型是可以接收到的，但是如果传的是20101112这样类型的字符串，用date类型是获取不到，并且会出现错误的，struts2提供了一种类型转换器供我们使用。
以下为局部类型转换器的开发步骤
a.首先要写一个类来继承DefaultTypeConverter
b.然后覆盖convertValue这个方法，在里面进行数据转型 
c.在action类所在的包下放置ActionClassName-conversion.properties文件，ActionClassName是类名，后面的-conversion.properties是固定的写法， 
如：HelloWorldAction-conversion.properties
d.Properties文件里面的内容为：属性名称=类型转换器的全类名(既包名.类名)
如：birthday=com.ljq.type.converter.DateTypeConverter
(2)全局类型转换器 
如果业务需求所有的日期都要转换，则可以使用全局类型转换器，只要在src根目录下面放置xwork-conversion.properties文件,并且properties文件中的内容为： 
待转换的类型=类型转换器的全类名 
如：java.util.Date = com.type.Converter.DateTypeConverter 即可 
![](http://pic002.cnblogs.com/images/2011/270324/2011031623074589.png)
代码
Action类
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.ljq.action;
import java.util.Date;
public class HelloWorldAction
 {
// 日期
private Date birthday;
// 枚举
private Gender gender;
public String execute() {
return "success";
}
public Date getBirthday() {
return birthday;
}
public void setBirthday(Date
 birthday) {
System.out.println("birthday="+birthday);
this.birthday = birthday;
}
// 自定义枚举
public enum Gender
 {
MAN,WOMEN
}
public Gender getGender() {
return gender;
}
public void setGender(Gender
 gender) {
System.out.println("gender="+gender);
this.gender = gender;
}
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
日期类型转换器
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.ljq.type.converter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;
import com.opensymphony.xwork2.conversion.impl.DefaultTypeConverter;
/**
* 日期自定义类型转换器
* 
* @author jiqinlin
*
*/
public class DateTypeConverter extends DefaultTypeConverter
 {
@SuppressWarnings("unchecked")
@Override
public Object convertValue(Map<String,
 Object> context, Object value,
Class toType) {
SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd
 HH:mm:ss");
try {
if (toType == Date.class)
 { // 当字符串向Date类型转换时
String[] params = (String[]) value;
return sdf.parseObject(params[0]);
} else if (toType == String.class)
 { // 当Date转换成字符串时
Date date=(Date)value;
return sdf.format(date);
}
} catch (java.text.ParseException e) {
e.printStackTrace();
}
return null;
}
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
枚举类型转换器
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.ljq.type.converter;
import java.util.Map;
import com.ljq.action.HelloWorldAction.Gender;
import com.opensymphony.xwork2.conversion.impl.DefaultTypeConverter;
/**
* 枚举自定义类型转换器
* 
* @author jiqinlin
*
*/
public class GenderTypeConverter extends DefaultTypeConverter{
@Override
public Object convertValue(Map<String,
 Object> context, Object value,
Class toType) {
if(toType==Gender.class){ //当字符串向Gender类型转换时
String[] params=(String[])value;
return Gender.valueOf(params[0]);
}else if (toType==String.class)
 { //当Gender转换成字符串时
Gender gender=(Gender)value;
return gender.toString();
}
return null;
}
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
配置类型转换器
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
测试路径
日期
http://localhost:8083/struts2/control/employee/list_execute.do?birthday=20110315
 23:34:55
枚举
http://localhost:8083/struts2/control/employee/list_execute.do?gender=WOMEN
局部类型转换器： HelloWorldAction-conversion.properties
birthday=com.ljq.type.converter.DateTypeConverter
gender=com.ljq.type.converter.GenderTypeConverter
全局类型转换器： xwork-conversion.properties
java.util.Date=com.ljq.type.converter.DateTypeConverter
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
在页面打印日期和枚举的值
  birthday=${birthday }
  gender=${gender }
