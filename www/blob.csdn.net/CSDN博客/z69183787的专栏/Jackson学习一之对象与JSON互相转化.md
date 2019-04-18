# Jackson学习一之对象与JSON互相转化 - z69183787的专栏 - CSDN博客
2015年01月06日 21:42:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4426
**简介:**
Jackson是一个流行的基于Java的JSON处理框架
官方wiki:[http://wiki.fasterxml.com/JacksonInFiveMinutes](http://wiki.fasterxml.com/JacksonInFiveMinutes)
官方文档:[http://wiki.fasterxml.com/JacksonDocumentation](http://wiki.fasterxml.com/JacksonDocumentation)
学习参考: [Json转换利器Gson之实例](http://blog.csdn.net/lk_blog/article/details/7685169)系列,顺便也学习一下GSON
**本篇简介如何使用Jackson进行Java对象与JSON的互相转换**
新建基本Student类:
Java代码  ![收藏代码](http://lijingshou.iteye.com/images/icon_star.png)
- package com.jingshou.pojo;  
- 
- import java.util.Date;  
- 
- publicclass Student {  
- privateint id;  
- private String name;  
- private Date birthDay;  
- 
- publicint getId() {  
- return id;  
-     }  
- 
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- public Date getBirthDay() {  
- return birthDay;  
-     }  
- 
- publicvoid setBirthDay(Date birthDay) {  
- this.birthDay = birthDay;  
-     }  
- 
- @Override
- public String toString() {  
- return"Student [birthDay=" + birthDay + ", id=" + id + ", name="
-                 + name + "]";  
-     }  
- 
- }  
 班级类:
Java代码  ![收藏代码](http://lijingshou.iteye.com/images/icon_star.png)
- package com.jingshou.pojo;  
- 
- import java.util.List;  
- 
- publicclass Class {  
- private String cname;  
- private List<Student> members;  
- public String getCname() {  
- return cname;  
-     }  
- publicvoid setCname(String cname) {  
- this.cname = cname;  
-     }  
- public List<Student> getMembers() {  
- return members;  
-     }  
- publicvoid setMembers(List<Student> members) {  
- this.members = members;  
-     }  
- 
- public String toString(){  
- return"The Class Name is: " + cname + " " + "The members are: " + members;  
- 
-     }  
- 
- 
- }  
测试代码:
Java代码  ![收藏代码](http://lijingshou.iteye.com/images/icon_star.png)
- package com.jingshou.jackson;  
- 
- import java.io.IOException;  
- import java.util.ArrayList;  
- import java.util.Date;  
- import java.util.List;  
- 
- import com.fasterxml.jackson.databind.ObjectMapper;  
- import com.jingshou.pojo.Class;  
- import com.jingshou.pojo.Student;  
- 
- publicclass JacksonTest1 {  
- 
- publicstaticvoid main(String[] args) throws IOException {  
-         Student student1 = new Student();    
-         student1.setId(5237);  
-         student1.setName("jingshou");  
-         student1.setBirthDay(new Date());  
- 
-         ObjectMapper mapper = new ObjectMapper();  
- 
- // Convert object to JSON string
-         String Json =  mapper.writeValueAsString(student1);  
-         System.out.println("Change Object to JSON String: " + Json);  
- 
- // Convert Json string to Object
-         Student student2 = mapper.readValue(Json, Student.class);  
-         System.out.println(student2);  
- 
- // Create a student list add it to Bj
-         Student student3 = new Student();    
-         student3.setId(5117);    
-         student3.setName("saiya");    
-         student3.setBirthDay(new Date());    
- 
-         List<Student> stuList = new ArrayList<Student>();  
-         stuList.add(student1);  
-         stuList.add(student3);  
- 
-         Class bj = new Class();  
-         bj.setCname("五年二班");  
-         bj.setMembers(stuList);  
- 
-         String json2 = mapper.writeValueAsString(bj);  
-         System.out.println("The JSON from Class is: " + json2);  
- 
-         Class bj2 = mapper.readValue(json2, Class.class);  
-         System.out.println(bj2);  
- 
- 
-     }  
- 
- }  
 运行结果:
Text代码  ![收藏代码](http://lijingshou.iteye.com/images/icon_star.png)
- Change Object to JSON String: {"id":5237,"name":"jingshou","birthDay":1389513906622}  
- Student [birthDay=Sun Jan 1216:05:06 CST 2014, id=5237, name=jingshou]  
- The JSON from Class is: {"cname":"五年二班","members":[{"id":5237,"name":"jingshou","birthDay":1389513906622},{"id":5117,"name":"saiya","birthDay":1389513906859}]}  
- The Class Name is: 五年二班 The members are: [Student [birthDay=Sun Jan 1216:05:06 CST 2014, id=5237, name=jingshou], Student [birthDay=Sun Jan 1216:05:06 CST 2014, id=5117, name=saiya]]  
**小结:**
- 转换的关键就是ObjectMapper 
- 在使用ObjectMapper对象readValue或者writeValue的时候,JSON可以以字符串的形式传入,也可以以文件的形式传入,官方的例子是让将JSON保存在文件中
- 明显JSON字串中的key与Java类中的field名字应该要相同
- Student类如果有自定义构造函数的时候,进行JSON->对象转换会出错,接下去再研究这个问题..
- Date域是以long类型输出的,有必要的话,需要进行格式转换
