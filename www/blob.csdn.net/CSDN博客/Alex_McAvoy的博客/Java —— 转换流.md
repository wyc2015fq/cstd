# Java —— 转换流 - Alex_McAvoy的博客 - CSDN博客





2018年10月28日 20:48:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：37
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【转换流】

转换流用于将字节流转换为字符流，其根据编码方式，将字节转换为字符。

编码：字符串 ——> 字符数组

解码：字节数组 ——> 字符串

# 【InputStreamReader 类】

InputStreamReader 类是字节流通往字符流的桥梁，其可以根据指定的编码方式，将字节输入流转换为字符输入流，其方法均是从 Reader 类继承的，创建文件字符输入流常用的构造方法有以下两种：
- InputStreamReader(InputStream in)：该方法使用默认字符集创建实例对象
- InputStreamReader(InputStream in,String cname)：使用已命名的字符编码方式创建实例对象，cname 为使用的编码方式名

```java
/* 
 * 在 D 盘存在 Example.txt 文件，文件内容为 Hello World!
 * 创建一个 File 类对象，然后创建文件字符输入流对象 fis
 * 从输入流中读取文件的信息
 */

File file = new File("D:\\Example.txt");//创建一个File类对象
try{
    System.out.print("The content is:");

    //创建一个FileInputStream类对象
    fis=new FileInputStream(file);

    //创建一个InputSteamReader类对象
    InputSteamReader isr=new InputStreamReader(fis);
    
    //调用InputSteamReader类方法来读取文件信息
    int rs=isr.read();
    while( res!=-1 ){ //读取输入流数据
        System.out.print(res);
        res=isr.read();
    }

    //关闭输入流
    fis.close();
} catch (IOException e){
    e.printStackTrace();
}
```

# 【OutputStreamWriter 类】

OutputStreamWriter 类是字节流通往字符流的桥梁，其可以根据指定的编码方式，将字节输出流转换为字符输出流，其方法均是从 Writer 类继承的，创建文件字符输出流常用的构造方法有以下两种：
- OutputStreamWriter(OutputStream in)：该方法使用默认字符集创建实例对象
- OutputStreamWriter(OutputStream in,String cname)：使用已命名的字符编码方式创建实例对象，cname 为使用的编码方式名

```java
/* 
 * 创建一个 File 类对象，首先判断此配置文件是否存在
 * 如果不存在，则调用方法创建一个文件
 * 创建文件输出流，写入到文件，最终结果存入 Example.txt 文件
 */

File file = new File("D:\\Example.txt");//创建一个File类对象
try{
    //判断文件是否存在
    if(!file.exists())
        file.createNewFile();

    //创建一个FileOutputStream类对象
    FileOutputStream fos=new FileOutputStream(file,true);

    //创建一个OutputSteamWriter类对象
    OutputSteamWriter out=new OutputSteamWriter(fos)
    
    //调用OutputSteamWriter类方法来写入文件信息
    out.write();

    //关闭输入流
    out.close();
} catch (IOException e){
    e.printStackTrace();
}
```





