# Jsch初步 Java SFTP协议 实现 - z69183787的专栏 - CSDN博客
2016年12月01日 11:52:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1801
1.去[官网](http://www.jcraft.com/jsch/)下载最新的jar包 
jsch-0.1.51.jar 
2.运行[官方例子](http://www.jcraft.com/jsch/examples/)
Shell.java 
演示了如何连接到sshd服务器 
Exec.java 
演示了登陆上去并执行一条命令 
注意以上2个例子让你输入密码同样也是输入cyg_server用户的密码 
运行成功后可以看下它的代码。 
3.再做一个SFTP试验 
Java代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- import java.io.*;  
- import java.util.*;  
- import com.jcraft.jsch.*;  
- 
- publicclass Test1 {  
- 
- publicstaticvoid main(String[] args) throws Exception {  
-         Test1.sshSftp("127.0.0.1", "Administrator", "cyg_server", 22);  
-         Test1.sshSftp2("127.0.0.1", "Administrator", 22, "C:/Users/Administrator/rsa_my", "");  
-     }  
- 
- /**
-      * 利用JSch包实现SFTP下载、上传文件(用户名密码方式登陆)
-      * @param ip 主机IP
-      * @param user 主机登陆用户名
-      * @param psw  主机登陆密码
-      * @param port 主机ssh2登陆端口，如果取默认值(默认值22)，传-1
-      * 
-      */
- publicstaticvoid sshSftp(String ip, String user, String psw   
-             ,int port) throws Exception{  
-         System.out.println("开始用户名密码方式登陆");  
-         Session session = null;  
- 
-         JSch jsch = new JSch();  
- 
- if(port <=0){  
- //连接服务器，采用默认端口
-             session = jsch.getSession(user, ip);  
-         }else{  
- //采用指定的端口连接服务器
-             session = jsch.getSession(user, ip ,port);  
-         }  
- 
- //如果服务器连接不上，则抛出异常
- if (session == null) {  
- thrownew Exception("session is null");  
-         }  
- 
- //设置登陆主机的密码
-         session.setPassword(psw);//设置密码   
- //设置第一次登陆的时候提示，可选值：(ask | yes | no)
-         session.setConfig("StrictHostKeyChecking", "no");  
- //设置登陆超时时间   
-         session.connect(30000);  
- 
-         sftp(session, "aa.log");  
-         System.out.println("sftp成功");  
-     }  
- 
- /**
-      * 利用JSch包实现SFTP下载、上传文件(秘钥方式登陆)
-      * @param ip 主机IP
-      * @param user 主机登陆用户名
-      * @param port 主机ssh2登陆端口，如果取默认值(默认值22)，传-1
-      * @param privateKey 密钥文件路径
-      * @param passphrase 密钥的密码
-      * 
-      */
- publicstaticvoid sshSftp2(String ip, String user  
-             ,int port ,String privateKey ,String passphrase) throws Exception{  
-         System.out.println("开始秘钥方式登陆");  
-         Session session = null;  
- 
-         JSch jsch = new JSch();  
- 
- //设置密钥和密码
- //支持密钥的方式登陆，只需在jsch.getSession之前设置一下密钥的相关信息就可以了
- if (privateKey != null && !"".equals(privateKey)) {  
- if (passphrase != null && "".equals(passphrase)) {  
- //设置带口令的密钥
-                    jsch.addIdentity(privateKey, passphrase);  
-                } else {  
- //设置不带口令的密钥
-                    jsch.addIdentity(privateKey);  
-                }  
-         }  
- 
- 
- if(port <=0){  
- //连接服务器，采用默认端口
-             session = jsch.getSession(user, ip);  
-         }else{  
- //采用指定的端口连接服务器
-             session = jsch.getSession(user, ip ,port);  
-         }  
- 
- //如果服务器连接不上，则抛出异常
- if (session == null) {  
- thrownew Exception("session is null");  
-         }  
- 
- //设置第一次登陆的时候提示，可选值：(ask | yes | no)
-         session.setConfig("StrictHostKeyChecking", "no");  
- //设置登陆超时时间
-         session.connect(30000);  
- 
-         sftp(session, "bb.log");  
-         System.out.println("sftp成功");  
-     }  
- 
- privatestaticvoid sftp(Session session, String uploadFileName) throws Exception {  
-         Channel channel = null;  
- try {  
- //创建sftp通信通道
-             channel = (Channel) session.openChannel("sftp");  
-             channel.connect(1000);  
-             ChannelSftp sftp = (ChannelSftp) channel;  
- 
- 
- //进入服务器指定的文件夹
-             sftp.cd("testsftp");  
- 
- //列出服务器指定的文件列表
-             Vector v = sftp.ls("*.txt");  
- for(int i=0;i<v.size();i++){  
-                 System.out.println(v.get(i));  
-             }  
- 
- //以下代码实现从本地上传一个文件到服务器，如果要实现下载，对换以下流就可以了
-             OutputStream outstream = sftp.put(uploadFileName);  
-             InputStream instream = new FileInputStream(new File("C:/ftptest/aa.txt"));  
- 
- byte b[] = newbyte[1024];  
- int n;  
- while ((n = instream.read(b)) != -1) {  
-                 outstream.write(b, 0, n);  
-             }  
- 
-             outstream.flush();  
-             outstream.close();  
-             instream.close();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         } finally {  
-             session.disconnect();  
-             channel.disconnect();  
-         }  
-     }  
- }  
代码是引用了[http://my.oschina.net/hetiangui/blog/137357](http://my.oschina.net/hetiangui/blog/137357)
首先这句sftp.ls("*.txt")是列出指定文件夹下所有txt文件， 
然后sftp.put("3.txt")是上传一个文件到SFTP服务器上。 
注意main函数里有2种登陆方式， 
第一种是通过用户名Administrator，密码cyg_server的方式登陆。 
第二种是通过秘钥方式登陆，我们的私钥目录是"C:/Users/Administrator/rsa_my",而passphrase则为空。 
