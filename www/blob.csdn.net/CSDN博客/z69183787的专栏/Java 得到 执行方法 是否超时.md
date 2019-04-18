# Java 得到 执行方法 是否超时 - z69183787的专栏 - CSDN博客
2015年06月12日 16:08:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1305
```java
package com.cmb.util;
import java.util.concurrent.*;
public class MethodOver {
    public static void main(String[] args) {
        Executor executor= Executors.newSingleThreadExecutor();
        FutureTask future=new FutureTask(new Callable() {
            public String call() throws Exception {
// TODO Auto-generated method stub
                MethodOver m=new MethodOver();
                return m.getValue();
            }
        });
        executor.execute(future);
        try{
            String result=(String)future.get(1, TimeUnit.SECONDS);
            System.out.println(result);
        }catch (InterruptedException e) {
// TODO: handle exception
            System.out.println("方法执行中断");
// future.cancel(true);
        }catch (ExecutionException e) {
            System.out.println("Excution异常");
// TODO: handle exception
            future.cancel(true);
        }catch (TimeoutException e) {
// TODO: handle exception
            System.out.println("方法执行时间超时");
//future.cancel(true);
        }
        System.out.println("sas");
    }
    public String getValue()
    {
        try{
            Thread.sleep(5000);
        }catch (Exception e) {
            e.printStackTrace();// TODO: handle exception
        }
        return "ssssssssssssssss";
    }
}
```
