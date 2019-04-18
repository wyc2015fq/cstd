# gson的@Expose注解和@SerializedName注解 - z69183787的专栏 - CSDN博客
2014年01月27日 10:08:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16919
1.使用@Expose可以区分实体中不想被序列化的属性
@Expose标签的2个属性. `**  1.1[deserialize](http://google-gson.googlecode.com/svn/trunk/gson/docs/javadocs/com/google/gson/annotations/Expose.html#deserialize%28%29)**` (boolean)
 反序列化 默认 true
1.2 `**[serialize](http://google-gson.googlecode.com/svn/trunk/gson/docs/javadocs/com/google/gson/annotations/Expose.html#serialize%28%29)**` 
 (boolean) 序列化 默认 true
    使用** new** GsonBuilder().excludeFieldsWithoutExposeAnnotation().create();创建Gson对象，没有@Expose注释的属性将不会被序列化
**public****class** User {
> 
@Expose
**private** String username;
@Expose(serialize=**false**)
**private****int**age ;
**private** List<String> list;
**public** User(String username, **int** age) {
**super**();
**this**.username = username;
**this**.age = age;
}
**public** String getUsername() {
**return**username;
}
**public****void** setUsername(String username) {
**this**.username = username;
}
**public****int** getAge() {
**return**age;
}
**public****void** setAge(**int** age) {
**this**.age = age;
}
**public** List<String> getList() {
**return**list;
}
**public****void** setList(List<String> list) {
**this**.list = list;
}
**public****static****void** main(String []args){
User user = **new** User("lemon",27);
List<String> list = **new** ArrayList<String>();
list.add("l1");
list.add("l2");
user.setList(list);
Gson g1 = **new** Gson();
//使用 new Gson();
System.*out*.println(g1.toJson(user)); //{"username":"lemon","age":27,"list":["l1","l2"]}
//使用 new GsonBuilder().excludeFieldsWithoutExposeAnnotation().create();
Gson g2 = **new** GsonBuilder().excludeFieldsWithoutExposeAnnotation().create();
System.*out*.println(g2.toJson(user)); //{"username":"lemon"}
}
}
2.使用@SerializedName标签定义属性序列化后的名字
> 
@Expose
@SerializedName("name")
**private** String username;
> 
**public****static****void** main(String []args){
> 
User user = **new** User("lemon",27);
List<String> list = **new** ArrayList<String>();
list.add("l1");
list.add("l2");
user.setList(list);
Gson g1 = **new** Gson();
//使用 new Gson();
//{"name":"lemon","age":27,"list":["l1","l2"]}
System.*out*.println(g1.toJson(user)); 
//使用 new GsonBuilder().excludeFieldsWithoutExposeAnnotation().create();
Gson g2 = **new** GsonBuilder().excludeFieldsWithoutExposeAnnotation().create();
//{"name":"lemon"}
System.*out*.println(g2.toJson(user)); 
}
