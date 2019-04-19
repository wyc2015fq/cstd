# java classloader 代码示例 - walkabc - 博客园
# [java classloader 代码示例](https://www.cnblogs.com/hitfire/articles/8789806.html)
看了网上的关于classloader的文章不少，但是示例代码却很少见，不得不自己研究下。
下面演示一下如何加载外部的jar文件，并使用jar中的类。
1.创建2个类
```
public class Message {
   private String msg;
   public Message(String msg) {
       this.msg = msg;
   }
   public String toString() {
       return msg;
   }
}
```
```
public class HelloWorld {
    public void print(String body) {
        Message msg = new Message(body);
        System.out.println(msg.toString());
    }
}
```
2.编译这2个类
```
javac Message.java
javac HelloWorld.java
```
3.把Message.class、HelloWorld.class打包成jar文件
```
jar cf hello.jar Message.class HelloWorld.class
```
4.加载jar文件，然后调用HelloWorld
```
public static void main(String[] args) throws Exception {
        //hello.jar的路径按照改成自己电脑中的路径
        String path = "/path/to/hello.jar";
        File file = new File(path);
        URLClassLoader loader = new URLClassLoader(new URL[]{file.toURI().toURL()});
        Object helloworld = loader.loadClass("HelloWorld").newInstance();
        Method method = helloworld.getClass().getMethod("print", String.class);
        method.invoke(helloworld, "好人一生平安");
    }
```
以上就是如何使用URLClassLoader加载jar的示例，从上面的代码可以看出来，hello.jar的加载过程是URLClassLoader自动加载的。

