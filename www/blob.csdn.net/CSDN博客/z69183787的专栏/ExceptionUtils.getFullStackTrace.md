# ExceptionUtils.getFullStackTrace - z69183787的专栏 - CSDN博客
2017年07月31日 15:36:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：772

**[java]**[view
 plain](http://blog.csdn.net/lw4135/article/details/52208786#)[copy](http://blog.csdn.net/lw4135/article/details/52208786#)
- //打印全部异常堆栈
- publicclass ExceptionUtils {  
- publicstaticvoid main(String[] args) {  
- try {  
- int a=1/0;  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-             String fullStackTrace = org.apache.commons.lang.exception.ExceptionUtils.getFullStackTrace(e);  
-             System.out.println(fullStackTrace);  
- 
-         }  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/lw4135/article/details/52208786#)[copy](http://blog.csdn.net/lw4135/article/details/52208786#)
- /**
-     * <p>A way to get the entire nested stack-trace of an throwable.</p>
-     *
-     * <p>The result of this method is highly dependent on the JDK version
-     * and whether the exceptions override printStackTrace or not.</p>
-     *
-     * @param throwable  the <code>Throwable</code> to be examined
-     * @return the nested stack trace, with the root cause first
-     * @since 2.0
-     */
- publicstatic String getFullStackTrace(Throwable throwable) {  
-        StringWriter sw = new StringWriter();  
-        PrintWriter pw = new PrintWriter(sw, true);  
-        Throwable[] ts = getThrowables(throwable);  
- for (int i = 0; i < ts.length; i++) {  
-            ts[i].printStackTrace(pw);  
- if (isNestedThrowable(ts[i])) {  
- break;  
-            }  
-        }  
- return sw.getBuffer().toString();  
-    }  
另一种方式：
**[java]**[view
 plain](http://blog.csdn.net/lw4135/article/details/52208786#)[copy](http://blog.csdn.net/lw4135/article/details/52208786#)
- publicstatic String exception2String(Exception ex){  
-         String exceptionMessage = "";  
- if (ex != null) {  
-             StringWriter sw = new StringWriter();  
-             PrintWriter pw = new PrintWriter(sw);  
- try {  
-                 ex.printStackTrace(pw);  
-                 exceptionMessage = sw.toString();  
-             } finally {  
- try {  
-                     sw.close();  
-                     pw.close();   
-                 } catch (Exception e) {  
-                 }  
-             }  
-         }  
- return exceptionMessage;  
-     }  
- 
