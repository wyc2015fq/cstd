# maven-compiler-plugin 指定JDK版本和编码 - z69183787的专栏 - CSDN博客
2018年06月21日 16:28:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：649
[https://blog.csdn.net/ljj821061514/article/details/60867197](https://blog.csdn.net/ljj821061514/article/details/60867197)
最近使用maven install命令来打包项目时，出现了：
https://www.cnblogs.com/Hxinguan/p/6132446.html
[html][view plain](https://blog.csdn.net/ljj821061514/article/details/60867197#)[copy](https://blog.csdn.net/ljj821061514/article/details/60867197#)
- [ERROR] Failed to execute goal org.apache.maven.plugins:maven-compiler-plugin:3.1:compile (default-compile) on project server-api: Compilation failure: Compilation failure:  
- [ERROR] /E:/51/练习项目/server-api/src/main/java/com/mousycoder/server/core/page/PageUtil.java:[60,46] -source 1.5 中不支持 diamond 运算符  
- [ERROR] (请使用 -source 7 或更高版本以启用 diamond 运算符)  
- [ERROR] /E:/51/练习项目/server-api/src/main/java/com/mousycoder/server/core/util/PropUtil.java:[129,58] -source 1.5 中不支持 diamond 运算符  
- [ERROR] (请使用 -source 7 或更高版本以启用 diamond 运算符)  
- [ERROR] /E:/51/练习项目/server-api/src/main/java/com/mousycoder/server/dao/BaseDao.java:[137,51] -source 1.5 中不支持 diamond 运算符  
- [ERROR] (请使用 -source 7 或更高版本以启用 diamond 运算符)  
- [ERROR] /E:/51/练习项目/server-api/src/main/java/com/mousycoder/server/core/util/BeanUtil.java:[76,42] -source 1.5 中不支持 diamond 运算符  
- [ERROR] (请使用 -source 7 或更高版本以启用 diamond 运算符)  
- [ERROR] /E:/51/练习项目/server-api/src/main/java/com/mousycoder/server/core/util/BeanUtil.java:[87,48] -source 1.5 中不支持方法引用  
- [ERROR] (请使用 -source 8 或更高版本以启用方法引用)  
- [ERROR] -> [Help 1]  
很明显是maven的插件plugin的问题，这是因为没有指定jdk版本和编码（防止乱码），加入如下即可：
[html][view plain](https://blog.csdn.net/ljj821061514/article/details/60867197#)[copy](https://blog.csdn.net/ljj821061514/article/details/60867197#)
- <!-- 指定maven编译的jdk版本,如果不指定,maven3默认用jdk 1.5 maven2默认用jdk1.3 -->
- <plugin>
- <groupId>org.apache.maven.plugins</groupId>
- <artifactId>maven-compiler-plugin</artifactId>
- <version>3.1</version>
- <configuration>
- <source>1.8</source>
- <target>1.8</target>
- <encoding>UTF-8</encoding>
- </configuration>
- </plugin>
如图：
maven 2 默认使用jdk1.3来编译，maven 3默认使用jdk1.5来编译，如果项目使用jdk1.6版本的话，也要指定jdk版本为1.6.
