# java获取当前操作系统的信息 - z69183787的专栏 - CSDN博客
2014年07月15日 14:04:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8147
从网上收集的一些关于java获取操作系统信息的方法，现在总结一下：
1获取本机的ＩＰ地址：
Java代码  ![收藏代码](http://chenhua-1984.iteye.com/images/icon_star.png)
- privatestatic String getIpAddress() throws UnknownHostException {  
-         InetAddress address = InetAddress.getLocalHost();  
- 
- return address.getHostAddress();  
-     }  
2获得网卡地址 （http://vcsos.com/Article/pageSource/120207/20120207104300_1.shtml）
Java代码  ![收藏代码](http://chenhua-1984.iteye.com/images/icon_star.png)
- publicstatic String getMACAddress(){  
- 
-         String address = "";  
- 
-         String os = System.getProperty("os.name");  
-         String osUser=System.getProperty("user.name");  
- if (os != null && os.startsWith("Windows")) {  
- 
- try {  
- 
-                 String command = "cmd.exe /c ipconfig /all";  
- 
-                 Process p = Runtime.getRuntime().exec(command);  
- 
-                 BufferedReader br =new BufferedReader(new InputStreamReader(p.getInputStream()));  
- 
-                 String line;  
- 
- while ((line = br.readLine()) != null) {  
- 
- if (line.indexOf("Physical Address") > 0) {  
- 
- int index = line.indexOf(":");  
- 
-                         index += 2;  
- 
-                         address = line.substring(index);  
- 
- break;  
- 
-                     }  
- 
-                 }  
- 
-                 br.close();  
- 
- return address.trim();  
- 
-             }  
- 
- catch (IOException e) {  
-             }  
- 
-         }  
- return address;  
- 
-     }  
 3获得操作系统帐号
Java代码  ![收藏代码](http://chenhua-1984.iteye.com/images/icon_star.png)
- String osUser=System.getProperty("user.name");  
 ４获得操作系统版本
Java代码  ![收藏代码](http://chenhua-1984.iteye.com/images/icon_star.png)
- 1. import java.util.Properties;    
- 2.     
- 3. Properties props=System.getProperties(); //获得系统属性集  
- 4. String osName = props.getProperty("os.name"); //操作系统名称  
- 5. String osArch = props.getProperty("os.arch"); //操作系统构架  
- 6. String osVersion = props.getProperty("os.version"); //操作系统版本  
 ５一些常用的信息获得
Java代码  ![收藏代码](http://chenhua-1984.iteye.com/images/icon_star.png)
- publicstatic String getProperty(String key)   
- 键   相关值的描述  
- java.version    Java 运行时环境版本  
- java.vendor     Java 运行时环境供应商  
- java.vendor.url     Java 供应商的 URL  
- java.home   Java 安装目录  
- java.vm.specification.version   Java 虚拟机规范版本  
- java.vm.specification.vendor    Java 虚拟机规范供应商  
- java.vm.specification.name  Java 虚拟机规范名称  
- java.vm.version     Java 虚拟机实现版本  
- java.vm.vendor  Java 虚拟机实现供应商  
- java.vm.name    Java 虚拟机实现名称  
- java.specification.version  Java 运行时环境规范版本  
- java.specification.vendor   Java 运行时环境规范供应商  
- java.specification.name     Java 运行时环境规范名称  
- java.class.version  Java 类格式版本号  
- java.class.path     Java 类路径  
- java.library.path   加载库时搜索的路径列表  
- java.io.tmpdir  默认的临时文件路径  
- java.compiler   要使用的 JIT 编译器的名称  
- java.ext.dirs   一个或多个扩展目录的路径  
- os.name     操作系统的名称  
- os.arch     操作系统的架构  
- os.version  操作系统的版本  
- file.separator  文件分隔符（在 UNIX 系统中是“/”）  
- path.separator  路径分隔符（在 UNIX 系统中是“:”）  
- line.separator  行分隔符（在 UNIX 系统中是“/n”）  
- user.name   用户的账户名称  
- user.home   用户的主目录  
- user.dir    用户的当前工作目录  
