# Java 中使用动态代码 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [brucefeng](http://www.jobbole.com/members/brucefengofnju) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
O2O互联网的运营开发最大的特点就是每次运营活动规则千奇百怪，需要有许多个性化的配置，如何例A活动需要针对新用户做发红包的活动，B活动针对全部用户做发红包活动，而在B活动中针对新用户发x面额的红包，而针对老用户发y面值的红包。两个活动规则差别较大，如果每次都个性化开发，会非常浪费时间，因此如何支持规则的动态配置是个很大的挑战。
Java不是解决动态层问题的理想语言，这些动态层问题包括原型设计、脚本处理等。
公司的项目主要基于Java平台，在实践中发现主要有两种方式可以实现：
- 统一表达式语言
- 动态语言，如Groovy
### JUEL(Java 统一表达式语言)
Java**统一表达式语言**（英语：Unified Expression Language，简称**JUEL**）是一种特殊用途的编程语言，主要在[Java](https://zh.wikipedia.org/wiki/Java)[Web应用程序](https://zh.wikipedia.org/wiki/Web%E5%BA%94%E7%94%A8%E7%A8%8B%E5%BA%8F)用于将表达式嵌入到web页面。[Java](https://zh.wikipedia.org/wiki/Java)规范制定者和Java Web领域技术专家小组制定了统一的表达式语言。JUEL最初包含在[JSP](https://zh.wikipedia.org/wiki/JSP) 2.1规范JSR-245中，后来成为Java EE 7的一部分，改在JSR-341中定义。
主要的开源实现有：[OGNL](https://commons.apache.org/proper/commons-ognl/) ，[MVEL](http://java-source.net/open-source/expression-languages/mvel) ，[SpEL](http://docs.spring.io/spring/docs/current/spring-framework-reference/html/expressions.html) ,[JUEL](http://java-source.net/open-source/expression-languages/juel) ,[Java Expression Language (JEXL)](http://java-source.net/open-source/expression-languages/java-expression-language) ,[JEval](http://java-source.net/open-source/expression-languages/jeval) ,[Jakarta JXPath](http://java-source.net/open-source/expression-languages/jakarta-jxpath) 等。这里主要介绍在实践中使用较多的MVEL、OGNL和SpEL。
#### OGNL(Object Graph Navigation Library)
在Struts 2 的标签库中都是使用OGNL表达式访问ApplicationContext中的对象数据，OGNL主要有三个重要因素：
- Expression
Expression是整个OGNL的核心内容，所有的OGNL操作都是针对表达式解析后进行的。通过Expression来告知OGNL操作到底要干些什么。因此，Expression其实是一个带有语法含义的字符串，整个字符串将规定操作的类型和内容。OGNL表达式支持大量Expression，如“链式访问对象”、表达式计算、甚至还支持Lambda表达式。
- Root对象：
OGNL的Root对象可以理解为OGNL的操作对象。当我们指定了一个表达式的时候，我们需要指定这个表达式针对的是哪个具体的对象。而这个具体的对象就是Root对象，这就意味着，如果有一个OGNL表达式，那么我们需要针对Root对象来进行OGNL表达式的计算并且返回结果。
- ApplicationContext
有个Root对象和Expression，我们就可以使用OGNL进行简单的操作了，如对Root对象的赋值与取值操作。但是，实际上在OGNL的内部，所有的操作都会在一个特定的数据环境中运行。这个数据环境就是ApplicationContext（上下文环境）。OGNL的上下文环境是一个Map结构，称之为OgnlContext。Root对象也会被添加到上下文环境当中去。

```
Foo foo = new Foo();
foo.setName("test");
Map<String, Object> context = new HashMap<String, Object>();
context.put("foo",foo);
String expression = "foo.name == 'test'";
try {
    Boolean result = (Boolean) Ognl.getValue(expression,context);
    System.out.println(result);
} catch (OgnlException e) {
    e.printStackTrace();
}
```
这段代码就是判断对象foo的name属性是否为test。
OGNL的具体语法参见[OGNL language guide](https://commons.apache.org/proper/commons-ognl/language-guide.html) 。
#### [MVEL](https://github.com/mvel/mvel)
MVEL最初作为Mike Brock创建的 Valhalla项目的表达式计算器（expression evaluator）。Valhalla本身是一个早期的类似 Seam 的“开箱即用”的Web 应用框架，而 Valhalla 项目现在处于休眠状态， MVEL则成为一个继续积极发展的项目。相比最初的OGNL、JEXL和JUEL等项目，而它具有远超它们的性能、功能和易用性 – 特别是集成方面。它不会尝试另一种JVM语言，而是着重解决嵌入式脚本的问题。
MVEL特别适用于受限环境 – 诸如由于内存或沙箱（sand-boxing）问题不能使用字节码生成。它不是试图重新发明Java，而是旨在提供一种Java程序员熟悉的语法，同时还加入了简短的表达式语法。
MVEL主要使用在Drools，是[Drools规则引擎](http://www.jboss.org/drools/)不可分割的一部分。
MVEL语法较为丰富，不仅包含了基本的属性表达式，布尔表达式，变量复制和方法调用，还支持函数定义，详情参见[MVEL Language Guide](https://en.wikisource.org/wiki/MVEL_Language_Guide) 。
MVEL在执行语言时主要有解释模式（**Interpreted Mode**）和编译模式（**Compiled Mode** ）两种：
- 解释模式（Interpreted Mode）是一个无状态的，动态解释执行，不需要负载表达式就可以执行相应的脚本。
- 编译模式（Compiled Mode）需要在缓存中产生一个完全规范化表达式之后再执行。
解释模式

```
//解释模式
Foo foo = new Foo();
foo.setName("test");
Map context = new HashMap();
String expression = "foo.name == 'test'";
VariableResolverFactory functionFactory = new MapVariableResolverFactory(context);
context.put("foo",foo);
Boolean result = (Boolean) MVEL.eval(expression,functionFactory);
System.out.println(result);
```
编译模式

```
//编译模式
Foo foo = new Foo();foo.setName("test");Map context = new HashMap();String expression = "foo.name == 'test'";VariableResolverFactory functionFactory = new MapVariableResolverFactory(context);context.put("foo",foo);
Serializable compileExpression = MVEL.compileExpression(expression);
Boolean result = (Boolean) MVEL.executeExpression(compileExpression, context, functionFactory);
```
#### [SpEL](http://docs.spring.io/spring/docs/current/spring-framework-reference/html/expressions.html)
SpEl(Spring表达式语言）是一个支持查询和操作运行时对象导航图功能的强大的表达式语言。 它的语法类似于传统EL，但提供额外的功能，最出色的就是函数调用和简单字符串的模板函数。SpEL类似于Struts2x中使用的OGNL表达式语言，能在运行时构建复杂表达式、存取对象图属性、对象方法调用等等，并且能与Spring功能完美整合，如能用来配置Bean定义。
SpEL主要提供基本表达式、类相关表达式及集合相关表达式等，详细参见[Spring 表达式语言 (SpEL)](http://spring.cndocs.tk/expressions.html) 。
类似与OGNL，SpEL具有expression(表达式)，Parser（解析器），EvaluationContext(上下文）等基本概念；类似与MVEL，SpEl也提供了解释模式和编译模式两种运行模式。

```
//解释器模式
Foo foo = new Foo();
foo.setName("test");
// Turn on:
// - auto null reference initialization
// - auto collection growing
SpelParserConfiguration config = new SpelParserConfiguration(true,true);
ExpressionParser parser = new SpelExpressionParser(config);
String expressionStr = "#foo.name == 'test'";
StandardEvaluationContext context = new StandardEvaluationContext();
context.setVariable("foo",foo);
Expression expression = parser.parseExpression(expressionStr);
Boolean result = expression.getValue(context,Boolean.class);
//编译模式
config = new SpelParserConfiguration(SpelCompilerMode.IMMEDIATE, RunSpel.class.getClassLoader());
parser = new SpelExpressionParser(config);
context = new StandardEvaluationContext();
context.setVariable("foo",foo);
expression = parser.parseExpression(expressionStr);
result = expression.getValue(context,Boolean.class);
```
### [Groovy](http://www.groovy-lang.org/)
Groovy除了[Gradle](http://gradle.org/) 上的广泛应用之外，另一个大范围的使用应该就是结合Java使用动态代码了。Groovy的语法与Java非常相似，以至于多数的Java代码也是正确的Groovy代码。Groovy代码动态的被[编译器](https://zh.wikipedia.org/wiki/%E7%BC%96%E8%AF%91%E5%99%A8)转换成Java字节码。由于其运行在JVM上的特性，Groovy可以使用其他Java语言编写的库。
Groovy可以看作给Java静态世界补充动态能力的语言，同时Groovy已经实现了java不具备的语言特性：
- 函数字面值；
- 对集合的一等支持；
- 对正则表达式的一等支持；
- 对xml的一等支持；
Groovy作为基于JVM的语言，与表达式语言存在语言级的不同，因此在语法上比表达还是语言更灵活。Java在调用Groovy时，都需要将Groovy代码编译成Class文件。
Groovy 可以采用GroovyClassLoader、GroovyShell、GroovyScriptEngine和[JSR223](http://docs.oracle.com/javase/6/docs/technotes/guides/scripting/) 等方式与Java语言集成。
#### GroovyClassLoader
GroovyClassLoader是一个定制的类装载器，负责解释加载Java类中用到的Groovy类，也可以编译，Java代码可通过其动态加载Groovy脚本并执行。

```
class FooCompare{
    boolean compare(String toCompare){
        Foo foo = new Foo();
        foo.name = "test";
        return foo.name == toCompare;
    }
}
```


```
GroovyClassLoader loader = new GroovyClassLoader();
Class groovyClass = null;
try {
    String path = "FooCompare.groovy";
    groovyClass = loader.parseClass(new File(path));
} catch (IOException e) {
    e.printStackTrace();
}
GroovyObject groovyObject = null;
try {
    groovyObject = (GroovyObject) groovyClass.newInstance();
} catch (InstantiationException e) {
    e.printStackTrace();
} catch (IllegalAccessException e) {
    e.printStackTrace();
}
result = groovyObject.invokeMethod("compare", "test");
assert result.equals(Boolean.TRUE);
System.out.print(result);
```
#### GroovyShell
GroovyShell允许在Java类中（甚至Groovy类）求任意Groovy表达式的值。可以使用Binding对象输入参数给表达式，并最终通过GroovyShell返回Groovy表达式的计算结果。

```
Foo foo = new Foo();
foo.setName("test");
Binding binding = new Binding();
binding.setVariable("foo",foo);
GroovyShell shell = new GroovyShell(binding);
String expression = "foo.name=='test'";
Object result = shell.evaluate(expression);
assert result.equals(Boolean.TRUE);
```
#### GroovyScriptEngine
GroovyShell多用于推求对立的脚本或表达式，如果换成相互关联的多个脚本，使用GroovyScriptEngine会更好些。GroovyScriptEngine从您指定的位置（文件系统，URL，数据库，等等）加载Groovy脚本，并且随着脚本变化而重新加载它们。如同GroovyShell一样，GroovyScriptEngine也允许您传入参数值，并能返回脚本的值。
FooScript.groovy

```
package blog.brucefeng.info.groovy
foo.name=="test";
```


```
Foo foo = new Foo();
foo.setName("test");
Binding binding = new Binding();
binding.setVariable("foo",foo);
String[] paths = {"/demopath/"}
GroovyScriptEngine gse = new GroovyScriptEngine(paths);
try {
    result = gse.run("FooScript.groovy", binding);
} catch (ResourceException e) {
    e.printStackTrace();
} catch (ScriptException e) {
    e.printStackTrace();
}
assert result.equals(Boolean.TRUE);
```
#### JSR223
JSR223 是Java 6提供的一种从Java内部执行脚本编写语言的方便、标准的方式，并提供从脚本内部访问Java 资源和类的功能，可以使用其运行多种脚本语言如JavaScript和Groovy等。

```
Foo foo = new Foo();
foo.setName("test");
ScriptEngineManager factory = new ScriptEngineManager();
ScriptEngine engine1 = factory.getEngineByName("groovy");
engine1.put("foo",foo);
try {
   result =  engine1.eval(expression);
} catch (javax.script.ScriptException e) {
    e.printStackTrace();
}
assert result.equals(Boolean.TRUE);
```
#### 使用中经常出现的问题
因此Java每次调用Groovy代码都会将Groovy编译成Class文件，因此在调用过程中会出现JVM级别的问题。如使用GroovyShell的parse方法导致perm区爆满的问题，使用GroovyClassLoader加载机制导致频繁gc问题和CodeCache用满，导致JIT禁用问题等，相关问题可以参考[Groovy与Java集成常见的坑](https://yq.aliyun.com/articles/2357) 。
### 性能对比
在这里简单对上面介绍到的OGNL、MVEL、SpEL和Groovy2.4 的性能进行大致的性能测试（简单测试）：
|实现方式|耗时（ms)|
|----|----|
|Java|13|
|OGNL|2958|
|MVEL|225|
|SpEL|1023|
|Groovy|99|
通过这个简单测试发现，Groovy 2.4的性能已经足够的好，而MVEL的性能依然保持强劲，不过已经远远落后与Groovy，在对性能有一定要求的场景下已经不建议使用OGNL和SpEL。
**不过动态代码的执行效率还是远低于Java，因此在高性能的场景下慎用。**
以下是测试代码：

```
package blog.brucefeng.info.performance
class GroovyCal{
    Integer cal(int x,int y,int z){
        return x + y*2 - z;
    }
}
```


```
package blog.brucefeng.info.performance;
public class RunPerform {
    public static void main(String[] args) {
        try {
            int xmax = 100,ymax = 100,zmax= 10;
            runJava(xmax, ymax, zmax);
            runOgnl(xmax, ymax, zmax);
            runMvel(xmax, ymax, zmax);
            runSpel(xmax, ymax, zmax);
            runGroovyClass(xmax, ymax, zmax);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public static void runJava(int xmax, int ymax, int zmax) {
        Date start = new Date();
        Integer result = 0;
        for (int xval = 0; xval < xmax; xval++) {
            for (int yval = 0; yval < ymax; yval++) {
                for (int zval = 0; zval <= zmax; zval++) {
                    result += xval + yval * 2 - zval;
                }
            }
        }
        Date end = new Date();
        System.out.println("time is : " + (end.getTime() - start.getTime()) + ",result is " + result);
    }
    public static void runOgnl(int xmax, int ymax, int zmax) throws OgnlException {
        String expression = "x + y*2 - z";
        Map<String, Object> context = new HashMap<String, Object>();
        Integer result = 0;
        Date start = new Date();
        for (int xval = 0; xval < xmax; xval++) {
            for (int yval = 0; yval < ymax; yval++) {
                for (int zval = 0; zval <= zmax; zval++) {
                    context.put("x", xval);
                    context.put("y", yval);
                    context.put("z", zval);
                    Integer cal = (Integer) Ognl.getValue(expression, context);
                    result += cal;
                }
            }
        }
        Date end = new Date();
        System.out.println("Ognl:time is : " + (end.getTime() - start.getTime()) + ",result is " + result);
    }
    public static void runMvel(int xmax, int ymax, int zmax) {
        Map context = new HashMap();
        String expression = "x + y*2 - z";
        Serializable compileExpression = MVEL.compileExpression(expression);
        Integer result = 0;
        Date start = new Date();
        for (int xval = 0; xval < xmax; xval++) {
            for (int yval = 0; yval < ymax; yval++) {
                for (int zval = 0; zval <= zmax; zval++) {
                    context.put("x", xval);
                    context.put("y", yval);
                    context.put("z", zval);
                    VariableResolverFactory functionFactory = new MapVariableResolverFactory(context);
                    Integer cal = (Integer) MVEL.executeExpression(compileExpression, context, functionFactory);
                    result += cal;
                }
            }
        }
        Date end = new Date();
        System.out.println("MVEL:time is : " + (end.getTime() - start.getTime()) + ",result is " + result);
    }
    public static void runSpel(int xmax, int ymax, int zmax) {
        SpelParserConfiguration config;
        ExpressionParser parser;
        config = new SpelParserConfiguration(SpelCompilerMode.IMMEDIATE, RunSpel.class.getClassLoader());
        parser = new SpelExpressionParser(config);
        StandardEvaluationContext context = new StandardEvaluationContext();
        Integer result = 0;
        String expressionStr = "#x + #y*2 - #z";
        Date start = new Date();
        for (Integer xval = 0; xval < xmax; xval++) {
            for (Integer yval = 0; yval < ymax; yval++) {
                for (Integer zval = 0; zval <= zmax; zval++) {
                    context.setVariable("x", xval);
                    context.setVariable("y", yval);
                    context.setVariable("z", zval);
                    Expression expression = parser.parseExpression(expressionStr);
                    Integer cal = expression.getValue(context, Integer.class);
                    result += cal;
                }
            }
        }
        Date end = new Date();
        System.out.println("SpEL:time is : " + (end.getTime() - start.getTime()) + ",result is " + result);
    }
    public static void runGroovyClass(int xmax, int ymax, int zmax) {
        GroovyClassLoader loader = new GroovyClassLoader();
        Class groovyClass = null;
        try {
            groovyClass = loader.parseClass(new File(
                    "GroovyCal.groovy"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        GroovyObject groovyObject = null;
        try {
            groovyObject = (GroovyObject) groovyClass.newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        Integer result = 0;
        Date start = new Date();
        for (int xval = 0; xval < xmax; xval++) {
            for (int yval = 0; yval < ymax; yval++) {
                for (int zval = 0; zval <= zmax; zval++) {
                    Object[] args = {xval,yval,zval};
                    Integer cal = (Integer) groovyObject.invokeMethod("cal", args);
                    result += cal;
                }
            }
        }
        Date end = new Date();
        System.out.println("Groovy Class:time is : " + (end.getTime() - start.getTime()) + ",result is " + result);
    }
}
```
本文的代码可以参见[eldemo github](https://github.com/brucefengnju/eldemo) 。
**References**
- [Groovy vs Java Performance](https://news.ycombinator.com/item?id=4558272)
- [Groovy与Java集成常见的坑](https://yq.aliyun.com/articles/2357)
- [OGNL language guide](https://commons.apache.org/proper/commons-ognl/language-guide.html)
- [Groovy引发的PermGen区爆满问题定位与解决](http://xiongzheng.me/question/2015/01/02/groovy-permgen-out/?spm=5176.100239.blogcont.8.sMPc7M)
- [groovy脚本导致的FullGC问题](http://www.bubuko.com/infodetail-670047.html?spm=5176.100239.blogcont.9.sMPc7M)
- [Groovy性能问题](http://ju.outofmemory.cn/entry/129930?spm=5176.100239.blogcont.10.sMPc7M)
- [Groovy的classloader加载机制唤起的频繁GC](http://www.zgxue.com/120/1204001.html?spm=5176.100239.blogcont.11.sMPc7M)
- [Groovy深入探索——Groovy的ClassLoader体系](http://johnnyjian.iteye.com/blog/1847151?spm=5176.100239.blogcont.12.sMPc7M)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/b947d0acc8cb4e3b0649a27c60646636-1.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/04/f22f5454bd93395413ea7e7507db150a-1.jpg)
