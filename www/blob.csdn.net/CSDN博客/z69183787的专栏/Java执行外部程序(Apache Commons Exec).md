# Java执行外部程序(Apache Commons Exec) - z69183787的专栏 - CSDN博客
2018年05月02日 10:17:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：195
[https://blog.csdn.net/accountwcx/article/details/46787603](https://blog.csdn.net/accountwcx/article/details/46787603)
之前使用Runtime.getRuntime().exec调用外部程序，在Tomcat下会有当前线程一直等待的现象。当时为了解决这个问题，使用新建线程接收外部程序的输出信息，详情请看博客[http://blog.csdn.net/accountwcx/article/details/46785437](http://blog.csdn.net/accountwcx/article/details/46785437)。
后来在网上找到开源的Java调用外部程序类库Apache Commons Exce，这个类库提供非阻塞方法调用外部程序。
官方网址 [http://commons.apache.org/proper/commons-exec/](http://commons.apache.org/proper/commons-exec/)
maven地址 [http://mvnrepository.com/artifact/org.apache.commons/commons-exec/1.3](http://mvnrepository.com/artifact/org.apache.commons/commons-exec/1.3)
官方教程 [http://commons.apache.org/proper/commons-exec/tutorial.html](http://commons.apache.org/proper/commons-exec/tutorial.html) 官方教程提供的非阻塞方法在1.3版中不适用
Commons Exec对调用外部程序进行了封装，只需要少量代码即可实现外部程序调用，如执行命令"AcroRd32.exe /p /h c:\help.pdf"。
[java][view plain](https://blog.csdn.net/accountwcx/article/details/46787603#)[copy](https://blog.csdn.net/accountwcx/article/details/46787603#)
- String line = "AcroRd32.exe /p /h c:\help.pdf";  
- CommandLine cmdLine = CommandLine.parse(line);  
- DefaultExecutor executor = new DefaultExecutor();  
- 
- //设置命令执行退出值为1，如果命令成功执行并且没有错误，则返回1
- executor.setExitValue(1);  
- 
- int exitValue = executor.execute(cmdLine);  
Commons Exec支持通过添加参数方式构建命令，执行命令"AcroRd32.exe /p /h c:\help.pdf"也可以按如下方法创建。
[java][view plain](https://blog.csdn.net/accountwcx/article/details/46787603#)[copy](https://blog.csdn.net/accountwcx/article/details/46787603#)
- CommandLine cmdLine = new CommandLine("AcroRd32.exe");  
- cmdLine.addArgument("/p");  
- cmdLine.addArgument("/h");  
- 
- Map map = new HashMap();  
- map.put("file", new File("c:\help.pdf"));  
- cmdLine.addArgument("${file}");  
- cmdLine.setSubstitutionMap(map);  
- 
- DefaultExecutor executor = new DefaultExecutor();  
- executor.setExitValue(1);  
- int exitValue = executor.execute(cmdLine);  
Commons Exec支持设置外部命令执行等待时间，如果超过等等时间则中断执行。
[java][view plain](https://blog.csdn.net/accountwcx/article/details/46787603#)[copy](https://blog.csdn.net/accountwcx/article/details/46787603#)
- CommandLine cmdLine = new CommandLine("AcroRd32.exe");  
- cmdLine.addArgument("/p");  
- cmdLine.addArgument("/h");  
- 
- Map map = new HashMap();  
- map.put("file", new File("c:\help.pdf"));  
- cmdLine.addArgument("${file}");  
- cmdLine.setSubstitutionMap(map);  
- 
- DefaultExecutor executor = new DefaultExecutor();  
- 
- //创建监控时间60秒，超过60秒则中端执行
- ExecuteWatchdog watchdog = new ExecuteWatchdog(60*1000);  
- executor.setWatchdog(watchdog);  
- 
- executor.setExitValue(1);  
- int exitValue = executor.execute(cmdLine);  
上面的执行外部命令都是阻塞式，也就是在执行外部命令时，当前线程是阻塞的。如果不想在执行外部命令的时候，把当前线程阻塞，可以使用DefaultExecuteResultHandler处理外部命令执行的结果，释放当前线程。
[java][view plain](https://blog.csdn.net/accountwcx/article/details/46787603#)[copy](https://blog.csdn.net/accountwcx/article/details/46787603#)
- CommandLine cmdLine = new CommandLine("AcroRd32.exe");  
- cmdLine.addArgument("/p");  
- cmdLine.addArgument("/h");  
- 
- Map map = new HashMap();  
- map.put("file", new File("c:\help.pdf"));  
- cmdLine.addArgument("${file}");  
- cmdLine.setSubstitutionMap(map);  
- 
- DefaultExecuteResultHandler resultHandler = new DefaultExecuteResultHandler();  
- DefaultExecutor executor = new DefaultExecutor();  
- executor.setExitValue(1);  
- executor.execute(cmdLine, resultHandler);  
- resultHandler.waitFor();  
博客[http://blog.csdn.net/accountwcx/article/details/46785437](http://blog.csdn.net/accountwcx/article/details/46785437)的HtmlToPdf类可以改成如下。
[java][view plain](https://blog.csdn.net/accountwcx/article/details/46787603#)[copy](https://blog.csdn.net/accountwcx/article/details/46787603#)
- import java.io.File;  
- 
- import org.apache.commons.exec.CommandLine;  
- import org.apache.commons.exec.DefaultExecuteResultHandler;  
- import org.apache.commons.exec.DefaultExecutor;  
- 
- publicclass HtmlToPdf {  
- //wkhtmltopdf在系统中的路径
- privatestaticfinal String toPdfTool = "c:\\wkhtmltopdf.exe";  
- 
- /**
-      * @param srcPath html路径，可以本地硬盘路径或者url
-      * @param destPath pdf保存路径
-      * @return 转换成功返回true
-      */
- publicstaticboolean convert(String srcPath, String destPath){       
-         File file = new File(destPath);  
-         File parent = file.getParentFile();  
- //如果pdf保存路径不存在，则创建路径
- if(!parent.exists()){  
-             parent.mkdirs();  
-         }  
- 
-         CommandLine cmdLine = new CommandLine(toPdfTool);  
-         cmdLine.addArgument(srcPath, true);  
-         cmdLine.addArgument(destPath, true);  
- 
-         DefaultExecutor executor = new DefaultExecutor();  
- 
- //设置执行命令成功的退出值为1
-         executor.setExitValue(1);  
- 
- //非阻塞
-         DefaultExecuteResultHandler resultHandler = new DefaultExecuteResultHandler();  
- 
- boolean result = true;  
- try {  
-             executor.execute(cmdLine, resultHandler);  
-             resultHandler.waitFor();  
-         } catch (Exception e) {  
-             result = false;  
-             e.printStackTrace();  
-         }  
- 
- return result;  
-     }  
- }  
