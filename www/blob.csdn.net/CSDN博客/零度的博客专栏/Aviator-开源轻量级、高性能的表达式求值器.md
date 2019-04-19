# Aviator-开源轻量级、高性能的表达式求值器 - 零度的博客专栏 - CSDN博客
2016年11月25日 10:18:37[零度anngle](https://me.csdn.net/zmx729618)阅读数：1995
**一、轮子的必要性**
    表达式的求值上，java的选择非常多，强大的如Groovy、JRuby，N年没维护的beanshell，包括javaeye上朋友的IKExpression。为什么还需要Aviator？或者说Aviator的特点是什么？
    我将Aviator定位在Groovy这样全功能的脚本和IKExpression这样的简易的表达式求值之间的东西，如果你不希望带上Groovy那么庞大的jar却只用上一点点的功能，如果你希望功能和性能上比IKExpression好那么一些，那么也许你可以考虑Aviator。
    Aviator的设计思路跟利用GroovyObject的求值是一样，通过编译并动态生成字节码的方式将表达式编译成一个类，然后反射执行这个类，因此会在效率上比纯解释执行的IKExpression好一些。
![aviator结构图](http://www.blogjava.net/images/blogjava_net/killme2008/aviator.jpg)
**二、让轮子转起来。**
***求算术表达式***：
Java代码 
- import com.googlecode.aviator.AviatorEvaluator;  
- 
- 
- publicclass SimpleExample {  
- publicstaticvoid main(String[] args) {  
-         Long result = (Long) AviatorEvaluator.execute("1+2+3");  
-         System.out.println(result);  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
执行入口统一为AviatorEvaluator类，它有一系列静态方法。
***逻辑表达式和关系运算***：
Java代码 
- AviatorEvaluator.execute("3>1 && 2!=4 || true");  
![](http://static.blog.csdn.net/images/save_snippets.png)
Aviator支持所有的关系运算符和算术运算符，不支持位运算，同时支持表达式的优先级，优先级跟Java的运算符一样，并且支持通过括号来强制优先级。
**使用变量和字符串相加**：
Java代码 
- String yourname = “aviator”;  
-  Map<String, Object> env = new HashMap<String, Object>();  
-  env.put("yourname", yourname);  
-  String result = (String) AviatorEvaluator.execute(" 'hello ' + yourname ", env);  
-  System.out.println(result);  
![](http://static.blog.csdn.net/images/save_snippets.png)
 打印：
Java代码 
- hello aviator  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
字符串可以单引号也可以双引号括起来，并且支持转义字符。变量名称只要是合法的java identifer即可，变量需要用户传入，通过Map<String,Object>指定变量名和值是什么，这里的变量是yourname。
***变量的访问支持嵌套访问***，也就是dot操作符来访问变量里的属性，假设我们有一个Foo类：
Java代码 
- publicstaticclass Foo {  
- int i;  
- float f;  
-        Date date = new Date();  
- 
- public Foo(int i, float f, Date date) {  
- super();  
- this.i = i;  
- this.f = f;  
- this.date = date;  
-        }  
- 
- publicint getI() {  
- return i;  
-        }  
- 
- publicvoid setI(int i) {  
- this.i = i;  
-        }  
- 
- publicfloat getF() {  
- return f;  
-        }  
- 
- publicvoid setF(float f) {  
- this.f = f;  
-        }  
- 
- public Date getDate() {  
- return date;  
-        }  
- 
- publicvoid setDate(Date date) {  
- this.date = date;  
-        }  
- 
-    }  
![](http://static.blog.csdn.net/images/save_snippets.png)
然后在使用一个表达式来描述Foo里的各种属性：
Java代码 
- Foo foo = new Foo(100, 3.14f, new Date());  
- Map<String, Object> env = new HashMap<String, Object>();  
- env.put("foo", foo);  
- 
- String result =  
-         (String) AviatorEvaluator.execute(  
- " '[foo i='+ foo.i + ' f='+foo.f+' year='+(foo.date.year+1900)+ ' month='+foo.date.month +']' ",  
-             env);  
![](http://static.blog.csdn.net/images/save_snippets.png)
 我们可以通过foo.date.year的方式来访问变量foo中date属性的year值，这是利用commons-beanutils的反射功能实现的，前提是你的变量是合法的JavaBean(public、getter缺一不可)。
***三元表达式***：
Java代码 
- AviatorEvaluator.execute("3>0? 'yes':'no'");  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面都还是一个求值器表达式的常见功能，下面要描述的是Aviator的一些偏脚本性的功能。
**类Ruby、Perl的正则匹配**，匹配email地址：
Java代码 
- AviatorEvaluator.execute("'killme2008'=~/([\\w0-8]+@\\w+[\\.\\w+]+)/ ");  
![](http://static.blog.csdn.net/images/save_snippets.png)
 成功的话返回true，否则返回false。//括起来的字符序列成为正则表达式，=~操作符用于匹配。匹配只能在String和Pattern之间。
**匹配成功，获得匹配的分组，利用变量$digit**：
Java代码 
- AviatorEvaluator.execute("'killme2008@gmail.com'=~/([\\w0-8]+@\\w+[\\.\\w+]+)/ ? $1:'unknow'");  
![](http://static.blog.csdn.net/images/save_snippets.png)
 匹配成功返回$1，表示第一个匹配的分组，也就是用户名 killme2008
**函数调用**：
Java代码 
- AviatorEvaluator.execute("sysdate()");  
![](http://static.blog.csdn.net/images/save_snippets.png)
 sysdate()是一个内置函数，返回当前日期，跟new java.util.Date()效果相同。
更多内置函数:
Java代码 
- AviatorEvaluator.execute("string.length('hello')");    // 求字符串长度
- AviatorEvaluator.execute("string.contains('hello','h')");  //判断字符串是否包含字符串
- AviatorEvaluator.execute("string.startsWith('hello','h')");  //是否以子串开头
- AviatorEvaluator.execute("string.endsWith('hello','llo')");  是否以子串结尾  
- 
- AviatorEvaluator.execute("math.pow(-3,2)");   // 求n次方
- AviatorEvaluator.execute("math.sqrt(14.0)");   //开平方根
- AviatorEvaluator.execute("math.sin(20)");    //正弦函数
![](http://static.blog.csdn.net/images/save_snippets.png)
可以看到Aviator的函数调用风格非常类似lua或者c。
***自定义函数***，实现AviatorFunction接口并注册即可，比如我们实现一个add函数用于相加：
Java代码 
- import com.googlecode.aviator.AviatorEvaluator;  
- import com.googlecode.aviator.runtime.function.FunctionUtils;  
- import com.googlecode.aviator.runtime.type.AviatorDouble;  
- import com.googlecode.aviator.runtime.type.AviatorFunction;  
- import com.googlecode.aviator.runtime.type.AviatorObject;  
- class AddFunction implements AviatorFunction {  
- 
- public AviatorObject call(Map<String, Object> env, AviatorObject... args) {  
- if (args.length != 2) {  
- thrownew IllegalArgumentException("Add only supports two arguments");  
-             }  
-             Number left = FunctionUtils.getNumberValue(0, args, env);  
-             Number right = FunctionUtils.getNumberValue(1, args, env);  
- returnnew AviatorDouble(left.doubleValue() + right.doubleValue());  
-         }  
- 
- 
- public String getName() {  
- return"add";  
-         }  
- 
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
注册并调用：
Java代码 
- AviatorEvaluator.addFunction(new AddFunction());  
- System.out.println(AviatorEvaluator.execute("add(1,2)"));  
- System.out.println(AviatorEvaluator.execute("add(add(1,2),100)"));  
![](http://static.blog.csdn.net/images/save_snippets.png)
函数可以嵌套调用。
**三、不公平的性能测试**
  基本介绍完了，最后给些测试的数据，下列的测试场景都是每个表达式预先编译，然后执行1000万次，测量执行耗时。
场景1：
算术表达式
1000+100.0*99-(600-3*15)/(((68-9)-3)*2-100)+10000%7*71
![](http://static.blog.csdn.net/images/save_snippets_01.png)
结果：
|测试|耗时（单位:秒）|
|----|----|
|Aviator|14.0|
|Groovy|79.6|
|IKExpression|159.2|
场景2：
计算逻辑表达式和三元表达式混合：
6.7-100>39.6?5==5?4+5:6-1:!(100%3-39.0<27)?8*2-199:100%3
![](http://static.blog.csdn.net/images/save_snippets.png)
测试结果：
|测试|耗时（单位:秒）|
|----|----|
|Aviator|11.0|
|Groovy|13.0|
|IKExpression|168.8|
场景3：
计算算术表达式和逻辑表达式的混合，带有5个变量的表达式：
i * pi +(d * b -199)/(1- d * pi)-(2+100- i / pi)%99==i * pi +(d * b -199)/(1- d * pi)-(2+100- i / pi)%99
![](http://static.blog.csdn.net/images/save_snippets.png)
变量设定为：
int i =100;float pi =3.14f;double d =-3.9;byte b =(byte)4;booleanbool=false;
![](http://static.blog.csdn.net/images/save_snippets.png)
每次执行前都重新设置这些变量的值。
结果：
|测试|耗时（单位:秒）|
|----|----|
|Aviator|31.2|
|Groovy|9.7|
|IKExpression|编译错误|
场景4：
- Aviator执行 sysdate()
- groovy执行 new java.util.Date()
- IKExpression执行 $SYSDATE()
结果:
|测试|耗时（单位:秒）|
|----|----|
|Aviator|22.6|
|Groovy|13.9|
|IKExpression|25.4|
原始的测试报告在[这里](http://code.google.com/p/aviator/wiki/Performance)。
**四、结语**
能看到这里，并且感兴趣的朋友请点击项目主页：
[http://code.google.com/p/aviator/](http://code.google.com/p/aviator/)
下载地址：
[http://code.google.com/p/aviator/downloads/list](http://code.google.com/p/aviator/downloads/list)
完整的用户手册：
[http://code.google.com/p/aviator/wiki/User_Guide_zh](http://code.google.com/p/aviator/wiki/User_Guide_zh)
目前版本仍然是1.0.0-RC，希望更多朋友试用并最终release。有什么疑问或者建议请跟贴。
来自: [http://www.javaeye.com/topic/701496](http://www.javaeye.com/topic/701496)
