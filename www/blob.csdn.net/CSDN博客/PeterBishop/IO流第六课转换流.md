# IO流第六课转换流 - PeterBishop - CSDN博客





2018年11月11日 15:30:25[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：20
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课来讲讲转换流



  在讲转换流之前了，我们先讲讲字符的编码与解码。



  首先，我们知道计算机里所有数据都是以二进制的形式存储的，.java文件想要运行必须先被编译成.class字节码文件，这里我们提到了一个词，字节码，或者说字节，以往一直没机会，这次就来带大家看看字节码文件。

![](https://img-blog.csdnimg.cn/20181111144052125.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





这些以.class为后缀名的文件就是.class文件，这些文件的内容全是字节。我们打开来看看

![](https://img-blog.csdnimg.cn/20181111144057512.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





看不懂，对不对？我也看不懂，因为这是给计算机看的。



人看的懂的是字符，计算机能看懂的是字节，那么如何将计算机看得懂的字节变成人看得懂的字符呢？又如何将人看得懂的字符变成计算机看得懂的字节呢？



编码:

  将字符转换成字节的过程叫编码，编码指按照一定的规则将字符转换成字节



解码:

  将字节转换车字符的过程叫解码，解码值按照一定的规则将字节转换成字符



下面来看一段java代码



![](https://img-blog.csdnimg.cn/20181111144106780.png)

看介绍里可以看出String 的构造器可以对字节进行解码，而且可以按照指定的字符集进行解码



说一下字符集: 就是一种编码或者解码的规则，常见的字符集有: GBK,gb2312,UTF-8(这个我们以后用的最多)

![](https://img-blog.csdnimg.cn/20181111144115624.png)







String 可以解码，自然也提供了编码的方法，就是使用getBytes方法。





Demo 01:
public static void main(String[] args) throws UnsupportedEncodingException {



    //先来new一个byte数组

    byte[] bytes = "我爱java".getBytes("UTF-8");



    String str = new String(bytes, "UTF-8");



    System.*out*.println(str);

}


输出:

 Java



注意: 如果编码字符集和解码字符集不相同的话，会出现乱码

















Demo 02:
public static void main(String[] args) throws UnsupportedEncodingException {



    //先来new一个byte数组

    byte[] bytes = "我爱java".getBytes("UTF-8");



    String str = new String(bytes, "GBK");



    System.*out*.println(str);

}


输出:

鎴戠埍java



顺便一提:如果字符集不同的话，只有不在0-127之间的字符才会出现乱码





接下来看看乱码的第二个原因: 字节不完整



首先有一个问题，把一个字符转换成一个字节数组，那么这个字节数组的长度是多少?



Demo 03:        
public static void main(String[] args) throws UnsupportedEncodingException {



    byte[] bytes = "1".getBytes();



    System.*out*.println(bytes.length);   //1



    bytes = "a".getBytes();

    System.*out*.println(bytes.length);   //1



    bytes = "我".getBytes();System.*out*.println("我".length());   //1System.*out*.println(bytes.length);   //3

}


结论: 0-127之间的字符转字节一个字符对应一个字节，中文的一个汉字对应3个字节,而且，一个汉字只是一个字符



Demo 04:
public static void main(String[] args) throws UnsupportedEncodingException {



    byte[] bytes = "我".getBytes();

    

    String str = new String(bytes,0,1);

    System.*out*.println(str);    //乱码

    str = new String(bytes,0,3);

    System.*out*.println(str);    //正常

}


结论: 乱码的第二个原因: 字节不完整



OK，讲清楚编码和解码之后，我们再来讲讲转换流 。



转换流: 将字节流转换成字符流的流，主要作用是处理字符编码



转换流常用的两个类: InputStreamReader, OutputStreamWriter





InputStreamReader:

InputStreamReader 是字节流通向字符流的桥梁：它使用指定的charset读取字节并将其解码为字符。它使用的字符集可以由名称指定或显式给定，或者可以接受平台默认的字符集。

构造器:

![](https://img-blog.csdnimg.cn/20181111144135115.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





主要用最后一个 InputSteamReader(InputStream in, String charsetName)





下面来看一个小例子:





Demo 05:
public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {

        //这里我么有异常直接抛出，就不捕获了，方便看



//        BufferedReader reader = new BufferedReader(new FileReader(new File("F:\\code\\java\\2.txt")));

        //这里我们就发现了一个问题，我们在用字符流读取文件的时候，没办法指定字符集编码，这是有很大问题的

        //因为编译器的默认编码可能和文件的编码不一样，这样就会造成乱码的现象

        //所以我们希望能指定编码，同时又去用字符流去读取(加缓冲流是提高效率)，这个时候就需要转换流

        //转换流有两个: InputStreamReader, OutputStreamWriter

        

        //这样我们就可以使用指定字符集编码的字符缓冲流来读取文件了,看起来有点麻烦，实际上了解了原理以后非常的好写，我们来重新写一遍



        */**         * **第一步 new一个BufferedReader,构造器需要一个Reader类型的参数         * 第二步 由于转换流InputStreamReader是Reader的子类，所以可以把InnputStreamReader对象 传进去         * 第三步 InputStreamReader需要一个InputStream 和 一个 字符串(表示字符集编码)         */        *BufferedReader reader = new BufferedReader(

                new InputStreamReader(new FileInputStream(new File("F:\\code\\java\\2.txt")), "UTF-8")

        );   

        //这样，一个可以指定字符编码集的字符 缓冲流就构造好了，剩下的就和以前讲的一样即可

        //OutputStreamWriter和InputStreamReader差不多，只不过一个读，一个写

        BufferedWriter writer = new BufferedWriter(

                new OutputStreamWriter(new FileOutputStream(new File("F:\\code\\java\\3.txt")),"UTF-8")

        );

    }




