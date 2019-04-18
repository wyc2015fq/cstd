# Java基础：IO 流中的 flush - z69183787的专栏 - CSDN博客
2012年11月20日 22:35:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1078
输出流呢，类似于一根管道，输出的时候先放到管道里，然后 管道满了存到介质上（硬盘或其他地方），当我们输出完后管道里面可能还有剩余，就用flush（）清空管道即全部存到介质上。好象java默认的缓冲区是 8kb?(我也忘了),就是说只有每填满8kb才会提交一次,当少于这个值时就不会提交，所以最后为防止有未提交数据就flush（）一下，强行提交生于
 数据 
也就是说，一般输出流关闭之前要用下这个方法。
Java设计Io流可谓是煞费苦心，如果你是初学者我敢保证第一次接触Java的IO类，一定会“狂晕！！”，晕，倒不是因为它有多么难学，而是太多，而且及其让人容易迷惑。在编程日子中，尤其是在网络编程中，几乎离不开Java的IO，关于Java的IO流的分类，可以到网上soso，今天跟大家分享一下flush方法。
**1.  OutputStream类的flush方法**
该类实现了Flushable接口，所以重写了flush方法，看看flush()源码，会更加的让你明白：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- publicvoid flush() throws IOException {  
- }  
sorry，该实现为空。就是一个空方法，什么也不做。看清楚啊，该方法不是抽象方法，是一个实实在在的方法。除了方法体中一无所有，其它还好！！！汗！！！看JDK的api如何解释！
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- flush  
- publicvoid flush()  
- throws IOException  
- 刷新此输出流并强制写出所有缓冲的输出字节。flush 的常规协定是：如果此输出流的实现已经缓冲了以前写入的任何字节，则调用此方法指示应将这些字节立即写入它们预期的目标。  
- 如果此流的预期目标是由基础操作系统提供的一个抽象（如一个文件），则刷新此流只能保证将以前写入到流的字节传递给操作系统进行写入，但不保证能将这些字节实际写入到物理设备（如磁盘驱动器）。  
- OutputStream 的 flush 方法不执行任何操作。  
- 指定者：  
- 接口 Flushable 中的 flush  
- 抛出：  
- IOException - 如果发生 I/O 错误。  
开始，我安慰自己，该类是一个抽象类，它的子类肯定重写了该方法。好吧，OutputStream的直接子类有：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- ByteArrayOutputStream  
- FileOutputStream   
- FilterOutputStream  
- ObjectOutputStream  
- OutputStream  
- PipedOutputStream  
*注意：这里的子类OutputStream是包 org.omg.CORBA.portable 的。*
对于FileOutputStream、ByteArrayOutputStream、org.omg.CORBA.portable.OutputStream类它们的flush()方法均是从父类继承的flush方法。
FilterOutputStream类重写了flush()方法，但是实质还是调用父类的flush方法。
ObjectOutputStream、PipedOutputStream类重写了flush()方法。
好吧，来两个个小例子，很简单，第一个例子主要是向文本中写入字符串，第二个例子向文本中写入一定字节的数据，如下代码：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- package mark.zhang;  
- import java.io.BufferedOutputStream;  
- import java.io.DataOutputStream;  
- import java.io.File;  
- import java.io.FileOutputStream;  
- publicclass Test {  
- publicstaticvoid main(String[] args) throws Exception {  
-         File file = new File("text.txt");  
- if(!file.exists()) {  
-             file.createNewFile();  
-         }  
-         FileOutputStream fos = new FileOutputStream(file);  
-         BufferedOutputStream bos = new BufferedOutputStream(fos);  
-         DataOutputStream dos = new DataOutputStream(fos);  
-         dos.writeBytes("java io");  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- package mark.zhang;  
- import java.io.BufferedOutputStream;  
- import java.io.File;  
- import java.io.FileOutputStream;  
- publicclass Test {  
- publicstaticvoid main(String[] args) throws Exception {  
-         File file = new File("text.txt");  
- if(!file.exists()) {  
-             file.createNewFile();  
-         }  
-         FileOutputStream fos = new FileOutputStream(file);  
-         BufferedOutputStream bos = new BufferedOutputStream(fos);  
- byte[] b = newbyte[1024*8];  
-         bos.write(b);  
-         bos.flush();  
-     }  
- }  
这两段代执行后，分别会在当前目录下产生7字节的文件（内容为java io）和1KB字节的文件。说到这里，有些人会说，这有什么稀奇，至于吗？？？呵呵，别急，淡定！！现在修改第二个代码，主要是注释掉调用flush()方法，如下：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- package mark.zhang;  
- import java.io.BufferedOutputStream;  
- import java.io.File;  
- import java.io.FileOutputStream;  
- publicclass Test {  
- publicstaticvoid main(String[] args) throws Exception {  
-         File file = new File("text.txt");  
- if(!file.exists()) {  
-             file.createNewFile();  
-         }  
-         FileOutputStream fos = new FileOutputStream(file);  
-         BufferedOutputStream bos = new BufferedOutputStream(fos);  
- byte[] b = newbyte[1024];  
-         bos.write(b);  
- //bos.flush();
-     }  
- }  
ok，再次运行代码，额的神啊？？？文件大小居然是o字节。why？？？？flush()方法有那么神奇，汗？？！！！
仔细的你会发现，第一个代码并没有调用flush()方法，居然可以。为什么第二个就不可以呢？还是看源码，有说服力。
DataOutputStream继承FilterOutputStream，实现了DataOutput接口。我们知道FilterOutputStream类重写了flush()方法，但是实质还是调用父类的flush方法。DataOutputStream类的flush()方法效仿其父类FilterOutputStream的做法，如下：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- publicvoid flush() throws IOException {  
-     out.flush();  
- }  
那么，即使你在代码后面加上dos.flush();与不加是一样的效果，因为它们的父类flush()方法均为空，这就是为什么第一个代码的神奇所在。再看看第二个代码的病因在哪里？先看看BufferedOutputStream类的结构：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- publicclass BufferedOutputStream extends FilterOutputStream   
再看看，它的flush()方法：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- publicsynchronizedvoid flush() throws IOException {  
-         flushBuffer();  
-     out.flush();  
- }  
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- /** Flush the internal buffer */
- privatevoid flushBuffer() throws IOException {  
- if (count > 0) {  
-  out.write(buf, 0, count);  
-  count = 0;  
-     }  
- }  
不错，该类重写了flush()方法，不像前面几个那样不是继承就是山寨父类的flush()方法。BufferedOutputStream 类是一个使用了缓冲技术的类。这种类一把都会自己实现flush()方法。
那么，有人会问使用这种类的时候，难道必须使用flush()方法吗，当然不是喽？？！！不过有个前提，你的字节数据必须不能小于8KB。实例代码，*注意没有flush()方法。*
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- package mark.zhang;  
- import java.io.BufferedOutputStream;  
- import java.io.File;  
- import java.io.FileOutputStream;  
- publicclass Test {  
- publicstaticvoid main(String[] args) throws Exception {  
-         File file = new File("text.txt");  
- if(!file.exists()) {  
-             file.createNewFile();  
-         }  
-         FileOutputStream fos = new FileOutputStream(file);  
-         BufferedOutputStream bos = new BufferedOutputStream(fos);  
- byte[] b = newbyte[1024*8];  
-         bos.write(b);  
- //bos.flush();
-     }  
- }  
执行代码，会产生8KB的文本文件。当然，怎么可能你每时每刻都知道你的数据一定会不小于8KB呢，所以还是调用flush()方法比较安全。不过，话又说回来，一般用完IO流之后（如果你有一个好的习惯）我们都会去调用close()方法，看源码可以知道该方法也是调用相对应的flush()方法。所以，大多数情况下你不必要担心。这里提醒一下，如果你的文件读写没有达到预期目的，十之八九是因为你没有调用flush()或者close()方法。
另外，字符流类大多数都实现了flush()或者close()方法，只不过，它们调用的是StreamEncoder类的该方法。该类位于sun.nio.cs包下面，其源码在我们jdk中是没有的。源码地址：[http://www.docjar.com/html/api/sun/nio/cs/StreamEncoder.java.html](http://www.docjar.com/html/api/sun/nio/cs/StreamEncoder.java.html)。在此，ctrl+v其源码，如下：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- /*
-  * Copyright 2001-2005 Sun Microsystems, Inc.  All Rights Reserved.
-  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
-  *
-  * This code is free software; you can redistribute it and/or modify it
-  * under the terms of the GNU General Public License version 2 only, as
-  * published by the Free Software Foundation.  Sun designates this
-  * particular file as subject to the "Classpath" exception as provided
-  * by Sun in the LICENSE file that accompanied this code.
-  *
-  * This code is distributed in the hope that it will be useful, but WITHOUT
-  * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
-  * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
-  * version 2 for more details (a copy is included in the LICENSE file that
-  * accompanied this code).
-  *
-  * You should have received a copy of the GNU General Public License version
-  * 2 along with this work; if not, write to the Free Software Foundation,
-  * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
-  *
-  * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
-  * CA 95054 USA or visit www.sun.com if you need additional information or
-  * have any questions.
-  */
- 
- 
- 
- package sun.nio.cs;  
- 
- import java.io;  
- import java.nio;  
- import java.nio.channels;  
- import java.nio.charset;  
- 
- publicclass StreamEncoder extends Writer  
- {  
- 
- privatestaticfinalint DEFAULT_BYTE_BUFFER_SIZE = 8192;  
- 
- privatevolatileboolean isOpen = true;  
- 
- privatevoid ensureOpen() throws IOException {  
- if (!isOpen)  
- thrownew IOException("Stream closed");  
-     }  
- 
- // Factories for java.io.OutputStreamWriter
- publicstatic StreamEncoder forOutputStreamWriter(OutputStream out,  
-                                                       Object lock,  
-                                                       String charsetName)  
- throws UnsupportedEncodingException  
-     {  
-         String csn = charsetName;  
- if (csn == null)  
-             csn = Charset.defaultCharset().name();  
- try {  
- if (Charset.isSupported(csn))  
- returnnew StreamEncoder(out, lock, Charset.forName(csn));  
-         } catch (IllegalCharsetNameException x) { }  
- thrownew UnsupportedEncodingException (csn);  
-     }  
- 
- publicstatic StreamEncoder forOutputStreamWriter(OutputStream out,  
-                                                       Object lock,  
-                                                       Charset cs)  
-     {  
- returnnew StreamEncoder(out, lock, cs);  
-     }  
- 
- publicstatic StreamEncoder forOutputStreamWriter(OutputStream out,  
-                                                       Object lock,  
-                                                       CharsetEncoder enc)  
-     {  
- returnnew StreamEncoder(out, lock, enc);  
-     }  
- 
- 
- // Factory for java.nio.channels.Channels.newWriter
- 
- publicstatic StreamEncoder forEncoder(WritableByteChannel ch,  
-                                            CharsetEncoder enc,  
- int minBufferCap)  
-     {  
- returnnew StreamEncoder(ch, enc, minBufferCap);  
-     }  
- 
- 
- // -- Public methods corresponding to those in OutputStreamWriter --
- 
- // All synchronization and state/argument checking is done in these public
- // methods; the concrete stream-encoder subclasses defined below need not
- // do any such checking.
- 
- public String getEncoding() {  
- if (isOpen())  
- return encodingName();  
- returnnull;  
-     }  
- 
- publicvoid flushBuffer() throws IOException {  
- synchronized (lock) {  
- if (isOpen())  
-                implFlushBuffer();  
- else
- thrownew IOException("Stream closed");  
-        }  
-    }  
- 
- publicvoid write(int c) throws IOException {  
- char cbuf[] = newchar[1];  
-        cbuf[0] = (char) c;  
-        write(cbuf, 0, 1);  
-    }  
- 
- publicvoid write(char cbuf[], int off, int len) throws IOException {  
- synchronized (lock) {  
-            ensureOpen();  
- if ((off < 0) || (off > cbuf.length) || (len < 0) ||  
-                ((off + len) > cbuf.length) || ((off + len) < 0)) {  
- thrownew IndexOutOfBoundsException();  
-            } elseif (len == 0) {  
- return;  
-            }  
-            implWrite(cbuf, off, len);  
-        }  
-    }  
- 
- publicvoid write(String str, int off, int len) throws IOException {  
- /* Check the len before creating a char buffer */
- if (len < 0)  
- thrownew IndexOutOfBoundsException();  
- char cbuf[] = newchar[len];  
-        str.getChars(off, off + len, cbuf, 0);  
-        write(cbuf, 0, len);  
-    }  
- 
- publicvoid flush() throws IOException {  
- synchronized (lock) {  
-            ensureOpen();  
-            implFlush();  
-        }  
-    }  
- 
- publicvoid close() throws IOException {  
- synchronized (lock) {  
- if (!isOpen)  
- return;  
-            implClose();  
-            isOpen = false;  
-        }  
-    }  
- 
- privateboolean isOpen() {  
- return isOpen;  
-    }  
- 
- 
- // -- Charset-based stream encoder impl --
- 
- private Charset cs;  
- private CharsetEncoder encoder;  
- private ByteBuffer bb;  
- 
- // Exactly one of these is non-null
- privatefinal OutputStream out;  
- private WritableByteChannel ch;  
- 
- // Leftover first char in a surrogate pair
- privateboolean haveLeftoverChar = false;  
- privatechar leftoverChar;  
- private CharBuffer lcb = null;  
- 
- private StreamEncoder(OutputStream out, Object lock, Charset cs) {  
- this(out, lock,  
-         cs.newEncoder()  
-         .onMalformedInput(CodingErrorAction.REPLACE)  
-         .onUnmappableCharacter(CodingErrorAction.REPLACE));  
-    }  
- 
- private StreamEncoder(OutputStream out, Object lock, CharsetEncoder enc) {  
- super(lock);  
- this.out = out;  
- this.ch = null;  
- this.cs = enc.charset();  
- this.encoder = enc;  
- 
- // This path disabled until direct buffers are faster
- if (false && out instanceof FileOutputStream) {  
-                ch = ((FileOutputStream)out).getChannel();  
- if (ch != null)  
-                    bb = ByteBuffer.allocateDirect(DEFAULT_BYTE_BUFFER_SIZE);  
-        }  
- if (ch == null) {  
-        bb = ByteBuffer.allocate(DEFAULT_BYTE_BUFFER_SIZE);  
-        }  
-    }  
- 
- private StreamEncoder(WritableByteChannel ch, CharsetEncoder enc, int mbc) {  
- this.out = null;  
- this.ch = ch;  
- this.cs = enc.charset();  
- this.encoder = enc;  
- this.bb = ByteBuffer.allocate(mbc < 0
-                                  ? DEFAULT_BYTE_BUFFER_SIZE  
-                                  : mbc);  
-    }  
- 
- privatevoid writeBytes() throws IOException {  
-        bb.flip();  
- int lim = bb.limit();  
- int pos = bb.position();  
- assert (pos <= lim);  
- int rem = (pos <= lim ? lim - pos : 0);  
- 
- if (rem > 0) {  
- if (ch != null) {  
- if (ch.write(bb) != rem)  
- assertfalse : rem;  
-        } else {  
-            out.write(bb.array(), bb.arrayOffset() + pos, rem);  
-        }  
-        }  
-        bb.clear();  
-        }  
- 
- privatevoid flushLeftoverChar(CharBuffer cb, boolean endOfInput)  
- throws IOException  
-    {  
- if (!haveLeftoverChar && !endOfInput)  
- return;  
- if (lcb == null)  
-            lcb = CharBuffer.allocate(2);  
- else
-            lcb.clear();  
- if (haveLeftoverChar)  
-            lcb.put(leftoverChar);  
- if ((cb != null) && cb.hasRemaining())  
-            lcb.put(cb.get());  
-        lcb.flip();  
- while (lcb.hasRemaining() || endOfInput) {  
-            CoderResult cr = encoder.encode(lcb, bb, endOfInput);  
- if (cr.isUnderflow()) {  
- if (lcb.hasRemaining()) {  
-                    leftoverChar = lcb.get();  
- if (cb != null && cb.hasRemaining())  
-                        flushLeftoverChar(cb, endOfInput);  
- return;  
-                }  
- break;  
-            }  
- if (cr.isOverflow()) {  
- assert bb.position() > 0;  
-                writeBytes();  
- continue;  
-            }  
-            cr.throwException();  
-        }  
-        haveLeftoverChar = false;  
-    }  
- 
- void implWrite(char cbuf[], int off, int len)  
- throws IOException  
-    {  
-        CharBuffer cb = CharBuffer.wrap(cbuf, off, len);  
- 
- if (haveLeftoverChar)  
-        flushLeftoverChar(cb, false);  
- 
- while (cb.hasRemaining()) {  
-        CoderResult cr = encoder.encode(cb, bb, false);  
- if (cr.isUnderflow()) {  
- assert (cb.remaining() <= 1) : cb.remaining();  
- if (cb.remaining() == 1) {  
-                haveLeftoverChar = true;  
-                leftoverChar = cb.get();  
-            }  
- break;  
-        }  
- if (cr.isOverflow()) {  
- assert bb.position() > 0;  
-            writeBytes();  
- continue;  
-        }  
-        cr.throwException();  
-        }  
-    }  
- 
- void implFlushBuffer() throws IOException {  
- if (bb.position() > 0)  
-        writeBytes();  
-    }  
- 
- void implFlush() throws IOException {  
-        implFlushBuffer();  
- if (out != null)  
-        out.flush();  
-    }  
- 
- void implClose() throws IOException {  
-        flushLeftoverChar(null, true);  
- try {  
- for (;;) {  
-                CoderResult cr = encoder.flush(bb);  
- if (cr.isUnderflow())  
- break;  
- if (cr.isOverflow()) {  
- assert bb.position() > 0;  
-                    writeBytes();  
- continue;  
-                }  
-                cr.throwException();  
-            }  
- 
- if (bb.position() > 0)  
-                writeBytes();  
- if (ch != null)  
-                ch.close();  
- else
-                out.close();  
-        } catch (IOException x) {  
-            encoder.reset();  
- throw x;  
-        }  
-    }  
- 
-    String encodingName() {  
- return ((cs instanceof HistoricallyNamedCharset)  
-            ? ((HistoricallyNamedCharset)cs).historicalName()  
-            : cs.name());  
-    }  
更多源码查看[http://www.docjar.com/projects/Open-JDK-6.b17-src-code.htm](http://www.docjar.com/projects/Open-JDK-6.b17-src-code.html)l 
**2. Writer类的flush方法**
该类是一个抽象类，声明如下：
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- publicabstractclass Writer implements Appendable, Closeable, Flushable   
Writer类的flush()方法是一个抽象方法，其子类一般都实现了该方法。所以，一般使用字符流之后，调用一下flush()或者close()方法。
**[java]**[view
 plain](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[copy](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[print](http://blog.csdn.net/androidbluetooth/article/details/6460726#)[?](http://blog.csdn.net/androidbluetooth/article/details/6460726#)
- abstractpublicvoid flush() throws IOException;  
细节请看jdk的api，或者Java的源码以及上面的StreamEncoder类源码。
ok，说到这里吧，这里主要借助Java的IO中字节流与字符流的flush()方法，来说明学Java看源码和思考是很重要的。
总之，不管你使用哪种流（字符、字节、具有缓冲的流）技术，不妨调用一下flush()/close()方法，防止数据无法写到输出流中。
