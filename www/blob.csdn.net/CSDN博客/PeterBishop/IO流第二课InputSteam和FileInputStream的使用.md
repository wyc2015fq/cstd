# IO流第二课InputSteam和FileInputStream的使用 - PeterBishop - CSDN博客





2018年11月02日 21:34:05[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：122








  上节课我们讲了File类的简单使用，这节课开始我们将IO流。

  IO流里涉及了一大堆类，首先来看下IO流的体系。

![](https://img-blog.csdnimg.cn/20181102212804566.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





这节课讲抽象基类里的InputStream和以及访问文件的FileInputStream



首先说下字节流和字符流的区别:

其实就是单位的区别，字节流以字节为单位进行输入输出，字符流以字符为单位进行输入输出。

字节对应的类是Byte(基本数据类型是byte)，字符对应的类是Character(基本数据类型是char)



下面来看看InputStream


public abstract class InputStream implements Closeable 
可以看到InputStream是一个抽象类，所以 不能直接new对象

然后，InputStream实现了一个接口Closeable，顾名思义就是可关闭的，



来看看Closeable


public interface Closeable extends AutoCloseable {public void close() throws IOException;

}


Closeable继承自AutoCloseable接口，并且声明了一个方法close()


public interface AutoCloseable {void close() throws Exception;

}


AutoCloseable里也有个close方法，只不过这个方法时抛出的Exception异常



Closeable的close方法和AutoCloseable的close方法都是无参，所以Closeable是重写了AutoCloseable的close方法，因此实现Closeable接口的类在实现close方法时抛出的是IOException异常。



所以InputStream需要实现close方法，但InputStream是抽象类，也可以交给子类去实现close，自己本身不需要实现



  以下是InputStream自己实现的close方法，可以看见什么也没做


public void close() throws IOException {}




源代码比较复杂，来看看API文档吧，别看错了，是java.io.InputStream





超类就是父类的意思，super class嘛



构造器:

![](https://img-blog.csdnimg.cn/20181102212905653.png)

只有一个无参构造器



方法:

![](https://img-blog.csdnimg.cn/20181102212915756.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



常用的就close和read以及skip(这个都不怎么用)



然后来看看FileInputStream


public

class FileInputStream extends InputStream
FileInputStream继承自InputStream



可以看到FileInputStream自己又重写了close方法，而且非常复杂，不用管，只用知道close是关闭系统资源的就可以了。



还是来看API文档：



构造器:

![](https://img-blog.csdnimg.cn/20181102212955543.png)



最常用的是第一个和第三个

FileInputStream没有无参构造器，所以new的时候必须传入参数


public FileInputStream(String name) throws FileNotFoundException {

    this(name != null ? new File(name) : null);

}
可以看到FileInputStream(String name)这个方法就是根据name来构造一个File对象然后调用自身的构造器即FileInputStream(File file)



方法摘要:

![](https://img-blog.csdnimg.cn/20181102213001340.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





常用方法:

常用的就close和read以及skip(这个都不怎么用)



来用的试试



Demo 1:
public static void main(String[] args) {

    InputStream is = null;

    try {

        File file = new File("F:\\code\\java\\123.txt");

        is = new FileInputStream(file);



        //获取文件内容长度

        long length = file.length();

        byte[] fileContent = new byte[(int)length];



        is.read(fileContent);



        String content = new String(fileContent);

        System.*out*.println(content);



    } catch (FileNotFoundException e) { //由于FileNotFoundException是IOException的子类，所以先捕获FileNotFoundException

        e.printStackTrace();

    } catch (IOException e) {

        e.printStackTrace();

    } finally {

        //我们前面说过finally这部分用来存放释放资源的代码

        //现在我们就用来释放InputStream占用的资源

        try {

            if (is != null)

            {

                is.close();

            }

        } catch (IOException e) {   //可以看到抛出的是IOException，证明了Closeable确实重写了AutoCloseable的close方法

            e.printStackTrace();

        }

    }

}


输出:

123456

![](https://img-blog.csdnimg.cn/20181102213344661.png)





可以看到我们通过FileInputStream成功读取了文件的内容，这就是通过字符输入流读取文件的办法



