# JAVA中序列化和反序列化中的静态成员问题 - Spark高级玩法 - CSDN博客
2018年07月23日 00:10:53[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：146
关于这个标题的内容是面试笔试中比较常见的考题，大家跟随我的博客一起来学习下这个过程。
    JAVA中的序列化和反序列化主要用于：
（1）将对象或者异常等写入文件，通过文件交互传输信息；
（2）将对象或者异常等通过网络进行传输。
    那么为什么需要序列化和反序列化呢？简单来说，如果你只是自己同一台机器的同一个环境下使用同一个JVM来操作，序列化和反序列化是没必要的，当需要进行数据传输的时候就显得十分必要。比如你的数据写到文件里要被其他人的电脑的程序使用，或者你电脑上的数据需要通过网络传输给其他人的程序使用，像服务器客户端的这种模型就是一种应用，这个时候，大家想想，每个人的电脑配置可能不同，运行环境可能也不同，字节序可能也不同，总之很多地方都不能保证一致，所以为了统一起见，我们传输的数据或者经过文件保存的数据需要经过序列化和编码等操作，相当于交互双方有一个公共的标准，按照这种标准来做，不管各自的环境是否有差异，各自都可以根据这种标准来翻译出自己能理解的正确的数据。
    在JAVA中有专门用于此类操作的API，供开发者直接使用，对象的序列化和反序列化可以通过将对象实现Serializable接口，然后用对象的输入输出流进行读写，下面看一个完整的例子。
```
package test2;
import java.io.Serializable;
public class DataObject implements Serializable {
  
  /**
   * 序列化的UID号
   */
  private static final long serialVersionUID = -3737338076212523007L;
  
  public static int i =  0;
  private String word = "";
  
  public static void setI(int i){
    DataObject.i = i;
  }
  
  public void setWord(String word){
    this.word = word;
  }
  
  public static int getI() {
    return i;
  }
  public String getWord() {
    return word;
  }
  @Override
  public String toString() {
    return "word = " + word + ", " + "i = " + i;
  }
}
```
上面这段程序是定义了要被序列化和反序列化的类DataObject，这个类实现了Serializable接口，里面有几点需要注意：
（1）类中有一个静态成员变量i，这个变量能不能被序列化呢？等下通过测试程序看一下；
（2）类中重写了toString方法，是为了打印结果。
    接下来我们看一下测试该类的对象序列化和反序列化的一个测试程序版本，提前说明，这个版本是有问题的。
```
package test2;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
/**
 * Description: 测试对象的序列化和反序列
 */
public class TestObjSerializeAndDeserialize {
    public static void main(String[] args) throws Exception {
      
      // 序列化DataObject对象
        Serialize();
        
        // 反序列DataObject对象
        DataObject object = Deserialize();
        
        // 静态成员属于类级别的，所以不能序列化，序列化只是序列化了对象而已，
        // 这里的不能序列化的意思，是序列化信息中不包含这个静态成员域，下面
        // 之所以i输出还是2，是因为测试都在同一个机器（而且是同一个进程），因为这个jvm
        // 已经把i加载进来了，所以获取的是加载好的i，如果是传到另一台机器或者关掉程序重新
        // 写个程序读入DataObject.txt，此时因为别的机器或新的进程是重新加载i的，所以i信息就是初始时的信息，即0
        System.out.println(object);
    }
    
    /**
     * MethodName: SerializePerson 
     * Description: 序列化Person对象
     * @author 
     * @throws FileNotFoundException
     * @throws IOException
     */
    private static void Serialize() throws FileNotFoundException, IOException {
      
      DataObject object = new DataObject();
      object.setWord("123");
      object.setI(2);
      
        // 创建ObjectOutputStream对象输出流，其中用到了文件的描述符对象和文件输出流对象
        ObjectOutputStream oo = new ObjectOutputStream(new FileOutputStream(
                new File("DataObject.txt")));
        
        // 将DataObject对象存储到DataObject.txt文件中，完成对DataObject对象的序列化操作
        oo.writeObject(object);
        
        System.out.println("Person对象序列化成功！");
        
        // 最后一定记得关闭对象描述符！！！
        oo.close();
    }
    /**
     * MethodName: DeserializePerson 
     * Description: 反序列DataObject对象
     * @author 
     * @return
     * @throws Exception
     * @throws IOException
     */
    private static DataObject Deserialize() throws Exception, IOException {
      
      // 创建ObjectInputStream对象输入流，其中用到了文件的描述符对象和文件输入流对象 
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(
                new File("DataObject.txt")));
        
        // 从DataObject.txt文件中读取DataObject对象，完成对DataObject对象的反序列化操作
        DataObject object = (DataObject) ois.readObject();
        System.out.println("Person对象反序列化成功！");
        
        // 最后一定记得关闭对象描述符！！！
        ois.close();
        
        return object;
    }
}
```
上面这段程序大家可以直接运行。注意，这里定义了两个方法Serialize()和Deserialize()，分别实现了序列化和反序列化的功能，里面的主要用到了对象输入输出流和文件输入输出流，大家看一下程序中的注释就可以理解。在序列化的方法中，将对象的成员变量word设置成了"123"，i设置成了"2"，注意这里的i是静态变量，那么以通常的序列化和反序列化的理解来看，无非就是一个正过程和一个逆过程，最终经过反序列化后，输出对象中的word和i时，大家一般都觉得应该还是"123"和"2"，那么上面程序的运行结果确实就是：
`word = "123", i = 2`
 这样会使得大家觉得理应就是如此，其实这是错误的。大家要记住： 
    静态成员属于类级别的，所以不能序列化，序列化只是序列化了对象而已，这里“不能序列化”的意思是序列化信息中不包含这个静态成员域，下面之所以i输出还是2，是因为测试都在同一个机器（而且是同一个进程），因为这个jvm已经把i加载进来了，所以获取的是加载好的i，如果是传到另一台机器或者关掉程序重新写个程序读入DataObject.txt，此时因为别的机器或新的进程是重新加载i的，所以i信息就是初始时的信息，即0。所以，总结来看，静态成员是不能被序列化的，静态成员定以后的默认初始值是0，所以正确的运行结果应该是：
`word = "123", i = 0`
那么既然如此，怎样才能测试出正确的结果呢？大家注意，上面的程序是直接在一个JVM一个进程中操作完了序列化和反序列化的所有过程，故而JVM中已经保存了i = 2，所以i的值没有变化，所以再次读出来肯定还是2。如果想得出正确的结果，必须在两个JVM中去测试，但是大家的电脑很难做到这种测试环境，所以可以通过以下方法来测试。
```
package test2;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
/**
 * Description: 测试对象的序列化
 */
public class SerializeDataobject {
    public static void main(String[] args) throws Exception {
      
      // 序列化DataObject对象
        Serialize();
        
    }
    
    /**
     * MethodName: SerializePerson 
     * Description: 序列化Person对象
     * @author 
     * @throws FileNotFoundException
     * @throws IOException
     */
    private static void Serialize() throws FileNotFoundException, IOException {
      
      DataObject object = new DataObject();
      object.setWord("123");
      object.setI(2);
      
        // 创建ObjectOutputStream对象输出流，其中用到了文件的描述符对象和文件输出流对象
        ObjectOutputStream oo = new ObjectOutputStream(new FileOutputStream(
                new File("DataObject.txt")));
        
        // 将DataObject对象存储到DataObject.txt文件中，完成对DataObject对象的序列化操作
        oo.writeObject(object);
        
        System.out.println("Person对象序列化成功！");
        
        // 最后一定记得关闭对象描述符！！！
        oo.close();
    }
}
```
 上面这个类只用来进行序列化，对象被序列化后保存在文件"DataObject.txt"中，然后程序运行结束，JVM退出。接下来看另一段程序。
```
package test2;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
/**
 * Description: 测试对象的反序列
 */
public class DeserializeDataobject {
    public static void main(String[] args) throws Exception {
        
        // 反序列DataObject对象
        DataObject object = Deserialize();
        
        // 静态成员属于类级别的，所以不能序列化，序列化只是序列化了对象而已，
        // 这里的不能序列化的意思，是序列化信息中不包含这个静态成员域，下面
        // 之所以i输出还是2，是因为测试都在同一个机器（而且是同一个进程），因为这个jvm
        // 已经把i加载进来了，所以获取的是加载好的i，如果是传到另一台机器或者关掉程序重新
        // 写个程序读入DataObject.txt，此时因为别的机器或新的进程是重新加载i的，所以i信息就是初始时的信息，即0
        System.out.println(object);
    }
    /**
     * MethodName: DeserializePerson 
     * Description: 反序列DataObject对象
     * @author 
     * @return
     * @throws Exception
     * @throws IOException
     */
    private static DataObject Deserialize() throws Exception, IOException {
      
      // 创建ObjectInputStream对象输入流，其中用到了文件的描述符对象和文件输入流对象 
        ObjectInputStream ois = new ObjectInputStream(new FileInputStream(
                new File("DataObject.txt")));
        
        // 从DataObject.txt文件中读取DataObject对象，完成对DataObject对象的反序列化操作
        DataObject object = (DataObject) ois.readObject();
        System.out.println("Person对象反序列化成功！");
        
        // 最后一定记得关闭对象描述符！！！
        ois.close();
        
        return object;
    }
}
```
上面这段程序用来实现对象的反序列化，它从文件"DataObject.txt"中读出对象的相关信息，然后进行了反序列化，最终输出对象中word和i的值，这个程序输出的结果才是word = "123", i = 0 这个才是正确的结果，这是因为序列化和反序列化都有自己的main方法，先序列化，然后JVM退出，再次运行反序列化，JVM重新加载DataObject类，此时i = 0，"DataObject.txt"文件中其实是没有i的信息的，只有word的信息。这里通过先后执行序列化和反序列化，让JVM得到一次重新加载类的机会，模拟了两个JVM下运行的结果。
    总之，大家要记住以下几点：
（1）序列化和反序列化的实现方法和应用场合；
（2）静态成员是不能被序列化的，因为静态成员是随着类的加载而加载的，与类共存亡，并且静态成员的默认初始值都是0；
（3）要明白错误的那个测试程序的原因，搞明白JVM的一些基本机制；
（4）要想直接通过打印对象而输出对象的一些属性信息，要重写toString方法。
   上面只是我的一些个人总结，欢迎大家指正和补充。
本文转自：https://blog.csdn.net/dan15188387481/article/details/49977421
**相关阅读：**
[如何准备Java初级和高级的技术面试](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484877&idx=1&sn=623c80aef584808391cb6a590428a5c8&chksm=9f38e6e5a84f6ff3c1ee7397a17b570ee0a07801d5006132482ba0f9e3963765e4d5165d6253&scene=21#wechat_redirect)
[Java transient关键字使用小记](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484445&idx=1&sn=c4e2c0a10042c2759eb7186cc05e0bfb&chksm=9f38e735a84f6e232910b89ad80f3486a5a02c772c31ff81eaedb3ca223c7d2cc030c5e568e9&scene=21#wechat_redirect)
