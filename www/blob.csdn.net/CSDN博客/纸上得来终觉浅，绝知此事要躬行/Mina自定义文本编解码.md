# Mina自定义文本编解码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年05月02日 21:42:10[boonya](https://me.csdn.net/boonya)阅读数：1436
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









前言：正如我们想的那样，文本的内容一次解析不完该怎么办呢？答案是用一个对象将这些未解析的对象存起来，等待解析。这个对象一般保存在session中。

1、文本编码器



```java
package com.boonya.protocol.codec.self; 
import java.nio.charset.Charset; 
import org.apache.mina.core.buffer.IoBuffer; 
import org.apache.mina.core.session.IoSession; 
import org.apache.mina.filter.codec.ProtocolEncoder; 
import org.apache.mina.filter.codec.ProtocolEncoderOutput; 

public class MyTextLineCodecEncoder implements ProtocolEncoder { 

     private Charset charset; // 编码格式 

     private String delimiter; // 文本分隔符 

  
     public MyTextLineCodecEncoder(Charset charset, String delimiter) { 
         this.charset = charset; 
         this.delimiter = delimiter; 

     } 
  

     public void encode(IoSession session, Object message, 

             ProtocolEncoderOutput out) throws Exception { 

         if (delimiter == null || "".equals(delimiter)) { // 如果文本换行符未指定，使用默认值 

             delimiter = "\r\n"; 

         } 

         if (charset == null) { 

             charset = Charset.forName("utf-8"); 

         } 

  

         String value = message.toString(); 

         IoBuffer buf = IoBuffer.allocate(value.length()).setAutoExpand(true); 

         buf.putString(value, charset.newEncoder()); // 真实数据 

         buf.putString(delimiter, charset.newEncoder()); // 文本换行符 

         buf.flip(); 

         out.write(buf); 

     } 

  

     public void dispose(IoSession session) throws Exception { 

     } 

  
 }
```



2、文本解码器【解码是一个单例，所以要利用session来处理不同对象的解码】


```java
package com.boonya.protocol.codec.self;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.ProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolDecoderOutput;
 
public class MyTextLineCodecDecoder implements ProtocolDecoder {

    private Charset charset; // 编码格式
 
    private String delimiter; // 文本分隔符

    private IoBuffer delimBuf; // 文本分割符匹配的变量
 

    // 定义常量值，作为每个IoSession中保存解码任务的key值
    private static String CONTEXT = MyTextLineCodecDecoder.class.getName()+ ".context";

 
    // 构造函数，必须指定Charset和文本分隔符
    public MyTextLineCodecDecoder(Charset charset, String delimiter) {

        this.charset = charset;

        this.delimiter = delimiter;

    }


    public void decode(IoSession session, IoBuffer in, ProtocolDecoderOutput out)

            throws Exception {

        Context ctx = getContext(session);

        if (delimiter == null || "".equals(delimiter)) { // 如果文本换行符未指定，使用默认值

            delimiter = "\r\n";

        }

        if (charset == null) {

            charset = Charset.forName("utf-8");

        }

        decodeNormal(ctx, in, out);

    }

 

    // 从IoSession中获取Context对象
    private Context getContext(IoSession session) {

        Context ctx;

        ctx = (Context) session.getAttribute(CONTEXT);

        if (ctx == null) {

            ctx = new Context();

            session.setAttribute(CONTEXT, ctx);

        }

        return ctx;

    }

 

    // 解码
    private void decodeNormal(Context ctx, IoBuffer in,

            ProtocolDecoderOutput out) throws CharacterCodingException {

        // 取出未完成任务中已经匹配的文本换行符的个数

        int matchCount = ctx.getMatchCount();

 

        // 设置匹配文本换行符的IoBuffer变量

        if (delimBuf == null) {

            IoBuffer tmp = IoBuffer.allocate(2).setAutoExpand(true);

            tmp.putString(delimiter, charset.newEncoder());

            tmp.flip();

            delimBuf = tmp;

        }

        int oldPos = in.position(); // 解码的IoBuffer中数据的原始信息

        int oldLimit = in.limit();

        while (in.hasRemaining()) { // 变量解码的IoBuffer

            byte b = in.get();

            if (delimBuf.get(matchCount) == b) { // 匹配第matchCount位换行符成功

                matchCount++;

                if (matchCount == delimBuf.limit()) { // 当前匹配到字节个数与文本换行符字节个数相同，匹配结束

                    int pos = in.position(); // 获得当前匹配到的position（position前所有数据有效）
                    in.limit(pos);
                    in.position(oldPos); // position回到原始位置
 

                    ctx.append(in); // 追加到Context对象未完成数据后面

                    in.limit(oldLimit); // in中匹配结束后剩余数据
                    in.position(pos);

 

                    IoBuffer buf = ctx.getBuf();
                    buf.flip();
                    buf.limit(buf.limit() - matchCount);// 去掉匹配数据中的文本换行符
                    try {
                        out.write(buf.getString(ctx.getDecoder())); // 输出解码内容
                    } finally {
                        buf.clear(); // 释放缓存空间

                    }

                    oldPos = pos;

                    matchCount = 0;

                }

            } else {

                // 如果matchCount==0，则继续匹配

                // 如果matchCount>0，说明没有匹配到文本换行符的中的前一个匹配成功字节的下一个字节，

                // 跳转到匹配失败字符处，并置matchCount=0，继续匹配

                in.position(in.position() - matchCount);

                matchCount = 0; // 匹配成功后，matchCount置空

            }

        }
 
        // 把in中未解码内容放回buf中
        in.position(oldPos);

        ctx.append(in);

        ctx.setMatchCount(matchCount);

    }

    public void dispose(IoSession session) throws Exception {


    }

    public void finishDecode(IoSession session, ProtocolDecoderOutput out)

            throws Exception {

    }


    // 内部类，保存IoSession解码时未完成的任务
    private class Context {

        private CharsetDecoder decoder;

        private IoBuffer buf; // 保存真实解码内容

        private int matchCount = 0; // 匹配到的文本换行符个数

 

        private Context() {

            decoder = charset.newDecoder();

            buf = IoBuffer.allocate(80).setAutoExpand(true);

        }

 

        // 重置
        @SuppressWarnings("unused")

        public void reset() {

            matchCount = 0;

            decoder.reset();

        }

 

        // 追加数据
        public void append(IoBuffer in) {

            getBuf().put(in);

        }

 
        // ======get/set方法=====================
        public CharsetDecoder getDecoder() {

            return decoder;

        }

 
        public IoBuffer getBuf() {

            return buf;

        }

 

        public int getMatchCount() {

            return matchCount;

        }

 

        public void setMatchCount(int matchCount) {

            this.matchCount = matchCount;

        }

    } // end class Context;

}
```




还是提醒要注意解码对象是否是当前的这个对象，加于区别避免解码过程出错。




