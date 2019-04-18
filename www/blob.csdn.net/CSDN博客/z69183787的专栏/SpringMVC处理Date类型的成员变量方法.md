# SpringMVC处理Date类型的成员变量方法 - z69183787的专栏 - CSDN博客
2014年10月22日 11:25:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：30073
在使用SpringMVC的时候，我们可能需要将一个对象从View传递给Controller。而当这个Object只是一些简单的String，int或者boolean类型的成员变量时，SpringMVC能自动将View层的JSON包含的String类型转换为Object成员变量相应的类型。但是当这个Object包Date类型的成员变量的时候，SpringMVC在将String转换成Date类型时，就会出错，报异常。但是我们又需要使用Date类型的时候，其实Spring给我们提供了简单的操作方式可以完成这个任务的。
     SpringMVC 提供了一个注解 @DateTimeFormat 。可以将 View 传过来的 String类型转换为 Date 类型。具体使用方式很简单，直接在成员变量上加入注解就可以了，同时还可以指定 format 的格式，如下所示：
```java
public class Person {
  private String name;
  //直接在date类型上加入注解，同时指定格式样式
  @DateTimeFormat( pattern = "yyyy-MM-dd" )
  private Date birthday;
  //setterAndGetter
}
```
       至此，不要以为完事大吉了，你还需要完成以下两个步骤才可以。
第一需要加入 joda 的 jar 包。因为在 @DateTimeFormat 注解中使用到了 joda 包中的相关东西，所以缺少这个包也是会报异常的。如果使用的直接导入 jar 包的话，去下载 joda-Jar 导入即可，如果使用的是 Maven 管理项目的 jar ，那么在配置文件文件中加入依赖：
<dependency><groupId>joda-time</groupId><artifactId>joda-time</artifactId><version>2.3</version></dependency>
第二需要在 SpringMVC 配置 xml 文件中（一般是 dispatchServlet.xml 文件）中加入配置： <mvc:annotation-driven /> 。这一句配置是一种简写，其实是给 Spring 容器中注入了两个 Bena ，分别是： DefaultAnnotationHandlerMapping 和AnnotationMethodHandlerAdapter 。 @DateTimeFormat 注解的内部同样需要使用到前面注入的两个 bean 去处理，所以缺少这个配置， Spring 容器中没有对应的 bean 去处理注解同样也会报错。至此，所有的步骤都完成了，可以跑了。
接下来我们跑跑测试一下，测试过程：
首先需要一个表单 :
<formaction="test"method="post"><inputtype="text"name="name"><inputtype="text"name="birthday"><inputtype="submit"name="提交"></form>
    用一个 Controller 接收：
```java
@RequestMapping( "/test" )
public ModelAndView test(HttpServletRequest request,
     @ModelAttribute Person person) {
  ModelAndView view = new ModelAndView();
  System.out.println(person.toString());
  view.setViewName("/test/data");
  return view;
}
```
好了，总结一下整个过程，其实就 3 步：
**1 、   在 Date 类型的属性上加入 @DateTimeFormat 注解**
**2、   加入 joda 相关的包**
**3、   在 SpringMVC 配置文件中加入 <mvc:annotation-driven />**
