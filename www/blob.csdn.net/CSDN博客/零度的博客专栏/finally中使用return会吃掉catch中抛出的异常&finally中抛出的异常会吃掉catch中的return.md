# finally中使用return会吃掉catch中抛出的异常&finally中抛出的异常会吃掉catch中的return - 零度的博客专栏 - CSDN博客
2017年04月06日 11:28:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：336
1：当一个方法既有throws又有catch的时候，直接catch异常的引用不再throws
2：finally中的return语句会使异常丢失
3：finally中重新抛出的异常会覆盖catch的异常
今天学习大神的文章：[深入理解java异常处理机制](http://blog.csdn.net/hguisu/article/details/6155636) 学到一个有意思的知识点。如果在finally中使用return会吃掉catch中抛出的异常。
看例子：
**[java]**[view
 plain](http://blog.csdn.net/tiantiandjava/article/details/46776403#)[copy](http://blog.csdn.net/tiantiandjava/article/details/46776403#)
- publicclass TestException {  
- public TestException() {  
-     }  
- 
- boolean testEx() throws Exception {  
- boolean ret = true;  
- try {  
-             ret = testEx1();  
-         } catch (Exception e) {  
-             System.out.println("testEx, catch exception");  
-             ret = false;  
- throw e;  
-         } finally {  
-             System.out.println("testEx, finally; return value=" + ret);  
- return ret;  
-         }  
-     }  
- 
- boolean testEx1() throws Exception {  
- boolean ret = true;  
- try {  
-             ret = testEx2();  
- if (!ret) {  
- returnfalse;  
-             }  
-             System.out.println("testEx1, at the end of try");  
- return ret;  
-         } catch (Exception e) {  
-             System.out.println("testEx1, catch exception");  
-             ret = false;  
- throw e;  
-         } finally {  
-             System.out.println("testEx1, finally; return value=" + ret);  
- return ret;  
-         }  
-     }  
- 
- boolean testEx2() throws Exception {  
- boolean ret = true;  
- try {  
- int b = 12;  
- int c;  
- for (int i = 2; i >= -2; i--) {  
-                 c = b / i;  
-                 System.out.println("i=" + i);  
-             }  
- returntrue;  
-         } catch (Exception e) {  
-             System.out.println("testEx2, catch exception");  
-             ret = false;  
- throw e;  
-         } finally {  
-             System.out.println("testEx2, finally; return value=" + ret);  
- return ret;  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         TestException testException1 = new TestException();  
- try {  
-             testException1.testEx();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
运行结果：
i=2
i=1
testEx2, catch exception
testEx2, finally; return value=false
testEx1, finally; return value=false
testEx, finally; return value=false
有点奇怪，下层方法抛出的异常竟然没有被捕获。
如果把return和throw放在一起，直接会提示错误。"Unreachable statement"（无法被执行）.
然而finally却可以成功骗过编译器让两者并存（是不是可以算是编译器的一个小bug呢），结果是后执行的会覆盖前者。finally如果有return会覆盖catch里的throw，同样如果finally里有throw会覆盖catch里的return。
进而如果catch里和finally都有return finally中的return会覆盖catch中的。throw也是如此。
这样就好理解一些了，retrun和throw都是使程序跳出当前的方法，自然就是冲突的。如果非要跳出两次那么后者会覆盖前者。
在《[Java](http://lib.csdn.net/base/javase)编程思想》中也有类似的例子，放在这里一起讨论。
“9.6.2 缺点：丢失的违例
一般情况下，Java的违例实施方案都显得十分出色。不幸的是，它依然存在一个缺点。尽管违例指出程序里存在一个危机，而且绝不应忽略，但一个违例仍有可能简单地“丢失”。在采用finally从句的一种特殊配置下，便有可能发生这种情况”
**[java]**[view
 plain](http://blog.csdn.net/tiantiandjava/article/details/46777051#)[copy](http://blog.csdn.net/tiantiandjava/article/details/46777051#)
- class VeryImportantException extends Exception{  
- public String toString(){  
- return"A very important exception";  
-     }  
- }  
- 
- class HoHumException extends Exception{  
- public String toString() {  
- return"A trivial exception";  
-     }  
- }  
- publicclass LostMessage {  
- void f() throws VeryImportantException{  
- thrownew VeryImportantException();  
-     }  
- 
- void dispose() throws HoHumException{  
- thrownew HoHumException();  
-     }  
- 
- publicstaticvoid main(String[] args) throws Exception{  
-         LostMessage lm = new LostMessage();  
- try{  
-             lm.f();  
-         }finally {  
-             lm.dispose();  
-         }  
- 
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
输出：
Exception in thread "main" A trivial exception
at com.test.exception.LostMessage.dispose(LostMessage.java:24)
at com.test.exception.LostMessage.main(LostMessage.java:32)
“这是一项相当严重的缺陷，因为它意味着一个违例可能完全丢失。而且就象前例演示的那样，这种丢失显得非常“自然”，很难被人查出蛛丝马迹。而与此相反，C++里如果第二个违例在第一个违例得到控制前产生，就会被当作一个严重的编程错误处理。或许Java以后的版本会纠正这个问题（上述结果是用[Java](http://lib.csdn.net/base/java)1.1生成的）。”
书中的例子更加犀利，我们尽量避免这样的事情发生吧。
