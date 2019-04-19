# Failed to create the Java Virtual Machine(zt) - =朝晖= - 博客园
# [Failed to create the Java Virtual Machine(zt)](https://www.cnblogs.com/dhcn/p/7100250.html)
[http://lixueli26.iteye.com/blog/711152](http://lixueli26.iteye.com/blog/711152)
在以下版本也发生类似情况，采用同样方法得以解决。
版本：**eclipse-jee-indigo-win32**
**自己电脑上装的jdk版本是：jdk-6u26**
解压后启动，发现弹出提示框“ **Failed to create the [Java](http://lib.csdn.net/base/java)Virtual Machine**”
**解决方法：**
**找到eclipse目录下的eclipse.ini，可以看到如下内容：**-startup
plugins/org.eclipse.equinox.launcher_1.1.0.v20100507.jar
–launcher.library
plugins/org.eclipse.equinox.launcher.win32.win32.x86_1.1.0.v20100503
-product
org.eclipse.epp.package.jee.product
–launcher.defaultAction
openFile
–launcher.XXMaxPermSize
256M
-showsplash
org.eclipse.platform
–launcher.XXMaxPermSize
256m
–launcher.defaultAction
openFile
-vmargs
-Dosgi.requiredJavaVersion=1.5
-Xms40m
-Xmx512m
修改–launcher.XXMaxPermSize  128
如下：
-startup
plugins/org.eclipse.equinox.launcher_1.1.0.v20100507.jar
–launcher.library
plugins/org.eclipse.equinox.launcher.win32.win32.x86_1.1.0.v20100503
-product
org.eclipse.epp.package.jee.product
–launcher.defaultAction
openFile
–launcher.XXMaxPermSize
128M
-showsplash
org.eclipse.platform
–launcher.XXMaxPermSize
128m
–launcher.defaultAction
openFile
-vmargs
-Dosgi.requiredJavaVersion=1.5
-Xms40m
-Xmx512m

