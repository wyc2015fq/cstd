# SavedFileParser 源码分析 - zj360202的专栏 - CSDN博客





2012年12月06日 18:02:08[zj360202](https://me.csdn.net/zj360202)阅读数：356标签：[TimeTunnel](https://so.csdn.net/so/search/s.do?q=TimeTunnel&t=blog)
个人分类：[TimeTunnel](https://blog.csdn.net/zj360202/article/category/1298101)









```
package com.taobao.timetunnel.savefile.app;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;

import com.taobao.timetunnel.client.Message;
import com.taobao.timetunnel.client.message.MessageFactory;
import com.taobao.timetunnel.savefile.util.BytesUtil;

public class SavedFileParser {

 public static void main(String[] args) throws IOException {
  convertDataFile("D:/2010_09_09_14_16#d06ca100-e29e-45f7-a9cf-9a6a01a1b487");
 }

 private static void convertDataFile(String path) throws IOException {

  RandomAccessFile srvRandomAccessFile = new RandomAccessFile(new File(path), "r");

  FileWriter fw = new FileWriter(new File(path + ".converted"));
  ByteBuffer dataBuffer = ByteBuffer.allocate(40 * 1024 * 1024);//字节流的容量为40M

  int size = srvRandomAccessFile.read(dataBuffer.array());
  dataBuffer.position(size);
  dataBuffer.flip();//弹一下数据流应该和flush()功能一样
  int count = 0;

  byte[] messLenBytes = new byte[4];
  //每一次写messLenBytes[0]<<24+messLenBytes[1]<<16+messLenBytes[2]<<8+messLenBytes[3]的数据
  //直到写完
  while (dataBuffer.hasRemaining()) {//取出的40M数据流
   dataBuffer.get(messLenBytes, 0, 4);
   int len = BytesUtil.bytesToInt(messLenBytes);
   System.out.println("the len from file in convertDataFile is " + len);
   byte[] messageBuffer = new byte[len];
   if (dataBuffer.remaining() < len) {
    System.out.println("incomplete message " + messageBuffer.length);
    break;
   }
   dataBuffer.get(messageBuffer, 0, len);
   // System.out.println("message Buffer:  "+messageBuffer);
   // System.out.println("messageBuffer len is "+
   // messageBuffer.length);
   // System.out.println("dataBuffer remain is "+
   // dataBuffer.remaining());
   try {
    Message m = MessageFactory.getInstance().createMessageFrom(messageBuffer);
    if (m.isCompressed())
     m.decompress();

    // System.out.println("the len of content is: "+m.getContent().length);
    //     
    // System.out.println("content is: "+m.getContent());
    // System.out.println("content is: "+m.getCreatedTime());
    // System.out.println("content is: "+m.getIpAddress());
    // System.out.println("content is: "+m.getTopic());
    // System.out.println("content is: "+m.getId());
    fw.write(new String(m.getContent(), "UTF-8"));
    fw.flush();
    count++;

   } catch (Throwable e) {
    e.printStackTrace();
    System.out.println("Throwable messageBuffer len is " + messageBuffer.length);
    System.out.println("Throwable dataBuffer remain is " + dataBuffer.remaining());
    System.out.println("Throwable the remain incomplete byte is:" + messageBuffer);
    continue;
   }

  }
  System.out.println("the message in file total count: " + count);
  fw.close();

 }
}
```






