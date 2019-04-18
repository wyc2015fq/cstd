# Java异常捕获之try-catch-finally-return的执行顺序 - z69183787的专栏 - CSDN博客
2017年03月13日 17:23:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1876
情况1：try块中没有抛出异常try和finally块中都有return语句
**[java]**[view
 plain](http://blog.csdn.net/aaoxue/article/details/8535754#)[copy](http://blog.csdn.net/aaoxue/article/details/8535754#)
- publicstaticint NoException(){  
- int i=10;  
- try{  
-   System.out.println("i in try block is"+i);  
- return --i;  
-  }catch(Exception e){  
-   --i;  
-   System.out.println("i in catch - form try block is"+i);  
- return --i;  
-  }finally{  
- 
-   System.out.println("i in finally - from try or catch block is"+i);  
- return --i;  
-  }  
- }  
执行结果：
i in try block is10
i in finally - from try or catch block is9
the method value is8
**执行顺序：执行try块，执行到return语句时，先执行return的语句，--i，但是不返回到main 方法，执行finally块，遇到finally块中的return语句，执行--i,并将值返回到main方法，这里就不会再回去返回try块中计算得到的值**
**情况2：try块中没有抛出异常，仅try中有return语句**
**代码：**
**[java]**[view
 plain](http://blog.csdn.net/aaoxue/article/details/8535754#)[copy](http://blog.csdn.net/aaoxue/article/details/8535754#)
- publicstaticint NoException(){  
- int i=10;  
- try{  
-         System.out.println("i in try block is--"+i);  
- return --i;  
-     }catch(Exception e){  
-         --i;  
-         System.out.println("i in catch - form try block is--"+i);  
- return --i;  
-     }finally{  
- 
-         System.out.println("i in finally - from try or catch block is--"+i);  
-         --i;  
-         System.out.println("i in finally block is--"+i);  
- //return --i;
-     }  
- }  
执行结果：
i in try block is--10
i in finally - from try or catch block is--9
i in finally block is--8
the method value is--9
顺序：try中执行完return的语句后，不返回，执行finally块，finally块执行结束后，返回到try块中，返回i在try块中最后的值
情况3：try块中抛出异常try,catch,finally中都有return语句
代码：
**[java]**[view
 plain](http://blog.csdn.net/aaoxue/article/details/8535754#)[copy](http://blog.csdn.net/aaoxue/article/details/8535754#)
- publicstaticint WithException(){  
- int i=10;  
- try{  
-         System.out.println("i in try block is--"+i);  
-         i = i/0;  
- return --i;  
-     }catch(Exception e){  
-         System.out.println("i in catch - form try block is--"+i);  
-         --i;  
-         System.out.println("i in catch block is--"+i);  
- return --i;  
-     }finally{  
- 
-         System.out.println("i in finally - from try or catch block is--"+i);  
-         --i;  
-         System.out.println("i in finally block is--"+i);  
- return --i;  
-     }  
- }  
执行结果：
i in try block is--10
i in catch - form try block is--10
i in catch block is--9
i in finally - from try or catch block is--8
i in finally block is--7
the method value is--6
**顺序，抛出异常后，执行catch块，在catch块的return的--i执行完后，并不直接返回而是执行finally，因finally中有return语句，所以，执行，返回结果6**
**情况4，catch中有return,finally中没有，同上，执行完finally语句后，依旧返回catch中的执行return语句后的值，而不是finally中修改的值**
**情况5：try和catch中都有异常，finally中无return语句**
**[java]**[view
 plain](http://blog.csdn.net/aaoxue/article/details/8535754#)[copy](http://blog.csdn.net/aaoxue/article/details/8535754#)
- publicstaticint CatchException(){  
- int i=10;  
- try{  
-         System.out.println("i in try block is--"+i);  
-         i=i/0;  
- return --i;  
-     }catch(Exception e){  
-         System.out.println("i in catch - form try block is--"+i);  
- int j = i/0;  
- return --i;  
-     }finally{  
- 
-         System.out.println("i in finally - from try or catch block is--"+i);  
-         --i;  
-         System.out.println("i in finally block is--"+i);  
- //return --i;
-     }  
- }  
结果：
i in try block is--10
i in catch - form try block is--10
i in finally - from try or catch block is--10
i in finally block is--9
Exception in thread "main" [Java](http://lib.csdn.net/base/javase).lang.ArithmeticException: / by zero
 at exception.ExceptionTest0123.CatchException(ExceptionTest0123.java:29)
 at exception.ExceptionTest0123.main(ExceptionTest0123.java:17)
**执行顺序：在try块中出现异常，到catch中，执行到异常，到finally中执行，finally执行结束后判断发现异常，抛出**
**情况6：try,catch中都出现异常，在finally中有返回**
**[javascript]**[view
 plain](http://blog.csdn.net/aaoxue/article/details/8535754#)[copy](http://blog.csdn.net/aaoxue/article/details/8535754#)
- publicstaticint CatchException(){  
- int i=10;  
- try{  
-         System.out.println("i in try block is--"+i);  
-         i=i/0;  
- return --i;  
-     }catch(Exception e){  
-         System.out.println("i in catch - form try block is--"+i);  
- int j = i/0;  
- return --i;  
-     }finally{  
- 
-         System.out.println("i in finally - from try or catch block is--"+i);  
-         --i;  
-         System.out.println("i in finally block is--"+i);  
- return --i;  
-     }  
- }  
运行结果：
i in try block is--10
i in catch - form try block is--10
i in finally - from try or catch block is--10
i in finally block is--9
the method value is--8
**执行顺序：try块中出现异常到catch，catch中出现异常到finally，finally中执行到return语句返回，不检查异常**
**没有catch，只有try和finally时，执行顺序和上面的几种情况差不多，只是少了catch块的执行**
