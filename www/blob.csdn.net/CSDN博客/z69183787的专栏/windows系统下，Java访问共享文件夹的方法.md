# windows系统下，Java访问共享文件夹的方法 - z69183787的专栏 - CSDN博客
2013年07月09日 22:00:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1239
http://hi.baidu.com/iduany/item/911685dc6f978911e1f46f72
## 如何通过windows服务访问网络资源(通过jcifs实现java访问网络共享文件)
问题：
1，windows本地服务localsystem帐户默认对本地操作系统具有最高操作权限，但是却不能对网络资源访问。   
2，经过测试发现用windows服务不能访问网络共享文件夹，同样的程序在windows应用程序及控制台程序中运行却可以。
注：通过共享文件夹 能够访问
考虑方法：
方法一：一般service服务默认的启动身份是local SYSTEM，把自定义服务的属性修改一下，把log on改成用管理员帐户登入（在windows服务列表中点该服务的属性）
试过还是不行.
方法二：现在是通过windows的网络映射驱动器，把该共享文件映射到本地为“I盘“，java代码如下：
    File file = new File("I:/Share");
    file.exists();
无法访问，报文件不存在。
方法三：就是下面要讲述的基于SMB协议，现在又叫CIFS协议的Java开源项目JCIFS。
SMB（Server Message Block）:通信协议是微软（Microsoft）和英特尔(Intel)在1987年制定的协议，主要是作为Microsoft网络的通讯协议。SMB 是在会话层（session layer）和表示层（presentation layer）以及小部分应用层（application layer）的协议。SMB使用了NetBIOS的应用程序接口 （Application Program Interface，简称API）。另外，它是一个开放性的协议，允许了协议扩展——使得它变得更大而且复杂；大约有65个最上层的作业，而每个作业都超过120个函数，甚至Windows
 NT也没有全部支持到，最近微软又把 SMB 改名为 CIFS（Common Internet ile System），并且加入了许多新的特色。　　
SMB协议是基于TCP－NETBIOS下的，一般端口使用为139，445
CIFS(Common Internet File System)：通用Internet文件系统 
　在windows主机之间进行网络文件共享是通过使用微软公司自己的CIFS服务实现的。　 
　　CIFS 是一个新提出的协议，它使程序可以访问远程Internet计算机上的文件并要求此计算机的服务。CIFS 使用客户/服务器模式。客户程序请求远在服务器上的服务器程序为它提供服务。服务器获得请求并返回响应。CIFS是公共的或开放的SMB协议版本，并由Microsoft使用。SMB协议现在是局域网上用于服务器文件访问和打印的协议。象SMB协议一样，CIFS在高层运行，而不象TCP/IP协议那样运行在底层。CIFS可以看做是应用程序协议如文件传输协议和超文本传输协议的一个实现。
　　CIFS 可以使您达到以下功能： 
　　1.访问服务器本地文件并读写这些文件 
　　2.与其它用户一起共享一些文件块 
　　3.在断线时自动恢复与网络的连接 
　　4.使用西欧字符文件名 
　　一般来说，CIFS使用户得到比FTP更好的对文件的控制。它提供潜在的更直接地服务器程序接口，这比使用HTTP协议的浏览器更好。CIFS最典型的应用是windows用户能够从“网上邻居”中找到网络中的其他主机并访问其中的共享文件夹. 
CIFS 是开放的标准而且已经被作为Internet应用程序标准被提交到IETF。
开源项目—JCIFS
JCIFS是CIFS 在JAVA中的一个实现，是samba组织负责维护开发的一个开源项目,专注于使用java语言对cifs协议的设计和实现。他们将jcifs设计成为一个完整的，丰富的，具有可扩展能力且线程安全的客户端库。这一库可以应用于各种java虚拟机访问遵循CIFS/SMB网络传输协议的网络资源。类似于java.io.File的接口形式，在多线程的工作方式下被证明是有效而容易使用的。 
　　JCIFS的开发方法类似java的文件操作功能，它的资源url定位：smb://{user}:{password}@{host}/{path}，smb为协议名，user和password分别为共享文件机子的登陆名和密码，@后面是要访问的资源的主机名或IP地址。最后是资源的共享文件夹名称和共享资源名。例如 smb://administrator:122122@192.168.0.22/test/response.txt。 
　　在JAVA程序中，使用如下方式获得一个远程共享文件的句柄：SmbFile file = new SmbFile("smb://guest:1234@192.168.3.56/share/a.txt");这里的句柄不仅限于远程的共享文件，还可能是共享文件夹。isFile()方法和isDirectory()用来判断这个句柄对应的资源的真实属性。如果是共享文件夹，通过调用它的list()方法将获得其中资源的列表。List方法支持过滤器机制，有两种过滤器可供使用，一种是SmbFileFilter,另一种是SmbFilenameFilter，这两个在jcifs中作为接口出现，你可以根据自己的需要派生出个性化的过滤器，实现接口中的accept方法，以满足不同业务的需求。
参考网站：[http://jcifs.samba.org/](http://jcifs.samba.org/)
JAVA中的CIFS实现
　Jcifs是CIFS在JAVA中的一个实现，是samba组织本着linux的精神，负责维护开发的一个开源项目。这个项目专注于使用java语言对cifs协议的设计和实现。他们将jcifs设计成为一个完整的，丰富的，具有可扩展能力且线程安全的客户端库。这一库可以应用于各种java虚拟机访问遵循CIFS/SMB网络传输协议的网络资源。类似于java.io.File的接口形式，在多线程的工作方式下被证实是有效而轻易使用的。
简单示例：
import jcifs.smb.SmbFile;
import jcifs.smb.SmbFileInputStream;
public class RemoteAccessData {
public static void main(String[] args) {
    try {
      SmbFile smbFile = new SmbFile("smb://guest:1234@192.168.3.56/share/a.txt");
      int length = smbFile.getContentLength();//得到文件的大小
      byte buffer[] = new byte[length];
      SmbFileInputStream in = new SmbFileInputStream(smbFile); //建立smb文件输入流
      while ((in.read(buffer)) != -1) {
        System.out.write(buffer);
        System.out.println(buffer.length);
      }
      in.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
}
}
remoteUrl说明:
如果是无需密码的共享，则类似如下格式：
   smb://ip/sharefolder（例如：smb://192.168.0.77/test）
如果需要用户名、密码，则类似如下格式：
   Smb://username:password@ip/sharefolder（例如：smb://chb:123456@192.168.0.1/test）
注：jcifs.smb包下的很多方法，如SmbFile，SmbFileInputStream，SmbRandomAccessFile等等很多类，基本上和java.io包下的文件读写类
基本相同的，大体上在jdk的io类前面加上了它的标识“Smb”。
