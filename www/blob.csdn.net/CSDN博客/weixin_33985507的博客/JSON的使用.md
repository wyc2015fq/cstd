# JSON的使用 - weixin_33985507的博客 - CSDN博客
2017年05月24日 22:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# JSON的使用
## 前言
JOSN，全称为JavaScript Object Notation，是一种轻量级的，读写方便的文本格式，主要用于数据的传输，比如服务器与客户端之前的信息交流等。
## JSON的语法
JOSN的语法相对简单，仅由`[]`、`{}`以及对应的键值对组成，不得不说，设计出这个格式的作者确实厉害，通过这些简单的符号就能表示出海量的内容了。
在JSON中，主要有两种数据类型，分别是由`[]`表示的数组类型以及由`{}`表示的对象类型。每种类型都可以包含多个元素，其中数组类型可以嵌套数组，也可以包括多个对象，数组中的元素使用`,`分隔；对象中包含由`:`分隔的一个或者多个键值对，代表对象的属性以及属性值，多个键值对之间使用`,`分隔，一个简单的JSON格式数据如下所示：
```
{"name":"Tom","age":22}
```
比较复杂的可以如下所示：
```
{
  "students":[
    {
      "student": {
        "name": "xuhuanfeng",
        "age": 22,
        "gender": "male"
      }
    },
    {
      "student": {
        "name":"Tom",
        "age":23,
        "gender":"male"
      }
    }
  ]
}
```
上面中的JSON文件包含了一个数组，数组中有两个对象，每个对象包含三个键值对
相信看到上面的内容，你会不禁感慨，JOSN的这种表示方式确实优雅
## JOSN与XML
与JSON相比，还有另外一种比较常用的数据格式：XML，在此之前我们也谈到过关于XML的简单格式，相比于XML，JOSN没有繁琐、冗长的标签，所以表示起数据来，比价简练，在用于数据的传输中，也能有效减少不必要数据的传输
## JOSN与对象之间的转换
为了在Java中更好、更方便地使用JSON，许多开发者们经过了努力，推出了几个比较常用的JOSN解析库类，这里我们使用Google公司的GSON类库来继续我们的JOSN学习，[文件下载](https://link.jianshu.com?t=http://repo1.maven.org/maven2/com/google/code/gson/gson/2.8.0/)
### 对象转JSON
这里我们演示如何使用GSON将对象转化为JSON格式
建立一个简单的对象如下所示：
```
package cn.xuhuanfeng.json;
/**
* Created by Huanfeng.Xu on 2017-05-24.
*/
public class Student {
    private String name;
    private String gender;
    private int age;
    public Student(String name, String gender, int age) {
            this.name = name;
            this.gender = gender;
            this.age = age;
    }
    public Student() {
    }
    /**
    * 篇幅原因，这里省略了set、get方法
    */
}
```
接下来我们使用GSON来将student对象转化为JOSN格式，具体如下代码所示
```
public void objToJSON(){
        // 建立一个GSON对象
        Gson gson = new Gson();
        Student student1 = new Student("xuhuanfeng", "male", 22);
        Student student2 = new Student("Tom", "male", 22);
        Student student3 = new Student("Lily", "femal", 23);
        Student[] students = {student1, student2, student3};
        // 将student1转化为JOSN格式
        String s = gson.toJson(student1);
        System.out.println(s);
        // 将students数组转化为JSON格式
        String ss = gson.toJson(students);
        System.out.println(ss);
}
```
对应的输出内容如下所示
```
{"name":"xuhuanfeng","gender":"male","age":22}
[{"name":"xuhuanfeng","gender":"male","age":22},{"name":"Tom","gender":"male","age":22},{"name":"Lily","gender":"femal","age":23}]
```
可以看到，GSON已经成功地将我们的student对象及其数组转化为JOSN格式的字符串了
### JOSN转对象
这里我们简单地演示将JOSN格式的字符串转化为对象，这里沿用前面的student类，具体代码如下所示
```
public void jsonToObj(){
        String data = "{\"name\":\"xuhuanfeng\",\"gender\":\"male\",\"age\":22}";
        String data1 = "[{\"name\":\"xuhuanfeng\",\"gender\":\"male\",\"age\":22},{\"name\":\"Tom\",\"gender\":\"male\",\"age\":22},{\"name\":\"Lily\",\"gender\":\"femal\",\"age\":23}]";
        Gson gson = new Gson();
        Student student = gson.fromJson(data, Student.class);
        System.out.println(student);
        // 根据情况传入不同的对象所对应的class
        Student[] student1 = gson.fromJson(data1, Student[].class);
        System.out.println(Arrays.toString(student1));
}
```
## 总结
本小节我们简单地了解了JOSN数据格式，简单地对比了JOSN格式以及XML的区别，以及JSON的优势，最后，我们借助Google的GSON类库，实现了将对象转化为JOSN格式以及将JSON格式的内容转化为对应的对象，如果你想更加深入的了解GSON的使用，可以参考[GSON的官方文档](https://link.jianshu.com?t=https://github.com/google/gson/blob/master/UserGuide.md)
