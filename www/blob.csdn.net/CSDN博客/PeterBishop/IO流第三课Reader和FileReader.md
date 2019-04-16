# IO流第三课Reader和FileReader - PeterBishop - CSDN博客





2018年11月02日 21:56:09[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：58








  上节课我们学了字节输入流InputStream，这节课来学学字节输入流Reader


public abstract class Reader implements Readable, Closeable
可以看到Reader是一个抽象类，实现了Readable和Closeable接口

来看看Readable接口


public interface Readable {public int read(java.nio.CharBuffer cb) throws IOException;

}


这个Readable里面只有一个方法read，但是参数没见过，这个是NIO的内容，以后讲。



接下来来看看Reader都有哪些构造器:

![](https://img-blog.csdnimg.cn/20181102213450196.png)

凡是介绍里有同步两个字的暂时不管，这个是线程的内容，以后讲。



所以我们可以看到Reader有一个无参构造器



常用方法:

![](https://img-blog.csdnimg.cn/20181102213521334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

常用的有close read skip,ready有时候也用，不过我没怎么用过。

read(CharBuffer target)那个不用管，NIO的内容



接下来看看具体使用：

首先看看FileReader
public class FileReader extends InputStreamReader


然后看InputStreamReader


public class InputStreamReader extends Reader
由此可见FileReader是Reader的子类



然后看看构造器:

![](https://img-blog.csdnimg.cn/20181102213530552.png)

和FileInputStream类似，常用的就第一个和第三个



常用方法:

![](https://img-blog.csdnimg.cn/20181102213534712.png)



接下来来看看InputStreamReader的常用方法

![](https://img-blog.csdnimg.cn/20181102213555104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)









接下来是代码演示,通过FileReader来读取文件内容


public class ReaderTeach {



    public static void main(String[] args) {

        Reader reader = null;

        try {

            File file = new File("F:\\code\\java\\123.txt");

            reader = new FileReader(file);



            int a = 0;

            String content = "";

            do {

                a = reader.read();

                content += (char)a;

            }while (a != -1);



            System.*out*.println(content);

        } catch (FileNotFoundException e) {

            e.printStackTrace();

        } catch (IOException e) {

            e.printStackTrace();

        } finally {

            try {

                reader.close();

            } catch (IOException e) {

                e.printStackTrace();

            }

        }

    }

}


输出:

123456?



可以看到这样读取的话最后会出现乱码，所以我一般通过字节流读，当然不是说字符流不好，字符流可以读取固定长度的字符，比如


public static void main(String[] args) {

    Reader reader = null;

    try {

        File file = new File("F:\\code\\java\\123.txt");

        reader = new FileReader(file);



        char[] charContent = new char[3];

        reader.read(charContent);

        String content = new String(charContent);

        System.*out*.println(content);



    } catch (FileNotFoundException e) {

        e.printStackTrace();

    } catch (IOException e) {

        e.printStackTrace();

    } finally {

        try {

            reader.close();

        } catch (IOException e) {

            e.printStackTrace();

        }

    }

}


输出:

123



我们来输出下 文件大小吧



可以看到文件大小为6，我们完全可以像FileInputStream那样来读取:


public static void main(String[] args) {

    Reader reader = null;

    try {

        File file = new File("F:\\code\\java\\123.txt");

        reader = new FileReader(file);



        char[] charContent = new char[(int)file.length()];

        reader.read(charContent);

        String content = new String(charContent);

        System.*out*.println(content);



    } catch (FileNotFoundException e) {

        e.printStackTrace();

    } catch (IOException e) {

        e.printStackTrace();

    } finally {

        try {

            reader.close();

        } catch (IOException e) {

            e.printStackTrace();

        }

    }

}


这样也是可以的，但如果文件是字节码文件就没法这样读了，比如.CLASS文件或者图片、音频、视频之类的文件就只能通过字节流来读取，字符流是以字符为单位，所以只能读取文本文件(注意，代码即文本，所以字符流是可以读java源文件的，包括以后要学的其他编程语言的源文件都可以用字符流读取)



