# HttpURLConnection的流式输出的缺陷和解决方法 - z69183787的专栏 - CSDN博客
2012年11月15日 14:53:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1733
最近在用applet写文件上传控件的时候发现使用URLConnection来对服务器进行流式输出时的一些问题.我们通常要对服务器上的某个地址进行写流操作,那么我们一般的做法就是: 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- URLConnection con = new URL("/test.do").openConnection();  
- con.setDoOutput(true); // 允许输出流,默认是false
    这样我们就获取一个到/test.do地址的HTTP连接了,我们打印con的class后发现其实是:sun.net.www.protocol.http.HttpURLConnection这个类,我们在写大数据流到服务器上时就会发现总是会出现OutOfMemoryError的错误,当然不同的机器上出现错误所对应的文件输出流的大小是不一样的.这个主要就是取决于本机的JVM的内存空间的大小了.出现OutOfMemoryError错误的主要原因就是:sun公司实现的HttpURLConnection的输出流是首先在本地内存进行缓存,然后再一次性输出的(在close操作时).我们可以追踪到sun自己的HttpURLConnection使用的OutputStream是sun.net.www.http.PosterOutputStream这个类,我们查看这个类的源码就会发现它是继承自ByteArrayOutputStream的,而且基本上这个类没有做任何事情,大家可以参看其源码.而我们的ByteArrayOutputStream则是每次将要输出的内容复制到一个byte数组中,从而导致的结果是将整个输出流全部存储在内存中，这样当我们输出流一大的时候就会出现内存不够用的情况.请看ByteArrayOutputStream的部分源码: 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicsynchronizedvoid write(int i)   
- {   
- int j = count + 1;   
- if(j > buf.length) {   
- byte abyte0[] = newbyte[Math.max(buf.length << 1, j)];   
-         System.arraycopy(buf, 0, abyte0, 0, count);   
-         buf = abyte0;   
-     }   
-     buf[count] = (byte)i;   
-     count = j;   
- }   
- 
- publicsynchronizedvoid write(byte abyte0[], int i, int j)   
- {   
- if(i < 0 || i > abyte0.length || j < 0 || i + j > abyte0.length || i + j < 0) thrownew IndexOutOfBoundsException();   
- if(j == 0)  return;   
- int k = count + j;   
- if(k > buf.length) {   
- byte abyte1[] = newbyte[Math.max(buf.length << 1, k)];   
-         System.arraycopy(buf, 0, abyte1, 0, count);   
-         buf = abyte1;   
-     }   
-     System.arraycopy(abyte0, i, buf, count, j);   
-     count = k;   
- }  
    我们可以看到它是使用System.arraycopy的功能来将所有的输出流存放在一个数组中的.因此,在使用HttpURLConnection进行流式输出的时候如果输出流比较大,那么就该考虑使用其他方式了(当然,修改JVM的堆栈空间是一种方法,但是不可取). 
    这是我们直接使用java.net.HttpURLConnection类的相关方法来进行输出文件流,我们查看sun提供的HttpURLConnection的源码,会发现其默认是采用上面提高的PosterOutputStream类来进行缓冲输出的,即首先将所有的文件流在本地内存中进行缓存,等到输出结束执行close的时候一次性输出到服务器端.同时我们看到sun的HttpURLConnection中的getOutputStream()中有如下代码: 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- if(streaming()) {  
- if(fixedContentLength != -1)   
-         strOutputStream = new StreamingOutputStream(ps, fixedContentLength);   
- elseif(chunkLength != -1)   
-         strOutputStream = new StreamingOutputStream(new ChunkedOutputStream(ps, chunkLength), -1);   
- return strOutputStream;   
- }  
    其中strmeanming()方法是用来判断是否是流式的输出,其代码为:return fixedContentLength != -1 || chunkLength != -1;它的判断方法就是如果设置了输出流的固定长度或是设置了块的长度,那么将采用流式输出.因此,我们可以在输出的时候可以设置其长度来达到流式输出这样的效果.另外,StreamingOutputString类是sun提供的HttpURLConnection的内部类,继承自FilterOutputStream,而非ByteArrayOutputStream,所以不会在本地内存中进行缓存. 
    而jdk中的HttpURLConnection并没有提供设置流的固定长度或块输出的长度方法,所以我们需要显示的将new URL("url").openConnection()返回的URLConnection转换成sun的HttpURLConnection,从而我们就可以很方便的使用setFixedLengthStreamingMode方法来设置流的固定长度,那么也就会采用流式的输出了.那么也就不会出现OutOfMemoryError的错误了.另外,ChunkedOutputStream也是不会在本地进行缓存的,它是使用固定大小的数组来缓存输出流,等缓存满的时候就自动的调用基础流进行输出,这个主要是用在无法确定输出流的具体长度但是又不想在本地进行缓存时用到.同理,我们通过设置setChunkedStreamingMode就可以达到这样的效果,三种方式的代码如下: 
第1种方式:使用直接输出流的方式(已知输出流的长度): 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- import sun.net.www.protocol.http.HttpURLConnection;  
- ......  
- HttpURLConnection con = (HttpURLConnection)new URL("url").openConnection();  
- con.setFixedLengthStreamingMode(输出流的固定长度);  
- ......  
第2种方式:使用直接输出流的方式(未知输出流的长度): 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- import sun.net.www.protocol.http.HttpURLConnection;  
- ......  
- HttpURLConnection con = (HttpURLConnection)new URL("url").openConnection();  
- con.setChunkedStreamingMode(块的大小);  //单位 byte
- ......  
第3种方式:本地缓存后一次性输出: 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- import java.net.HttpURLConnection;  
- ......  
- HttpURLConnection con = (HttpURLConnection)new URL("url").openConnection();  
- ......  
    通过设置直接输出后,我传送文件的大小为200M的时候也不会出现OutOfMemoryError的错误,而之前使用第3种方式进行文件流输出的时候不到40M就出现了OutOfMemoryError的错误了. 
