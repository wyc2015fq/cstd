# Java7新特性 try-with-resource - westbrook1998的博客 - CSDN博客





2018年03月20日 22:11:02[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：86








```java
package com.zxc.csdn;

/**
 * @author russell 
 */
public class ResExceptionDemo {

    public static void main(String[] args){
        try(ResException resException=new ResException()){
            resException.doSomething();
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}

class ResException implements AutoCloseable{

    public void doSomething(){
        System.out.println("doing...");
    }

    @Override
    public void close() throws Exception {
        System.out.println("close!");
    }
}
```

### try-with-resource的出现是为了避免在语句（文件资源操作类）中抛出异常而导致后面的资源关闭方法（close()）无法正常执行，引起错误。

### 使用try-with-resource的前提是资源类必须实现AutoCloseable接口，如果想清除一些没有实现AutoCloseable接口的资源，则使用finally字句

```java
package com.zxc.csdn;

/**
 * @author russell
 */
public class NormalTryException {

    public static void main(String[] args){
        TryException tryException=new TryException();
        try{
            tryException.doSomething();
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            if(tryException !=null){
                try{
                    tryException.close();
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }
    }
}

class TryException{

    public void doSomething(){
        System.out.println("doing...");
    }

    public void close() throws Exception {
        System.out.println("close!");
    }
}
```

### 要注意资源类实例的创建要放在try语句块外部，否则finally字句无法访问该实例，同时注意close方法也会抛出异常，因此`通常将复杂的try/catch/finally语句改为try-with-resource，或者在try/finally语句中嵌套try/catch语句`



