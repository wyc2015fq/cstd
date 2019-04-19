# Struts2 - LC900730的博客 - CSDN博客
2017年07月24日 14:59:03[lc900730](https://me.csdn.net/LC900730)阅读数：162标签：[struts2.0](https://so.csdn.net/so/search/s.do?q=struts2.0&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## struts.xml
struts.xml文件中配置Action时候，不仅需要指定Action的name属性和class属性，还需要为Action元素指定系列的result子元素，每个result子元素定义一个逻辑视图和物理视图之间的映射。JSP作为视图，没有指定type属性。如果需要使用其他视图技术，则可以在配置result子元素时，指定相应type属性即可。
struts.xml文件主要负责管理应用中的action映射，以及该Action包含的Result定义等。除此之外，Struts2框架还包含一个struts.properities文件，该文件定义了Struts2框架的大量属性。
[http://localhost:8080/stru2/hello](http://localhost:8080/stru2/hello)
获取请求路径，得到路径的值，到src下面找到struts.xml，使用dom4j解析，得到xml文件中的内容，拿着hello的值到xml中找到action中name属性值为hello的 
如action name=”hello”, 那么根据class属性找到这个action属性的全路径，使用反射来实现功能。
```
Clas clazz=Class.forName("action全路径");
Method m=clazz.getMethod("execute");
Object obj=m.invoke();//得到action方法的返回值
//在struts.xml中找到action标签里面有result与返回值相匹配，匹配成功就跳转到相应页面
```
1.过滤器在服务器启动时候创建，创建过滤器时候执行init(在init方法中主要加载配置文件，包含自己创建的配置文件和struts自带的配置文件，struts.xml和web.xml)
