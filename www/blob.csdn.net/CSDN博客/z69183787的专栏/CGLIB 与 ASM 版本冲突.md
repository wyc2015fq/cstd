# CGLIB 与 ASM 版本冲突 - z69183787的专栏 - CSDN博客
2017年09月21日 12:47:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2515
CGLIB 版本3.1 自带 ASM 4.2 版本
与引入的 ASM 3.3.1 冲突，
故依赖CGLIB 自身依赖的ASM 即可
```
<!-- asm cglib -->
        <!--<dependency>-->
            <!--<groupId>asm</groupId>-->
            <!--<artifactId>asm</artifactId>-->
            <!--<version>${asm.version}</version>-->
        <!--</dependency>-->
        <dependency>
            <groupId>cglib</groupId>
            <artifactId>cglib</artifactId>
            <version>${cglib.version}</version>
        </dependency>
        <!-- asm cglib -->
```
cglib3.1- pom
```
<dependencies>
    <dependency>
      <groupId>org.ow2.asm</groupId>
      <artifactId>asm</artifactId>
      <version>4.2</version>
    </dependency>
    <dependency>
      <groupId>org.ow2.asm</groupId>
      <artifactId>asm-util</artifactId>
      <version>4.2</version>
      <optional>true</optional>
    </dependency>
```
网上的解决方案：
cglib版本为3.0或3.1，org.objectweb.asm版本为3.1.0时
版本冲突，报错java.lang.IncompatibleClassChangeError: class net.sf.cglib.core.DebuggingClassWriter has interface org.objectweb.asm.ClassVisitor as super class
使用cglib 2.2 可解决此问题,该版本中的DebuggingClassWriter的父类为ClassWriter
按照此方案依旧报错。
lib文件不匹配，我们下载最新的ASM文件，则解决了以上问题。
asm-5.2.jar
http://forge.ow2.org/projects/asm/
![](https://img-blog.csdn.net/20170325184413112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveF9peWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
