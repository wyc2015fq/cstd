# Java 获取系统信息 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月12日 13:25:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：29
个人分类：[J2EE](https://blog.csdn.net/leixiaohua1020/article/category/1362940)










Java 获取系统信息可以用以下方法：



```java
public class testsysteminfo {
	public static void main(String[] args) {
		// TODO Auto-generated method stub
        System.out.println(System.getProperty("java.version"));   //java版本号
        System.out.println(System.getProperty("java.vendor"));    //Java提供商名称
        System.out.println(System.getProperty("java.vendor.url"));   //Java提供商网站
        System.out.println(System.getProperty("java.home"));   //jre目录
        System.out.println(System.getProperty("java.vm.specification.version"));   //Java虚拟机规范版本号
        System.out.println(System.getProperty("java.vm.specification.vendor"));   //Java虚拟机规范提供商
        System.out.println(System.getProperty("java.vm.specification.name"));   //Java虚拟机规范名称
        System.out.println(System.getProperty("java.vm.version"));   //Java虚拟机版本号
        System.out.println(System.getProperty("java.vm.vendor"));   //Java虚拟机提供商
        System.out.println(System.getProperty("java.vm.name"));   //Java虚拟机名称
        System.out.println(System.getProperty("java.specification.version"));   //Java规范版本号
        System.out.println(System.getProperty("java.specification.vendor"));   //Java规范提供商
        System.out.println(System.getProperty("java.specification.name"));   //Java规范名称
        System.out.println(System.getProperty("java.class.version"));   //Java类版本号
        System.out.println(System.getProperty("java.class.path"));   //Java类路径
        System.out.println(System.getProperty("java.library.path"));   //Java lib路径
        System.out.println(System.getProperty("java.io.tmpdir"));   //Java输入输出临时路径
        System.out.println(System.getProperty("java.compiler"));   //Java编译器
        System.out.println(System.getProperty("java.ext.dirs"));   //Java执行路径
        System.out.println(System.getProperty("os.name"));   //操作系统名称
        System.out.println(System.getProperty("os.arch"));   //
        System.out.println(System.getProperty("os.version"));   //版本号
        System.out.println(System.getProperty("file.separator"));   //文件分隔符
        System.out.println(System.getProperty("path.separator"));   //路径分隔符
        System.out.println(System.getProperty("line.separator"));   //直线分隔符
        System.out.println(System.getProperty("user.name"));   //用户名
        System.out.println(System.getProperty("user.home"));   //用户所在目录         
        System.out.println(System.getProperty("user.dir")); //当前应用所在目录
	}
}
```










