
# java的System类常用方法 - 李昆鹏的博客 - CSDN博客


2018年03月16日 16:40:54[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：2204


**-------------------------java的System类常用方法-------------------**
## java.lang
## 类 System
java.lang.Object**java.lang.System**
---

public final class**System**extendsObject`System`类包含一些有用的类字段和方法。它不能被实例化。
在`System`类提供的设施中，有标准输入、标准输出和错误输出流；对外部定义的属性和环境变量的访问；加载文件和库的方法；还有快速复制数组的一部分的实用方法。
|字段摘要
|
|static|PrintStream
|err
|“标准”错误输出流。
|
|static|InputStream
|in
|“标准”输入流。
|
|static|PrintStream
|out
|“标准”输出流。
|
|方法摘要
|
|static void
|arraycopy|(|Object|src, int srcPos,|Object|dest,  int destPos, int length)
|从指定源数组中复制一个数组，复制从指定的位置开始，到目标数组的指定位置结束。
|
|static|String
|clearProperty|(|String|key)
|移除指定键指示的系统属性。
|
|static|Console
|console|()
|返回与当前 Java 虚拟机关联的唯一|Console|对象（如果有）。
|
|static long
|currentTimeMillis|()
|返回以毫秒为单位的当前时间。
|
|static void
|exit|(int status)
|终止当前正在运行的 Java 虚拟机。
|
|static void
|gc|()
|运行垃圾回收器。
|
|static|Map|<|String|,|String|>
|getenv|()
|返回一个不能修改的当前系统环境的字符串映射视图。
|
|static|String
|getenv|(|String|name)
|获取指定的环境变量值。
|
|static|Properties
|getProperties|()
|确定当前的系统属性。
|
|static|String
|getProperty|(|String|key)
|获取指定键指示的系统属性。
|
|static|String
|getProperty|(|String|key,|String|def)
|获取用指定键描述的系统属性。
|
|static|SecurityManager
|getSecurityManager|()
|获取系统安全接口。
|
|static int
|identityHashCode|(|Object|x)
|返回给定对象的哈希码，该代码与默认的方法  hashCode() 返回的代码一样，无论给定对象的类是否重写 hashCode()。
|
|static|Channel
|inheritedChannel|()
|返回从创建此 Java 虚拟机的实体中继承的信道。
|
|static void
|load|(|String|filename)
|从作为动态库的本地文件系统中以指定的文件名加载代码文件。
|
|static void
|loadLibrary|(|String|libname)
|加载由|libname|参数指定的系统库。
|
|static|String
|mapLibraryName|(|String|libname)
|将一个库名称映射到特定于平台的、表示本机库的字符串中。
|
|static long
|nanoTime|()
|返回最准确的可用系统计时器的当前值，以毫微秒为单位。
|
|static void
|runFinalization|()
|运行处于挂起终止状态的所有对象的终止方法。
|
|static void
|runFinalizersOnExit|(boolean value)
|已过时。|该方法具有固有的不安全性。它可能对正在使用的对象调用终结方法，而其他线程同时正在操作这些对象，从而导致不正确的行为或死锁。
|
|static void
|setErr|(|PrintStream|err)
|重新分配“标准”错误输出流。
|
|static void
|setIn|(|InputStream|in)
|重新分配“标准”输入流。
|
|static void
|setOut|(|PrintStream|out)
|重新分配“标准”输出流。
|
|static void
|setProperties|(|Properties|props)
|将系统属性设置为|Properties|参数。
|
|static|String
|setProperty|(|String|key,|String|value)
|设置指定键指示的系统属性。
|
|static void
|setSecurityManager|(|SecurityManager|s)
|设置系统安全性。
|
获得系统属性参数：
### getProperties
public staticProperties**getProperties**()确定当前的系统属性。
首先，如果有安全管理器，则不带参数直接调用其`checkPropertiesAccess`方法。这可能导致一个安全性异常。
将`getProperty(String)`方法使用的当前系统属性集合作为`Properties`对象返回。如果没有当前系统属性集合，则先创建并初始化一个系统属性集合。这个系统属性集合总是包含以下键的值：
|键
|相关值的描述
|
|java.version
|Java 运行时环境版本
|
|java.vendor
|Java 运行时环境供应商
|
|java.vendor.url
|Java 供应商的 URL
|
|java.home
|Java 安装目录
|
|java.vm.specification.version
|Java 虚拟机规范版本
|
|java.vm.specification.vendor
|Java 虚拟机规范供应商
|
|java.vm.specification.name
|Java 虚拟机规范名称
|
|java.vm.version
|Java 虚拟机实现版本
|
|java.vm.vendor
|Java 虚拟机实现供应商
|
|java.vm.name
|Java 虚拟机实现名称
|
|java.specification.version
|Java 运行时环境规范版本
|
|java.specification.vendor
|Java 运行时环境规范供应商
|
|java.specification.name
|Java 运行时环境规范名称
|
|java.class.version
|Java 类格式版本号
|
|java.class.path
|Java 类路径
|
|java.library.path
|加载库时搜索的路径列表
|
|java.io.tmpdir
|默认的临时文件路径
|
|java.compiler
|要使用的 JIT 编译器的名称
|
|java.ext.dirs
|一个或多个扩展目录的路径
|
|os.name
|操作系统的名称
|
|os.arch
|操作系统的架构
|
|os.version
|操作系统的版本
|
|file.separator
|文件分隔符（在 UNIX 系统中是“/”）
|
|path.separator
|路径分隔符（在 UNIX 系统中是“:”）
|
|line.separator
|行分隔符（在 UNIX 系统中是“/n”）
|
|user.name
|用户的账户名称
|
|user.home
|用户的主目录
|
|user.dir
|用户的当前工作目录
|
系统属性值中的多个路径是用平台的路径分隔符分隔的。
注意，即使安全管理器不允许执行`getProperties`操作，它可能也会选择允许执行`getProperty(String)`操作。
**返回：**
系统属性
**抛出：**
`SecurityException`- 如果安全管理器存在并且其`checkPropertiesAccess`方法不允许访问系统属性。
**另请参见：**
`setProperties(java.util.Properties)`,`SecurityException`,`SecurityManager.checkPropertiesAccess()`,`Properties`
---

常用例子：
**public****class**Test1 {
**public****static****void**main(String[]args) {
**int**[]arr1={5,6,7,0,6,6,1,2,3};
**int**[]arr2={12,14,67};
//数组拷贝
//第一个参数：要拷贝数组
//第二个参数：要拷贝数组的起始索引
//第三个参数：目标数组
//第四个参数：目标数组的索引起始位置
//第五个参数：要拷贝的长度
//把原来的位置的参数替换
System.*arraycopy*(arr2, 1 ,arr1, 3 , 2);
**for**(**int**i= 0;i<arr1.length;i++) {
System.**out**.print(arr1[i]+",");
}
System.**out**.println();
//获得当前时间的毫秒数，从1970年到现在的毫秒数
**long**tm= System.*currentTimeMillis*();
System.**out**.println("从1970年到现在的毫秒数:"+tm);
//计算程序的运行时间
**long**startTime= System.*currentTimeMillis*();
**try**{
Thread.*sleep*(200);
}**catch**(InterruptedExceptione) {
//**TODO**Auto-generated catch block
e.printStackTrace();
}
**long**endTime= System.*currentTimeMillis*();
System.**out**.println("程序的运行时间是："+ (endTime-startTime));
//退出jvm
//System.exit(0);
//调用垃圾回收器
System.*gc*();
//
Stringpath= System.*getProperty*("java.home");
System.**out**.println(path);
}
}
结果：
5,6,7,14,67,6,1,2,3,
从1970年到现在的毫秒数:1521189519381
程序的运行时间是：200
C:\Program Files\Java\jre1.8.0_161


