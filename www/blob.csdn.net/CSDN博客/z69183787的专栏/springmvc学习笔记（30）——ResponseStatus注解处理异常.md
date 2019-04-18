# springmvc学习笔记（30）——ResponseStatus注解处理异常 - z69183787的专栏 - CSDN博客
2017年01月22日 10:51:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1134
# ResponseStatus
ResponseStatus注解的使用非常简单，我们创建一个异常类，加上注解
```java
package com.zj.exception;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;
@ResponseStatus(value=HttpStatus.FORBIDDEN,reason="用户不匹配")
public class UserNotMatchException extends RuntimeException{
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
> - ResponseStatus注解是修饰类的
- 它有两个属性，value属性是http状态码，比如404，500等。reason是错误信息
写一个目标方法抛出该异常
```java
@RequestMapping("/testResponseStatus")
    public String testResponseStatus(int i){
        if(i==0)
            throw new UserNotMatchException();
        return "hello";
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
当传入参数i==0的时候将抛异常。下图是效果图 
![这里写图片描述](https://img-blog.csdn.net/20150430111907552)
> 
使用了ResponseStatus注解之后，用户看到的异常界面正是我们自己定义的异常，而不再是一大堆用户看不懂的代码。
# ResponseStatus修饰方法
讲到这里，我就想呵呵以下。ResponseStatus如果修饰目标方法，将会发生什么事咧？且看：
```java
@ResponseStatus(value=HttpStatus.FORBIDDEN,reason="用户名不匹配")
    @RequestMapping("/testResponseStatus")
    public String testResponseStatus(int i){
        if(i==0)
            throw new UserNotMatchException();
        return "hello";
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
![这里写图片描述](https://img-blog.csdn.net/20150430112318945)
> - 仔细看这张结果图中的访问路径，我传入的参数是i=1，正常来说是不应该抛异常的，可是它抛了。。它真的抛了。。
- 结论：ResponseStatus修饰目标方法，无论它执行方法过程中有没有异常产生，用户都会得到异常的界面。而目标方法正常执行
