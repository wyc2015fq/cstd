# Spring MVC与mybatis学习-----参数配置文件说明 - BlueSky - CSDN博客
2015年12月28日 19:41:03[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：975
个人分类：[Java Spring](https://blog.csdn.net/ASIA_kobe/article/category/6039238)

springmvc.xml：
扫描注入器，即扫描自己的处理器的包；以及视图解析（即通过处理器返回来的值通过加上前后后缀来进行转向）
Controller：
为handller即处理一些函数：就是处理界面传递过来的操作利用这里面自己注入的函数来处理，处理完后将结果返回结果给视图解析器
@RequestMapping：
可以修饰方法，还可以修饰类；通过@RequestMapping 将请求与具体的类或者类联系起来
（1）.类定义处：提供初步的请求映射信息，相对于WEB应用的根目录
（2）.方法处：提供进一步的细分映射信息，相对于类定义处的URL。若类定义处未标注@RequestMapping,则方法处标记的URL相对于WEB应用的根目录
@RequestMapping（value = “” ， method = "", params = "", headers =""）:
如上，@RequestMapping支持注释的方式如上，value代表请求映射URL，method代表请求的方法，params代表请求参数，headers代表请求头；
eg：
@RequestMapping(value = "login",method=method = RequestMethod.POST, params={"username","age!=10"}, headers={Accept-Language=en-US,zh;q=0.8})
@PathVariable:
通过它可以将URl中占位符参数绑定到控制器处理方法的入参中，如：
@RequestMapping("/testPathVariable/{id}")
public String testPathVariable(@PathVariable("id") Integer id){
System.out.println("spring mvc test"+id);
return SUCCESS;
}
如上就将URL请求中的id参数传到控制器的处理方法中了
Spring MVC 会按请求参数名和POJO属性名进行自动匹配，自动为该对象填充属相值，支持级联属性（就是将请求参数传给处理器时处理器将收到的参数按照类的属性进行匹配并为该类的实际对象填充属性值）
@ModelAttribute标记的方法，会在每个目标执行之前被SpringMVC调用：
运行流程：1.执行@ModelAttribute 注解修饰方法：从数据库中取出对象，把对象放入到Map中，键为：user；
 2.SpringMVC从Map中取出User对象，并把表单的请求参数赋给该User对象的对应属性；
 3.SpringMVC把上述对象传入目标方法的参数。
注意：在@ModelAttribute修饰的方法中，放入到Map 时的键需要和目标方法入参类型的第一个字母小写的字符串一致！
eg：
@ModelAttribute
public void getUser(Map<String, Object> map){
//模拟从数据库中获取一个对象
User user = new User("asia","123456",18);
System.out.println("从数据库中获取一个对象："+user);
map.put("user", user);
}
@RequestMapping("/testModelAttribute")
public String testModelAttribute(User user){
System.out.println("修改："+user);
return SUCCESS;
}
表单传输数据操作请求给控制器控制器进行处理可能会调用模型
Dao类一般用于与数据库的操作
springmvc.xml用于配置：自动扫描的包、视图解析器
<mvc:annnotation-driven/>会自动注册相应的注解
处理JSON：
1.加入jar包：jackson相关的包；
2.编写目标方法，使其返回JSON对应的对象或者集合；
3.在方法上添加@ResponseBody注解。
eg：
@ResponseBody
@RequestMapping("/getUsers")
public List<User> testAjax(){
List<User> users = new ArrayList<>();
users.add(new User(1,"a",new Date(),1000));
return users；
}
Data Access Objects（DAO）；MyBatis 使用简单的 XML或注解用于配置和原始映射，将接口和 Java 的POJOs（Plain Old Java Objects，普通的 Java对象）映射成数据库中的记录。
![](https://img-blog.csdn.net/20160419172205977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
