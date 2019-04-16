# Java深入 - Java 序列化 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年09月10日 10:26:50[initphp](https://me.csdn.net/initphp)阅读数：1811
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









Java 串行化技术可以使你将一个对象的状态写入一个Byte 流里，并且可以从其它地方把该Byte 流里的数据读出来，重新构造一个相同的对象。这种机制允许你将对象通过网络进行传播，并可以随时把对象持久化到数据库、文件等系统里。

序列化的实现：将需要被序列化的类实现Serializable接口，然后使用一个输出流(如：FileOutputStream)来构造一个ObjectOutputStream(对象流)对象，接着，使用ObjectOutputStream对象的writeObject(Object obj)方法就可以将参数为obj的对象写出(即保存其状态)，要恢复的话则用输入流。


实现类的序列化，就需要implements Serializable这个接口类：



```java
public class TestBo implements java.io.Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = -2009384986168668986L;

    /**
     * 用户名
     */
    private String            userName;

    /**
     * 年龄
     */
    private String            age;

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getAge() {
        return age;
    }

    public void setAge(String age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return "TestBo [userName=" + userName + ", age=" + age + "]";
    }

}
```


我们可以看一个例子，将上面的TestBo类写入一个文件，并从这个文件中读取出来：





```java
public class Test {

    public static void main(String[] args) {
        System.out.println("Java序列化测试类");

        TestBo testBo = new TestBo();
        testBo.setAge("100");
        testBo.setUserName("zhuli");
        try {
            //将序列化数据写入文件
            FileOutputStream out = new FileOutputStream("D:/test2.txt"); //文件输出流
            ObjectOutputStream objectOutputStream = new ObjectOutputStream(out); //构造一个ObjectOutputStream
            objectOutputStream.writeObject(testBo); //写入对象
            System.out.println("序列化成功");
            
            out.close();
            objectOutputStream.close();
            
            
            //读取序列化
            FileInputStream in = new FileInputStream("D:/test2.txt"); //文件输出流
            ObjectInputStream objectInputStream = new ObjectInputStream(in); //构造一个ObjectInputStream
            TestBo testBo2 = (TestBo) objectInputStream.readObject(); //读取对象
            
            System.out.println(testBo2.toString());
            System.out.println("读取序列化成功");
            
            objectInputStream.close();
            in.close();
        } catch (Exception e) {
        }
    }

}
```


通过winhex 我们打开test2.txt文件，可以看到TestBo被序列化后的对象。



![](https://img-blog.csdn.net/20140910100714118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




如果需要在网络上传输，一般使用ByteArrayOutputStream，将对象序列化成byte[]数组，然后进行网络上的数据传输。可以看下下面的实现：



```java
public static void main(String[] args) {
        System.out.println("Java序列化测试类");

        TestBo testBo = new TestBo();
        testBo.setAge("100");
        testBo.setUserName("zhuli");
        try {
            //如果在网络上传输，一般使用ByteArrayOutputStream
            ByteArrayOutputStream byteOut = new ByteArrayOutputStream();
            ObjectOutputStream byteOutObj = new ObjectOutputStream(byteOut);
            byteOutObj.writeObject(testBo);
            byte[] testBoByte = byteOut.toByteArray();
            System.out.println("序列化成功");
            
            //读取byteArray
            ByteArrayInputStream byteIn = new ByteArrayInputStream(testBoByte);
            ObjectInputStream  byteInObj = new ObjectInputStream(byteIn);
            TestBo testBo3= (TestBo) byteInObj.readObject();
            System.out.println(testBo3.toString());
            System.out.println("读取序列化成功");
            byteInObj.close();
            byteIn.close();
            byteOut.close();
            byteOutObj.close();

        } catch (Exception e) {
        }
    }
```


序列化需要注意一些事项：



1. 对象需要被序列化，那么必须实现Serializable接口。

2. 如果要被序列化的对象中，**有域引用了一个其它的对象，那么这个对象也必须实现Serializable接口**，否则无法序列化。如果引用的这个对象不需要被序列化，则需要添加关键字**transient**

3. 如果一个类本身没有实现Serializable接口，但是它的**父类实现了Serializable接口，则可以被序列化**。

4. 如果一个类实现了Serializable接口，但是它的父类没有实现，这个类也是可以被序列化的，但是父类部分的域没有被序列化，所以要序列化的话父类也要实现Serializable接口。

5. 静态常量或者变量不会被序列化。（序列化了也没用）




