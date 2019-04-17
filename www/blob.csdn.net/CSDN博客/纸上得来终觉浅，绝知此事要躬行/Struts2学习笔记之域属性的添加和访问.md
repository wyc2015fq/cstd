# Struts2学习笔记之域属性的添加和访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月23日 20:06:37[boonya](https://me.csdn.net/boonya)阅读数：833标签：[struts																[application																[session																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记

第八记：域属性的添加和访问

（1）、设置访问属性

      1-1：通过ActionContext设置访问属性

           public String execute(){

                        ActionContext act=ActionContext.getContext();

                        act.getApplication().put("application","this is application scope");

                       act.getSession().put("session","this is session scope");

                       act.put("request","this is request scope");

                       return "SUCCESS";


          }

      1-2：通过ServletActonContext设置属性

      原理：获取到HttpServletRequest对象，利用setAtrribute()来实现。


       public String requestAndServletContext(){

                       HttpServletRequest   request=ServletActionContext.getRequest();

                       ServletContext servletContext=ServletActionContext.getServletContext();


                       servletContext.setAttributet("application","this is application scope");

                       request.getSession().setAttributet("session","this is session scope");

                       request.setAttributet("request","this is request scope");

                       return "SUCCESS";


          }

（2）、访问设置的属性

       在success.jsp页面中用EL表达式获取值：

      ${applicationScope.application}<br/>

      ${sessionScope.session}</br>

      ${requestScope.request}<br/>](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




