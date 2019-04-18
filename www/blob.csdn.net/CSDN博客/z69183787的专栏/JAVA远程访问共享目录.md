# JAVA远程访问共享目录 - z69183787的专栏 - CSDN博客
2013年07月10日 13:27:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1427标签：[smb](https://so.csdn.net/so/search/s.do?q=smb&t=blog)
个人分类：[Java-知识																[文件处理-Smb上传																[文件处理-Jcifs共享](https://blog.csdn.net/z69183787/article/category/2186129)](https://blog.csdn.net/z69183787/article/category/2186127)](https://blog.csdn.net/z69183787/article/category/2175279)
1 相关知识介绍
　　1.1 SMB
　　Microsoft网络配置中主要采用SMB形式实现文件共享和打印服务，SMB（[服务器](http://server.chinaitlab.com/)消息块）是一种客户端/[服务器](http://server.chinaitlab.com/)文件共享[协议](http://cisco.chinaitlab.com/List_11.html)。IBM于20世纪80年代末期开发了服务器信息块（SMB），用于规范共享网络资源（如目录、文件、打印机以及串行端口）的结构。这是一种请求/响应[协议](http://cisco.chinaitlab.com/List_11.html)。与FTP[协议](http://cisco.chinaitlab.com/List_11.html)支持的文件共享不同，SMB协议中的客户端要与服务器建立长期连接。一旦建立连接，客户端用户就可以访问服务器上的资源，就如同资源位于客户端主机上一样。
　　从[Windows](http://windows.chinaitlab.com/) 2000系列软件开始，Microsoft修改了软件的基础结构，使其适用SMB协议。而在以前的Microsoft产品中，SMB服务需要使用非TCP/IP协议族来执行域名解析。从[Windows](http://windows.chinaitlab.com/) 2000开始，Microsoft的所有产品都采用DNS系统。由此，TCP/IP协议族可以直接支持SMB资源共享。
　　SMB协议中规定了文件系统访问和客户如何请求文件的方式以及SMB协议进程间通信的方式。所有的SMB消息都采用一种格式。该格式采用固定大小的文件头，后跟可变 <script src="[http://hi.images.csdn.net/js/blog/tiny_mce/themes/advanced/langs/zh.js](http://hi.images.csdn.net/js/blog/tiny_mce/themes/advanced/langs/zh.js)"
 type="text/javascript"></script><script src="[http://hi.images.csdn.net/js/blog/tiny_mce/plugins/syntaxhl/langs/zh.js](http://hi.images.csdn.net/js/blog/tiny_mce/plugins/syntaxhl/langs/zh.js)"
 type="text/javascript"></script> 大小的参数以及数据组件。
　　1.2 jcifs
　　Jcifs <script src="[http://hi.images.csdn.net/js/blog/tiny_mce/themes/advanced/langs/zh.js](http://hi.images.csdn.net/js/blog/tiny_mce/themes/advanced/langs/zh.js)"
 type="text/javascript"></script><script src="[http://hi.images.csdn.net/js/blog/tiny_mce/plugins/syntaxhl/langs/zh.js](http://hi.images.csdn.net/js/blog/tiny_mce/plugins/syntaxhl/langs/zh.js)"
 type="text/javascript"></script> pan>是一个用JAVA开发的SMB客户端库，利用jcifs可以操作windows共享文件，可以得到域用户，实现单点登录，最新版本为：1.3.12，官方网址：[http://jcifs.samba.org/](http://jcifs.samba.org/)
　　2. 代码实现
　　package uploadSMB;
　　import java.io.BufferedInputStream;
　　import java.io.BufferedOutputStream;
　　import java.io.File;
　　import java.io.FileInputStream;
　　import java.io.FileOutputStream;
　　import java.io.IOException;
　　import java.io.InputStream;
　　import java.io.OutputStream;
　　import jcifs.smb.SmbFile;
　　import jcifs.smb.SmbFileInputStream;
　　import jcifs.smb.SmbFileOutputStream;
　　public class UploadDownloadUtil {
　　/**
　　* Description: 从共享目录拷贝文件到本地
　　* @Version1.0 Sep 25, 2009 3:48:38 PM
　　* @param remoteUrl 共享目录上的文件路径
　　* @param localDir 本地目录
　　*/
　　public void smbGet(String remoteUrl,String localDir) {
　　InputStream in = null;
　　OutputStream out = null;
　　try {
　　SmbFile remoteFile = new SmbFile(remoteUrl);
　　if(remoteFile==null){
　　System.out.println("共享文件不存在");
　　return;
　　}
　　String fileName = remoteFile.getName();
　　File localFile = new File(localDir+File.separator+fileName);
　　in = new BufferedInputStream(new SmbFileInputStream(remoteFile));
　　out = new BufferedOutputStream(new FileOutputStream(localFile));
　　byte[] buffer = new byte[1024];
　　while(in.read(buffer)!=-1){
　　out.write(buffer);
　　buffer = new byte[1024];
　　}
　} catch (Exception e) {
　　e.printStackTrace();
　　} finally {
　　try {
　　out.close();
　　in.close();
　　} catch (IOException e) {
　　e.printStackTrace();
　　}
　　}
　　}
　　/**
　　* Description: 从本地上传文件到共享目录
　　* @Version1.0 Sep 25, 2009 3:49:00 PM
　　* @param remoteUrl 共享文件目录
　　* @param localFilePath 本地文件绝对路径
　　*/
　　public void smbPut(String remoteUrl,String localFilePath) {
　　InputStream in = null;
　　OutputStream out = null;
　　try {
　　File localFile = new File(localFilePath);
　　String fileName = localFile.getName();
　　SmbFile remoteFile = new SmbFile(remoteUrl+"/"+fileName);
　　in = new BufferedInputStream(new FileInputStream(localFile));
　　out = new BufferedOutputStream(new SmbFileOutputStream(remoteFile));
　　byte[] buffer = new byte[1024];
　　while(in.read(buffer)!=-1){
　　out.write(buffer);
　　buffer = new byte[1024];
　　}
　　} catch (Exception e) {
　　e.printStackTrace();
　　} finally {
　　try {
　　out.close();
　　in.close();
　　} catch (IOException e) {
　　e.printStackTrace();
　　}
　　}
　　}
　　public static void main(String[] args){
　　UploadDownloadUtil test = new UploadDownloadUtil() ;
　　// smb:域名;用户名:密码@目的IP/文件夹/文件名.xxx
　　//test.smbGet("smb://szpcg;jiang.t:xxx@192.168.193.13/Jake/test.txt", "c://") ;
　　test.smbPut("smb://szpcg;jiang.t:xxx@192.168.193.13/Jake", "c://test.txt") ;
　　}
　　}
　　2.3 remoteUrl说明
　　remoteUrl如何填写是值得注意的
　　如果是无需密码的共享，则类似如下格式：
　　smb://ip/sharefolder（例如：smb://192.168.0.77/test）
　　如果需要用户名、密码，则类似如下格式：
　　Smb://username:password@ip/sharefolder（例如：smb://chb:123456@192.168.0.1/test）
　　// smb:域名;用户名:密码@目的IP/文件夹/文件名.xxx
　　//test.smbGet("smb://szpcg;jiang.t:xxx@192.168.193.13/Jake/test.txt", "c://") ;
　　test.smbPut("smb://szpcg;jiang.t:xxx@192.168.193.13/Jake", "c://test.txt") ;
