# 静态代码扫描——FindBugs自定义规则入门 - oggboy的专栏 - CSDN博客





2016年07月06日 17:46:34[oggboy](https://me.csdn.net/oggboy)阅读数：6602








> 
阅读本文前，建议先了解一下[FindBugs的介绍和使用方法](http://baike.baidu.com/link?url=dqZYFKoRLAfdTV6SibbVzaIfTYL8lQqcXmNcHLxxgzO4wv-unDFtUPejxxo1OFyZfJVFJ3zsxoAQMQ4BROMwRq#2)。


## 准备工作

由于FindBugs是分析编译后的class文件，也就是字节码文件。我们需要了解FindBugs底层的处理机制。根据[FindBugs官网](http://findbugs.sourceforge.net)文档描述，FindBugs使用了[BCEL](http://commons.apache.org/proper/commons-bcel/project-summary.html)来分析Java字节码文件。从1.1版本开始，FindBugs也支持使用[ASM](http://asm.ow2.org/index.html)字节码框架来编写bug探测器。 

我们需要下载FindBugs源码版用来新增自定义探测器：findbugs-3.0.1-source.zip 

也需要下载FindBugs标准版，将findbugs.jar替换为我们的自定义版本后，运行查看结果。
## 自定义规则

自定义规则思路： 

1. 明确要定义的规则。 

2. 分析样例代码的字节码内容。 

3. 编写探测器。 

4. 将规则加入规则文件中。
### 1. 明确要定义的规则

我将以一个非常简单的规则举例：代码中避免使用有类似System.out的输出语句。

```java
package main;

public class TestFindBugs {
    public static void main(String[] args) {
        System.out.println("123"); //bug
        System.err.println("123"); //bug
    }
}
```

### 2. 分析样例代码的字节码内容

为了更方便的分析样例代码的字节码内容，这里推荐一个Eclipse上用来查看java文件字节码内容的插件： 

Bytecode Outline 

官网地址：[http://andrei.gmxhome.de/bytecode/index.html](http://andrei.gmxhome.de/bytecode/index.html)

在安装完成后，通过Bytecode工具编译后的字节码文件内容：
```java
// class version 51.0 (51)
// access flags 0x21
public class main/TestFindBugs {

  // compiled from: TestFindBugs.java

  // access flags 0x1
  public <init>()V
   L0
    LINENUMBER 3 L0
    ALOAD 0
    INVOKESPECIAL java/lang/Object.<init> ()V
    RETURN
   L1
    LOCALVARIABLE this Lmain/TestFindBugs; L0 L1 0
    MAXSTACK = 1
    MAXLOCALS = 1

  // access flags 0x9
  public static main([Ljava/lang/String;)V
   L0
    LINENUMBER 5 L0
    GETSTATIC java/lang/System.out : Ljava/io/PrintStream;
    LDC "123"
    INVOKEVIRTUAL java/io/PrintStream.println (Ljava/lang/String;)V
   L1
    LINENUMBER 6 L1
    GETSTATIC java/lang/System.err : Ljava/io/PrintStream;
    LDC "123"
    INVOKEVIRTUAL java/io/PrintStream.println (Ljava/lang/String;)V
   L2
    LINENUMBER 7 L2
    RETURN
   L3
    LOCALVARIABLE args [Ljava/lang/String; L0 L3 0
    MAXSTACK = 2
    MAXLOCALS = 1
}
```

通过查看字节码文件分析，我们找到了一些关键语句： 
`GETSTATIC java/lang/System.out : Ljava/io/PrintStream;`
`GETSTATIC java/lang/System.err : Ljava/io/PrintStream;`
### 3. 编写探测器

我们通过刚才找到的关键语句，结合我们的逻辑，进行探测器编写：

```java
package edu.umd.cs.findbugs.detect;

import org.apache.bcel.classfile.Code;

import edu.umd.cs.findbugs.BugInstance;
import edu.umd.cs.findbugs.BugReporter;
import edu.umd.cs.findbugs.bcel.OpcodeStackDetector;

/** * @author yuanwei3-iri * @category 代码中避免使用有类似System.out的输出语句 */
public class ForbiddenSystemClass extends OpcodeStackDetector {
    BugReporter bugReporter;

    public ForbiddenSystemClass(BugReporter bugReporter) {
        this.bugReporter = bugReporter;
    }

    /**     * visit方法，在每次进入字节码方法的时候调用 在每次进入新方法的时候清空标志位     */
    @Override
    public void visit(Code obj) {
        super.visit(obj);
    }

    /**     * 每扫描一条字节码就会进入sawOpcode方法     *      * @param seen     *            字节码的枚举值     */
    @Override
    public void sawOpcode(int seen) {
        if (seen == GETSTATIC) {
            if (getClassConstantOperand().equals("java/lang/System")) {
                if(getNameConstantOperand().equals("out") || getNameConstantOperand()
                        .equals("err")){
                    BugInstance bug = new BugInstance(this, "CJ_SYSTEMCLASS",
                            NORMAL_PRIORITY).addClassAndMethod(this).addSourceLine(
                                    this, getPC());
                    bugReporter.reportBug(bug);
                }
            }
        }
    }
}
```

### 4. 将规则加入规则文件中

我们刚才在编写探测器的时候，已经给定了规则的名称`CJ_SYSTEMCLASS`。现在我们需要将这个规则添加在配置文件中。 

配置findbugs.xml：

```xml
<FindbugsPlugin>  
  <Detector class="edu.umd.cs.findbugs.detect.ForbiddenSystemClass"  speed="fast" reports="CJ_SYSTEMCLASS" hidden="false" />  
  <BugPattern abbrev="CJ_SYSTEMCLASS" type="CJ_SYSTEMCLASS" category="PERFORMANCE" />  
</FindbugsPlugin>
```

配置message.xml：

```xml
<?xml version="1.0" encoding="UTF-8"?>  
<MessageCollection>  
  <Plugin>  
    <ShortDescription>Default FindBugs plugin</ShortDescription>  
    <Details>  
    <![CDATA[     <p>     This plugin contains all of the standard FindBugs detectors.     </p>     ]]>  
    </Details>  
  </Plugin>  
    <Detector class="edu.umd.cs.findbugs.detect.ForbiddenSystemClass">  
       <Details>  
        <![CDATA[         <p>代码不能出现System.out         <p>请使用log日志形式打印         ]]>  
       </Details>  
    </Detector>  
    <BugPattern type="CJ_SYSTEMCLASS">  
        <ShortDescription>代码不能出现System.out</ShortDescription>  
        <LongDescription>{1}代码不能出现System.out，请使用log形式输出</LongDescription>  
        <Details>  
      <![CDATA[         <p>不能使用System.out和System.err，请使用log</p>       ]]>  
        </Details>  
      </BugPattern>  
    <BugCode abbrev="CJ_SYSTEMCLASS">影响性能的输出System.out</BugCode>  
</MessageCollection>
```

规则添加完成后，重新打包findbugs.jar： 
`mvn clean install -Dmaven.test.skip=true`

![这里写图片描述](https://img-blog.csdn.net/20160708152104324)

打包成功后，在可运行版本的findbugs中替换原来的`/lib/findbugs.jar`

执行findbugs命令，扫描样例文件的class文件，查看运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20160708152413330)
![这里写图片描述](https://img-blog.csdn.net/20160708152625846)

在扫描结果中，可以看出确实扫描到了我们设定的问题语句。 

使用FindBugs自定义规则成功！

## 参考文献

> 
IBM.[FindBugs，第 1 部分: 提高代码质量](https://www.ibm.com/developerworks/cn/java/j-findbug1/)

  IBM.[FindBugs，第 2 部分: 编写自定义检测器](https://www.ibm.com/developerworks/cn/java/j-findbug2/)









