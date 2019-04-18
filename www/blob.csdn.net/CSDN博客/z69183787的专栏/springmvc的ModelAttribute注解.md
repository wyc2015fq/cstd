# springmvc的ModelAttribute注解 - z69183787的专栏 - CSDN博客
2017年09月20日 16:04:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：311
参考资料:spring-framework-reference的Using @ModelAttribute on a method和Using @ModelAttribute on a method argument
先看一个没有使用@ModelAttribute的Controller方法.
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @RequestMapping("/save")  
- public String save(User user) {  
-     user.setUsername("U love me");  
-     userService.save(user);  
- return"result";  
- }  
其中User包含id和username两个私有属性,含有公共setter和getter方法.
执行此方法时会将key为"user"(注意:这里即使参数名称是user1,key一样还是"user"),value为user的对象加入到model.在jsp页面可以通过${user.id}和${user.name}得到值的,即上面方法和下面方法是相当的.
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @RequestMapping("/save")  
- public String save(Model model,int id,String username) {  
-     User user=new User();  
- //这里是通过反射从request里面拿值再set到user
-     user.setId(id);  
-     user.setUsername(username);  
-     model.addAttribute("user",user);  
- 
-     user.setUsername("U love me");  
-     userService.save(user);  
- return"result";  
- }  
一.下面再来看看@ModelAttribute的基本用法.
1.在方法上使用@ModelAttribute
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @ModelAttribute("user1")  
- public User addUser(User user) {  
- returnnew User(520,"I love U");  
- }  
假设此方法是写在UserController内,那么执行UserController内带有@RequestMapping的方法之前,都会先执行此addUser方法.并且执行addUser过程中会添加两个对象到model,先将key为"user"的对象(由addUser方法的User user引起的),再添加key为"user1"的对象(由注解@ModelAttribute("user1")引起的).
2.在方法参数上使用@ModelAttribute.
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @RequestMapping("/save")  
- public String save(@ModelAttribute User user) {  
-     user.setUsername("U love me");  
-     userService.save(user);  
- return"result";  
- }  
此方法会先从model去获取key为"user"的对象,如果获取不到会通过反射实例化一个User对象,再从request里面拿值set到这个对象,然后把这个User对象添加到model(其中key为"user").
使用了@ModelAttribute可修改这个key,不一定是"user",此情况下,用与不用@ModelAttribute没有区别.
3.再来看看在方法和方法参数上结合使用@ModelAttribute,即上面两步的两个方法都添加UserController,如下:
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @ModelAttribute("user1")  
- public User addUser(User user) {  
- returnnew User(520,"I love U");  
- }  
- @RequestMapping("/save")  
- public String save(@ModelAttribute User user) {  
-     user.setUsername("U love me");  
-     userService.save(user);  
- return"result";  
- }  
假设要执行保存用户操作,根据一分析可知,先执行完会产生两个User类型的对象(一个key是"user",另一个key是"user1")添加到model,再执行save方法,此时会先从model去找key为"user"的对象,能找到再从request取值set到这个User对象.最后返回到jsp页面,model里也只有两个User类型对象.
再来个小小假设,将上面@ModelAttribute("user1")的user1改为user,其它不变.虽然执行了addUser方法,那么执行到save方法内,user对象的字段值还是来源于请求,最后返回到jsp页面,model里也只有一个User类型对象.
4.另类:
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @ModelAttribute
- @RequestMapping("/save")  
- public String save(@ModelAttribute User user) {  
-     user.setUsername("U love me");  
-     userService.save(user);  
- return"result";  
- }  
此种情况,会添加一个key为"user"的User对象到model,还会添加一个key为"string",value为"result"的对象到model,而视图名称则变为了"save"而不是"result".相信一般开发者都不会这样用.
二.下面再看看结合@SessionAttributes的用法.
1.先看看这段代码
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @Controller
- @RequestMapping("user")  
- @SessionAttributes("test")  
- publicclass UserController {  
- @RequestMapping(value = "test1")  
- public String test1(Map<String, Object> model, SessionStatus sessionStatus) {  
-         model.put("test","something");  
- //sessionStatus.setComplete();
- return"user/list";  
-     }  
- 
- @RequestMapping(value = "test2")  
- public String test2(Map<String,?>  model) {  
-         Object test = model.get("test");  
-         System.out.println(test);  
- return"user/list";  
-     }  
- }  
在类级别上使用@SessionAttributes("test"),它的作用是在controller共享 model 属性,直到调用org.springframework.web.bind.support.SessionStatus#setComplete会清除此session值.否则长期保留(session过期,这个值也不再保留).因此先调用/user/test1,再调用/user/test2.在test2方法能得到test1方法put进去的值.当移除上面的注解,因为属性为test的session也被清除了,所以test2方法会得到是null.
2.再看结合@ModelAttribute的用法.
**[java]**[view
 plain](http://blog.csdn.net/xiejx618/article/details/43638537#)[copy](http://blog.csdn.net/xiejx618/article/details/43638537#)
- @Controller
- @RequestMapping("user")  
- @SessionAttributes("test")  
- publicclass UserController {  
- @RequestMapping(value = "test1")  
- public String test1(HttpServletRequest request) {  
-         User user = new User("xiejx618");  
-         request.getSession().setAttribute("test",user);  
- return"user/list";  
-     }  
- @RequestMapping(value = "test2")  
- public String test2(@ModelAttribute("test") User user,SessionStatus sessionStatus) {  
-         System.out.println(user.getUsername());  
-         sessionStatus.setComplete();  
- return"user/list";  
-     }  
- }  
一样先调用/user/test1,再调用/user/test2.在test2方法能得到test1方法set进去的值.使用了@SessionAttributes,如果在controller的方法参数上有@ModelAttribute,那么此方法会确保属性test的session会存在,否则会抛org.springframework.web.HttpSessionRequiredException异常,即上面代码先不调/user/test1,直接调/user/test2就会抛此异常
我个人很少这样用,用原始的写法就清晰很多了,但要看明白别人写的代码。
