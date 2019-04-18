# fastjson使用 - z69183787的专栏 - CSDN博客
2017年01月10日 12:01:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1019
```java
package jms.test; 
import java.util.ArrayList; 
import java.util.Date; 
import java.util.List; 
import java.util.Map; 
import jms.model.Dept; 
import jms.model.Employee; 
import jms.model.Student; 
import org.junit.Test; 
import com.alibaba.fastjson.JSON; 
import com.alibaba.fastjson.JSONObject; 
import com.alibaba.fastjson.TypeReference; 
import com.alibaba.fastjson.serializer.SerializeConfig; 
import com.alibaba.fastjson.serializer.SerializerFeature; 
import com.alibaba.fastjson.serializer.SimpleDateFormatSerializer; 
import com.alibaba.fastjson.serializer.SimplePropertyPreFilter; 
public class TestFastjson { 
//fastjson序列化单个对象 与反序列化 
@Test 
public void test1() { 
Employee e = new Employee("001", "张三", 23, new Date()); 
//序列化 
String jsonStr = JSON.toJSONString(e); 
System.out.println(jsonStr); 
//反序列化 
Employee emp = JSON.parseObject(jsonStr, Employee.class); 
System.out.println(emp.getName()); 
} 
//fastjson序列化list集合 与反序列化 
@Test 
public void test2() { 
Employee e = new Employee("001", "张三", 23, new Date()); 
Employee e2 = new Employee("002", "李四", 29, new Date()); 
List<Employee> emps = new ArrayList<Employee>(); 
emps.add(e); 
emps.add(e2); 
//fastjson序列化list， 返回来的是一个json数组，由[]包含两个json 
String jsonArryStr = JSON.toJSONString(emps); 
System.out.println(jsonArryStr); 
// //反序列化 
//法一 
// List<Employee> empList = JSON.parseObject(jsonArryStr, new TypeReference<List<Employee>>(){} ); 
//法二 
List<Employee> empList = JSON.parseArray(jsonArryStr,Employee.class); 
for (Employee employee : empList) { 
System.out.println(employee.getName()); 
System.out.println(employee.getBirthDay()); 
} 
} 
//fastjson序列化复杂对象 与反序列化 
@Test 
public void test3() { 
Employee e = new Employee("001", "张三", 23, new Date()); 
Employee e2 = new Employee("002", "李四", 29, new Date()); 
List<Employee> emps = new ArrayList<Employee>(); 
emps.add(e); 
emps.add(e2); 
Dept dept = new Dept("d001", "研发部", emps); 
//序列化 
String jsonStr = JSON.toJSONString(dept); 
System.out.println(jsonStr); 
//反序列化 
Dept d = JSON.parseObject(jsonStr, Dept.class); 
System.out.println(d.getName()); 
//json转map 
//法一 
Map<String, Object> map1 = JSON.parseObject(jsonStr);//返回JSONObject，JSONObject实现Map<String, Object>接口 
//法二 
// Map<String, Object> map1 = (Map<String, Object>)JSON.parse(jsonStr); 
for (String key : map1.keySet()) { 
System.out.println(key + ":" + map1.get(key)); 
} 
} 
//fastjson 的 JSONObject的使用 
@Test 
public void test4() { 
Employee e = new Employee("001", "张三", 23, new Date()); 
//序列化 
String jsonStr = JSON.toJSONString(e); 
System.out.println(jsonStr); 
//反序列化 (可以和test1比较)  
JSONObject emp = JSON.parseObject(jsonStr, JSONObject.class); 
System.out.println(emp); 
System.out.println(emp.getString("name")); 
//再放一个Employee不存在的字段 
emp.put("salary", "8000"); 
System.out.println(emp.toJSONString()); 
System.out.println(emp.get("salary")); 
} 
//fastjson序列化字符串 
@Test 
public void test5(){ 
List<String> strs = new ArrayList<String>(); 
strs.add("hello"); 
strs.add("world"); 
strs.add("banana"); 
//序列化 
String jsonStr = JSON.toJSONString(strs); 
System.out.println(jsonStr); 
//反序列化 
List<String> strList = JSON.parseObject(jsonStr, new TypeReference<List<String>>(){} ); 
// List<String> strList = JSON.parseArray(jsonStr, String.class);//等同于上一句 
for (String str : strList) { 
System.out.println(str); 
} 
} 
//fastjson过滤字段 
@Test 
public void test6() { 
Employee e = new Employee("001", "张三", 23, new Date()); 
Employee e2 = new Employee("002", "李四", 29, new Date()); 
List<Employee> emps = new ArrayList<Employee>(); 
emps.add(e); 
emps.add(e2); 
//构造过滤器 
SimplePropertyPreFilter filter = new SimplePropertyPreFilter(Employee.class, "id", "age"); 
String jsonStr =JSON.toJSONString(emps, filter); 
System.out.println(jsonStr); 
} 
//fastjson 日期处理 
@Test 
public void test7(){ 
Date date = new Date(); 
String dateStr = JSON.toJSONString(date); 
System.out.println(dateStr); 
String dateStr2 = JSON.toJSONStringWithDateFormat(date, "yyyy-MM-dd HH:mm:ss"); 
System.out.println(dateStr2); 
//序列化实体 
Employee emp = new Employee("001", "张三", 23, new Date()); 
//法一 
String empStr = JSON.toJSONStringWithDateFormat(emp, "yyyy-MM-dd HH:mm:ss"); 
System.out.println(empStr); 
//法二 
String empStr2 = JSON.toJSONString(emp, SerializerFeature.WriteDateUseDateFormat); 
System.out.println(empStr2); 
//法三 
SerializeConfig config = new SerializeConfig(); 
config.put(Date.class, new SimpleDateFormatSerializer("yyyy年MM月dd日 HH时mm分ss秒")); 
String empStr3 = JSON.toJSONString(emp, config); 
System.out.println(empStr3); 
} 
//fastjson 去掉值的双引号 实现JSONAware接口 
@Test 
public void test8(){ 
//见同级目录的Function.java 
} 
//fastjson 注解形式  (别名命名, 过滤字段, 日期格式) 
@Test 
public void test9(){ 
Student stu = new Student("001", "张三", 23, new Date()); 
String jsonStr = JSON.toJSONString(stu); 
System.out.println(jsonStr); 
} 
}
```
