
# java文件构造器的使用及常用的功能介绍 - 李昆鹏的博客 - CSDN博客


2018年04月03日 17:30:49[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：69


-------------------------------java文件构造器的使用及常用的功能介绍--------------------------
**java.io**
**类 File**
java.lang.Object
**java.io.File**
**所有已实现的接口：**
Serializable,Comparable<File>
**我们操作系统中所有的设备的体现都是文件（包括文件夹），java****对文件做了抽象，在java中IO包提供了文件的操作的功能**
|构造方法摘要
|
|File|(|File|parent,|String|child)
|根据 parent 抽象路径名和 child 路径名字符串创建一个新  File 实例。
|
|File|(|String|pathname)
|通过将给定路径名字符串转换为抽象路径名来创建一个新 File 实例。
|
|File|(|String|parent,|String|child)
|根据 parent 路径名字符串和 child 路径名字符串创建一个新  File 实例。
|
|File|(|URI|uri)
|通过将给定的 file: URI 转换为一个抽象路径名来创建一个新的 File 实例。
|
======================================================
构造器的使用例子
**public****class**FileDemo {
**public****static****void**main (String[]args) {
//用第一种构造器创建文件的对象,建议使用这一种
Filefile=**new**File("E:\\李昆鹏\\java\\javaseWord文件");
//还可以使用斜线来创建对象
Filefile1=**new**File("E:/李昆鹏/java/javaseWord文件");
//直接打印的就是文件的路径
System.**out**.println(file);
System.**out**.println(file1);
//输出
//E:\李昆鹏\java\javaseWord文件
//E:\李昆鹏\java\javaseWord文件
//第二种构造器，第一个参数是路径，第二个参数是文件名
Filefile2=**new**File("E:/李昆鹏/java/","javaseWord文件");
System.**out**.println(file2);
//输出
//E:\李昆鹏\java\javaseWord文件
//第三种构造器，创建文件的父级文件
Filefile3=**new**File("E:/李昆鹏/java/");
Filefile4=**new**File(file3,"javaseWord文件");
System.**out**.println(file4);
//输出
//E:\李昆鹏\java\javaseWord文件

}

--------------------------------------------
文件的创建
**public****class**FileDemo2 {
**public****static****void**main (String[]args)**throws**IOException {
//创建文件的对象
Filefile=**new**File("E:\\test\\test.java");
//根据对象来创建文件
**boolean**isSuccess=file.createNewFile();
//如果创建成功显示true，反之false
//如果文件已经存在就会创建失败返回false
System.**out**.println(isSuccess);
//创建目录
Filefile1=**new**File("E:/test/我的目录");
//单级目录创建
**boolean**isSuc1=file1.mkdir();
System.**out**.println(isSuc1);
//多级目录创建
Filefile2=**new**File("E:/test/我的目录/我的目录1/我的目录2");
//多级目录创建
**boolean**isSuc2=file2.mkdirs();
System.**out**.println(isSuc2);
}
}
---------------------------------------------------------
文件的删除
**public****class**FileDemo3 {
**public****static****void**main (String[]args)**throws**IOException {
//创建文件的对象
Filefile=**new**File("E:\\test\\test.java");
//根据对象来创建文件
**boolean**isSuccess=file.createNewFile();
//如果创建成功显示true，反之false
//如果文件已经存在就会创建失败返回false
//删除文件,如果删除成功返回true,反之false
//如果要删除的文件正在使用会删除失败返回false
**boolean**isDelete=file.delete();
//删除的文件不会在回收站里面
System.**out**.println(isDelete);
Filefile1=**new**File("E:/test/我的目录/我的目录1");
**boolean**isSuc=file1.mkdirs();
//删除文件夹
//如果目录中不是空就删除不掉，一定是空目录才可以删除
**boolean**isDeleteDir=file1.delete();
System.**out**.println(isDeleteDir);

}
}

------------------------------------------------------
文件的相对路径，就是项目工程目录
**public****class**FileDemo4 {
**public****static****void**main (String[]args)**throws**IOException {
//创建文件的对象
//使用相对路径来创建文件，不指定判读，相对路径就是工程文件夹
//位置E:\eclipseWork\file_demo\test.java
//在工程目录下查看时需要刷新下refresh
Filefile=**new**File("test.java");
file.createNewFile();
System.**out**.println(file);
//输出test.java
//创建相对文件夹
//E:\eclipseWork\file_demo\aa\bb
Filefile1=**new**File("aa/bb");
file1.mkdirs();
//在package里面来创建文件
Filefile2=**new**File("src/cn/tx/file/hello.mp3");
file2.createNewFile();

}
}
----------------------------------------------------------
文件的判断
**public****class**FileDemo5 {
**public****static****void**main (String[]args)**throws**IOException {
Filefile=**new**File("hello");
Filefile1=**new**File("E:/test/hello.java");
//创建文件夹
file.mkdir();
file1.createNewFile();
System.**out**.println("判断文件是否存在："+file.exists() );
//输出判断文件是否存在：true
System.**out**.println("判断是否是文件："+file.isFile() );
//输出判断是否是文件：false
System.**out**.println("判断是否是文件夹："+file.isDirectory());
//输出判断是否是文件夹：true
System.**out**.println("判断文件是否是绝对路径："+file.isAbsolute());
System.**out**.println("判断文件是否是绝对路径："+file1.isAbsolute());
//输出
//判断文件是否是绝对路径：false
//判断文件是否是绝对路径：true
System.**out**.println("判断文件是否是可读："+file1.canRead());
//输出判断文件是否是可读：true
System.**out**.println("判断文件是否可写："+file1.canWrite());
//输出判断文件是否可写：true
System.**out**.println("判断文件是否是隐藏的"+file1.isHidden());
//输出判断文件是否是隐藏的false
}
}

------------------------------------------------------
文件的获取功能
**public****class**FileDemo6 {
**public****static****void**main (String[]args)**throws**IOException {
Filefile=**new**File("hello.txt");
//获取文件的绝对路径
StringaPath=file.getAbsolutePath();
System.**out**.println(aPath);
//输出E:\eclipseWork\file_demo\hello.txt
//获得相对路径
Stringpath=file.getPath();
System.**out**.println(path);
//输出hello.txt
//获得文件的名字,包含文件的后缀
Stringname=file.getName();
System.**out**.println(name);
//输出hello.txt
//获得文件的大小，大小的单位为字节
**long**length=file.length();
System.**out**.println(length);
//输出0
//获得文件最后的修改时间,以毫秒为单位
**long**time=file.lastModified();
System.**out**.println(time);
//输出1522742858272
//将毫秒转换为具体的时间
//创建是个格式化的对象
SimpleDateFormatsdf=**new**SimpleDateFormat("yyyy-MM-dd  HH:mm:ss");
//创建时间对象
Datedate=**new**Date(time);
StringtimeStr=sdf.format(date);
System.**out**.println(timeStr);
//输出2018-04-03  16:07:38
}
}

----------------------------------------------------
文件目录的子文件获取
**public****class**FileDemo7 {
**public****static****void**main (String[]args)**throws**IOException {
//获得系统的根目录
File[]files= File.*listRoots*();
**for**(Filefile2:files) {
System.**out**.print(file2+",");
}
//输出C:\,D:\,E:\,
System.**out**.println();
//创建一个文件对象
Filefile=**new**File("E:/eclipseWork/PlaneGame0.8");
//获得指定目录下的所有子文件
File[]file1=file.listFiles();
**for**(Filef:file1)
System.**out**.println(f);
//输出
//E:\eclipseWork\PlaneGame0.8\.classpath
//E:\eclipseWork\PlaneGame0.8\.project
//E:\eclipseWork\PlaneGame0.8\.settings
//E:\eclipseWork\PlaneGame0.8\bin
//E:\eclipseWork\PlaneGame0.8\src

//获得指定目录下的所有子文件的名字
String[]fileNames=file.list();
**for**(Strings:fileNames)
System.**out**.print(s+"、");
//输出.classpath、.project、.settings、bin、src、
}
}


