# Java和groovy相互调用 - z69183787的专栏 - CSDN博客
2017年03月28日 18:21:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3419

java interface
Java代码  ![收藏代码](http://daimojingdeyu.iteye.com/images/icon_star.png)
- package groovy;  
- 
- publicinterface IFoo {  
- public Object run(Object foo, Object bar);  
- }  
groovy implements
Java代码  ![收藏代码](http://daimojingdeyu.iteye.com/images/icon_star.png)
- package groovy;  
- class Foo implements IFoo {  
-     def x  
- public Object run(Object foo, Object bar) {  
-         println 'Hello World!'
-         x = 123
-         println foo * 10
-         println bar  
- return'success'
-     }  
- }   
java call groovy
Java代码  ![收藏代码](http://daimojingdeyu.iteye.com/images/icon_star.png)
- package groovy;  
- 
- import groovy.lang.GroovyClassLoader;  
- import groovy.lang.GroovyObject;  
- 
- import java.io.File;  
- 
- publicclass InvokeGroovy {  
- publicstaticvoid main(String[] args) {  
-         ClassLoader cl = new InvokeGroovy().getClass().getClassLoader();  
-         GroovyClassLoader groovyCl = new GroovyClassLoader(cl);  
- try {  
- @SuppressWarnings("rawtypes")  
-             Class groovyClass = groovyCl.parseClass(new File(InvokeGroovy.class.getResource("/groovy/Foo.groovy").toURI()));  
-             IFoo foo = (IFoo) groovyClass.newInstance();  
-             System.out.println(foo.run(new Integer(2), "More parameter..."));  
- 
-             System.out.println("=============================");  
- 
-             GroovyObject groovyObject = (GroovyObject) groovyClass.newInstance();  
-             System.out.println(groovyObject.invokeMethod("run", new Object[]{new Integer(2),"More parameter..."}));   
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
java execute groovy script
Java代码  ![收藏代码](http://daimojingdeyu.iteye.com/images/icon_star.png)
- package groovy;  
- 
- import groovy.lang.Binding;  
- import groovy.lang.GroovyShell;  
- 
- import javax.script.ScriptEngine;  
- import javax.script.ScriptEngineManager;  
- import javax.script.ScriptException;  
- 
- publicclass TestEval {  
- 
- public Object doit() {  
-         ScriptEngineManager factory = new ScriptEngineManager(TestEval.class.getClassLoader());  
-         ScriptEngine scriptEngine =  factory.getEngineByName("groovy");//或者"Groovy" groovy版本要1.6以上的
- try {  
-             scriptEngine.put("test", "hello world!");  
-             scriptEngine.put("outer", this);  
-             scriptEngine.eval("println test; outer.java_out()");  
-         } catch (ScriptException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
- 
- 
-         Binding bb = new Binding();  
-         bb.setVariable("test", "hello world!");  
-         bb.setProperty("outer", this);  
-         GroovyShell gs = new GroovyShell(bb);  
- 
- 
- return gs.evaluate("println test; outer.java_out()");  
-     }  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         TestEval te = new TestEval();  
-         te.doit();  
- 
-     }  
- 
- publicvoid java_out(){  
-         System.out.println("out from java");  
-     }  
- 
- }  
- 
