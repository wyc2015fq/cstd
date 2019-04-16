# jdbc篇第5课：properties文件和Properties类 - PeterBishop - CSDN博客





2018年11月27日 19:15:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：13
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来学习一种新的文件和一个新的类



  properties文件:

特点：

纯文本

每一行都是 xxx=yyy的形式，类似键值对

常被用来当做配置文件





 Properties类:

可以读取properties文件，将其内容转化成一个Map

Properties是HashTable的子类

HashTable实现了Map接口



Demo:
public static void main(String[] args) throws IOException {

    Properties properties = new Properties();

    //通过load()方法来加载properties文件，将里面的内容变成一个Map

    //load()方法需要一个InputStream类型的参数

    File file = new File("src/com/resource/teach.properties");

    properties.load(new FileInputStream(file));



    //我们说了，properties是Map的实现类，但我们并不适用Map接口的方法

    //Properties类给我们提供好了更好的方法,getProperties()和setProperties()方法

    //其实和get()，put()方法没区别，主要是把类型限定了一下

    //Properties 其实就相当于一个 Map<String,String>，键和值都是字符串的Map

    String name = properties.getProperty("person.name");

    System.*out*.println(name);

}


teach.properties文件:
person.id=1

person.name=xiaoye

person.age=20




结果:

xiaoye







