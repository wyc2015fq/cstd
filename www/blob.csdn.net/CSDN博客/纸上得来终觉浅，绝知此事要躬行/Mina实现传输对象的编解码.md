# Mina实现传输对象的编解码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年04月25日 00:55:44[boonya](https://me.csdn.net/boonya)阅读数：1750
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









前言：协议编解码器是在使用Mina 的时候你最需要关注的对象，因为在网络传输的数据都是二进制数据（byte），而你在程序中面向的是JAVA 对象，这就需要你实现在发送数据时将JAVA 对象编码二进制数据，而接收数据时将二进制数据解码为JAVA 对象（注意这里不是JAVA 对象的序列化和反序列化）。

1）、编码：编码是将普通对象的属性内容依次装换成二进制对象属性内容的过程。

2）、解码：由转换成的二进制对象字段转换成普通对象属性字段内容的过程。

**1、编解码工厂：提供编码和解码实现的调用入口**


Mina 中的协议编解码器通过过滤器ProtocolCodecFilter 构造，这个过滤器的构造方法需要一个ProtocolCodecFactory,

ProtocolCodecFactory 中有如下两个方法：

```java
public interface ProtocolCodecFactory {

     ProtocolEncoder getEncoder(IoSession session) throws Exception;//ProtocolEncoder是自定义编码器要实现的接口

     ProtocolDecoder getDecoder(IoSession session) throws Exception;//ProtocolDecoder是自定义解码器要实现的接口

}
```

以消息传递工厂为例：

```java
//编解码器生成工厂
  public class MessageProtocolCodecFactory implements ProtocolCodecFactory {
      private ProtocolEncoder encoder;
      private ProtocolDecoder decoder;
      
      public MessageProtocolCodecFactory()
      {
          this(Charset.forName("UTF-8"));
      }
     
     public MessageProtocolCodecFactory(Charset charset)
     {
         encoder = new MessageEncoder(charset);
         decoder = new MessageDecoder(charset);
     }
     
     @Override
     public ProtocolDecoder getDecoder(IoSession arg0) throws Exception {
         return decoder;
     }
 
     @Override
     public ProtocolEncoder getEncoder(IoSession arg0) throws Exception {
         return encoder;
     }

 }
```

**2、消息传输对象：装载传输数据的实体对象**


下面是示例代码：（模拟手机信息的编解码，消息格式：消息头，发送人，接收人，内容长度，内容信息）


MsgObject.java: 消息实体类

```java
public class MsgObject {
      //发送者
      private String sender;
      //接收者
      private String receiver;
      //信息内容
      private String content;
  
     public String getSender() {
         return sender;
     }
 
     public void setSender(String sender) {
         this.sender = sender;
     }
 
     public String getReceiver() {
         return receiver;
     }
 
     public void setReceiver(String receiver) {
         this.receiver = receiver;
     }
 
     public String getContent() {
         return content;
     }
 
    public void setContent(String content) {
         this.content = content;
    }
}
```


**3、消息编码器 ：MessageEncoder.java，该类实现ProtocolEncoder或继承自ProtocolEncoderAdapter**

```java
//消息编码器
 public class MessageEncoder extends ProtocolEncoderAdapter {
      private Charset charset;
      
     public MessageEncoder(Charset charset){
          this.charset = charset;
      }
      
     @Override
     public void encode(IoSession session, Object message, ProtocolEncoderOutput out)
             throws Exception {
        MsgObject msg = (MsgObject) message;
        //生成字符编码器
         CharsetEncoder charsetEncoder = charset.newEncoder();
        //得到要发送对象属性内容，准备进行编码
         String status = "M sip:wap.fetion.com.cn SIP-C/2.0";
        String sender = msg.getSender();
        String receiver = msg.getReceiver();
        String content = msg.getContent();
        //开辟一个缓存空间，设置为自动调整大小
         IoBuffer ioBuffer = IoBuffer.allocate(100);
        ioBuffer.setAutoExpand(true);
        //将要发送的信息放入缓存空间
         //消息头
         ioBuffer.putString(status + "\n", charsetEncoder);
         //消息发送者
         ioBuffer.putString("S: " + sender + "\n", charsetEncoder);
         //消息接收者
         ioBuffer.putString("R: " + receiver + "\n", charsetEncoder);
         //消息内容长度
         ioBuffer.putString("L: " + content.getBytes(charset).length + "\n", charsetEncoder);
         //消息内容
         ioBuffer.putString(content + "\n", charsetEncoder);
         //编码后的信息已放入ioBuffer中，进行写回
         ioBuffer.flip();
        out.write(ioBuffer);
     }
 
 }
```

