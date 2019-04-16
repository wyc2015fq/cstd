# Java 学习笔记30：gson的深入使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年02月18日 13:37:38[initphp](https://me.csdn.net/initphp)阅读数：1263
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









> 
gson和其他现有java json类库最大的不同时gson需要序列化得实体类不需要使用annotation来标识需要序列化得字段，同时gson又可以通过使用annotation来灵活配置需要序列化的字段。


**对象转JSON：**

> 
对象转JSON很简单，只要将对象放进toJson函数的参数中即可

```java
HashMap<String, String> map = new HashMap<String, String>(); //实例化一个hashmap  
map.put("username", "initphp"); //新增一个数据  
map.put("password", "test");  
map.put("age", "100");    
String mapToJson = gson.toJson(map);
```


返回结果：


`{"username":"initphp","age":"100","age2":"age2","age1":"age1","password":"test"}`



**JSON字符串转对象：**

> 
JSON字符串转对象稍微复杂点。分多种情况。

第一种，我们无法确定JSON里面的数据结构，但是能判断这个JSON是一维的结构，我们可以用HashMap来装载这个数据。



```java
String hashString = "{\"username\":\"initphp\", \"age\":\"100\"}";
HashMap<String, String> testBeanListFromJson = gson.fromJson(hashString,  new TypeToken<HashMap<String, String>>(){}.getType()); 
testBeanListFromJson.get("age")
```




第二种，有确定的数据结构：

有一个person的类了



```java
publicclass Person {

private String name;
privateint age;

/**
* @return the name
*/
public String getName() {
return name;
}

/**
* @param name the name to set
*/
publicvoid setName(String name) {
this.name = name;
}

/**
* @return the age
*/
publicint getAge() {
return age;
}

/**
* @param age the age to set
*/
publicvoid setAge(int age) {
this.age = age;
}

@Override
public String toString()
{
return name +":"+age;
}
}
```


```java
Gson gson =new Gson();
List<Person> persons =new ArrayList<Person>();
for (int i =0; i <10; i++) {
Person p =new Person();
p.setName("name"+ i);
p.setAge(i *5);
persons.add(p);
}
String str = gson.toJson(persons);
```


结果：



```java
[{"name":"name0","age":0},{"name":"name1","age":5},{"name":"name2","age":10},{"name":"name3","age":15},{"name":"name4","age":20},{"name":"name5","age":25},{"name":"name6","age":30},{"name":"name7","age":35},{"name":"name8","age":40},{"name":"name9","age":45}]
```


单个数据，可以这样：



```java
Person person = gson.fromJson(str, Person.class);
```












