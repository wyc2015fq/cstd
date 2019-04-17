# finally的作用 - PeterBishop - CSDN博客





2018年10月18日 12:46:20[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：69








  上节课我们学习了异常的抛出和捕获，这节课我们再来学一个关键字: finally

  finally和final是不一样的哦，final是声明常量的，而finally是做异常处理的。



  finally语法:

try{

可能包含异常的代码

}catch(异常类 变量名){

异常处理代码

}...(多个catch)

finally{

后置处理代码

}



来看一个小例子:



Demo:

```
public static void main(String[] args) {

    try {

        test();

    } catch (Exception e) {

        e.printStackTrace();

    }

}



public static void test() throws Exception {

    try {

        throw new Exception("lalalala");

    }catch (RuntimeException e){

        e.printStackTrace();

    }finally {

        System.out.println("finally");

    }

}
```



输出:

finally

java.lang.Exception: lalalala

         at helloworld.exception.FinallyTeach.test(FinallyTeach.java:15)

         at helloworld.exception.FinallyTeach.main(FinallyTeach.java:7)



结论:

异常如果没有被捕获，会执行finally的代码





Demo 2:

```
public static void main(String[] args) {

    try {

        test2();

    } catch (Exception e) {

        e.printStackTrace();

    }

}



public static void test2() throws Exception {

    try {

        throw new RuntimeException("abcd");

    }catch (RuntimeException e){

        e.printStackTrace();

    }finally {

        System.out.println("finally");

    }

}
```



输出:

finally



java.lang.RuntimeException: abcd

         at helloworld.exception.FinallyTeach.test2(FinallyTeach.java:25)

         at helloworld.exception.FinallyTeach.main(FinallyTeach.java:7)



结论: 如果异常被捕获，依然会执行finally里的代码



最终结论:
- 无论异常是否被捕获，finally的代码一定会被执行。
- finally里适合存放释放资源、后续处理的代码

![](https://img-blog.csdn.net/20181017185912378?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



