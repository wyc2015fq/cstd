# Jetty8不能編譯JSP，報"There is an error in invoking javac. A full JDK (not just JRE) is required"錯誤 - z69183787的专栏 - CSDN博客
2014年03月16日 11:47:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2940
編譯JSP時報“org.apache.jasper.JasperException: PWC6345: There is an error in invoking javac.  A full JDK (not just JRE) is required”錯誤。
Jetty啟動參數加-Dorg.apache.jasper.compiler.disablejsr199=true
如果命令行啟動Jetty的話，把start.ini中的“-Dorg.apache.jasper.compiler.disablejsr199=true”解開註釋。
解開“--exec”的註釋，解決一個Warning。
