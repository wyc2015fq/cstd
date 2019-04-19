# Java中IO - LC900730的博客 - CSDN博客
2017年07月14日 09:39:18[lc900730](https://me.csdn.net/LC900730)阅读数：118标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
程序中操作文件和目录都可以通过File类来完成，File能新建、删除和重命名文件和目录，File不能访问文件内容本身。如果需要访问文件内容本身，需要使用输入/输出流。
File类可以使用文件路径字符串来创建File实例，默认情况下系统是根据用户的工作路径来解释相对路径，这个路径是有系统属性”user.dir”指定，通常也是运行java虚拟机时所作的路径。 
一旦创建了File对象，就可以调用File对象的方法来访问。 
getName：返回此File对象的文件名或者路径，如果是路径的话，返回最后一级子路径名称。 
getPath():返回此File对象的路径名 
getAbsoluteFile():返回绝对路径名称 
getParent():返回File对象对应目录的的父目录名 
boolean renameTo(File newName) 
boolean exist()
编写java类，继承javaServlet类；
