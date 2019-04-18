# java转换json的实用工具－Gson - z69183787的专栏 - CSDN博客
2013年09月25日 14:30:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1012标签：[JSON](https://so.csdn.net/so/search/s.do?q=JSON&t=blog)
个人分类：[Json转换-Gson](https://blog.csdn.net/z69183787/article/category/2175429)

Gson 是 Google 提供的用来在 Java 对象和 JSON 数据之间进行映射的 Java 类库。可以将一个 JSON 字符串转成一个 Java 对象，或者反过来。比之前使用的json-lib好用很多,特别是gson对日期的操作比起json-lib更是天壤之别.不多说了直接上代码
Gson的应用主要为toJson与fromJson两个转换函数，而在使用这种对象转换之前需先创建好对象的类型以及其成员才能成功的将JSON字符串成功转换成相对应的对象。
一、将对象转换成json
Student s1=newStudent();
		s1.setName("张三");		s1.setBirthday(newDate());//设置日期格式Gson gson=newGsonBuilder().setDateFormat("yyyy/MM/dd HH:mm:ss").create();String s=gson.toJson(s1);System.out.println(s);
结果
{"name":"张三","birthday":"2013/06/28 22:43:27"}
二、将对象转换成json
Student s1=newStudent();	s1.setName("张三");	s1.setBirthday(newDate());Student s2=newStudent();	s2.setName("李四");	s2.setBirthday(newDate());List<Student> list=newArrayList<Student>();	list.add(s1);	list.add(s2);//设置日期对象的格式Gson gson=newGsonBuilder().setDateFormat("yyyy/MM/dd HH:mm:ss").create();String str=gson.toJson(list);
结果
[{"name":"张三","birthday":"2013/06/28 22:45:23"},{"name":"李四","birthday":"2013/06/28 22:45:23"}]
三、将json转换成对象
String s="[{\"name\":\"张三\",\"birthday\":\"2013/06/28 22:45:23\"},{\"name\":\"李四\",\"birthday\":\"2013/06/28 22:45:23\"}]";
List<Student> list2=gson.
fromJson(str,
newTypeToken<List<Student>>(){}.
getType());
这里需要注意必须使用TypeToken的泛型方式声明转换的类型,才可以将json字符串转换成对应的集合对象.
总结:Gson是google开发的一套实用json转换工具,简单实用,强烈推荐朋友们使用,
