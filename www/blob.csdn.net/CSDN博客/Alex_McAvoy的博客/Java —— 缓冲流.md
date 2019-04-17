# Java —— 缓冲流 - Alex_McAvoy的博客 - CSDN博客





2018年10月31日 16:34:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【字节流】

## 1.BufferedReader 类

BufferedReader 类是 Reader 类的子类，该类提供一 Reader 类中没有的 readerLine() 方法，可以行为单位读取数据，其是作用在 FileReader 之上的缓冲流，速度比 FileReader 要快。

构造方法为：
- BufferedReader(Reader in)：该方法使用 Reader 类对象创建实例对象

## 2.BufferedWriter

BufferedWriter 类是 Writer 类的子类，该类提供一 Writer 类中没有的 nextLine() 方法，可以行为单位写入数据，其是作用在 FileWriter 之上的缓冲流，速度比 FileWriter 要快。

构造方法为：
- BufferedWriter(Writer out)：该方法使用 Writer 类对象创建实例对象

## 3.实例

```java
// 指定要读取文件的缓冲输入字节流  
BufferedInputStream in = new BufferedInputStream(new FileInputStream("F:\\test.jpg"));  
File file = new File("E:\\test.jpg");  
if (file != null) {  
    file.createNewFile();  
}  

// 指定要写入文件的缓冲输出字节流  
BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(file));  

byte[] bb = new byte[1024];//用来存储每次读取到的字节数组  
int n;// 每次读取到的字节数组的长度  
while ((n = in.read(bb)) != -1) {  
    out.write(bb, 0, n);// 写入到输出流  
}  

//关闭流 
out.close(); 
in.close();
```

# 【字符流】

## 1.BufferedInputStream 类

BufferedInputStream 类是 InputStream 类的子类，该类提供一 InputStream 类中没有的 readerLine() 方法，可以行为单位读取数据，其是作用在 InputStream 之上的缓冲流，速度比 InputStream 要快。

构造方法为：
- BufferedInputStream (InputStream in)：该方法使用 InputStream 类对象创建实例对象

## 2.BufferedOutputStream 类

BufferedOutputStream 类是 OutputStream 类的子类，该类提供一 OutputStream 类中没有的 nextLine() 方法，可以行为单位写入数据，其是作用在 OutputStream 之上的缓冲流，速度比 OutputStream 要快。

构造方法为：
- BufferedOutputStream(OutputStream out)：该方法使用 OutputStream 类对象创建实例对象

## 3.实例

```java
//读取信息
private static void read() throws FileNotFoundException, IOException {  
    //指定要读取的文件 
    File file = new File("E:\\a.txt"); 

    //获得该文件的缓冲输入流  
    BufferedReader bufferedReader = new BufferedReader(new FileReader(file));  
    String line = "";/ 用来保存每次读取一行的内容  
    while ((line = bufferedReader.readLine()) != null) {  
        System.out.println(line);  
    }  
    //关闭输入流  
    bufferedReader.close();
}  
  
//写入信息 
private static void write() throws IOException {  
    //指定要写入的文件 
    File file = new File("E:\\a.txt"); 
    
    //如果文件不存在则创建  
    if (!file.exists()) {
        file.createNewFile();  
    }  
    
    //获取该文件的缓冲输出流  
    BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(file)); 
 
    // 写入信息  
    bufferedWriter.write("你好世界");  
    bufferedWriter.newLine();//表示换行  
    bufferedWriter.write("hello world");  

    bufferedWriter.flush();//清空缓冲区  
    bufferedWriter.close();//关闭输出流  
}
```





