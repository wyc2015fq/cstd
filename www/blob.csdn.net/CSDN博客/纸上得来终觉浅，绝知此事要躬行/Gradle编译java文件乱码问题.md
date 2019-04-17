# Gradle编译java文件乱码问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月27日 16:04:07[boonya](https://me.csdn.net/boonya)阅读数：3439








已经安装Gradle很久了，一直没有深入地去研究。今天就简单试了一下将项目打成jar包。



### 本篇文章用到的Gradle命令



```java
gradle build
```

### 编译Java文件提示乱码

![](https://img-blog.csdn.net/20160727155946731)


### **build.gradle 编码处理**

在build.gradle文件中加入字符类型设置：



```java
tasks.withType(JavaCompile) {  
    options.encoding = 'UTF-8'
}
```
完整的build.gradle文件内容：





```java
apply plugin: 'java'
apply plugin: 'eclipse'

tasks.withType(JavaCompile) {  
    options.encoding = 'UTF-8'
}

jar {
   baseName='Https'
   version='1.0.0'
}

repositories {
    mavenLocal()
    mavenCentral()
}

dependencies {
    compile group: 'commons-collections', name: 'commons-collections', version: '3.2'
    testCompile group: 'junit', name: 'junit', version: '4.+'
}
```



### 控制台编译成功输出

![](https://img-blog.csdn.net/20160727155726621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我得项目大多是maven依赖的，以后会逐步向gradle看齐。

项目例子：[http://download.csdn.net/detail/boonya/9587671](http://download.csdn.net/detail/boonya/9587671)

注：例子需要修改任务工作的编码类型。




