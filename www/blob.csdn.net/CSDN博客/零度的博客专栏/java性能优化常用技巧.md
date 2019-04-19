# java性能优化常用技巧 - 零度的博客专栏 - CSDN博客
2016年04月18日 09:47:32[零度anngle](https://me.csdn.net/zmx729618)阅读数：256
一、避免在循环条件中使用复杂表达式
在不做编译优化的情况下，在循环中，循环条件会被反复计算，如果不使用复杂表达式，而使循环条件值不变的话，程序将会运行的更快。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.util.vector;  
- class cel {  
- void method (vector vector) {  
- for (int i = 0; i < vector.size (); i++)  // violation
-             ; // ...
-     }  
- }  
```java
import java.util.vector;
class cel {
    void method (vector vector) {
        for (int i = 0; i < vector.size (); i++)  // violation
            ; // ...
    }
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- class cel_fixed {  
- void method (vector vector) {  
- int size = vector.size ()  
- for (int i = 0; i < size; i++)  
-             ; // ...
-     }  
- }  
```java
class cel_fixed {
    void method (vector vector) {
        int size = vector.size ()
        for (int i = 0; i < size; i++)
            ; // ...
    }
}
```
二、为'vectors' 和 'hashtables'定义初始大小 
jvm为vector扩充大小的时候需要重新创建一个更大的数组，将原原先数组中的内容复制过来，最后，原先的数组再被回收。可见vector容量的扩大是一个颇费时间的事。 
通常，默认的10个元素大小是不够的。你最好能准确的估计你所需要的最佳大小。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.util.vector;  
- publicclass dic {  
- publicvoid addobjects (object[] o) {  
- // if length > 10, vector needs to expand
- for (int i = 0; i< o.length;i++) {      
-             v.add(o);   // capacity before it can add more elements.
-         }  
-     }  
- public vector v = new vector();  // no initialcapacity.
- }  
```java
import java.util.vector;
public class dic {
    public void addobjects (object[] o) {
        // if length > 10, vector needs to expand
        for (int i = 0; i< o.length;i++) {    
            v.add(o);   // capacity before it can add more elements.
        }
    }
    public vector v = new vector();  // no initialcapacity.
}
```
更正： 
自己设定初始大小。 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- public vector v = new vector(20);    
- public hashtable hash = new hashtable(10);  
```java
public vector v = new vector(20);  
    public hashtable hash = new hashtable(10);
```
参考资料： 
dov bulka, "java performance and scalability volume 1: server-side programming 
techniques" addison wesley, isbn: 0-201-70429-3 pp.55 – 57 
三、在finally块中关闭stream
程序中使用到的资源应当被释放，以避免资源泄漏。这最好在finally块中去做。不管程序执行的结果如何，finally块总是会执行的，以确保资源的正确关闭。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.io.*;  
- publicclass cs {  
- publicstaticvoid main (string args[]) {  
-         cs cs = new cs ();  
-         cs.method ();  
-     }  
- publicvoid method () {  
- try {  
-             fileinputstream fis = new fileinputstream ("cs.java");  
- int count = 0;  
- while (fis.read () != -1)  
-                 count++;  
-             system.out.println (count);  
-             fis.close ();  
-         } catch (filenotfoundexception e1) {  
-         } catch (ioexception e2) {  
-         }  
-     }  
- }  
```java
import java.io.*;
public class cs {
    public static void main (string args[]) {
        cs cs = new cs ();
        cs.method ();
    }
    public void method () {
        try {
            fileinputstream fis = new fileinputstream ("cs.java");
            int count = 0;
            while (fis.read () != -1)
                count++;
            system.out.println (count);
            fis.close ();
        } catch (filenotfoundexception e1) {
        } catch (ioexception e2) {
        }
    }
}
```
更正： 
在最后一个catch后添加一个finally块 
参考资料： 
peter haggar: "practical java - programming language guide". 
addison wesley, 2000, pp.77-79 
四、使用'system.arraycopy ()'代替通过来循环复制数组
'system.arraycopy ()' 要比通过循环来复制数组快的多。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass irb  
- {  
- void method () {  
- int[] array1 = newint [100];  
- for (int i = 0; i < array1.length; i++) {  
-             array1 [i] = i;  
-         }  
- int[] array2 = newint [100];  
- for (int i = 0; i < array2.length; i++) {  
-             array2 [i] = array1 [i];                 // violation
-         }  
-     }  
- }  
```java
public class irb
{
    void method () {
        int[] array1 = new int [100];
        for (int i = 0; i < array1.length; i++) {
            array1 [i] = i;
        }
        int[] array2 = new int [100];
        for (int i = 0; i < array2.length; i++) {
            array2 [i] = array1 [i];                 // violation
        }
    }
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass irb  
- {  
- void method () {  
- int[] array1 = newint [100];  
- for (int i = 0; i < array1.length; i++) {  
-             array1 [i] = i;  
-         }  
- int[] array2 = newint [100];  
-         system.arraycopy(array1, 0, array2, 0, 100);  
-     }  
- }  
```java
public class irb
{
    void method () {
        int[] array1 = new int [100];
        for (int i = 0; i < array1.length; i++) {
            array1 [i] = i;
        }
        int[] array2 = new int [100];
        system.arraycopy(array1, 0, array2, 0, 100);
    }
}
```
参考资料： 
http://www.cs.cmu.edu/~jch/java/speed.html 
五、让访问实例内变量的getter/setter方法变成”final”
简单的getter/setter方法应该被置成final，这会告诉编译器，这个方法不会被重载，所以，可以变成”inlined” 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- class maf {  
- publicvoid setsize (int size) {  
-          _size = size;  
-     }  
- privateint _size;  
- }  
```java
class maf {
    public void setsize (int size) {
         _size = size;
    }
    private int _size;
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- class daf_fixed {  
- finalpublicvoid setsize (int size) {  
-          _size = size;  
-     }  
- privateint _size;  
- }  
```java
class daf_fixed {
    final public void setsize (int size) {
         _size = size;
    }
    private int _size;
}
```
参考资料： 
warren n. and bishop p. (1999), "java in practice", p. 4-5 
addison-wesley, isbn 0-201-36065-9 
六、避免不需要的instanceof操作
如果左边的对象的静态类型等于右边的，instanceof表达式返回永远为true。 
例子：         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass uiso {  
- public uiso () {}  
- }  
- class dog extends uiso {  
- void method (dog dog, uiso u) {  
-         dog d = dog;  
- if (d instanceof uiso) // always true.
-             system.out.println("dog is a uiso");  
-         uiso uiso = u;  
- if (uiso instanceof object) // always true.
-             system.out.println("uiso is an object");  
-     }  
- }  
```java
public class uiso {
    public uiso () {}
}
class dog extends uiso {
    void method (dog dog, uiso u) {
        dog d = dog;
        if (d instanceof uiso) // always true.
            system.out.println("dog is a uiso");
        uiso uiso = u;
        if (uiso instanceof object) // always true.
            system.out.println("uiso is an object");
    }
}
```
更正：         
删掉不需要的instanceof操作。 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- class dog extends uiso {  
- void method () {  
-         dog d;  
-         system.out.println ("dog is an uiso");  
-         system.out.println ("uiso is an uiso");  
-     }  
- }  
```java
class dog extends uiso {
    void method () {
        dog d;
        system.out.println ("dog is an uiso");
        system.out.println ("uiso is an uiso");
    }
}
```
七、避免不需要的造型操作
所有的类都是直接或者间接继承自object。同样，所有的子类也都隐含的“等于”其父类。那么，由子类造型至父类的操作就是不必要的了。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- class unc {  
-     string _id = "unc";  
- }  
- class dog extends unc {  
- void method () {  
-         dog dog = new dog ();  
-         unc animal = (unc)dog;  // not necessary.
-         object o = (object)dog;         // not necessary.
-     }  
- }  
```java
class unc {
    string _id = "unc";
}
class dog extends unc {
    void method () {
        dog dog = new dog ();
        unc animal = (unc)dog;  // not necessary.
        object o = (object)dog;         // not necessary.
    }
}
```
更正：         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- class dog extends unc {  
- void method () {  
-         dog dog = new dog();  
-         unc animal = dog;  
-         object o = dog;  
-     }  
- }  
- 
```java
class dog extends unc {
    void method () {
        dog dog = new dog();
        unc animal = dog;
        object o = dog;
    }
}
```
参考资料： 
nigel warren, philip bishop: "java in practice - design styles and idioms 
for effective java".  addison-wesley, 1999. pp.22-23 
八、如果只是查找单个字符的话，用charat()代替startswith()
用一个字符作为参数调用startswith()也会工作的很好，但从性能角度上来看，调用用string api无疑是错误的! 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass pcts {  
- privatevoid method(string s) {  
- if (s.startswith("a")) { // violation
- // ...
-         }  
-     }  
- }  
```java
public class pcts {
    private void method(string s) {
        if (s.startswith("a")) { // violation
            // ...
        }
    }
}
```
更正         
将'startswith()' 替换成'charat()'. 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass pcts {  
- privatevoid method(string s) {  
- if ('a' == s.charat(0)) {  
- // ...
-         }  
-     }  
- }  
```java
public class pcts {
    private void method(string s) {
        if ('a' == s.charat(0)) {
            // ...
        }
    }
}
```
参考资料： 
dov bulka, "java performance and scalability volume 1: server-side programming 
techniques"  addison wesley, isbn: 0-201-70429-3 
九、使用移位操作来代替'a / b'操作
"/"是一个很“昂贵”的操作，使用移位操作将会更快更有效。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass sdiv {  
- publicstaticfinalint num = 16;  
- publicvoid calculate(int a) {  
- int div = a / 4;            // should be replaced with "a >> 2".
- int div2 = a / 8;         // should be replaced with "a >> 3".
- int temp = a / 3;  
-     }  
- }  
```java
public class sdiv {
    public static final int num = 16;
    public void calculate(int a) {
        int div = a / 4;            // should be replaced with "a >> 2".
        int div2 = a / 8;         // should be replaced with "a >> 3".
        int temp = a / 3;
    }
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass sdiv {  
- publicstaticfinalint num = 16;  
- publicvoid calculate(int a) {  
- int div = a >> 2;    
- int div2 = a >> 3;  
- int temp = a / 3;       // 不能转换成位移操作
-     }  
- }  
```java
public class sdiv {
    public static final int num = 16;
    public void calculate(int a) {
        int div = a >> 2;  
        int div2 = a >> 3;
        int temp = a / 3;       // 不能转换成位移操作
    }
}
```
十、使用移位操作代替'a * b'
同上。 
[i]但我个人认为，除非是在一个非常大的循环内，性能非常重要，而且你很清楚你自己在做什么，方可使用这种方法。否则提高性能所带来的程序晚读性的降低将是不合算的。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass smul {  
- publicvoid calculate(int a) {  
- int mul = a * 4;            // should be replaced with "a << 2".
- int mul2 = 8 * a;         // should be replaced with "a << 3".
- int temp = a * 3;  
-     }  
- }  
```java
public class smul {
    public void calculate(int a) {
        int mul = a * 4;            // should be replaced with "a << 2".
        int mul2 = 8 * a;         // should be replaced with "a << 3".
        int temp = a * 3;
    }
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- package opt;  
- publicclass smul {  
- publicvoid calculate(int a) {  
- int mul = a << 2;    
- int mul2 = a << 3;  
- int temp = a * 3;       // 不能转换
-     }  
- }  
```java
package opt;
public class smul {
    public void calculate(int a) {
        int mul = a << 2;  
        int mul2 = a << 3;
        int temp = a * 3;       // 不能转换
    }
}
```
十一、在字符串相加的时候，使用 ' ' 代替 " "，如果该字符串只有一个字符的话
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass str {  
- publicvoid method(string s) {  
-         string string = s + "d"// violation.
-         string = "abc" + "d"// violation.
-     }  
- }  
```java
public class str {
    public void method(string s) {
        string string = s + "d"  // violation.
        string = "abc" + "d"      // violation.
    }
}
```
更正： 
将一个字符的字符串替换成' ' 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass str {  
- publicvoid method(string s) {  
-         string string = s + 'd'
-         string = "abc" + 'd'
-     }  
- }  
```java
public class str {
    public void method(string s) {
        string string = s + 'd'
        string = "abc" + 'd'   
    }
}
```
十二、不要在循环中调用synchronized(同步)方法
方法的同步需要消耗相当大的资料，在一个循环中调用它绝对不是一个好主意。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.util.vector;  
- publicclass syn {  
- publicsynchronizedvoid method (object o) {  
-     }  
- privatevoid test () {  
- for (int i = 0; i < vector.size(); i++) {  
-             method (vector.elementat(i));    // violation
-         }  
-     }  
- private vector vector = new vector (5, 5);  
- }  
```java
import java.util.vector;
public class syn {
    public synchronized void method (object o) {
    }
    private void test () {
        for (int i = 0; i < vector.size(); i++) {
            method (vector.elementat(i));    // violation
        }
    }
    private vector vector = new vector (5, 5);
}
```
更正： 
不要在循环体中调用同步方法，如果必须同步的话，推荐以下方式： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.util.vector;  
- publicclass syn {  
- publicvoid method (object o) {  
-     }  
- privatevoid test () {  
- synchronized{//在一个同步块中执行非同步方法
- for (int i = 0; i < vector.size(); i++) {  
-                 method (vector.elementat(i));     
-             }  
-         }  
-     }  
- private vector vector = new vector (5, 5);  
- }  
```java
import java.util.vector;
public class syn {
    public void method (object o) {
    }
private void test () {
    synchronized{//在一个同步块中执行非同步方法
            for (int i = 0; i < vector.size(); i++) {
                method (vector.elementat(i));   
            }
        }
    }
    private vector vector = new vector (5, 5);
}
```
十三、将try/catch块移出循环
把try/catch块放入循环体内，会极大的影响性能，如果编译jit被关闭或者你所使用的是一个不带jit的jvm，性能会将下降21%之多! 
例子：         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.io.fileinputstream;  
- publicclasstry {  
- void method (fileinputstream fis) {  
- for (int i = 0; i < size; i++) {  
- try {                                      // violation
-                 _sum += fis.read();  
-             } catch (exception e) {}  
-         }  
-     }  
- privateint _sum;  
- }  
```java
import java.io.fileinputstream;
public class try {
    void method (fileinputstream fis) {
        for (int i = 0; i < size; i++) {
            try {                                      // violation
                _sum += fis.read();
            } catch (exception e) {}
        }
    }
    private int _sum;
}
```
更正：         
将try/catch块移出循环         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- void method (fileinputstream fis) {  
- try {  
- for (int i = 0; i < size; i++) {  
-                _sum += fis.read();  
-            }  
-        } catch (exception e) {}  
-    }  
```java
void method (fileinputstream fis) {
        try {
            for (int i = 0; i < size; i++) {
                _sum += fis.read();
            }
        } catch (exception e) {}
    }
```
参考资料： 
peter haggar: "practical java - programming language guide". 
addison wesley, 2000, pp.81 – 83 
十四、对于boolean值，避免不必要的等式判断
将一个boolean值与一个true比较是一个恒等操作(直接返回该boolean变量的值). 移走对于boolean的不必要操作至少会带来2个好处： 
1)代码执行的更快 (生成的字节码少了5个字节)； 
2)代码也会更加干净 。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass ueq  
- {  
- boolean method (string string) {  
- return string.endswith ("a") == true;   // violation
-     }  
- }  
```java
public class ueq
{
    boolean method (string string) {
        return string.endswith ("a") == true;   // violation
    }
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- class ueq_fixed  
- {  
- boolean method (string string) {  
- return string.endswith ("a");  
-     }  
- }  
```java
class ueq_fixed
{
    boolean method (string string) {
        return string.endswith ("a");
    }
}
```
十五、对于常量字符串，用'string' 代替 'stringbuffer'
常量字符串并不需要动态改变长度。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass usc {  
-     string method () {  
-         stringbuffer s = new stringbuffer ("hello");  
-         string t = s + "world!";  
- return t;  
-     }  
- }  
```java
public class usc {
    string method () {
        stringbuffer s = new stringbuffer ("hello");
        string t = s + "world!";
        return t;
    }
}
```
更正： 
把stringbuffer换成string，如果确定这个string不会再变的话，这将会减少运行开销提高性能。 
十六、用'stringtokenizer' 代替 'indexof()' 和'substring()'
字符串的分析在很多应用中都是常见的。使用indexof()和substring()来分析字符串容易导致 stringindexoutofboundsexception。而使用stringtokenizer类来分析字符串则会容易一些，效率也会高一些。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass ust {  
- void parsestring(string string) {  
- int index = 0;  
- while ((index = string.indexof(".", index)) != -1) {  
-             system.out.println (string.substring(index, string.length()));  
-         }  
-     }  
- }  
```java
public class ust {
    void parsestring(string string) {
        int index = 0;
        while ((index = string.indexof(".", index)) != -1) {
            system.out.println (string.substring(index, string.length()));
        }
    }
}
```
参考资料： 
graig larman, rhett guthrie: "java 2 performance and idiom guide" 
prentice hall ptr, isbn: 0-13-014260-3 pp. 282 – 283 
十七、使用条件操作符替代"if (cond) return; else return;" 结构
条件操作符更加的简捷 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclassif {  
- publicint method(boolean isdone) {  
- if (isdone) {  
- return0;  
-         } else {  
- return10;  
-         }  
-     }  
- }  
```java
public class if {
    public int method(boolean isdone) {
        if (isdone) {
            return 0;
        } else {
            return 10;
        }
    }
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclassif {  
- publicint method(boolean isdone) {  
- return (isdone ? 0 : 10);  
-     }  
- }  
```java
public class if {
    public int method(boolean isdone) {
        return (isdone ? 0 : 10);
    }
}
```
十八、使用条件操作符代替"if (cond) a = b; else a = c;" 结构
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass ifas {  
- void method(boolean istrue) {  
- if (istrue) {  
-             _value = 0;  
-         } else {  
-             _value = 1;  
-         }  
-     }  
- privateint _value = 0;  
- }  
```java
public class ifas {
    void method(boolean istrue) {
        if (istrue) {
            _value = 0;
        } else {
            _value = 1;
        }
    }
    private int _value = 0;
}
```
更正： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass ifas {  
- void method(boolean istrue) {  
-         _value = (istrue ? 0 : 1);       // compact expression.
-     }  
- privateint _value = 0;  
- }  
```java
public class ifas {
    void method(boolean istrue) {
        _value = (istrue ? 0 : 1);       // compact expression.
    }
    private int _value = 0;
}
```
十九、不要在循环体中实例化变量
在循环体中实例化临时变量将会增加内存消耗 
例子：         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.util.vector;  
- publicclass loop {  
- void method (vector v) {  
- for (int i=0;i < v.size();i++) {  
-             object o = new object();  
-             o = v.elementat(i);  
-         }  
-     }  
- }  
```java
import java.util.vector;
public class loop {
    void method (vector v) {
        for (int i=0;i < v.size();i++) {
            object o = new object();
            o = v.elementat(i);
        }
    }
}
```
更正：         
在循环体外定义变量，并反复使用         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- import java.util.vector;  
- publicclass loop {  
- void method (vector v) {  
-         object o;  
- for (int i=0;i<v.size();i++) {  
-             o = v.elementat(i);  
-         }  
-     }  
- }  
```java
import java.util.vector;
public class loop {
    void method (vector v) {
        object o;
        for (int i=0;i<v.size();i++) {
            o = v.elementat(i);
        }
    }
}
```
二十、确定 stringbuffer的容量
stringbuffer的构造器会创建一个默认大小(通常是16)的字符数组。在使用中，如果超出这个大小，就会重新分配内存，创建一个更大的数组，并将原先的数组复制过来，再丢弃旧的数组。在大多数情况下，你可以在创建stringbuffer的时候指定大小，这样就避免了在容量不够的时候自动增长，以提高性能。 
例子：         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass rsbc {  
- void method () {  
-         stringbuffer buffer = new stringbuffer(); // violation
-         buffer.append ("hello");  
-     }  
- }  
```java
public class rsbc {
    void method () {
        stringbuffer buffer = new stringbuffer(); // violation
        buffer.append ("hello");
    }
}
```
更正：         
为stringbuffer提供寝大小。         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass rsbc {  
- void method () {  
-         stringbuffer buffer = new stringbuffer(max);  
-         buffer.append ("hello");  
-     }  
- privatefinalint max = 100;  
- }  
```java
public class rsbc {
    void method () {
        stringbuffer buffer = new stringbuffer(max);
        buffer.append ("hello");
    }
    private final int max = 100;
}
```
参考资料： 
dov bulka, "java performance and scalability volume 1: server-side programming 
techniques" addison wesley, isbn: 0-201-70429-3 p.30 – 31 
二十一、尽可能的使用栈变量
如果一个变量需要经常访问，那么你就需要考虑这个变量的作用域了。static? local?还是实例变量？访问静态变量和实例变量将会比访问局部变量多耗费2-3个时钟周期。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass usv {  
- void getsum (int[] values) {  
- for (int i=0; i < value.length; i++) {  
-             _sum += value[i];           // violation.
-         }  
-     }  
- void getsum2 (int[] values) {  
- for (int i=0; i < value.length; i++) {  
-             _staticsum += value[i];  
-         }  
-     }  
- privateint _sum;  
- privatestaticint _staticsum;  
- }       
```java
public class usv {
    void getsum (int[] values) {
        for (int i=0; i < value.length; i++) {
            _sum += value[i];           // violation.
        }
    }
    void getsum2 (int[] values) {
        for (int i=0; i < value.length; i++) {
            _staticsum += value[i];
        }
    }
    private int _sum;
    private static int _staticsum;
}
```
更正：         
如果可能，请使用局部变量作为你经常访问的变量。 
你可以按下面的方法来修改getsum()方法：         
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- void getsum (int[] values) {  
- int sum = _sum;  // temporary local variable.
- for (int i=0; i < value.length; i++) {  
-         sum += value[i];  
-     }  
-     _sum = sum;  
- }  
```java
void getsum (int[] values) {
    int sum = _sum;  // temporary local variable.
    for (int i=0; i < value.length; i++) {
        sum += value[i];
    }
    _sum = sum;
}
```
参考资料：         
peter haggar: "practical java - programming language guide". 
addison wesley, 2000, pp.122 – 125 
二十二、不要总是使用取反操作符(!)
取反操作符(!)降低程序的可读性，所以不要总是使用。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass dun {  
- boolean method (boolean a, boolean b) {  
- if (!a)  
- return !a;  
- else
- return !b;  
-     }  
- }  
```java
public class dun {
    boolean method (boolean a, boolean b) {
        if (!a)
            return !a;
        else
            return !b;
    }
}
```
更正： 
如果可能不要使用取反操作符(!) 
二十三、与一个接口 进行instanceof操作
基于接口的设计通常是件好事，因为它允许有不同的实现，而又保持灵活。只要可能，对一个对象进行instanceof操作，以判断它是否某一接口要比是否某一个类要快。 
例子： 
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/8706200#)[copy](http://blog.csdn.net/z69183787/article/details/8706200#)[print](http://blog.csdn.net/z69183787/article/details/8706200#)[?](http://blog.csdn.net/z69183787/article/details/8706200#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/287345/fork)
- publicclass insof {  
- privatevoid method (object o) {  
- if (o instanceof interfacebase) { }  // better
- if (o instanceof classbase) { }   // worse.
-     }  
- }  
- 
- class classbase {}  
- interface interfacebase {}  
```java
public class insof {
    private void method (object o) {
        if (o instanceof interfacebase) { }  // better
        if (o instanceof classbase) { }   // worse.
    }
}
class classbase {}
interface interfacebase {}
```
参考资料： 
graig larman, rhett guthrie: "java 2 performance and idiom guide" 
prentice hall ptr, 2000.  pp.207
            