**4、消息解码器:MessageDecoder.java，该类实现ProtocolDecoder或继承自ProtocolDecoderAdapter**


```java
//消息解码器
  public class MessageDecoder extends ProtocolDecoderAdapter{
      private Charset charset;
  
      public MessageDecoder(Charset charset) {
          this.charset = charset;
      }
  
      @Override
     protected boolean doDecode(IoSession session, IoBuffer in,  ProtocolDecoderOutput out) throws Exception {
         CharsetDecoder charDecoder = charset.newDecoder();
         IoBuffer buffer = IoBuffer.allocate(100).setAutoExpand(true);
         // 接收解码后的信息
         String status = "";
         String sender = "";
         String receiver = "";
         String contentLen = "";
         String content = "";
 
         int textLineNumber = 1;
         int columnNumber = 0;
         // 如果缓存区还有消息
         while (in.hasRemaining()) {
             byte bt = in.get();
             buffer.put(bt);
             //换行
             if (bt == 10 && textLineNumber < 5) {
                 columnNumber++;
                 if (textLineNumber == 1) {
                     buffer.flip();
                     status = buffer.getString(columnNumber, charDecoder);
                     status = status.substring(0, status.length() - 1);
                     columnNumber = 0;
                     buffer.clear();
                 }
                 if (textLineNumber == 2) {
                     buffer.flip();
                     sender = buffer.getString(columnNumber, charDecoder);
                     sender = sender.substring(0, sender.length() - 1);
                     columnNumber = 0;
                     buffer.clear();
                 }
                 if (textLineNumber == 3) {
                     buffer.flip();
                     receiver = buffer.getString(columnNumber, charDecoder);
                     receiver = receiver.substring(0, receiver.length() - 1);
                     columnNumber = 0;
                     buffer.clear();
                 }
                 if (textLineNumber == 4) {
                     buffer.flip();
                     contentLen = buffer.getString(columnNumber, charDecoder);
                     contentLen = contentLen.substring(0,
                             contentLen.length() - 1);
                     columnNumber = 0;
                     buffer.clear();
                 }
                 textLineNumber++;
             } else if (textLineNumber == 5) {
                 columnNumber++;
                 if (columnNumber == Long.parseLong(contentLen.split(": ")[1])) {
                     buffer.flip();
                     content = buffer.getString(columnNumber, charDecoder);
                     textLineNumber++;
                     break;
                 }
             } else {
                 columnNumber++;
             }
 
         }
         MsgObject smsObject = new MsgObject();
         smsObject.setSender(sender.split(": ")[1]);
         smsObject.setReceiver(receiver.split(": ")[1]);
         smsObject.setContent(content);
         out.write(smsObject);
         return false;
     }
 }
```


**5、Mina编解码需要知道的一些事情**

1）、最好了解下Mina IoBuffer的读取操作。


2）、在过滤器中调用这些编解码器来进行对象的传输，服务器端和客户端的主程序编写。 


3）、考虑是否分批实现对象解码

上面的消息解码器( MessageDecoder.java)中的解码考虑的情况是消息一次性从服务器发送过来，但有时消息可能不是一次性从服务器发送过来，而是分成了几次分批过来，这时就会重复调用解码器的deCode()方法，这时状态变量textLineNumber和columnNumber就会被重置，所以要把状态变量保存起来。可能你会想到将状态变量保存在解码器的成员变量中，但是Mina不保证每次调用deCode()方法的都是同一个线程，所以状态变量不是线程安全的。所以要将状态变量保存到IoSession中,因为IoSession用了一个同步的HashMap保存对象(客户端和服务端发起的会话不是同一个进程，所以session不是同一个session，这里可以体现Mina是事件驱动异步的API)。

以下是在IoSession中保存状态变量:


```java
// 保存数据状态对象的key值
private final AttributeKey CONTEXT = new AttributeKey(getClass(), "context"); 
 

//通过IoSession.setAttribute和IoSession.getAttribute的保存和得到保存数据的对象
 private MsgContext getContext(IoSession session) {
         MsgContext context = (MsgContext) session.getAttribute(CONTEXT);
         if (null == context) {
             context = new MsgContext();
             session.setAttribute(CONTEXT, context);
         }
         return context;
  }
```





