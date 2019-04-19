# 在java重新认识TryCatch - Big Smile - CSDN博客
置顶2017年07月16日 21:56:00[王啸tr1912](https://me.csdn.net/tr1912)阅读数：424标签：[异常](https://so.csdn.net/so/search/s.do?q=异常&t=blog)
个人分类：[代码设计																[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)](https://blog.csdn.net/tr1912/article/category/6053292)
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        敲过代码的同志们都知道，在程序中处理异常我们常用Try  Catch结构的代码来接收，处理异常，那么我们是否真的已经了解了try catch了呢？
# 一、问题的提出
        在去面试的时候面试官曾经问过我这样的问题，如果我在try的方法体中写了一个return，在catch中写了一个return，在finaly中也写了一个return，问如果没有异常是哪个return触发，如果有异常是哪个return触发？当时就懵了，感觉似曾相识的问题竟然还可以这么问，然后再开发ITOO的晨会上，组长也明确了，异常的接收和处理的过程是不能有多个return作为出口的，要做到单入单出，这样才能让我们的程序更加规范。所以很有必要深入研究一下。
        请看下面的例子，用到了try catch的一些实现。
# 二、验证
        做验证么，我们就要有一个程序作为铺垫在模拟这个过程，首先我在service层定义了一个方法，用来被调用，他会输出字符或者抛出异常。
```java
public String testTry(String asd) throws Exception {
        throw new Exception("抛出");
    }
```
        然后在facade层写了一个方法用来接收处理异常：
```java
public String testTry(String asd){
        try {
            return teachSecretariesCurriculumScheduleService.testTry(asd) + asd+"正常";
        }catch(Exception e){
            return asd+"catch";
        }finally {
            return asd + "finally";
        }
}
```
        最后写一个测试用的方法来调用：
```java
@Test
    public void testTry()
    {
        String asd="结果为：";
        String Result = teachSecretariesCurriculumFacade.testTry(asd);
        System.out.println(Result);
    }
```
      我们运行这个结果，可以看到最后系统输出为
                      结果为：finally
      我们可以见到出现异常的时候通常是走finally里面的return的，如果我们把finally删除之后会如何呢？
                      结果为：catch
     这里可以说明，异常被catch了，走了catch里面的return方法。这样就可以说明了在出现异常并且catch和finally都有return的时候，只走finally的return。
      我们再来看一下，如果不返回异常，而是返回一个确定值的话会怎么样呢？
      改造service层的代码，facade有finally的情况：
```java
public String testTry(String asd) throws Exception {
        return "true";
    }
```
        然后再看我们的程序，运行会发现结果为：
                       结果为：finally
        我们可以看到在没有报异常的情况下，还是会走finally下的return，而不是走try下面的return。如果我们去掉finally会怎么样呢？
                       结果为：true
         这说明什么呢，说明了去掉了finally之后我们的程序直接走的try下面的return方法，这个说明了finally存在的含义就是必须执行，所以程序会自动屏蔽除了finally之外的步骤里面的return语句（如果有的话），但是非return语句是不受影响的，我们可以在try，catch，finally后面加上system.out.println("try/catch/finally")  然后我们输出可以看到相应的结果，这里不再举例。
# 三、总结
        总的来说，try/catch语句的原则还是以finally为重的，当每个段内都有return的这种只能执行一次的代码的时候，编译器会自动屏蔽掉除了finally之外的阶段里面的return语句的执行，但是其他的代码并不会，所以如果写了try/catch的语句，并且用了多个return，那么，不用担心，因为系统只会执行一个return语句，只是你知不知道他走的是哪一个的问题把了，还有你的小伙伴能不能理解你写的代码的意思了。
