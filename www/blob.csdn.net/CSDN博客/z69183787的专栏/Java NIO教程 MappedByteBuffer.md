# Java NIO教程 MappedByteBuffer - z69183787的专栏 - CSDN博客
2016年12月16日 17:13:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3560
之前跟大家说过，要讲MappedByteBuffer,现在我来履行承诺了。
首先从大体上讲一下MappedByteBuffer究竟是什么。从继承结构上来讲，MappedByteBuffer继承自ByteBuffer，所以ByteBuffer有的能力它全有；像变动position和limit指针啦、包装一个其他种类Buffer的视图啦，都可以。“MappedByteBuffer”为何而来？吾辈心中亦有惑（熊猫人之谜的梗）用一个字来概括就是**快**
为什么**快**？因为它使用*direct buffer*的方式读写文件内容，这种方式的学名叫做**内存映射**。这种方式直接调用系统底层的缓存，没有JVM和系统之间的复制操作，所以效率大大的提高了。而且由于它这么快，还可以用它来在进程（或线程）间传递消息，基本上能达到和**“共享内存页”**相同的作用，只不过它是依托实体文件来运行的。
而且它还有另一种能力。就是它可以让我们读写那些因为太大而不能放进内存中的文件。有了它，我们就可以假定整个文件都放在内存中（实际上，大文件放在内存和虚拟内存中），基本上都可以将它当作一个特别大的数组来访问，这样极大的简化了对于大文件的修改等操作。
下面我们开始介绍它的用法了
FileChannel提供了map方法来把文件映射为MappedByteBuffer： MappedByteBuffer map(int mode,long position,long size); 可以把文件的从position开始的size大小的区域映射为MappedByteBuffer，mode指出了可访问该内存映像文件的方式，共有三种，分别为：
`MapMode.READ_ONLY`（只读）：
 试图修改得到的缓冲区将导致抛出 ReadOnlyBufferException。
`MapMode.READ_WRITE`（读/写）：
 对得到的缓冲区的更改最终将写入文件；但该更改对映射到同一文件的其他程序不一定是可见的（无处不在的**“一致性问题”**又出现了）。
`MapMode.PRIVATE`（专用）：
 可读可写,但是修改的内容不会写入文件,只是buffer自身的改变，这种能力称之为”copy on write”
再简单的说一下，MappedByteBuffer较之ByteBuffer新增的三个方法
- force()缓冲区是READ_WRITE模式下，此方法对缓冲区内容的修改强行写入文件
- load()将缓冲区的内容载入内存，并返回该缓冲区的引用
- isLoaded()如果缓冲区的内容在物理内存中，则返回真，否则返回假
下面代码终于出场了
```java
```java
int length = 0x8FFFFFF;//一个byte占1B，所以共向文件中存128M的数据
try (FileChannel channel = FileChannel.open(Paths.get("src/c.txt"),
        StandardOpenOption.READ, StandardOpenOption.WRITE);) {
    MappedByteBuffer mapBuffer = channel.map(FileChannel.MapMode.READ_WRITE, 0, length);
    for(int i=0;i<length;i++) {
        mapBuffer.put((byte)0);
    }
    for(int i = length/2;i<length/2+4;i++) {
        //像数组一样访问
        System.out.println(mapBuffer.get(i));
    }
}
```
```
上面是MappedByteBuffer最基本的应用，而下面这段代码主要是测试它到底有多快，
```java
```java
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
public class TestMappedByteBuffer {
    private static int length = 0x2FFFFFFF;//1G
    private abstract static class Tester {
        private String name;
        public Tester(String name) {
            this.name = name;
        }
        public void runTest() {
            System.out.print(name + ": ");
            long start = System.currentTimeMillis();
            test();
            System.out.println(System.currentTimeMillis()-start+" ms");
        }
        public abstract void test();
    }
    private static Tester[] testers = {
        new Tester("Stream RW") {
            public void test() {
                try (FileInputStream fis = new FileInputStream(
                        "src/a.txt");
                        DataInputStream dis = new DataInputStream(fis);
                        FileOutputStream fos = new FileOutputStream(
                                "src/a.txt");
                        DataOutputStream dos = new DataOutputStream(fos);) {
                    
                    byte b = (byte)0;
                    for(int i=0;i<length;i++) {
                        dos.writeByte(b);
                        dos.flush();
                    }                   
                    while (dis.read()!= -1) {
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        },
        new Tester("Mapped RW") {
            public void test() {
                try (FileChannel channel = FileChannel.open(Paths.get("src/b.txt"),
                        StandardOpenOption.READ, StandardOpenOption.WRITE);) {
                    MappedByteBuffer mapBuffer = channel.map(FileChannel.MapMode.READ_WRITE, 0, length);
                    for(int i=0;i<length;i++) {
                        mapBuffer.put((byte)0);
                    }
                    mapBuffer.flip();
                    while(mapBuffer.hasRemaining()) {
                        mapBuffer.get();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        },
        new Tester("Mapped PRIVATE") {
            public void test() {
                try (FileChannel channel = FileChannel.open(Paths.get("src/c.txt"),
                        StandardOpenOption.READ, StandardOpenOption.WRITE);) {
                    MappedByteBuffer mapBuffer = channel.map(FileChannel.MapMode.PRIVATE, 0, length);
                    for(int i=0;i<length;i++) {
                        mapBuffer.put((byte)0);
                    }
                    mapBuffer.flip();
                    while(mapBuffer.hasRemaining()) {
                        mapBuffer.get();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    };
    public static void main(String[] args) {
        for(Tester tester:testers) {
            tester.runTest();
        }
    }
}
```
```
先从整体上提一句上面的代码，runTest()是一个模板方法，并且引用了一个未实现的test()方法；通过匿名内部类的实现，填充了测试内容。
再来说上面代码的测试结果。用传统流的方式，当然是最慢的，但应该是由于用的数据量是1G，无法全部读入内存，所以它根本无法完成测试。剩下两种`MapMode.READ_WRITE`和`MapMode.PRIVATE`各有特点，首先说`MapMode.READ_WRITE`，它的速度每次差别较大，在0.6s和8s之间波动，而且很不稳定。但`MapMode.PRIVATE`就稳得出奇，一直是1.1s到1.2s之间。但无论是哪个速度都是十分惊人的。但是MappedByteBuffer也有不足，就是在数据量很小的时候，表现比较糟糕，那是因为*direct buffer*的初始化时间较长，所以建议大家只有在数据量较大的时候，在用MappedByteBuffer。
还要强调的一点是，MappedByteBuffer存在内存占用和文件关闭等不确定问题。被MappedByteBuffer打开的文件只有在垃圾收集时才会被关闭，而这个点是不确定的。javadoc里是这么说的：
> 
A mapped byte buffer and the file mapping that it represents remain valid until the buffer itself is garbage-collected. ——[JavaDoc](http://docs.oracle.com/javase/8/docs/api/java/nio/MappedByteBuffer.html)
关于MappedByteBuffer就告诉你这么多了，有什么问题尽管提、有什么想法随时找我交流。
