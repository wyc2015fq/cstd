# 关于使用myeclipse进行export 打包的一些注意点 - z69183787的专栏 - CSDN博客
2012年12月26日 15:13:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2902
打包时有2个选项：
基本 jar 与 runnable jar
![](https://img-my.csdn.net/uploads/201212/26/1356505919_1875.jpg)
2个我都尝试过，结果发现runnable jar 打包方式需要选择一个单独 存在main函数的 类，
![](https://img-my.csdn.net/uploads/201212/26/1356505982_1626.jpg)
原因，大家应该也能够想到，exe嘛，应该只有1个主程序。
这样打出来的jar包，在装有jre环境变量的电脑上可以直接执行。
而默认的jar包打包方式可以包含一个项目中的所有文件。
2种打包方式，其中有一个manifest.mf文件可以设置，jar包运行时的一些环境变量。
比如 classpath，author，main-class 等。具体可以在晚上查找manifest.mf文件的作用。
可以说让 jar包可以直接运行，很关键的文件就是 manifest.mf 文件。
[http://blog.163.com/xiaokangzhijia@126/blog/static/1659548562010112991634509/](http://blog.163.com/xiaokangzhijia@126/blog/static/1659548562010112991634509/)
执行jar包的方式有 
java -jar xxx.jar
或者直接 打开方式选择jar方式。
