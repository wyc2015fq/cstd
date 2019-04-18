# springmvc的contronller之间携带参数的跳转 - z69183787的专栏 - CSDN博客
2017年09月06日 21:36:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：812
[http://blog.csdn.net/qq_36769100/article/details/70886951](http://blog.csdn.net/qq_36769100/article/details/70886951)
### **1、首先说一下不带参数的重定向格式：**
**[java]**[view
 plain](http://blog.csdn.net/qq_36769100/article/details/70886951#)[copy](http://blog.csdn.net/qq_36769100/article/details/70886951#)
- @RequestMapping("/controller")  
- public String toController(){  
- return"redirect:otherController.do";//重定向跳转到其他controller
- }  
- 
- @RequestMapping("/otherController")  
- public String toOtherController(String param,Model model){  
- return"success"; //第二个controller负责跳转到页面
- }  
这是不带参数的重定向，这种情况一般很少出现。
注意：
> 
这里需要强调一下路径问题，上图实例中跳转到的controller是同一个anction下，路径中貌似是不需要带”/”的。至于跳转到其他action中的controller，路径可能会有不同。
**2、重定向带参数的形式（get）**
springmvc中有一个RedirectAttributes的类，这个类可以帮我们实现参数的传递：
> 
RedirectAttributes.addAttribute(“参数名1”，参数1)；
> 
RedirectAttributes.addAttribute(“参数名2”，参数2)；
然后重定向。
上代码:
**[java]**[view
 plain](http://blog.csdn.net/qq_36769100/article/details/70886951#)[copy](http://blog.csdn.net/qq_36769100/article/details/70886951#)
- @RequestMapping("/controller")  
- public String toController(RedirectAttributes attr){//第一个controller中传入RedirectAttributes参数
-         attr.addAttribute("param","test");    //封装跳转是需要携带的参数
- return"redirect:otherController.do";  
-     }  
- 
- @RequestMapping("/otherController")  
- public String toOtherController(String param,Model model){//这里传入param需要和attr中的key一样
-         System.out.println("param-----:"+param);//这里输出的为:"param-----:test"
-         model.addAttribute("param1", param);  //Model封装参数,携带到前台
- return"success1";  
-     }  
这种方式相当于系统帮我们在重定向路径的后面携带了参数，类似于:
> 
Return redirect:***.do?参数名1=参数1&参数名2=参数2;
注意：
> 
这种提交方式采用的get，重定向的时候会把参数暴露在网址上，不安全，不建议使用。
**2、重定向带参数的形式（隐藏参数）**
SpringmvcRedirectAttribute中还有一个addFlashAttribute的方法，这个方法可以实现隐藏信息。
> 
RedirectAttribute.addFlashAttribute(“参数名，参数)；
但是使用addFlashAttribute的时候，在目标controller方法的参数列表中需要增加一个注解：
> 
@ModelAttribute(“参数名”)String param.
上代码：
**[java]**[view
 plain](http://blog.csdn.net/qq_36769100/article/details/70886951#)[copy](http://blog.csdn.net/qq_36769100/article/details/70886951#)
- @RequestMapping("/controller2")  
- public String toController2(RedirectAttributes attr){//依然需要传入RedirectAttributes 参数
-         attr.addFlashAttribute("param", "test");  //使用的是addFlashAttribute方法
-         attr.addFlashAttribute("param1", "test1");  
- return"redirect:otherController2.do";  
-     }  
- 
- @RequestMapping("/otherController2")  
- public String totoOtherController2(@ModelAttribute("param")String param){//需要添加注解@ModelAttribute
-         System.out.println("param----------:"+param);//框架会自动帮我们获取
- return"success2";  
-     }  
通过这种方式，跳转过程中携带的参数，就会被隐藏掉。
