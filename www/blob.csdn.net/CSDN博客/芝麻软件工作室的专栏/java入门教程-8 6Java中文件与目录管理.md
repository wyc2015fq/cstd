
# java入门教程-8.6Java中文件与目录管理 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:37:59[seven-soft](https://me.csdn.net/softn)阅读数：142个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



目录是管理文件的特殊机制，同类文件保存在同一个目录下不仅可以简化文件管理，而且还可以提高工作效率。Java 语言在 java.io 包中定义了一个 File 类专门用来管理磁盘文件和目录。
每个 File 类对象表示一个磁盘文件或目录，其对象属性中包含了文件或目录的相关信息。通过调用 File 类提供的各种方法，能够创建、删除、重名名文件、判断文件的读写权限以及是否存在，设置和查询文件的最近修改时间等。不同操作系统具有不同的文件系统组织方式，通过使用
 File 类对象，Java 程序可以用与平台无关的、统一的方式来处理文件和目录。
## 创建 File 类的对象
创建 File 类对象需要给出其所对应的文件名或目录名，File 类的构造方法如表 10-9 所示。
表 10-9 File 类的构造方法|构造方法|功能描述|
|public File(String path)|指定与 File 对象关联的文件或目录名，path 可以包含路径及文件和目录名|
|public File(String path, String name)|以 path 为路径，以 name 为文件或目录名创建 File 对象|
|public File(File dir, String name)|用现有的 File 对象 dir 作为目录，以 name 作为文件或目录名创建 File 对象|
|public File(UR ui)|使用给定的统一资源定位符来定位文件|
在使用 File 类的构造方法时，需要注意下面几点：
（1）path 参数可以是绝对路径，也可以是相对路径，也可以是磁盘上的某个目录。
（ 2）由于不同操作系统使用的目录分隔符不同，可以使用 System 类的一个静态变量System.dirSep，来实现在不同操作系统下都通用的路径。如：
"d:"+System.dirSep+"myjava"+System.dirSep+"file"
## 获取属性和操作
借助 File 对象，可以获取文件和相关目录的属性信息并可以对其进行管理和操作。表 10-10列出了其常用的方法及说明。
表 10-10 File 的常用方法|方法|功能描述|
|boolean canRead()|如果文件可读，返回真，否则返回假|
|boolean canWrite()|如果文件可写，返回真，否则返回假|
|boolean exists()|判断文件或目录是否存在|
|boolean createNewFile()|若文件不存在，则创建指定名字的空文件，并返回真，若不存在返回假|
|boolean isFile()|判断对象是否代表有效文件|
|boolean isDirectory()|判断对象是否代表有效目录|
|boolean equals(File f)|比较两个文件或目录是否相同|
|string getName()|返回文件名或目录名的字符串|
|string getPath()|返回文件或目录路径的字符串|
|long length()|返回文件的字节数，若 File 对象代表目录，则返回 0|
|long lastModified()|返回文件或目录最近一次修改的时间|
|String[] list()|将目录中所有文件名保存在字符串数组中并返回，若 File 对象不是目录返回 null|
|boolean delete()|删除文件或目录，必须是空目录才能删除，删除成功返回真，否则返回假|
|boolean mkdir()|创建当前目录的子目录，成功返回真，否则返回假|
|boolean renameTo(File newFile)|将文件重命名为指定的文件名|
【例 10-11】判断输入的绝对路径是代表一个文件或一个目录。若是文件输出此文件的绝对路径，并判断此文件的文件属性（是否可读写或隐藏）;若是目录则输出该目录下所有文件（不包括隐藏文件）（[查看源代码](http://www.weixueyuan.net/uploads/code/java/rumen/10-11.txt)）。
运行结果如图 10-11 所示：![图 10-11  输入一个文件路径后例 10_11 的运行结果](http://www.weixueyuan.net/uploads/allimg/130223/8-130223002415423.png)
图 10-11  输入一个文件路径后例 10_11 的运行结果

