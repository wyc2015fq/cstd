# Java学些笔记41：FTP4j - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年05月23日 13:31:38[initphp](https://me.csdn.net/initphp)阅读数：1302
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









                
ftp4j是一个FTP客户端Java类库，实现了FTP客户端应具有的大部分功能。可以将ftp4j嵌到你的Java应用中，来传输文件（包括上传和下载），浏览远程FTP服务器上的目录和文件，创建、删除、重命，移动远程目录和文件。ftp4j提供多种方式连接到远程FTP服务器包括：通过 TCP/IP直接连接，通过FTP代理、HTTP代理、SOCKS4/4a代理和SOCKS5代理连接，通过SSL安全连接。


简单应用---API介绍


The main class of the library is FTPClient (it.sauronsoftware.ftp4j.FTPClient).


1. 创建FTPClient实例

FTPClient client = new FTPClient();


连接到指定的FTP服务器(域名或IP) 不指定端口，则使用默认端口21

client.connect("ftp.host.com", /*21*/);


登录验证

client.login("user", "pswd");


下面是匿名登录

//client.login("anonymous", "密码任意设置");

client.login("anonymous", "ftp4j");


安全退出

client.disconnect(true);

//强制退出

//client.disconnect(false);


文件以及文件夹操作:


取得当前文件夹

String dir = client.currentDirectory();


改变当前文件夹

client.changeDirectory(newPath);


//绝对路径

//client.changeDirectory("/an/absolute/one");

//相对路径

//client.changeDirectory("relative");


//回退到上级目录

client.changeDirectoryUp();


//重命名文件或文件夹

client.rename("oldname", "newname");


//移动文件或文件夹

client.rename("myfile.txt", "myfolder/myfile.txt");


//删除文件

client.deleteFile(relativeOrAbsolutePath);

//client.deleteFile("useless.txt");


//创建目录

client.createDirectory("newfolder");


//删除目录(空目录)

client.deleteDirectory(absoluteOrRelativePath);

//client.deleteDirectory("oldfolder");


//浏览文件

FTPFile[] list = client.list();


//使用通配浏览文件

FTPFile[] list = client.list("*.jpg");


//显示文件或文件夹的修改时间

java.util.Date md = client.modifiedDate("filename.ext");



//上传和下载文件

//下载服务器上remoteFile.ext 下载到本地 localFile.ext

client.download("remoteFile.ext", new java.io.File("localFile.ext"));


//上传 本地 localFile.ext 到当前目录

client.upload(new java.io.File("localFile.ext"));


// 上传和下载文件时， 监听文件传输的状态

public class MyTransferListener implements FTPDataTransferListener {


    //文件开始上传或下载时触发

    public void started() {

    }

    //显示已经传输的字节数

    public void transferred(int length) {

    }

    //文件传输完成时，触发

    public void completed() {

    }

    //传输放弃时触发

    public void aborted() {

        // Transfer aborted

    }

    //传输失败时触发

    public void failed() {

    }


示例:

client.download("remoteFile.ext", new java.io.File("localFile.ext"), new MyTransferListener());

client.upload(new java.io.File("localFile.ext"), new MyTransferListener());


//ftp4j也支持断点续传功能

下面是一个简单示例：*参数 1056 跳过 1056字节点下载

client.download("remoteFile.ext", new java.io.File("localFile.ext"), 1056);



设置传输模式

//ASC码

client.setType(FTPClient.TYPE_TEXTUAL);

//二进制

client.setType(FTPClient.TYPE_BINARY);

//自动选择(根据文件内容)

client.setType(FTPClient.TYPE_AUTO);


//设置连接器

client.setConnector(connector);

SSL 套接字连接

client.setConnector(*it.sauronsoftware.ftp4j.connectors.SSLConnector)*client.setConnector(anyConnectorYouWant);


ftp4j 官网: [http://www.sauronsoftware.it/projects/ftp4j/](http://www.sauronsoftware.it/projects/ftp4j/)


Good Luck!

Yours Matthew!
            


