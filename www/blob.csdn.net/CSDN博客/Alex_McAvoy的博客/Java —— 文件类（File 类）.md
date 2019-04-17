# Java —— 文件类（File 类） - Alex_McAvoy的博客 - CSDN博客





2018年10月23日 14:28:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

File 类是一个与流无关的类，存放在 java.io 包下，File 类的对象可以获取文件、文件目录、文件长度等信息。

创建一个 File 类对象时，若其代表的文件不存在，系统不会自动创建，必须要调用 createNewFile() 方法来创建。

File 的方法仅涉及到文件的创建、删除、重命名等等，无法涉及到文件内容，对文件内容的修改，必须使用 IO 流来处理，因此，File 类的对象常用于做 IO 流具体类的构造器的形参。

# 【路径】

路径是在创建 File 类对象以及流使用时不可回避的问题，要注意的有两方面：反斜杠与斜杠的书写、绝对路径与相对路径

## 1.反斜杠与斜杠

由于反斜杠 " \ " 无法直接输出，因此在写路径时，要用两个反斜杠(前一个用于转义) " \\ "，例如：" D:\\xxx\\xx.txt "

如若觉得写两个反斜杠过于繁琐，则也可在写路径时用一个斜杠 " / " 代替，例如：" D:/xxx/xx.txt "

## 2.绝对路径与相对路径

绝对路径：包括盘符在内的完整的文件路径

相对路径：在当前文件目录下的文件的路径

例如：当前包的路径是：" D:\\Test "，其中有一文本文件 " hello.txt " 

则：绝对路径是：" D:\\Test\\hello.txt "，相对路径是：" hello.txt "

# 【构造器】

## 1.File(String pathname) 方法

File(String pathname) 构造方法通过指定的文件路径字符串来创建一个新的 File 实例对象

pathname：文件绝对路径字符串

```java
File file = new File("D:\\Test\\Example.txt");
```

## 2.File(String path,String filename) 方法

File(String path,String filename) 构造方法根据指定的父路径字符串和子路径字符串创建 File 类的实例对象

path：父路径字符串

filename：子路径的文件名字符串

```java
File file = new File("D:\\Test","Example.txt");
```

## 3.File(File file,String filename) 方法

File(File file,String filename) 构造方法根据指定的 File 类的父路径和字符串类型的子路径创建 File 类的实例对象

file：父路径的对象

filename：子路径文件名字符串

```java
File file1 = new File("D:\\Test");
File file2 =new File(file1,"example.txt");
```

# 【常用方法】

## 1.询问文件
|getName()|获取文件名|
|----|----|
|getParent()|获取文件父路径字符串|
|getPath()|获取文件相对路径字符串|
|getAbsolutePath()|获取文件绝对路径字符串|

## 2.文件检测
|exists()|判断文件或文件夹是否存在|
|----|----|
|canRead)(|判断文件是否可读|
|canWrite()|判断文件是否可写|
|isFile()|判断文件是否是一个文件而非目录|
|isDirectory()|判断文件是否是文件夹类型|
|isHidden()|判断是不是隐藏文件|
|isAbsolute()|判断是不是绝对路径|

## 3.文件及目录操作
|createNewFile()|创建一个新文件|
|----|----|
|delete()|删除文件或文件夹，删除成功返回 true|
|renameTo(File newName)|将当前文件名重命名为指定文件名 newName，要求当前文件必须存在，指定文件毋需存在|
|mkdir()|创建目录（只有上层目录存在的情况下），创建成功返回 true|
|mkdirs()|创建路径中包含的所有父目录和子目录（即上层目录不存在一并创建），若所有父目录和子目录都成功创建，则返回 true|

## 4.获取常规信息
|length()|获取文件长度|
|----|----|
|lastModified()|获取文件最后修改日期|
|list()|以 String 类型的形式读取文件夹中的文件名|
|listFiles()|以 File 类型的形式读取文件夹中的文件|

对于 list() 方法与 listFiles() 方法以下两种用法等价：

```java
File file=new File("D:\\Test");

String[] str=file.list();
for(int i=0;i<str.length;i++)
    System.out.println(str[i]);

File[] files=file.listFiles();
for(int i=0;i<files.length;i++)
    System.out.println(files[i].getName());
```





