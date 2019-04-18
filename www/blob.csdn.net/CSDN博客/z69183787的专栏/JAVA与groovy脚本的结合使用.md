# JAVA与groovy脚本的结合使用 - z69183787的专栏 - CSDN博客
2017年03月28日 17:35:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3331
java执行groovy shell脚本
<dependency>
<groupId>org.codehaus.groovy</groupId>
<artifactId>groovy-all</artifactId>
<version>2.4.0-beta-1</version>
</dependency>
添加groovy jar包支持
====================================================
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
====================================================直接运行groovy的脚本
++++++++++++++++++++++++++++++++++++++++++++++++++++JAVA中动态运行groovy脚本
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
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Groovy脚本引擎
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
