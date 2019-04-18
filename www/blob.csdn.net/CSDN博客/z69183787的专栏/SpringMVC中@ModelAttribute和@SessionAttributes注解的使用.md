# SpringMVC中@ModelAttribute和@SessionAttributes注解的使用 - z69183787的专栏 - CSDN博客
2016年12月16日 17:36:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7282
# 1.@ModelAttribute注解的使用
@ModelAttribute注解在两种地方可以使用
![](https://img-blog.csdn.net/20161216173338258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从这里我们可以看到该注解可以用在参数上也可以用在方法体上。
现在我们就来讲解这两种方式。
### 方式一，该注解用在方法体上：
在方法定义上使用 @ModelAttribute 注解：Spring MVC
在调用目标处理方法前，会先逐个调用在方法级上标注了
@ModelAttribute 的方法
```java
@ModelAttribute("user1")
    public User getUser() {
        User user = new User();
        user.setId(1L);
        user.setName("jjjj");
        user.setPassword("iiiii");
        return user;
    }
@RequestMapping(value = "hello4", method = {RequestMethod.GET, RequestMethod.POST})
    public String doHello4(Map<String, Object> map) {
      System.out.println(map.get("user1"));
        return "success";
    }
```
当我们发出/hello4.do这个请求时，SpringMvc 在执行该请求前会先逐个调用在方法级上标注了
@ModelAttribute 的方法，然后将该模型参数放入doHello4()函数的Map参数中。
如上执行结果如下：
![](https://img-blog.csdn.net/20161216173411464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 方式二，该注解用在参数上：
SpringMVC先从模型数据中获取对象，再将请求参数绑定到对象中，再传入形参，并且数据模型中的对象会被覆盖
```java
@RequestMapping(value = "hello7")
    public ModelAndView doHello6(
            @ModelAttribute("user1")
                    User user, ModelAndView modelAndView) {
  
        System.out.println(user + ":hello7");
        modelAndView.setViewName("redirect:/hello/hello4.do");
        return modelAndView;
    }
@RequestMapping(value = "hello4", method = {RequestMethod.GET, RequestMethod.POST})
    public String doHello4(Map<String, Object> map) {
        System.out.println(map.get("user1"));
        return "success";
    }
 @ModelAttribute("user1")
    public User getUser() {
        User user = new User();
        user.setId(1L);
        user.setName("jjjj");
        user.setPassword("iiiii");
        return user;
    }
```
测试结果如下：
![](https://img-blog.csdn.net/20161216173428636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
发出的请求信息如下：
http://localhost:8080/SSMProjectMaven/hello/hello7.do?id=5&name=Lily
我们只传入了id和name参数，但是在测试结果中我们发现password也有值。
这说明了,SpringMVC会先在数据模型中寻找属性user1，把user1的属性赋值给user中，然后再将请求中的参数绑定到user中，如果请求参数中有的参数已经绑定到了user中，那么请求参数会覆盖掉user中已存在的值，并且user对象会被放入数据模型中覆盖掉原来的user1对象。也就是模型数据中的user1的优先级低于请求参数。
# 2.@SessionAttributes注解的使用
@SessionAttributes只能使用在类定义上。
若希望在多个请求之间共用某个模型属性数据，则可以在
控制器类上标注一个 @SessionAttributes, Spring MVC
将在模型中对应的属性暂存到 HttpSession 中。
• @SessionAttributes 除了可以通过属性名指定需要放到会
话中的属性外，还可以通过模型属性的对象类型指定哪些
模型属性需要放到会话中
– @SessionAttributes(types=User.class) 会将隐含模型中所有类型
为 User.class 的属性添加到会话中。
– @SessionAttributes(value={“user1”, “user2”})
– @SessionAttributes(types={User.class, Dept.class})
– @SessionAttributes(value={“user1”, “user2”},
types={Dept.class})
```java
@Controller
@RequestMapping("hello")
@SessionAttributes(value = {"user1"})
public class HelloAction {
 @RequestMapping(value = "hello4", method = {RequestMethod.GET, RequestMethod.POST})
    public String doHello4(Map<String, Object> map, HttpSession session) {
        System.out.println(map.get("user1"));
        System.out.println("session:" + session.getAttribute("user1"));
        return "success";
    }
@ModelAttribute("user1")
    public User getUser() {
        User user = new User();
        user.setId(1L);
        user.setName("jjjj");
        user.setPassword("iiiii");
        return user;
    }
}
```
SpringMVC会把数据模型中的user1对象放入session中，所以我们在doHello4()中可以在session中取到user1对象。
测试结果如下：
![](https://img-blog.csdn.net/20161216173451136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意一下这种情况：
```java
@Controller
@RequestMapping("hello")
@SessionAttributes(value = {"user1", "msg"})
public class HelloAction {
   @RequestMapping(value = "hello7")
    public ModelAndView doHello6(
            @ModelAttribute("user1")
                    User user, ModelAndView modelAndView) {
        modelAndView.addObject("msg",
                user.getId() + ":" + user.getName() + ":" + user.getPassword() + ":" + user.getCars());
        System.out.println(user + ":hello7");
        modelAndView.setViewName("redirect:/hello/hello4.do");
        return modelAndView;
    }
 @RequestMapping(value = "hello4", method = {RequestMethod.GET, RequestMethod.POST})
    public String doHello4(Map<String, Object> map, HttpSession session) {
        System.out.println(map.get("user1"));
        System.out.println("session:" + session.getAttribute("user1"));
        System.out.println(map.get("msg"));
        System.out.println("session:" + session.getAttribute("msg"));
        return "success";
    }
 @ModelAttribute("user1")
    public User getUser() {
        User user = new User();
        user.setId(1L);
        user.setName("jjjj");
        user.setPassword("mmmmm");
        return user;
    }
}
```
测试结果如下：
![](https://img-blog.csdn.net/20161216173507228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这种情况表明了ModelAndView里的符合@SessionAttributes(value = {"user1", "msg"})这里面定义的value值，那么这个属性也会被放入到Session对象里。并且同时证明如果在一个方法里带有Map参数，SpringMVC不仅会把标有@ModelAttribute注解的属性放入Map中，也会把@SessionAttributes(value = {"user1", "msg"})中的属性放入Map中。注意如果通过
session.setAttribute("xxx", "kkkk");
这种方法放入到session中的属性是不会被放入到map中的。
以下这段代码的测试证明了该结论：
```java
@Controller
@RequestMapping("hello")
@SessionAttributes(value = {"user1", "msg"})
public class HelloAction {
   @RequestMapping(value = "hello4", method = {RequestMethod.GET, RequestMethod.POST})
    public String doHello4(Map<String, Object> map, HttpSession session) {
        System.out.println(map.get("user1"));
        System.out.println("session:" + session.getAttribute("user1"));
        System.out.println(map.get("msg"));
        System.out.println("session:" + session.getAttribute("msg"));
        System.out.println("map:" + map.get("xxx"));
        System.out.println("session:" + session.getAttribute("xxx"));
        return "success";
    }
 @RequestMapping(value = "hello7")
    public ModelAndView doHello6(
            @ModelAttribute("user1")
                    User user, ModelAndView modelAndView, HttpSession session) {
        session.setAttribute("xxx", "kkkk");
        modelAndView.addObject("msg",
                user.getId() + ":" + user.getName() + ":" + user.getPassword() + ":" + user.getCars());
        System.out.println(user + ":hello7");
        modelAndView.setViewName("redirect:/hello/hello4.do");
        return modelAndView;
    }
 @ModelAttribute("user1")
    public User getUser() {
        User user = new User();
        user.setId(1L);
        user.setName("jjjj");
        user.setPassword("mmmmm");
        return user;
    }
}
```
测试结果如下：
![](https://img-blog.csdn.net/20161216173523309?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们可以看到从map里取不出"xxx"的值，而从session里我们取出了“xxx”的值。
但是当我们在
@SessionAttributes(value = {"user1", "msg", "xxx"})
这里添加上“xxx”这个值，再执行上述代码可以看到map中取出了"xxx"的值。
![](https://img-blog.csdn.net/20161216173548416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
