# JAVA反射机制 - z69183787的专栏 - CSDN博客
2012年11月18日 19:57:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：778
Reflection是Java 程序开发语言的特征之一，它允许运行中的 Java 程序对自身进行检查，或者说"自审"，并能直接操作程序的内部属性。例如，使用它能获得 Java 类中各成员的名称并显示出来。 Java 的这一能力在实际应用中也许用得不是很多，但是在其它的程序设计语言中根本就不存在这一特性。例如，Pascal、C 或者 C++ 中就没有办法在程序中获得函数定义相关的信息。
JavaBean 是 reflection 的实际应用之一，它能让一些工具可视化的操作软件组件。这些工具通过 reflection 动态的载入并取得 Java 组件(类) 的属性。
1. 一个简单的例子
考虑下面这个简单的例子，让我们看看 reflection 是如何工作的。
- import java.lang.reflect.*; 
- 
- publicclass DumpMethods { 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class c = Class.forName("java.util.Stack"); 
- 
-            Method m[] = c.getDeclaredMethods(); 
- 
- for (int i = 0; i < m.length; i++) 
-                System.out.println(m[i].toString()); 
-       } 
- catch (Throwable e){ 
-             System.err.println(e); 
-       } 
-    } 
- }
它的结果输出为：
- publicsynchronized java.lang.Object java.util.Stack.pop()
- public java.lang.Object java.util.Stack.push(java.lang.Object)
- publicboolean java.util.Stack.empty()
- publicsynchronized java.lang.Object java.util.Stack.peek()
- publicsynchronizedint java.util.Stack.search(java.lang.Object)
这样就列出了java.util.Stack 类的各方法名以及它们的限制符和返回类型。
这个程序使用 Class.forName 载入指定的类，然后调用 getDeclaredMethods 来获取这个类中定义了的方法列表。java.lang.reflect.Methods 是用来描述某个类中单个方法的一个类。
2.开始使用 Reflection
用于 reflection 的类，如 Method，可以在 java.lang.relfect 包中找到。使用这些类的时候必须要遵循三个步骤：第一步是获得你想操作的类的 java.lang.Class 对象。在运行中的 Java 程序中，用 java.lang.Class 类来描述类和接口等。
下面就是获得一个 Class 对象的方法之一：
Class c = Class.forName("java.lang.String");
这条语句得到一个 String 类的类对象。还有另一种方法，如下面的语句：
Class c = int.class; 或者 Class c = Integer.TYPE;
它们可获得基本类型的类信息。其中后一种方法中访问的是基本类型的封装类 (如 Integer) 中预先定义好的 TYPE 字段。
第二步是调用诸如 getDeclaredMethods 的方法，以取得该类中定义的所有方法的列表。
一旦取得这个信息，就可以进行第三步了——使用 reflection API 来操作这些信息，如下面这段代码：
- Class c = Class.forName("java.lang.String"); 
- 
- Method m[] = c.getDeclaredMethods(); 
- 
- System.out.println(m[0].toString()); 
- 
它将以文本方式打印出 String 中定义的第一个方法的原型。
在下面的例子中，这三个步骤将为使用 reflection 处理特殊应用程序提供例证。
模拟 instanceof 操作符
得到类信息之后，通常下一个步骤就是解决关于 Class 对象的一些基本的问题。例如，Class.isInstance 方法可以用于模拟 instanceof 操作符：
- class S { 
- }  
- 
- publicclass IsInstance { 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("S"); 
- boolean b1 = cls.isInstance(new Integer(37)); 
-            System.out.println(b1); 
- boolean b2 = cls.isInstance(new S()); 
-            System.out.println(b2); 
-       } 
- catch (Throwable e) { 
-            System.err.println(e); 
-       } 
-    } 
- }
在这个例子中创建了一个S 类的 Class 对象，然后检查一些对象是否是S的实例。Integer(37) 不是，但 new S()是。
3.找出类的方法
找出一个类中定义了些什么方法，这是一个非常有价值也非常基础的 reflection 用法。下面的代码就实现了这一用法：
- import java.lang.reflect.*;  
- 
- publicclass Method1 { 
- privateint f1(Object p, int x) throws NullPointerException { 
- 
- if (p == null) 
- thrownew NullPointerException(); 
- return x; 
- }  
- 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("Method1"); 
-            Method methlist[] = cls.getDeclaredMethods(); 
- 
- for (int i = 0; i < methlist.length; i++) { 
-                Method m = methlist[i]; 
-                System.out.println("name = " + m.getName()); 
-                System.out.println("decl class = " + m.getDeclaringClass()); 
-                Class pvec[] = m.getParameterTypes(); 
- 
- for (int j = 0; j < pvec.length; j++) 
-                    System.out.println("param #" + j + " " + pvec[j]); 
- 
-                Class evec[] = m.getExceptionTypes(); 
- 
- for (int j = 0; j < evec.length; j++) 
-                    System.out.println("exc #" + j + " " + evec[j]); 
- 
-                System.out.println("return type = " + m.getReturnType()); 
-                System.out.println("-----"); 
-            } 
-        } 
- catch (Throwable e) { 
-            System.err.println(e); 
-        } 
-    } 
- }  
这个程序首先取得 method1 类的描述，然后调用 getDeclaredMethods 来获取一系列的 Method 对象，它们分别描述了定义在类中的每一个方法，包括 public 方法、protected 方法、package 方法和 private 方法等。如果你在程序中使用 getMethods 来代替 getDeclaredMethods，你还能获得继承来的各个方法的信息。
取得了 Method 对象列表之后，要显示这些方法的参数类型、异常类型和返回值类型等就不难了。这些类型是基本类型还是类类型，都可以由描述类的对象按顺序给出。
输出的结果如下：
- name = f1 
- decl class = class method1 
- param #0class java.lang.Object 
- param #1int
- exc #0class java.lang.NullPointerException 
- return type = int
- -----
- name = main 
- decl class = class method1 
- param #0class [Ljava.lang.String; 
- return type = void
4.获取构造器信息
获取类构造器的用法与上述获取方法的用法类似，如：
- import java.lang.reflect.*;
- 
- publicclass Constructor1 { 
- public Constructor1() { 
-    }  
- 
- protected Constructor1(int i, double d) { 
-    }  
- 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("Constructor1"); 
-            Constructor ctorlist[] = cls.getDeclaredConstructors(); 
- 
- for (int i = 0; i < ctorlist.length; i++) { 
-               Constructor ct = ctorlist[i]; 
-               System.out.println("name = " + ct.getName()); 
-               System.out.println("decl class = " + ct.getDeclaringClass()); 
-               Class pvec[] = ct.getParameterTypes(); 
- 
- for (int j = 0; j < pvec.length; j++) 
-                  System.out.println("param #" + j + " " + pvec[j]); 
- 
-               Class evec[] = ct.getExceptionTypes(); 
- 
- for (int j = 0; j < evec.length; j++) 
-                  System.out.println("exc #" + j + " " + evec[j]); 
- 
-               System.out.println("-----"); 
-            } 
-       } 
- catch (Throwable e) { 
-            System.err.println(e); 
-       } 
-    } 
- } 
这个例子中没能获得返回类型的相关信息，那是因为构造器没有返回类型。
这个程序运行的结果是：
- name = Constructor1
- decl class = class Constructor1
- param #0int
- param #1double
- -----
- name = Constructor1
- decl class = class Constructor1
- -----
5.获取类的字段(域)
找出一个类中定义了哪些数据字段也是可能的，下面的代码就在干这个事情：
- import java.lang.reflect.*;  
- 
- publicclass Field1 { 
- privatedouble d; 
- publicstatic final int i = 37; 
-    String s = "testing";  
- 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("Field1"); 
-            Field fieldlist[] = cls.getDeclaredFields(); 
- 
- for (int i = 0; i < fieldlist.length; i++) { 
-               Field fld = fieldlist[i]; 
-               System.out.println("name = " + fld.getName()); 
-               System.out.println("decl class = " + fld.getDeclaringClass()); 
-               System.out.println("type = " + fld.getType()); 
- int mod = fld.getModifiers(); 
-               System.out.println("modifiers = " + Modifier.toString(mod)); 
-               System.out.println("-----"); 
-            } 
-       } 
- catch (Throwable e) { 
-            System.err.println(e); 
-       } 
-    } 
- }  
这个例子和前面那个例子非常相似。例中使用了一个新东西 Modifier，它也是一个 reflection 类，用来描述字段成员的修饰语，如“private int”。这些修饰语自身由整数描述，而且使用 Modifier.toString 来返回以“官方”顺序排列的字符串描述 (如“static”在“final”之前)。这个程序的输出是：
- name = d
- decl class = class Field1
- type = double
- modifiers = private
- -----
- name = i
- decl class = class Field1
- type = int
- modifiers = publicstatic final
- -----
- name = s
- decl class = class Field1
- type = class java.lang.String
- modifiers = 
- -----
和获取方法的情况一下，获取字段的时候也可以只取得在当前类中申明了的字段信息 (getDeclaredFields)，或者也可以取得父类中定义的字段 (getFields) 。
6.根据方法的名称来执行方法
文本到这里，所举的例子无一例外都与如何获取类的信息有关。我们也可以用 reflection 来做一些其它的事情，比如执行一个指定了名称的方法。下面的示例演示了这一操作：
- import java.lang.reflect.*; 
- 
- publicclass Method2 { 
- publicint add(int a, int b) { 
- return a + b; 
-    } 
- 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("Method2"); 
- 
-            Class partypes[] = new Class[2]; 
-            partypes[0] = Integer.TYPE; 
-            partypes[1] = Integer.TYPE; 
- 
-            Method meth = cls.getMethod("add", partypes); 
- 
-            Method2 methobj = new Method2(); 
-            Object arglist[] = new Object[2]; 
-            arglist[0] = new Integer(37); 
-            arglist[1] = new Integer(47); 
- 
-            Object retobj = meth.invoke(methobj, arglist); 
-            Integer retval = (Integer) retobj; 
-            System.out.println(retval.intValue()); 
-       } 
- catch (Throwable e) { 
-            System.err.println(e); 
-       } 
-    } 
- }
假如一个程序在执行的某处的时候才知道需要执行某个方法，这个方法的名称是在程序的运行过程中指定的 (例如，JavaBean 开发环境中就会做这样的事)，那么上面的程序演示了如何做到。
上例中，getMethod用于查找一个具有两个整型参数且名为 add 的方法。找到该方法并创建了相应的Method 对象之后，在正确的对象实例中执行它。执行该方法的时候，需要提供一个参数列表，这在上例中是分别包装了整数 37 和 47 的两个 Integer 对象。执行方法的返回的同样是一个 Integer 对象，它封装了返回值 84。
7.创建新的对象
对于构造器，则不能像执行方法那样进行，因为执行一个构造器就意味着创建了一个新的对象 (准确的说，创建一个对象的过程包括分配内存和构造对象)。所以，与上例最相似的例子如下：
- import java.lang.reflect.*;  
- 
- publicclass Constructor2 { 
- public Constructor2() { 
-    }  
- 
- public Constructor2(int a, int b) { 
-       System.out.println("a = " + a + " b = " + b); 
-    }  
- 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("Constructor2"); 
-            Class partypes[] = new Class[2]; 
-            partypes[0] = Integer.TYPE; 
-            partypes[1] = Integer.TYPE; 
- 
-            Constructor ct = cls.getConstructor(partypes); 
-            Object arglist[] = new Object[2]; 
-            arglist[0] = new Integer(37); 
-            arglist[1] = new Integer(47); 
-            Object retobj = ct.newInstance(arglist); 
-       } 
- catch (Throwable e) { 
-            System.err.println(e); 
-       } 
-    } 
- } 
根据指定的参数类型找到相应的构造函数并执行它，以创建一个新的对象实例。使用这种方法可以在程序运行时动态地创建对象，而不是在编译的时候创建对象，这一点非常有价值。
8.改变字段(域)的值
reflection 的还有一个用处就是改变对象数据字段的值。reflection 可以从正在运行的程序中根据名称找到对象的字段并改变它，下面的例子可以说明这一点：
- import java.lang.reflect.*;  
- 
- publicclass Field2 { 
- publicdouble d;  
- 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("Field2"); 
-            Field fld = cls.getField("d"); 
-            Field2 f2obj = new Field2(); 
-            System.out.println("d = " + f2obj.d); 
-            fld.setDouble(f2obj, 12.34); 
-            System.out.println("d = " + f2obj.d); 
-       } 
- catch (Throwable e) { 
-            System.err.println(e); 
-       } 
-    } 
- } 
这个例子中，字段 d 的值被变为了 12.34。
9.使用数组
本文介绍的 reflection 的最后一种用法是创建的操作数组。数组在 Java 语言中是一种特殊的类类型，一个数组的引用可以赋给 Object 引用。观察下面的例子看看数组是怎么工作的：
- import java.lang.reflect.*;  
- 
- publicclass Array1 { 
- publicstaticvoid main(String args[]) { 
- try { 
-            Class cls = Class.forName("java.lang.String"); 
-            Object arr = Array.newInstance(cls, 10); 
-            Array.set(arr, 5, "this is a test"); 
-            String s = (String) Array.get(arr, 5); 
-            System.out.println(s); 
-       } 
- catch (Throwable e) { 
-            System.err.println(e); 
-       } 
-    } 
- } 
例中创建了 10 个单位长度的 String 数组，为第 5 个位置的字符串赋了值，最后将这个字符串从数组中取得并打印了出来。
下面这段代码提供了一个更复杂的例子：
- import java.lang.reflect.*;  
- 
- publicclass Array2 { 
- publicstaticvoid main(String args[]) { 
- int dims[] = newint[]{5, 10, 15}; 
-       Object arr = Array.newInstance(Integer.TYPE, dims); 
-       Object arrobj = Array.get(arr, 3); 
-       Class cls = arrobj.getClass().getComponentType(); 
-       System.out.println(cls); 
-       arrobj = Array.get(arrobj, 5); 
-       Array.setInt(arrobj, 10, 37); 
- int arrcast[][][] = (int[][][]) arr; 
-       System.out.println(arrcast[3][5][10]); 
-    } 
- } 
例中创建了一个 5 x 10 x 15 的整型数组，并为处于 [3][5][10] 的元素赋了值为 37。注意，多维数组实际上就是数组的数组，例如，第一个 Array.get 之后，arrobj 是一个 10 x 15 的数组。进而取得其中的一个元素，即长度为 15 的数组，并使用 Array.setInt 为它的第 10 个元素赋值。
注意创建数组时的类型是动态的，在编译时并不知道其类型。
