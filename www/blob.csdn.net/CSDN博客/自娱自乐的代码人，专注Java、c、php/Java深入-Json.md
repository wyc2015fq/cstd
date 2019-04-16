# Java深入 - Json - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月02日 10:41:45[initphp](https://me.csdn.net/initphp)阅读数：34238
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)[Java深入](https://blog.csdn.net/column/details/myjava.html)









### JSONObject与JSONArray

Java不像PHP解析和生产JSON总是一个比较痛苦的过程。但是使用JSONObject和JSONArray会让整个过程相对舒服一些。

JsonObject和JsonArray区别就是JsonObject是对象形式，JsonArray是数组形式


需要依赖的包：

commons-lang.jar 


commons-beanutils.jar 


commons-collections.jar 


commons-logging.jar  


ezmorph.jar 


json-lib-2.2.2-jdk15.jar

#### **生成JSON：**



```java
public class Test2 {

    public static void main(String[] args) {

        /* 创建JsonObject第一种方法 */
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("UserName", "ZHULI");
        jsonObject.put("age", "30");
        jsonObject.put("workIn", "ALI");
        System.out.println("jsonObject1：" + jsonObject);

        /* 创建JsonObject第二种方法 */
        HashMap<String, String> hashMap = new HashMap<String, String>();
        hashMap.put("UserName", "ZHULI");
        hashMap.put("age", "30");
        hashMap.put("workIn", "ALI");
        System.out.println("jsonObject2 FROM HASHMAP：" + JSONObject.fromObject(hashMap));

        /* 创建一个JsonArray方法1 */
        JSONArray jsonArray = new JSONArray();
        jsonArray.add(0, "ZHULI");
        jsonArray.add(1, "30");
        jsonArray.add(2, "ALI");
        System.out.println("jsonArray1：" + jsonArray);

        /* 创建JsonArray方法2 */
        ArrayList<String> arrayList = new ArrayList<String>();
        arrayList.add("ZHULI");
        arrayList.add("30");
        arrayList.add("ALI");
        System.out.println("jsonArray2 FROM ArrayList：" + JSONArray.fromObject(arrayList));

        /* 如果JSONArray解析一个HashMap，则会将整个对象的放进一个数组的值中 */
        System.out.println("jsonArray FROM HASHMAP：" + JSONArray.fromObject(hashMap));

        /* 组装一个复杂的JSONArray */
        JSONObject jsonObject2 = new JSONObject();
        jsonObject2.put("UserName", "ZHULI");
        jsonObject2.put("age", "30");
        jsonObject2.put("workIn", "ALI");
        jsonObject2.element("Array", arrayList);
        jsonObject2.element("Object", jsonObject);
        System.out.println("jsonObject2：" + jsonObject2);

    }
}
```


结果：



```
jsonObject1：{"workIn":"ALI","age":"30","UserName":"ZHULI"}
jsonObject2 FROM HASHMAP：{"workIn":"ALI","age":"30","UserName":"ZHULI"}
jsonArray1：["ZHULI","30","ALI"]
jsonArray2 FROM ArrayList：["ZHULI","30","ALI"]
jsonArray FROM HASHMAP：[{"workIn":"ALI","age":"30","UserName":"ZHULI"}]
jsonObject2：{"workIn":"ALI","age":"30","Array":["ZHULI","30","ALI"],"UserName":"ZHULI","Object":{"workIn":"ALI","age":"30","UserName":"ZHULI"}
```


#### **解析JSON：**



```
package com.xxx.video.resource.controller.web;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;


public class Test {

    public static void main(String[] args) {

        String jsonString = "{\"UserName\":\"ZHULI\",\"age\":\"30\",\"workIn\":\"ALI\",\"Array\":[\"ZHULI\",\"30\",\"ALI\"]}";
        /* 将Json字符串转为java对象 */
        JSONObject obj = JSONObject.fromObject(jsonString);

        /* 获取Object中的UserName */
        if (obj.has("UserName")) {
            System.out.println("UserName:" + obj.getString("UserName"));
        }

        /* 获取ArrayObject */
        if (obj.has("Array")) {
            JSONArray transitListArray = obj.getJSONArray("Array");
            for (int i = 0; i < transitListArray.size(); i++) {
                System.out.print("Array:" + transitListArray.getString(i) + " ");
            }
        }
    }
}
```


返回：





```
UserName:ZHULI
Array:ZHULI Array:30 Array:ALI
```




对象遍历


```java
if (!obj.isNull("sku")) {
            HashMap<String, String> skuMap = new HashMap<String, String>();
            JSONObject skuObject = obj.getJSONObject("sku");
            Iterator<String> keys = skuObject.keys();
            String key;
            Object o;
            while (keys.hasNext()) {
                key = keys.next();
                o = skuObject.get(key);
                skuMap.put(key, String.valueOf(o));
            }
        }
```




### gson使用

gson和其他现有java json类库最大的不同时gson需要序列化得实体类不需要使用annotation来标识需要序列化得字段，同时gson又可以通过使用annotation来灵活配置需要序列化的字段。

#### 生成JOSN



```java
public class Test2 {

    public static void main(String[] args) {
        Gson gson = new Gson();

        HashMap<String, String> map = new HashMap<String, String>(); //实例化一个hashmap  
        map.put("username", "initphp"); //新增一个数据  
        map.put("password", "test");
        map.put("age", "100");
        String mapToJson = gson.toJson(map);
        System.out.println(mapToJson);
    }

}
```


结果：



```java
{"username":"initphp","age":"100","password":"test"}
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
public class Test2 {

    public static void main(String[] args) {
        Gson gson = new Gson();

        List<Person> persons = new ArrayList<Person>();
        for (int i = 0; i < 10; i++) {
            Person p = new Person();
            p.setName("name" + i);
            p.setAge(i * 5);
            persons.add(p);
        }
        String str = gson.toJson(persons);
    }
}
```


结果：`[{"name":"name0","age":0},{"name":"name1","age":5},{"name":"name2","age":10},{"name":"name3","age":15},{"name":"name4","age":20},{"name":"name5","age":25},{"name":"name6","age":30},{"name":"name7","age":35},{"name":"name8","age":40},{"name":"name9","age":45}]`





#### 解析JSON



JSON字符串转对象稍微复杂点。分多种情况。

第一种，我们无法确定JSON里面的数据结构，但是能判断这个JSON是一维的结构，我们可以用HashMap来装载这个数据。



```java
public class Test2 {

    public static void main(String[] args) {
        Gson gson = new Gson();

        String hashString = "{\"username\":\"initphp\", \"age\":\"100\"}";
        HashMap<String, String> map = gson.fromJson(hashString,
                new TypeToken<HashMap<String, String>>() {
                }.getType());
        System.out.print(map.get("age"));
    }
}
```


第二种，有确定的数据结构：

```java
Person person = gson.fromJson(str, Person.class);
```





