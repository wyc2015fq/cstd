# JSch - Java实现的SFTP（文件上传详解篇） - z69183787的专栏 - CSDN博客
2016年12月01日 11:58:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：945
转载：[http://www.cnblogs.com/longyg/archive/2012/06/25/2556576.html](http://www.cnblogs.com/longyg/archive/2012/06/25/2556576.html)
```
<dependency>
    <groupId>com.jcraft</groupId>
    <artifactId>jsch</artifactId>
    <version>0.1.53</version>
</dependency>
```
JSch是Java Secure Channel的缩写。JSch是一个SSH2的纯Java实现。它允许你连接到一个SSH服务器，并且可以使用端口转发，X11转发，文件传输等，当然你也可以集成它的功能到你自己的应用程序。
  本文只介绍如何使用JSch实现的SFTP功能。
  SFTP是Secure File Transfer Protocol的缩写，安全文件传送协议。可以为传输文件提供一种安全的加密方法。SFTP 为 SSH的一部份，是一种传输文件到服务器的安全方式。SFTP是使用加密传输认证信息和传输的数据，所以，使用SFTP是非常安全的。但是，由于这种传输方式使用了加密/解密技术，所以传输效率比普通的FTP要低得多，如果您对网络安全性要求更高时，可以使用SFTP代替FTP。（来自百度的解释）
  要使用JSch，需要下载它的jar包，请从官网下载它：[http://www.jcraft.com/jsch/](http://www.jcraft.com/jsch/)
ChannelSftp类是JSch实现SFTP核心类，它包含了所有SFTP的方法，如：
put()：      文件上传
get()：      文件下载
cd()：       进入指定目录
ls()：       得到指定目录下的文件列表
rename()：   重命名指定文件或目录
rm()：       删除指定文件
mkdir()：    创建目录
rmdir()：    删除目录
等等（这里省略了方法的参数，put和get都有多个重载方法，具体请看源代码，这里不一一列出。）
JSch支持三种文件传输模式：
|OVERWRITE|完全覆盖模式，这是JSch的默认文件传输模式，即如果目标文件已经存在，传输的文件将完全覆盖目标文件，产生新的文件。|
|----|----|
|RESUME|恢复模式，如果文件已经传输一部分，这时由于网络或其他任何原因导致文件传输中断，如果下一次传输相同的文件，则会从上一次中断的地方续传。|
|APPEND|追加模式，如果目标文件已存在，传输的文件将在目标文件后追加。|
创建ChannelSftp对象
编写一个工具类，根据ip，用户名及密码得到一个SFTP channel对象，即ChannelSftp的实例对象，在应用程序中就可以使用该对象来调用SFTP的各种操作方法。
SFTPChannel.java
```java
SFTPChannel.java
package com.longyg.sftp;
import java.util.Map;
import java.util.Properties;
import org.apache.log4j.Logger;
import com.jcraft.jsch.Channel;
import com.jcraft.jsch.ChannelSftp;
import com.jcraft.jsch.JSch;
import com.jcraft.jsch.JSchException;
import com.jcraft.jsch.Session;
public class SFTPChannel {
    Session session = null;
    Channel channel = null;
    private static final Logger LOG = Logger.getLogger(SFTPChannel.class.getName());
    public ChannelSftp getChannel(Map<String, String> sftpDetails, int timeout) throws JSchException {
        String ftpHost = sftpDetails.get(SFTPConstants.SFTP_REQ_HOST);
        String port = sftpDetails.get(SFTPConstants.SFTP_REQ_PORT);
        String ftpUserName = sftpDetails.get(SFTPConstants.SFTP_REQ_USERNAME);
        String ftpPassword = sftpDetails.get(SFTPConstants.SFTP_REQ_PASSWORD);
        int ftpPort = SFTPConstants.SFTP_DEFAULT_PORT;
        if (port != null && !port.equals("")) {
            ftpPort = Integer.valueOf(port);
        }
        JSch jsch = new JSch(); // 创建JSch对象
        session = jsch.getSession(ftpUserName, ftpHost, ftpPort); // 根据用户名，主机ip，端口获取一个Session对象
        LOG.debug("Session created.");
        if (ftpPassword != null) {
            session.setPassword(ftpPassword); // 设置密码
        }
        Properties config = new Properties();
        config.put("StrictHostKeyChecking", "no");
        session.setConfig(config); // 为Session对象设置properties
        session.setTimeout(timeout); // 设置timeout时间
        session.connect(); // 通过Session建立链接
        LOG.debug("Session connected.");
        LOG.debug("Opening Channel.");
        channel = session.openChannel("sftp"); // 打开SFTP通道
        channel.connect(); // 建立SFTP通道的连接
        LOG.debug("Connected successfully to ftpHost = " + ftpHost + ",as ftpUserName = " + ftpUserName
                + ", returning: " + channel);
        return (ChannelSftp) channel;
    }
    public void closeChannel() throws Exception {
        if (channel != null) {
            channel.disconnect();
        }
        if (session != null) {
            session.disconnect();
        }
    }
}
```
SFTPConstants是一个静态成员变量类：
SFTPConstans.java
```java
SFTPConstants.java
package com.longyg.sftp;
public class SFTPConstants {
    public static final String SFTP_REQ_HOST = "host";
    public static final String SFTP_REQ_PORT = "port";
    public static final String SFTP_REQ_USERNAME = "username";
    public static final String SFTP_REQ_PASSWORD = "password";
    public static final int SFTP_DEFAULT_PORT = 22;
    public static final String SFTP_REQ_LOC = "location";
}
```
文件上传
实现文件上传可以调用ChannelSftp对象的put方法。ChannelSftp中有12个put方法的重载方法：
|public void put(String src, String dst)|将本地文件名为src的文件上传到目标服务器，目标文件名为dst，若dst为目录，则目标文件名将与src文件名相同。采用默认的传输模式：OVERWRITE|
|----|----|
|public void put(String src, String dst, int mode)|将本地文件名为src的文件上传到目标服务器，目标文件名为dst，若dst为目录，则目标文件名将与src文件名相同。指定文件传输模式为mode（mode可选值为：ChannelSftp.OVERWRITE，ChannelSftp.RESUME，ChannelSftp.APPEND）|
|public void put(String src, String dst, SftpProgressMonitor monitor)|将本地文件名为src的文件上传到目标服务器，目标文件名为dst，若dst为目录，则目标文件名将与src文件名相同。采用默认的传输模式：OVERWRITE并使用实现了SftpProgressMonitor接口的monitor对象来监控文件传输的进度。|
|public void put(String src, String dst, SftpProgressMonitor monitor, int mode)|将本地文件名为src的文件上传到目标服务器，目标文件名为dst，若dst为目录，则目标文件名将与src文件名相同。指定传输模式为mode并使用实现了SftpProgressMonitor接口的monitor对象来监控文件传输的进度。|
|public void put(InputStream src, String dst)|将本地的input stream对象src上传到目标服务器，目标文件名为dst，dst不能为目录。采用默认的传输模式：OVERWRITE|
|public void put(InputStream src, String dst, int mode)|将本地的input stream对象src上传到目标服务器，目标文件名为dst，dst不能为目录。指定文件传输模式为mode|
|public void put(InputStream src, String dst, SftpProgressMonitor monitor)|将本地的input stream对象src上传到目标服务器，目标文件名为dst，dst不能为目录。采用默认的传输模式：OVERWRITE并使用实现了SftpProgressMonitor接口的monitor对象来监控传输的进度。|
|public void put(InputStream src, String dst, SftpProgressMonitor monitor, int mode)|将本地的input stream对象src上传到目标服务器，目标文件名为dst，dst不能为目录。指定文件传输模式为mode并使用实现了SftpProgressMonitor接口的monitor对象来监控传输的进度。|
|public OutputStream put(String dst)|该方法返回一个输出流，可以向该输出流中写入数据，最终将数据传输到目标服务器，目标文件名为dst，dst不能为目录。采用默认的传输模式：OVERWRITE|
|public OutputStream put(String dst, final int mode)|该方法返回一个输出流，可以向该输出流中写入数据，最终将数据传输到目标服务器，目标文件名为dst，dst不能为目录。指定文件传输模式为mode|
|public OutputStream put(String dst, final SftpProgressMonitor monitor, final int mode)|该方法返回一个输出流，可以向该输出流中写入数据，最终将数据传输到目标服务器，目标文件名为dst，dst不能为目录。指定文件传输模式为mode并使用实现了SftpProgressMonitor接口的monitor对象来监控传输的进度。|
|public OutputStream put(String dst, final SftpProgressMonitor monitor, final int mode, long offset)|该方法返回一个输出流，可以向该输出流中写入数据，最终将数据传输到目标服务器，目标文件名为dst，dst不能为目录。指定文件传输模式为mode并使用实现了SftpProgressMonitor接口的monitor对象来监控传输的进度。offset指定了一个偏移量，从输出流偏移offset开始写入数据。|
应用实例：
SFTPTest.java
```java
SFTPTest.java
package com.longyg.sftp;
import java.util.HashMap;
import java.util.Map;
import com.jcraft.jsch.ChannelSftp;
public class SFTPTest {
    public SFTPChannel getSFTPChannel() {
        return new SFTPChannel();
    }
    /**
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        SFTPTest test = new SFTPTest();
        Map<String, String> sftpDetails = new HashMap<String, String>();
        // 设置主机ip，端口，用户名，密码
        sftpDetails.put(SFTPConstants.SFTP_REQ_HOST, "10.9.167.55");
        sftpDetails.put(SFTPConstants.SFTP_REQ_USERNAME, "root");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PASSWORD, "arthur");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PORT, "22");
        
        String src = "D:\\DevSoft\\HB-SnagIt1001.rar"; // 本地文件名
        String dst = "/home/omc/ylong/sftp/HB-SnagIt1001.rar"; // 目标文件名
              
        SFTPChannel channel = test.getSFTPChannel();
        ChannelSftp chSftp = channel.getChannel(sftpDetails, 60000);
        
        /**
         * 代码段1
        OutputStream out = chSftp.put(dst, ChannelSftp.OVERWRITE); // 使用OVERWRITE模式
        byte[] buff = new byte[1024 * 256]; // 设定每次传输的数据块大小为256KB
        int read;
        if (out != null) {
            System.out.println("Start to read input stream");
            InputStream is = new FileInputStream(src);
            do {
                read = is.read(buff, 0, buff.length);
                if (read > 0) {
                    out.write(buff, 0, read);
                }
                out.flush();
            } while (read >= 0);
            System.out.println("input stream read done.");
        }
        **/
        
        chSftp.put(src, dst, ChannelSftp.OVERWRITE); // 代码段2
        
        // chSftp.put(new FileInputStream(src), dst, ChannelSftp.OVERWRITE); // 代码段3
        
        chSftp.quit();
        channel.closeChannel();
    }
}
```
注：请分别将代码段1，代码段2，代码段3取消注释，运行程序来进行测试。这三段代码分别演示了如何使用JSch的不同的put方法来进行文件上传。
代码段1：采用向put方法返回的输出流中写入数据的方式来传输文件。 需要由程序来决定写入什么样的数据，这里是将本地文件的输入流写入输出流。采用这种方式的好处是，可以自行设定每次写入输出流的数据块大小，如本示例中的语句：
```
byte[] buff = new byte[1024 * 256]; // 设定每次传输的数据块大小为256KB
```
代码段2：直接将本地文件名为src的文件上传到目标服务器，目标文件名为dst。（注：使用这个方法时，dst可以是目录，当dst是目录时，上传后的目标文件名将与src文件名相同）
代码段3：将本地文件名为src的文件输入流上传到目标服务器，目标文件名为dst。
这三段代码实现的功能是一样的，都是将本地的文件src上传到了服务器的dst文件。使用时可根据具体情况选择使用哪种实现方式。
监控传输进度
从前面的介绍中知道，JSch支持在文件传输时对传输进度的监控。可以实现JSch提供的SftpProgressMonitor接口来完成这个功能。
SftpProgressMonitor接口类的定义为：
```java
SftpProgressMonitor.java
package com.jcraft.jsch;
public interface SftpProgressMonitor{
  public static final int PUT=0;
  public static final int GET=1;
  void init(int op, String src, String dest, long max);
  boolean count(long count);
  void end();
}
```
init():    当文件开始传输时，调用init方法。
count():   当每次传输了一个数据块后，调用count方法，count方法的参数为这一次传输的数据块大小。
end():     当传输结束时，调用end方法。
下面是一个简单的实现：
MyProgressMonitor.java
```java
MyProgressMonitor.java
package com.longyg.sftp;
import com.jcraft.jsch.SftpProgressMonitor;
public class MyProgressMonitor implements SftpProgressMonitor {
    private long transfered;
    @Override
    public boolean count(long count) {
        transfered = transfered + count;
        System.out.println("Currently transferred total size: " + transfered + " bytes");
        return true;
    }
    @Override
    public void end() {
        System.out.println("Transferring done.");
    }
    @Override
    public void init(int op, String src, String dest, long max) {
        System.out.println("Transferring begin.");
    }
}
```
此时如果改变SFTPTest main方法里调用的put方法，即可实现监控传输进度：
SFTPTest.java
```java
SFTPTest.java
package com.longyg.sftp;
import java.util.HashMap;
import java.util.Map;
import com.jcraft.jsch.ChannelSftp;
public class SFTPTest {
    public SFTPChannel getSFTPChannel() {
        return new SFTPChannel();
    }
    /**
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        SFTPTest test = new SFTPTest();
        Map<String, String> sftpDetails = new HashMap<String, String>();
        // 设置主机ip，端口，用户名，密码
        sftpDetails.put(SFTPConstants.SFTP_REQ_HOST, "10.9.167.55");
        sftpDetails.put(SFTPConstants.SFTP_REQ_USERNAME, "root");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PASSWORD, "arthur");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PORT, "22");
        
        String src = "D:\\DevSoft\\HB-SnagIt1001.rar"; // 本地文件名
        String dst = "/home/omc/ylong/sftp/HB-SnagIt1001.rar"; // 目标文件名
              
        SFTPChannel channel = test.getSFTPChannel();
        ChannelSftp chSftp = channel.getChannel(sftpDetails, 60000);
        
        /**
         * 代码段1
        OutputStream out = chSftp.put(dst, new MyProgressMonitor(), ChannelSftp.OVERWRITE); // 使用OVERWRITE模式
        byte[] buff = new byte[1024 * 256]; // 设定每次传输的数据块大小为256KB
        int read;
        if (out != null) {
            System.out.println("Start to read input stream");
            InputStream is = new FileInputStream(src);
            do {
                read = is.read(buff, 0, buff.length);
                if (read > 0) {
                    out.write(buff, 0, read);
                }
                out.flush();
            } while (read >= 0);
            System.out.println("input stream read done.");
        }
        **/
        
        chSftp.put(src, dst, new MyProgressMonitor(), ChannelSftp.OVERWRITE); // 代码段2
        
        // chSftp.put(new FileInputStream(src), dst, new MyProgressMonitor(), ChannelSftp.OVERWRITE); // 代码段3
        
        chSftp.quit();
        channel.closeChannel();
    }
}
```
注意修改的内容仅仅是put方法，在put方法中增加了SftpProgressMonitor的实现类对象monitor作为参数，即添加了对进度监控的支持。
运行，输出结果如下：
```
logs
Start to read input stream
Currently transferred total size: 262144 bytes
Currently transferred total size: 524288 bytes
Currently transferred total size: 786432 bytes
Currently transferred total size: 1048576 bytes
Currently transferred total size: 1310720 bytes
Currently transferred total size: 1572864 bytes
Currently transferred total size: 1835008 bytes
Currently transferred total size: 2097152 bytes
Currently transferred total size: 2359296 bytes
Currently transferred total size: 2621440 bytes
Currently transferred total size: 2883584 bytes
Currently transferred total size: 3145728 bytes
Currently transferred total size: 3407872 bytes
Currently transferred total size: 3670016 bytes
Currently transferred total size: 3848374 bytes
input stream read done.
```
当然这个SftpProgressMonitor的实现实在太简单。JSch每次传输一个数据块，就会调用count方法来实现主动进度通知。
现在我们希望每间隔一定的时间才获取一下文件传输的进度。。。看看下面的SftpProgressMonitor实现：
```java
FileProgressMonitor.java
package com.longyg.sftp;
import java.text.DecimalFormat;
import java.util.Timer;
import java.util.TimerTask;
import com.jcraft.jsch.SftpProgressMonitor;
public class FileProgressMonitor extends TimerTask implements SftpProgressMonitor {
    
    private long progressInterval = 5 * 1000; // 默认间隔时间为5秒
    
    private boolean isEnd = false; // 记录传输是否结束
    
    private long transfered; // 记录已传输的数据总大小
    
    private long fileSize; // 记录文件总大小
    
    private Timer timer; // 定时器对象
    
    private boolean isScheduled = false; // 记录是否已启动timer记时器
    
    public FileProgressMonitor(long fileSize) {
        this.fileSize = fileSize;
    }
    
    @Override
    public void run() {
        if (!isEnd()) { // 判断传输是否已结束
            System.out.println("Transfering is in progress.");
            long transfered = getTransfered();
            if (transfered != fileSize) { // 判断当前已传输数据大小是否等于文件总大小
                System.out.println("Current transfered: " + transfered + " bytes");
                sendProgressMessage(transfered);
            } else {
                System.out.println("File transfering is done.");
                setEnd(true); // 如果当前已传输数据大小等于文件总大小，说明已完成，设置end
            }
        } else {
            System.out.println("Transfering done. Cancel timer.");
            stop(); // 如果传输结束，停止timer记时器
            return;
        }
    }
    
    public void stop() {
        System.out.println("Try to stop progress monitor.");
        if (timer != null) {
            timer.cancel();
            timer.purge();
            timer = null;
            isScheduled = false;
        }
        System.out.println("Progress monitor stoped.");
    }
    
    public void start() {
        System.out.println("Try to start progress monitor.");
        if (timer == null) {
            timer = new Timer();
        }
        timer.schedule(this, 1000, progressInterval);
        isScheduled = true;
        System.out.println("Progress monitor started.");
    }
    
    /**
     * 打印progress信息
     * @param transfered
     */
    private void sendProgressMessage(long transfered) {
        if (fileSize != 0) {
            double d = ((double)transfered * 100)/(double)fileSize;
            DecimalFormat df = new DecimalFormat( "#.##"); 
            System.out.println("Sending progress message: " + df.format(d) + "%");
        } else {
            System.out.println("Sending progress message: " + transfered);
        }
    }
    /**
     * 实现了SftpProgressMonitor接口的count方法
     */
    public boolean count(long count) {
        if (isEnd()) return false;
        if (!isScheduled) {
            start();
        }
        add(count);
        return true;
    }
    /**
     * 实现了SftpProgressMonitor接口的end方法
     */
    public void end() {
        setEnd(true);
        System.out.println("transfering end.");
    }
    
    private synchronized void add(long count) {
        transfered = transfered + count;
    }
    
    private synchronized long getTransfered() {
        return transfered;
    }
    
    public synchronized void setTransfered(long transfered) {
        this.transfered = transfered;
    }
    
    private synchronized void setEnd(boolean isEnd) {
        this.isEnd = isEnd;
    }
    
    private synchronized boolean isEnd() {
        return isEnd;
    }
    public void init(int op, String src, String dest, long max) {
        // Not used for putting InputStream
    }
}
```
再次修改SFTPTest main方法里的put方法，改为使用新的SftpProgressMonitor的实现类对象monitor作为参数，注意新的monitor对象的构造函数需要传入文件大小作为参数：
```java
SFTPTest.java
package com.longyg.sftp;
import java.io.File;
import java.util.HashMap;
import java.util.Map;
import com.jcraft.jsch.ChannelSftp;
public class SFTPTest {
    public SFTPChannel getSFTPChannel() {
        return new SFTPChannel();
    }
    /**
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) throws Exception {
        SFTPTest test = new SFTPTest();
        Map<String, String> sftpDetails = new HashMap<String, String>();
        // 设置主机ip，端口，用户名，密码
        sftpDetails.put(SFTPConstants.SFTP_REQ_HOST, "10.9.167.55");
        sftpDetails.put(SFTPConstants.SFTP_REQ_USERNAME, "root");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PASSWORD, "arthur");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PORT, "22");
        
        String src = "D:\\DevSoft\\HB-SnagIt1001.rar"; // 本地文件名
        String dst = "/home/omc/ylong/sftp/HB-SnagIt1001.rar"; // 目标文件名
              
        SFTPChannel channel = test.getSFTPChannel();
        ChannelSftp chSftp = channel.getChannel(sftpDetails, 60000);
        
        File file = new File(src);
        long fileSize = file.length();
        
        /**
         * 代码段1
        OutputStream out = chSftp.put(dst, new FileProgressMonitor(fileSize), ChannelSftp.OVERWRITE); // 使用OVERWRITE模式
        byte[] buff = new byte[1024 * 256]; // 设定每次传输的数据块大小为256KB
        int read;
        if (out != null) {
            System.out.println("Start to read input stream");
            InputStream is = new FileInputStream(src);
            do {
                read = is.read(buff, 0, buff.length);
                if (read > 0) {
                    out.write(buff, 0, read);
                }
                out.flush();
            } while (read >= 0);
            System.out.println("input stream read done.");
        }
        **/
        
        chSftp.put(src, dst, new FileProgressMonitor(fileSize), ChannelSftp.OVERWRITE); // 代码段2
        
        // chSftp.put(new FileInputStream(src), dst, new FileProgressMonitor(fileSize), ChannelSftp.OVERWRITE); // 代码段3
        
        chSftp.quit();
        channel.closeChannel();
    }
}
```
再次运行，结果输出为： 
```
logs
Try to start progress monitor.
Progress monitor started.
Transfering is in progress.
Current transfered: 98019 bytes
Sending progress message: 2.55%
Transfering is in progress.
Current transfered: 751479 bytes
Sending progress message: 19.53%
Transfering is in progress.
Current transfered: 1078209 bytes
Sending progress message: 28.02%
......
Transfering is in progress.
Current transfered: 3430665 bytes
Sending progress message: 89.15%
transfering end.
Transfering done. Cancel timer.
Try to stop progress monitor.
Progress monitor stoped.
```
现在，程序每隔5秒钟才会打印一下进度信息。可以修改FileProgressMonitor类里的progressInterval变量的值，来修改默认的间隔时间。
