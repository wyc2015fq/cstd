# Java —— 节点流 - Alex_McAvoy的博客 - CSDN博客





2018年10月23日 14:29:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【字节流】

## 1.FileInputStream 类

FileInputStream 类是 InputStream 类的子类，适用于较简单的文件读取，其方法均是从 InputStream 类继承并重写的，创建文件字节输入流常用的构造方法有以下两种：
- FileInputStream(String filePath)：根据指定文件名称与路径，创建实例对象，其中 filePath 为文件的绝对路径或相对路径
- FileInputStream(File file)：使用指定的 File 类型文件对象创建实例对象，其中 file 为 File 类型的实例对象

```java
/* 
 * 在 D 盘存在 Example.txt 文件，文件内容为 Hello World!
 * 创建一个 File 类对象，然后创建文件字节输入流对象 fis
 * 从输入流中读取文件的信息
 */

File file = new File("D:\\Example.txt");//创建一个File类对象
try{
    System.out.print("The content is:");

    //创建一个FileInputStream类对象
    FileInputStream fis=new FileInputStream(file);
    
    //调用FileInputStream类方法来读取文件信息
    int res=fis.read();
    while( res!=-1 ){ //读取输入流数据
        System.out.print(res);
        res=fis.read();
    }

    //关闭输入流
    fis.close();
} catch (IOException e){
    e.printStackTrace();
}
```

## 2.FileOutputStream 类

FileOutStream 类是 OutputStream 类的子类，该类属于节点流，适用于较简单的文件写入，其方法均是从 OutputStream 类继承并重写的，创建文件字节输出流常用的构造方法有以下两种：
- FileOutStream(String filePath)：根据指定文件名称与路径，创建实例对象，其中 filePath 为文件的绝对路径或相对路径
- FileOutStream(File file)：使用指定的 File 类型文件对象创建实例对象，其中 file 为 File 类型的实例对象

```java
/* 
 * 创建一个 File 类对象，首先判断此配置文件是否存在
 * 如果不存在，则调用方法创建一个文件，从键盘输入字符存入数组
 * 创建文件输出流，把数组里的字符写入到文件，最终结果存入 Example.txt 文件
 */

File file = new File("D:\\Example.txt");//创建一个File类对象
byte bytes[]=new byte[512];
try{
    //判断文件是否存在
    if(!file.exists())
        file.createNewFile();

    //创建文件输出流
    int b=System.in.read(bytes);

    //创建一个FileOutputStream类对象
    FileOutputStream fos=new FileOutputStream(file,true);
    
    //调用FileOutputStream类方法来写入文件信息
    fos.write(bytes,0,b);

    //关闭输入流
    fos.close();
} catch (IOException e){
    e.printStackTrace();
}
```

# 【字符流】

## 1.FileReader 类

FileReader 类是 Reader 类的子类，其实现了从文件中读出字符数据，是文件字符输入流，属于节点流，其方法均是从 Reader 类中继承的，常用的构造方法有两种：
- FileReader(String filePath)：该方法根据指定的文件名称和路径，创建关联该文件的实例对象
- FileReader(File file)：该方法使用 File 类型的文件对象，创建关联该文件的实例对象

```java
/* 
 * 在 D 盘存在 Example.txt 文件，文件内容为 Hello World!
 * 从输入流中读取文件的信息
 */

File file = new File("D:\\Example.txt");//创建一个File类对象
try{
    System.out.print("The content is:");

    //创建一个FileReader类对象
    FileReader fr=new FileReader(file);
    
    //调用FileReader类方法来读取文件信息
    int rs=fr.read();
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

## 2.FileWriter 类

FileWriter 类是 Writer 类的子类，实现了将字符数据写入文件中，是文件字符输出流，其常用方法均是由 Writer 类继承而来的，其构造方法有以下两种：
- FileWriter(String filePath)：该方法根据指定的文件名称和路径，创建关联该文件的实例对象
- FileWriter(File file)：该方法使用 File 类型的文件对象，创建关联该文件的实例对象

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
    FileReader fos=new FileReader(file);
    
    //调用OutputSteamWriter类方法来写入文件信息
    fos.write();

    //关闭输入流
    out.close();
} catch (IOException e){
    e.printStackTrace();
}
```





