# JSch - Java实现的SFTP（文件下载详解篇） - z69183787的专栏 - CSDN博客
2016年12月01日 12:00:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4070
转载：[http://www.cnblogs.com/longyg/archive/2012/06/25/2561332.html](http://www.cnblogs.com/longyg/archive/2012/06/25/2561332.html)
上一篇讲述了使用JSch实现文件上传的功能，这一篇主要讲述一下JSch实现文件下载的功能。并介绍一些SFTP的辅助方法，如cd，ls等。
同样，JSch的文件下载也支持三种传输模式：OVERWRITE, RESUME和APPEND，请参考上篇随笔：[JSch
 - Java实现的SFTP（文件上传详解篇）](http://www.cnblogs.com/longyg/archive/2012/06/25/2556576.html)
文件下载
JSch文件下载是通过调用ChannelSftp对象的get方法来实现的。ChannelSftp中有9个get方法的重载方法：
|publicvoid get(String src, String dst)|将目标服务器上文件名为src的文件下载到本地，本地文件名为dst。若dst为目录，则下载到本地的文件名将与src文件名相同。（注：src必须是文件，不能为目录）采用默认的传输模式：OVERWRITE|
|----|----|
|publicvoid get(String src, String dst, SftpProgressMonitor monitor)|将目标服务器上文件名为src的文件下载到本地，本地文件名为dst。若dst为目录，则下载到本地的文件名将与src文件名相同。（注：src必须是文件，不能为目录）采用默认的传输模式：OVERWRITE并使用实现了SftpProgressMonitor接口的monitor对象来监控文件的传输进度。|
|publicvoid get(String src, String dst,SftpProgressMonitor monitor, int mode)|将目标服务器上文件名为src的文件下载到本地，本地文件名为dst。若dst为目录，则下载到本地的文件名将与src文件名相同。（注：src必须是文件，不能为目录）指定文件传输模式为mode（mode可选值为：ChannelSftp.OVERWRITE，ChannelSftp.RESUME，ChannelSftp.APPEND）并使用实现了SftpProgressMonitor接口的monitor对象来监控文件的传输进度。|
|publicvoid get(String src, OutputStream dst)|将目标服务器上文件名为src的文件下载到本地，下载的数据写入到输出流对象dst（如：文件输出流）。采用默认的传输模式：OVERWRITE|
|publicvoid get(String src, OutputStream dst, SftpProgressMonitor monitor)|将目标服务器上文件名为src的文件下载到本地，下载的数据写入到输出流对象dst（如：文件输出流）。采用默认的传输模式：OVERWRITE并使用实现了SftpProgressMonitor接口的monitor对象来监控文件的传输进度。|
|publicvoid get(String src, OutputStream dst, SftpProgressMonitor monitor, int mode, long skip)|将目标服务器上文件名为src的文件下载到本地，下载的数据写入到输出流对象dst（如：文件输出流）。指定文件传输模式为mode并使用实现了SftpProgressMonitor接口的monitor对象来监控文件的传输进度。skip指定了一个跳读量，即下载时从src文件跳过skip字节的数据。（一般不推荐使用该参数，默认设为0）|
|public InputStream get(String src)|该方法返回一个输入流，该输入流含有目标服务器上文件名为src的文件数据。可以从该输入流中读取数据，最终将数据传输到本地（如：读取数据后将数据写入到本地的文件中）（注：该方法不支持多种文件传输模式，如何读取与保存数据由应用程序自己确定）|
|public InputStream get(String src, SftpProgressMonitor monitor)|该方法返回一个输入流，该输入流含有目标服务器上文件名为src的文件数据。可以从该输入流中读取数据，最终将数据传输到本地（如：读取数据后将数据写入到本地的文件中）并使用实现了SftpProgressMonitor接口的monitor对象来监控文件的传输进度。（注：该方法不支持多种文件传输模式，如何读取与保存数据由应用程序自己确定）|
|public InputStream get(String src, final SftpProgressMonitor monitor, finallong skip)|该方法返回一个输入流，该输入流含有目标服务器上文件名为src的文件数据。可以从该输入流中读取数据，最终将数据传输到本地（如：读取数据后将数据写入到本地的文件中）并使用实现了SftpProgressMonitor接口的monitor对象来监控文件的传输进度。（注：该方法不支持多种文件传输模式，如何读取与保存数据由应用程序自己确定）skip指定了一个跳读量，即下载时从src文件跳过skip字节的数据。（一般不推荐使用该参数，默认设为0）|
应用实例：
```java
SFTPGetTest.java
package com.longyg.sftp;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map;
import com.jcraft.jsch.ChannelSftp;
import com.jcraft.jsch.SftpATTRS;
public class SFTPGetTest {
    public SFTPChannel getSFTPChannel() {
        return new SFTPChannel();
    }
    public static void main(String[] args) throws Exception {
        SFTPGetTest test = new SFTPGetTest();
        Map<String, String> sftpDetails = new HashMap<String, String>();
        // 设置主机ip，端口，用户名，密码
        sftpDetails.put(SFTPConstants.SFTP_REQ_HOST, "10.9.167.55");
        sftpDetails.put(SFTPConstants.SFTP_REQ_USERNAME, "root");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PASSWORD, "arthur");
        sftpDetails.put(SFTPConstants.SFTP_REQ_PORT, "22");
        
        SFTPChannel channel = test.getSFTPChannel();
        ChannelSftp chSftp = channel.getChannel(sftpDetails, 60000);
        
        String filename = "/home/omc/ylong/sftp/INTPahcfg.tar.gz";
        SftpATTRS attr = chSftp.stat(filename);
        long fileSize = attr.getSize();
        
        String dst = "D:\\INTPahcfg.tar.gz";
        OutputStream out = new FileOutputStream(dst);
        try {
            
            chSftp.get(filename, dst, new FileProgressMonitor(fileSize)); // 代码段1
            
            // chSftp.get(filename, out, new FileProgressMonitor(fileSize)); // 代码段2
            
            /**
             * 代码段3
             * 
            InputStream is = chSftp.get(filename, new MyProgressMonitor());
            byte[] buff = new byte[1024 * 2];
            int read;
            if (is != null) {
                System.out.println("Start to read input stream");
                do {
                    read = is.read(buff, 0, buff.length);
                    if (read > 0) {
                        out.write(buff, 0, read);
                    }
                    out.flush();
                } while (read >= 0);
                System.out.println("input stream read done.");
            }
            */
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            chSftp.quit();
            channel.closeChannel();
        }
    }
}
```
注：请分别将main方法中的代码段1，代码段2，代码段3取消注释，运行程序来进行测试。这三段代码分别演示了如何使用JSch的各种put方法来进行文件下载。
程序段：
```
SFTPChannel channel = test.getSFTPChannel();
ChannelSftp chSftp = channel.getChannel(sftpDetails, 60000);
```
这两行语句得到一个ChannelSftp对象，具体实现见上篇随笔：[JSch
 - Java实现的SFTP（文件上传详解篇）](http://www.cnblogs.com/longyg/archive/2012/06/25/2556576.html)
注意：每个get方法中都使用了一个SftpProgressMonitor接口的实现对象（FileProgressMonitor和MyProgressMonitor）来监控传输进度，具体实现见上篇随笔：[JSch
 - Java实现的SFTP（文件上传详解篇）](http://www.cnblogs.com/longyg/archive/2012/06/25/2556576.html)
代码段1：直接将目标服务器上文件名为src的文件下载到本地，本地文件名为dst。（注：使用这个方法时，dst可以是目录，若dst为目录，则下载到本地的文件名将与src文件名相同）
代码段2：将目标服务器上文件名为src的文件下载到本地的一个输出流对象，该输出流为一个文件输出流
代码段3：采用读取get方法返回的输入流数据的方式来下载文件。这个示例中，将读取的数据写入到了本地的一个文件中。采用这种方式，可以由应用程序设定每次读取输入流的数据块大小，也就是每次传输的数据块大小。比如这个示例中：
```
byte[] buff = new byte[1024 * 2];
```
这条语句指定了每次传输的数据块大小为2KB，可以从测试代码段3的输出中看出确实每次只传输了2KB，即2048字节：
```
logs
Transferring begin.
Start to read input stream
Currently transferred total size: 2048 bytes
Currently transferred total size: 4096 bytes
Currently transferred total size: 6144 bytes
Currently transferred total size: 8192 bytes
Currently transferred total size: 10240 bytes
Currently transferred total size: 12288 bytes
Currently transferred total size: 14336 bytes
...
```
JSch文件下载与文件上传的传输进度，都是通过实现SftpProgressMonitor接口来实现的。所以这里不在详述，请直接参见上篇随笔：[JSch
 - Java实现的SFTP（文件上传详解篇）](http://www.cnblogs.com/longyg/archive/2012/06/25/2556576.html)
