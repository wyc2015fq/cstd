# Java XML解析之Digester的使用 - z69183787的专栏 - CSDN博客
2015年07月19日 12:44:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2798
Java XML解析之Digester的使用
转自：[http://mopishv0.blog.163.com/blog/static/5445593220102209508914/?suggestedreading&wumii](http://mopishv0.blog.163.com/blog/static/5445593220102209508914/?suggestedreading&wumii)
Digester据说最早是为解析struts的配置文件开发的，后来慢慢的就发展成一个轻量，快速，单向（只能从xml到java对象）的一个绑定框架，在运行时实现绑定，无需定义xml文件的DTD或Schema，简单快速。
一、使用Digester的步骤
1． 创建一个Digester的实例。完成parse后，实例可以被安全的重用，但实例不是线程安全的。
2． 设置配置属性
3． 加入处理规则
4． 调用digester.parse()方法。
二、从一个例子说起
先看一个Digester文档上的简单例子
 对象树
package mypackage;
  public class Foo ...{
    public void addBar(Bar bar);
    public Bar findBar(int id);
    public Iterator getBars();
    public String getName();
    public void setName(String name);
  }
  package mypackage;
  public class Bar ...{
    public int getId();
    public void setId(int id);
    public String getTitle();
    public void setTitle(String title);
  }
XML文档片段
<foo name="The Parent">
    <bar id="123" title="The First Child"/>
    <bar id="456" title="The Second Child"/>
  </foo>
解析代码
 Digester digester = new Digester();
  digester.setValidating(false);
  digester.addObjectCreate("foo", "mypackage.Foo");
  digester.addSetProperties("foo");
  digester.addObjectCreate("foo/bar", "mypackage.Bar");
  digester.addSetProperties("foo/bar");
  digester.addSetNext("foo/bar", "addBar", "mypackage.Bar");
  Foo foo = (Foo) digester.parse();
    从例子可以看到Digester的执行步骤是扫描xml文档，匹配特定的xml节点，在每一个节点按照预定的处理规则操纵对象，包括创建对象，设置对象属性，建立对象树等。
三、节点匹配规则
<a>         -- Matches pattern "a"    <b>       -- Matches pattern "a/b"      <c/>    -- Matches pattern "a/b/c"      <c/>    -- Matches pattern "a/b/c"    </b>    <b>       -- Matches pattern "a/b"      <c/>    -- Matches pattern "a/b/c"      <c/>    -- Matches
 pattern "a/b/c"      <c/>    -- Matches pattern "a/b/c"    </b> </a>    可以看到匹配规则非常简单，此外，匹配还支持通配符，像*/a就匹配任意层次内的<a>。
    一个特定的XML元素可以被匹配多次，所对应的处理规则执行的顺序如下：如果是begin和body方法，按照规则被加入到Digester的顺序执行，如果是end方法，则相反（参见后面处理规则）。
    对于通配符匹配，当一个确定的匹配发生时（如果多个通配符匹配发生，最长的一个被认为是一个确定的匹配），通配符匹配将被忽略。这个意思就是说，当规则A被加入到*/a时，意思是这个规则匹配任意一个<a>元素，但是如果规则C被加入到x/a,则A对于<x><a>的匹配会被或略。假设规则A和B匹配*/a,如果想给x/a加入规则C，则需要给x/a重复加入规则A和规则B，就像下面这样：
Rule ruleA = new ObjectCreateRule();  Rule ruleB = new SetNextRule();  Rule ruleC = new SetPropertiesRule();  digester.addRule("*/a", ruleA);  digester.addRule("*/a", ruleB);digester.addRule("x/a", ruleA);  digester.addRule("x/a", ruleB);  digester.addRule("x/a",
 ruleC);  
四、处理规则
    处理规则的目的就是定义当匹配发生时做什么事情。每一个处理规则都是org.apache.commons.digester.Rule接口的实现类。当匹配发生时Rule接口的回调方法被调用，执行相应的操作。
Rule接口的方法：
begin       当进入匹配元素时调用
body        当进入匹配元素的内容时调用
end         当离开匹配元素时调用
finish      当paser结束时调用
常见的内置处理规则
ObjectCreateRule：进入匹配元素时调用，创建一个特定的java对象。
SetPropertiesRule：进入匹配元素时调用，利用Java的反射API设置栈顶对象的属性，要求Java对象的属性名匹配XML元素的属性名。
SetNextRule：离开匹配元素时调用，digester分析下一个栈顶对象，调用该对象的指定属性的setter方法，将当前栈顶对象作为参数传入到该方法，同时弹出当前栈顶对象。
SetTopRule：和SetNextRule含义相同，分析栈顶对象，调用该对象指定属性的setter方法，将下一个栈顶对象作为参数传给该方法（典型会被调用的方法是setParent）。 
CallMethodRule：调用栈顶对象指定的方法。设置这个规则需要指出调用的方法名，参数个数，参数Java类型（可选），参数值一般是匹配元素的body content，用CallParamRule指定。
CallParamRule ：标识CallMethodRule中的方法参数。参数值可能是匹配元素的某个属性，或匹配元素的body content。
Digester的处理过程
    Digester使用堆栈来存储对象，当ObjectCreateRule被执行时，将对象压入栈，像SetPropertiesRule，SetTopRule，CallMethodRule等规则都是对栈顶对象的操作。当SetNextRule被执行时，弹出栈顶对象，下一个栈顶对象成为栈顶对象，最后返回对象树的根节点。
Digester详细描述参见[http://commons.apache.org/digester/commons-digester-1.8/docs/api/org/apache/commons/digester/package-summary.html#doc.Description](http://commons.apache.org/digester/commons-digester-1.8/docs/api/org/apache/commons/digester/package-summary.html#doc.Description)
