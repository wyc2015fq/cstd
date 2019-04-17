# Mina Codec Filter对应协议实现编解码处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月07日 11:55:14[boonya](https://me.csdn.net/boonya)阅读数：4427
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina Filter(Apache Mina user guide Chapter 9 Codec Filter)](http://mina.apache.org/mina-project/userguide/ch9-codec-filter/ch9-codec-filter.html)

本教程试图解释为什么以及如何使用ProtocolCodecFilter.

### 1.1.1. **为什么使用ProtocolCodecFilter**

TCP 保证交付的所有数据包以正确的顺序。但是不能保证服务端发送方的写操作会导致一个接收端读取事件。参见：[http://en.wikipedia.org/wiki/IPv4#Fragmentation_and_reassembly](#Fragmentation_and_reassembly)和[http://en.wikipedia.org/wiki/Nagle%27s_algorithm](http://en.wikipedia.org/wiki/Nagle' rel=)中的Mina术语：

没有ProtocolCodecFilter调用IoSession.write(Object message) 的发送方可以导致多个messageReceived(IoSessionsession，Object
 message)事件接收者；并且多个调用IoSession.write(Object message)会导致单个messageReceived事件。当客户机和服务器运行在同一主机(或一个本地网络)，你可能不会遇到这种行为，但你的应用程序应该能够解决这个问题。

大多数网络应用程序需要一种方法来找出当前消息的结束和下一个消息开始。

在IoHandler中你可以实现所有这些逻辑,但添加一个ProtocolCodecFilter会使你的代码更加简洁和更容易维护。

它允许你分离协议逻辑与业务逻辑(IoHandler)。

### 1.1.2. **怎样使用**

你的应用程序只是收到一串字节码，你需要将这些字节转换为消息(更高级别的对象)。

有三种常见的分割技术将字节流转化为消息:
- 使用固定长度的消息
- 使用一个固定长度的标题表明消息体的长度
- 使用分隔符,例如许多基于文本的协议每条消息之后附加一个换行符(或 CR LF par)([http://www.faqs.org/rfcs/rfc977.html](http://www.faqs.org/rfcs/rfc977.html))



在本教程中我们将使用第一和第二种方法,因为他们更容易实现。之后我们将会看到使用分隔符。

### 1.1.3. **协议编解码示例**

我们将开发一个(非常无用的)图形字符生成服务器来说明如何实现自己的协议编解码器(ProtocolEncoder,ProtocolDecoder,ProtocolCodecFactory)。协议是很简单的。



**这是一个消息请求层:**


|**4 bytes**|**4 bytes**|**4 bytes**|
|----|----|----|
|width|height|numchars|
- width:要求图像的宽度在网络字节顺序(整数)
- height:要求的高度形象在网络字节顺序(整数)
- numchars:字符的数量生成网络字节顺序(整数)

服务器响应两张图片的请求的维度，与请求的数量的字符画。

**这是一个消息响应层:**
|**4 bytes**|**variable length body**|**4 bytes**|**variable length body**|
|----|----|----|----|
|length1|image1|length2|image2|



**概述我们需要编码和解码的类的请求和响应:**
- · **ImageRequest**: 一个简单的POJO 代表我们ImageServer请求。
- · **ImageRequestEncoder**: 所使用的编码ImageRequest对象为特定于协议的数据(客户端)。
- · **ImageRequestDecoder**: 解码特定于协议的数据到ImageRequest对象(服务器使用)。
- · **ImageResponse**: 一个简单的POJO代表我们ImageServer回应。
- · **ImageResponseEncoder**: 服务器使用的ImageResponseEncoder编码对象。
- · **ImageResponseDecoder**: 客户端使用的ImageResponseDecoder解码对象。
- · **ImageCodecFactory**: 这个类创建需要的编码器和解码器。

这是ImageRequest类：

```java
public class ImageRequest {

    private int width;

    private int height;

    private int numberOfCharacters;

    public ImageRequest(int width, int height, int numberOfCharacters) {

        this.width = width;

        this.height = height;

        this.numberOfCharacters = numberOfCharacters;

    }

    public int getWidth() {

        return width;

    }

    public int getHeight() {

        return height;

    }

    public int getNumberOfCharacters() {

        return numberOfCharacters;

    }
}
```

通常编码比解码简单,所以先从ImageRequestEncoder开始:

```java
public class ImageRequestEncoder implements ProtocolEncoder {

    public void encode(IoSession session, Object message, ProtocolEncoderOutput out) throws Exception {

        ImageRequest request = (ImageRequest) message;

        IoBuffer buffer = IoBuffer.allocate(12, false);

        buffer.putInt(request.getWidth());

        buffer.putInt(request.getHeight());

        buffer.putInt(request.getNumberOfCharacters());

        buffer.flip();

        out.write(buffer);

    }

    public void dispose(IoSession session) throws Exception {

        // nothing to dispose

    }
}
```

备注：
- Mina将调用编码函数IoSession的写入队列中的所有消息。因为我们的客户只会写ImageRequest对象,我们可以安全地把消息传给ImageRequest。
- 我们从堆中分配一个新的IoBuffer。最好避免使用直接缓冲区,因为一般堆缓冲区表现的更好。参见：[http://issues.apache.org/jira/browse/DIRMINA-289](http://issues.apache.org/jira/browse/DIRMINA-289)。
- 你不需要释放缓冲区,Mina帮你做了这个操作。参见：[http://mina.apache.org/mina-project/apidocs/org/apache/mina/core/buffer/IoBuffer.html](http://mina.apache.org/mina-project/apidocs/org/apache/mina/core/buffer/IoBuffer.html)
- 在dispose()方法应该释放所有资源指定会话期间获得编码。如果没有处理,你可以让你的编码器从ProtocolEncoderAdapter继承。



**现在让我们看一看解码器。**CumulativeProtocolDecoder为我们提供很大帮助实现编写自己的解码器:它会缓冲所有传入的数据,直到你的解码器决定它能做些什么。在这种情况下,消息都有一个固定大小的,所以是最容易等到所有可用数据:

```java
public class ImageRequestDecoder extends CumulativeProtocolDecoder {

 

    protected boolean doDecode(IoSession session, IoBuffer in, ProtocolDecoderOutput out) throws Exception {

        if (in.remaining() >= 12) {

            int width = in.getInt();

            int height = in.getInt();

            int numberOfCharachters = in.getInt();

            ImageRequest request = new ImageRequest(width, height, numberOfCharachters);

            out.write(request);

            return true;

        } else {

            return false;

        }

    }
}
```

备注：
- 每次一个完整信息解码,你应该把它写入ProtocolDecoderOutput;这些消息将沿着你IoHandler过滤器链,最终抵达messageReceived方法。
- 你不需要负责释放IoBuffer。
- 当没有足够的数据解码信息,返回false。

响应也是一个非常简单的POJO:

```java
public class ImageResponse {

    private BufferedImage image1;

    private BufferedImage image2;

    public ImageResponse(BufferedImage image1, BufferedImage image2) {

        this.image1 = image1;

        this.image2 = image2;

    }

    public BufferedImage getImage1() {

        return image1;

    }

    public BufferedImage getImage2() {

        return image2;

    }
}
```

编码的响应也简单:

```java
public class ImageResponseEncoder extends ProtocolEncoderAdapter {

 

    public void encode(IoSession session, Object message, ProtocolEncoderOutput out) throws Exception {

        ImageResponse imageResponse = (ImageResponse) message;

        byte[] bytes1 = getBytes(imageResponse.getImage1());

        byte[] bytes2 = getBytes(imageResponse.getImage2());

        int capacity = bytes1.length + bytes2.length + 8;

        IoBuffer buffer = IoBuffer.allocate(capacity, false);

        buffer.setAutoExpand(true);

        buffer.putInt(bytes1.length);

        buffer.put(bytes1);

        buffer.putInt(bytes2.length);

        buffer.put(bytes2);

        buffer.flip();

        out.write(buffer);

    }

 

    private byte[] getBytes(BufferedImage image) throws IOException {

        ByteArrayOutputStream baos = new ByteArrayOutputStream();

        ImageIO.write(image, "PNG", baos);

        return baos.toByteArray();

    }
}
```

备注：
- 当它不可能事先计算IoBuffer的长度时，你可以通过使用一个auto-expanding（动态扩展的）缓冲对象调用buffer.setAutoExpand(true);

现在让我们看一看解码的响应:

```java
public class ImageResponseDecoder extends CumulativeProtocolDecoder {

    private static final String DECODER_STATE_KEY = ImageResponseDecoder.class.getName() + ".STATE";

    public static final int MAX_IMAGE_SIZE = 5 * 1024 * 1024;

    private static class DecoderState {

        BufferedImage image1;

    }

    protected boolean doDecode(IoSession session, IoBuffer in, ProtocolDecoderOutput out) throws Exception {

        DecoderState decoderState = (DecoderState) session.getAttribute(DECODER_STATE_KEY);

        if (decoderState == null) {

            decoderState = new DecoderState();

            session.setAttribute(DECODER_STATE_KEY, decoderState);

        }

        if (decoderState.image1 == null) {

            // try to read first image

            if (in.prefixedDataAvailable(4, MAX_IMAGE_SIZE)) {

                decoderState.image1 = readImage(in);

            } else {

                // not enough data available to read first image

                return false;

            }

        }

        if (decoderState.image1 != null) {

            // try to read second image

            if (in.prefixedDataAvailable(4, MAX_IMAGE_SIZE)) {

                BufferedImage image2 = readImage(in);

                ImageResponse imageResponse = new ImageResponse(decoderState.image1, image2);

                out.write(imageResponse);

                decoderState.image1 = null;

                return true;

            } else {

                // not enough data available to read second image

                return false;

            }

        }

        return false;

    }


    private BufferedImage readImage(IoBuffer in) throws IOException {

        int length = in.getInt();

        byte[] bytes = new byte[length];

        in.get(bytes);

        ByteArrayInputStream bais = new ByteArrayInputStream(bytes);

        return ImageIO.read(bais);

    }
}
```

备注：

我们解码过程的状态存储在会话属性。它也可以存储这个状态在解码器对象本身,但这个有几个缺点:
- 每个IoSession需要自己的解码器实例
- Mina确保不会有多个线程同时执行同一IoSession的decode()函数,但它并不能保证它永远是相同的线程。假设第一个数据是由线程1决定它不能解码,一到下一块数据,这可能是由另一个线程处理。为了避免可见性问题,你必须正确地同步访问这个解码器状态(存储在一个ConcurrentHashMap
 IoSession属性,所以它们对其他线程自动可见)。
- 邮件列表上的讨论已经导致这一结论:选择存储状态IoSession或解码器实例本身更佳。以确保没有两个线程将运行相同的解码方法IoSession,Mina需要做某种形式的同步=
 >这种同步也会确保你不可能上面描述的可见性问题。(感谢亚当Fisk指出这点,)参见：[http://www.nabble.com/Tutorial-on-ProtocolCodecFilter,-state-and-threads-t3965413.html](http://www.nabble.com/Tutorial-on-ProtocolCodecFilter,-state-and-threads-t3965413.html)

IoBuffer.prefixedDataAvailable()很方便当你的协议使用长度前缀;它支持一个前缀1、2或4字节。

不要忘记重启解码器状态当你解码一个响应(删除会话属性是另一种方式)。



如果响应将包含一个图像,我们不需要存储解码器状态:

```java
protected boolean doDecode(IoSession session, IoBuffer in, ProtocolDecoderOutput out) throws Exception {

    if (in.prefixedDataAvailable(4)) {

        int length = in.getInt();

        byte[] bytes = new byte[length];

        in.get(bytes);

        ByteArrayInputStream bais = new ByteArrayInputStream(bytes);

        BufferedImage image = ImageIO.read(bais);

        out.write(image);

        return true;

    } else {

        return false;

    }
}
```

现在让我们将代码合并在一起:

```java
public class ImageCodecFactory implements ProtocolCodecFactory {

    private ProtocolEncoder encoder;

    private ProtocolDecoder decoder;

 

    public ImageCodecFactory(boolean client) {

        if (client) {

            encoder = new ImageRequestEncoder();

            decoder = new ImageResponseDecoder();

        } else {

            encoder = new ImageResponseEncoder();

            decoder = new ImageRequestDecoder();

        }

    }

 

    public ProtocolEncoder getEncoder(IoSession ioSession) throws Exception {

        return encoder;

    }

 

    public ProtocolDecoder getDecoder(IoSession ioSession) throws Exception {

        return decoder;

    }
}
```

备注：
- 每一个新会话,Mina会请求ImageCodecFactory编码器和译码器。
- 因为我们的编码器和译码器存储会话状态,它是安全的,让所有会话共享一个实例。



**这是服务器如何使用ProtocolCodecFactory：**

```java
public class ImageServer {

    public static final int PORT = 33789;

 

    public static void main(String[] args) throws IOException {

        ImageServerIoHandler handler = new ImageServerIoHandler();

        NioSocketAcceptor acceptor = new NioSocketAcceptor();

        acceptor.getFilterChain().addLast("protocol", new ProtocolCodecFilter(new ImageCodecFactory(false)));

        acceptor.setLocalAddress(new InetSocketAddress(PORT));

        acceptor.setHandler(handler);

        acceptor.bind();

        System.out.println("server is listenig at port " + PORT);

    }
}
```

客户机使用的是相同的:

```java
public class ImageClient extends IoHandlerAdapter {

    public static final int CONNECT_TIMEOUT = 3000;

 

    private String host;

    private int port;

    private SocketConnector connector;

    private IoSession session;

    private ImageListener imageListener;

 

    public ImageClient(String host, int port, ImageListener imageListener) {

        this.host = host;

        this.port = port;

        this.imageListener = imageListener;

        connector = new NioSocketConnector();

        connector.getFilterChain().addLast("codec", new ProtocolCodecFilter(new ImageCodecFactory(true)));

        connector.setHandler(this);

    }

 

    public void messageReceived(IoSession session, Object message) throws Exception {

        ImageResponse response = (ImageResponse) message;

        imageListener.onImages(response.getImage1(), response.getImage2());

    }

    ...
```

出于完整性的考虑,我将添加服务器端IoHandler的代码:

```java
public class ImageServerIoHandler extends IoHandlerAdapter {

 

    private final static String characters = "mina rocks abcdefghijklmnopqrstuvwxyz0123456789";

 

    public static final String INDEX_KEY = ImageServerIoHandler.class.getName() + ".INDEX";

 

    private Logger logger = LoggerFactory.getLogger(this.getClass());

 

    public void sessionOpened(IoSession session) throws Exception {

        session.setAttribute(INDEX_KEY, 0);

    }

 

    public void exceptionCaught(IoSession session, Throwable cause) throws Exception {

        IoSessionLogger sessionLogger = IoSessionLogger.getLogger(session, logger);

        sessionLogger.warn(cause.getMessage(), cause);

    }

 

    public void messageReceived(IoSession session, Object message) throws Exception {

        ImageRequest request = (ImageRequest) message;

        String text1 = generateString(session, request.getNumberOfCharacters());

        String text2 = generateString(session, request.getNumberOfCharacters());

        BufferedImage image1 = createImage(request, text1);

        BufferedImage image2 = createImage(request, text2);

        ImageResponse response = new ImageResponse(image1, image2);

        session.write(response);

    }

 

    private BufferedImage createImage(ImageRequest request, String text) {

        BufferedImage image = new BufferedImage(request.getWidth(), request.getHeight(), BufferedImage.TYPE_BYTE_INDEXED);

        Graphics graphics = image.createGraphics();

        graphics.setColor(Color.YELLOW);

        graphics.fillRect(0, 0, image.getWidth(), image.getHeight());

        Font serif = new Font("serif", Font.PLAIN, 30);

        graphics.setFont(serif);

        graphics.setColor(Color.BLUE);

        graphics.drawString(text, 10, 50);

        return image;

    }

 

    private String generateString(IoSession session, int length) {

        Integer index = (Integer) session.getAttribute(INDEX_KEY);

        StringBuffer buffer = new StringBuffer(length);

 

        while (buffer.length() < length) {

            buffer.append(characters.charAt(index));

            index++;

            if (index >= characters.length()) {

                index = 0;

            }

        }

        session.setAttribute(INDEX_KEY, index);

        return buffer.toString();

    }
}
```

**swing客户端的代码可以在Mina源码例子中找到（[在线阅读](http://www.boyunjian.com/javasrc/org.apache.mina/mina-example/2.0.4/_/org/apache/mina/example/imagine/step1/client/GraphicalCharGenClient.java)）：**


![](https://img-blog.csdn.net/20160607110131229)

### 1.1.4. **总结**

这里有有更多讲解编码和解码。但我希望这个教程已经让你开始。在不久的将来，我将试着加一些关于DemuxingProtocolCodecFactory，然后我们也会看一看如何使用分隔符,而不是长度的前缀。



