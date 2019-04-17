# Java访问Solr问题: Unsupported major.minor version 52.0 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月25日 23:16:39[boonya](https://me.csdn.net/boonya)阅读数：2190








Solr版本：6.4.1 需要JDK 1.8.x以上。


JDK 版本过低：



```
Exception in thread "main" java.lang.UnsupportedClassVersionError: org/apache/solr/common/params/SolrParams : Unsupported major.minor version 52.0
	at java.lang.ClassLoader.defineClass1(Native Method)
	at java.lang.ClassLoader.defineClass(Unknown Source)
	at java.security.SecureClassLoader.defineClass(Unknown Source)
	at java.net.URLClassLoader.defineClass(Unknown Source)
	at java.net.URLClassLoader.access$100(Unknown Source)
	at java.net.URLClassLoader$1.run(Unknown Source)
	at java.net.URLClassLoader$1.run(Unknown Source)
	at java.security.AccessController.doPrivileged(Native Method)
	at java.net.URLClassLoader.findClass(Unknown Source)
	at java.lang.ClassLoader.loadClass(Unknown Source)
	at sun.misc.Launcher$AppClassLoader.loadClass(Unknown Source)
	at java.lang.ClassLoader.loadClass(Unknown Source)
	at java.lang.Class.getDeclaredMethods0(Native Method)
	at java.lang.Class.privateGetDeclaredMethods(Unknown Source)
	at java.lang.Class.getMethod0(Unknown Source)
	at java.lang.Class.getMethod(Unknown Source)
	at sun.launcher.LauncherHelper.getMainMethod(Unknown Source)
	at sun.launcher.LauncherHelper.checkAndLoadMain(Unknown Source)
```

升级JDK版本即可解决此问题！





