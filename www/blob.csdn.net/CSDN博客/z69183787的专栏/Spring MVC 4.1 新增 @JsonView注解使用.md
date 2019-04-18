# Spring MVC 4.1 新增  @JsonView注解使用 - z69183787的专栏 - CSDN博客
2017年09月01日 18:11:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1210
对jsonview很久也没有理解怎么使用，今天看到一篇文章解释的很清楚 
，现总结如下：
@JsonView注解用来过滤序列化对象的字段属性，简单来说就是定义一个标签，根据controller的JsonView属性，将实体类中不同标签的属性进行分类显示。
例： 
首先定义一个view类，有两个接口Summary 和SummaryWithDetail ：
```java
public class View {
   public interface Summary {}
   public interface SummaryWithDetail extends Summary{}
}
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
然后定义一个实体类User，选择属性分别加上注解@JsonView(View.Summary.class) 或@JsonView(View.SummaryWithDetail .class)
```java
public class User { 
  @JsonView(View.Summary.class) 
  private Long id; 
  @JsonView(View.Summary.class) 
  private String firstname; 
  @JsonView(View.Summary.class) 
  private String lastname; 
  @JsonView(View.SummaryWithDetail .class) 
  private String email; 
  @JsonView(View.SummaryWithDetail .class) 
  private String address; 
  private String postalCode; 
  private String city; 
  private String country;
  ...
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
定义Controller类，加上@JsonView注解：
```java
@RestController
public class UserRestController{
  @Autowired 
  private UserService userService;
  @RequestMapping("/user")
  @JsonView(View.Summary.class) 
  public List<User> getUsers(){
    return userService.listUsers();
  }
  @RequestMapping("/userWithDetail")
  @JsonView(View.SummaryWithDetail.class) 
  public List<User> getUsersWithDetail(){
    return userService.listUsers();
  }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
访问/user时，使用的注解是@JsonView(View.Summary.class)，所以仅显示注解为@JsonView(View.Summary.class)的属性，结果为：
> 
[ 
{ “id” : 1, “firstname” : “Brian”, “lastname” : “Clozel” }, 
{ “id” : 2, “firstname” : “Stéphane”, “lastname” : “Nicoll” }, 
{ “id” : 3, “firstname” : “Rossen”, “lastname” : “Stoyanchev” } 
]
访问/userWithDetail时，使用的注解是@JsonView(View.SummaryWithDetail.class)，结果显示注解为@JsonView(View.SummaryWithDetail.class)的属性，同时由于SummaryWithDetail继承于Summary，结果也包含注解为@JsonView(View.Summary.class)的属性，结果为：
> 
[ 
{ 
“id” : 1, 
“firstname” : “Brian”, 
“lastname” : “Clozel”, 
“email”: “brian@163.com”, 
“address”: “beijing China” 
}, 
{ 
“id” : 2, 
“firstname” : “Stéphane”, 
“lastname” : “Nicoll”, 
“email”: “nicoll@163.com”, 
“address”: “beijing China” 
}, 
{ 
“id” : 3, 
“firstname” : “Rossen”, 
“lastname” : “Stoyanchev” , 
“email”: “stoyanchev@163.com”, 
“address”: “beijing China” 
} 
]
[](http://blog.csdn.net/dickysun1987/article/details/70142453#)[](http://blog.csdn.net/dickysun1987/article/details/70142453#)[](http://blog.csdn.net/dickysun1987/article/details/70142453#)[](http://blog.csdn.net/dickysun1987/article/details/70142453#)[](http://blog.csdn.net/dickysun1987/article/details/70142453#)[](http://blog.csdn.net/dickysun1987/article/details/70142453#)
