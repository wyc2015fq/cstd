# JAVA与groovy脚本的结合使用 - 零度的博客专栏 - CSDN博客
2017年03月29日 15:00:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：3829标签：[groovy																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=groovy&t=blog)
个人分类：[Groovy](https://blog.csdn.net/zmx729618/article/category/6356796)
java执行groovy shell脚本
### 1、添加groovy jar包支持
<dependency>
<groupId>org.codehaus.groovy</groupId>
<artifactId>groovy-all</artifactId>
<version>2.4.0-beta-1</version>
</dependency>
====================================================================
### 2、直接运行groovy的脚本
package groovy;
import groovy.lang.Binding;
import groovy.lang.GroovyShell;
public class Java2GroovyShell {
public static void main(String[] args) {
// TODO Auto-generated method stub
Binding binding = new Binding();
binding.setVariable("var", 5);
GroovyShell gs = new GroovyShell(binding);
Object value = gs.evaluate("println 'Hello Groovy !';abc=123;return var*10");//执行groovyshell脚本
System.out.println(value.equals(50));
System.out.println(binding.getVariable("abc").equals(123));
}
}
======================================================================
### 3、JAVA中动态运行groovy脚本
通过JAVA来加载如一个groovy脚本文件，然后调用该脚本中的方法
package groovy;
import groovy.lang.GroovyClassLoader;
import groovy.lang.GroovyObject;
import java.io.File;
public class Java2GroovyDynamic {
public static void main(String[] args) throws Exception {
// TODO Auto-generated method stub
ClassLoader parent = ClassLoader.getSystemClassLoader();
GroovyClassLoader loader = new GroovyClassLoader(parent);
Class groovyClass = loader.parseClass(new File("C:\\normandy_workspace\\groovy\\src\\groovy\\GroovyDemo.groovy"));
GroovyObject groovyObject = (GroovyObject)groovyClass.newInstance();
Object[] param = {123,321};
int res = (int) groovyObject.invokeMethod("add", param);
System.out.println("res="+res);
}
}
groovy脚本
package groovy
class GroovyDemo {
static int add(int x,int y) {
return x+y;
}
static main(args) {
int z = add(123,321);
println("x+y="+z);
}
}
======================================================================
### 4、Groovy脚本引擎
通过设定CLASSPATH来初始化groovy脚本引擎，可以运行该path下的任何groovy脚本文件了
package groovy;
import groovy.lang.Binding;
import groovy.util.GroovyScriptEngine;
public class Java2GroovyEngine {
public static void main(String[] args) throws Exception {
// TODO Auto-generated method stub
String path = "C:\\normandy_workspace\\groovy\\src\\groovy\\";
GroovyScriptEngine gse = new GroovyScriptEngine(path);
Binding binding = new Binding();
binding.setVariable("input", "Groovy");
gse.run("Test.groovy", binding);
System.out.println(binding.getVariable("output"));
}
}
groovy脚本
package groovy;
output = "Hello ${input}";
