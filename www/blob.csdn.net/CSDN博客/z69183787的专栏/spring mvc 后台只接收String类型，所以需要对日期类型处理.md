# spring mvc 后台只接收String类型，所以需要对日期类型处理 - z69183787的专栏 - CSDN博客
2015年01月06日 20:52:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2842
一、只需要在方法中加入
**[java]**[view
 plain](http://blog.csdn.net/jiana227/article/details/18663159#)[copy](http://blog.csdn.net/jiana227/article/details/18663159#)[print](http://blog.csdn.net/jiana227/article/details/18663159#)[?](http://blog.csdn.net/jiana227/article/details/18663159#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166829/fork)
- @InitBinder
- protectedvoid initBinder(WebDataBinder binder) {  
-     SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");  
-     binder.registerCustomEditor(Date.class, new CustomDateEditor(dateFormat, true));  
- }  
方法就可以
二、注解方式：   需要joda 包
**[java]**[view
 plain](http://blog.csdn.net/jiana227/article/details/18663159#)[copy](http://blog.csdn.net/jiana227/article/details/18663159#)[print](http://blog.csdn.net/jiana227/article/details/18663159#)[?](http://blog.csdn.net/jiana227/article/details/18663159#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166829/fork)
- @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss",timezone="GMT+8")  //取日期时使用
- @DateTimeFormat(pattern = "yyyy-MM-dd")//存日期时使用
- private Date startTime;  
