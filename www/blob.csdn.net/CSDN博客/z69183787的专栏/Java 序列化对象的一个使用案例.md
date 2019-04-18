# Java 序列化对象的一个使用案例 - z69183787的专栏 - CSDN博客
2015年07月12日 10:24:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1188
《 Effective Java 》中序列化一节关于java的序列化存在如下说法：
对象序列化(object serialization)API，它提供了一个框架，用来将对象编码成字节流(serializing)，并从字节流编码中重新构建对象(deserializing)。一旦对象被序列化后，它的编码就可以从一台正在运行的jvm传到另一台jvm上，或者被存储在磁盘上，供以后反序列化时用(如tomcat的session的可持久化)。序列化技术为 远程通信 提供了标准的线路级对象表示法，也为JavaBeans组件结构提供了标准的 持久化 数据格式。
关于java序列化，看过不少资料，一直没有完全搞明白里面的奥义，正好最近在重读Effective Java，然后加上这两天碰到一个需求，能很好的诠释序列化，所以在这边小结下。
需求：系统中自己写了个内存缓存cache，然后集群内每个节点都单独维护自己各自的内存缓存（ps：有人肯定该说艾斯比，怎么不用Memcached或者Redis，么法，leader的要求…… )，这个内存缓存其实就是基于Map的，key=String，value=Object对象。cache需要提供这样一个方法，要能够刷新集群内所有节点的map中单个KV。
分析如上需求，map中value保存的是Object，这个Object并不是一般的String之类的，而恰恰就是一个真正名符其实的JavaBean对象，刷新时，需要将新的bean发送各节点，由各节点再各自操作内存中的map。这样，就出现了雏形，各节点需要提供一个http接口，刷新时，将bean对象发到到http上，这样就形成了上述Effective Java说到的场景，为远程通信 提供标准的线路级对象表示法，也即 将Object bean以stream的形式post到http接口，http接口接到stream后，将stream反序列化成bean对象。
代码实现如下：
javabean就不列了，是一个很纯粹的bean类，只不过该类一定是序列化后的（关于序列化的高级进阶请参考Effective Java），笔者这里只 implements Serializable 即可满足需求。
http post 工具类：
```java
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import org.apache.commons.io.IOUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
/**
 * Http 请求访问工具类
 * 
 * @author will_awoke
 * @version 2014-6-26
 * @see HttpAccessUtil
 * @since
 */
public class HttpAccessUtil
{
  /**
   * 日志 
   */
  private static Logger log = Logger.getLogger(HttpAccessUtil.class);
   
  /**
   * 采取post方式提交序列化后的object对象 </br>
   * 另请参考：java.io.ObjectInputStream/ObjectOutputStream
   * @param requestUrl 请求地址
   * @param connTimeoutMills 设置连接主机超时，单位：毫秒
   * @param readTimeoutMills 设置从主机读取数据超时，单位：毫秒
   * @param serializedObject 序列化后的object对象
   * 
   * @return remoteHttp返回的结果
   */
  public static String httpPostSerialObject(String requestUrl, int connTimeoutMills,
                        int readTimeoutMills, Object serializedObject) throws Exception	 
  {
    HttpURLConnection httpUrlConn = null;
    InputStream inputStream = null;
    InputStreamReader inputStreamReader = null;
    BufferedReader bufferedReader = null;
    ObjectOutputStream oos = null;
    StringBuffer buffer = new StringBuffer();
    try
    {
      URL url = new URL(requestUrl);
      httpUrlConn = (HttpURLConnection)url.openConnection();
      // 设置content_type=SERIALIZED_OBJECT
      // 如果不设此项,在传送序列化对象时,当WEB服务默认的不是这种类型时可能抛java.io.EOFException
      httpUrlConn.setRequestProperty("Content-Type","application/x-java-serialized-object");
      httpUrlConn.setConnectTimeout(connTimeoutMills);
      httpUrlConn.setReadTimeout(readTimeoutMills);
      // 设置是否向httpUrlConn输出，因为是post请求，参数要放在http正文内，因此需要设为true, 默认情况下是false
      httpUrlConn.setDoOutput(true);
      // 设置是否从httpUrlConn读入，默认情况下是true
      httpUrlConn.setDoInput(true);
      // 不使用缓存   
      httpUrlConn.setUseCaches(false);
      // 设置请求方式，默认是GET
      httpUrlConn.setRequestMethod("POST");
      httpUrlConn.connect();
      if (serializedObject != null)
      {
        // 此处getOutputStream会隐含的进行connect，即：如同调用上面的connect()方法，   
        // 所以在开发中不调用上述的connect()也可以，不过建议最好显式调用
        // write object(impl Serializable) using ObjectOutputStream
        oos = new ObjectOutputStream(httpUrlConn.getOutputStream());
        oos.writeObject(serializedObject);
        oos.flush();
        // outputStream不是一个网络流，充其量是个字符串流，往里面写入的东西不会立即发送到网络， 
        // 而是存在于内存缓冲区中，待outputStream流关闭时，根据输入的内容生成http正文。所以这里的close是必须的
        oos.close();
      }
      // 将返回的输入流转换成字符串
      // 无论是post还是get，http请求实际上直到HttpURLConnection的getInputStream()这个函数里面才正式发送出去
      inputStream = httpUrlConn.getInputStream();//注意，实际发送请求的代码段就在这里 
      inputStreamReader = new InputStreamReader(inputStream, "UTF-8");
      bufferedReader = new BufferedReader(inputStreamReader);
      String str = null;
      while ((str = bufferedReader.readLine()) != null)
      {
        buffer.append(str);
      }
    }
    catch (Exception e)
    {
      log.error(requestUrl + " error ", e);
      throw e;
    }
    finally
    {
      try
      {
        IOUtils.closeQuietly(bufferedReader);
        IOUtils.closeQuietly(inputStreamReader);
        IOUtils.closeQuietly(inputStream);
        IOUtils.closeQuietly(oos);
        if (httpUrlConn != null)
        {
          httpUrlConn.disconnect();
        }
      }
      catch (Exception e)
      {
        log.error(e);
      }
    }
    return buffer.toString();
  }
}
```
这里有点要着重说明下，httpConnect的Content-Type的一定要设置为 application/x-java-serialized-object ，否则http接口处在接收stream的时候会出现java.io.EOFException 。另外：输出对象使用的是ObjectOutputStream的writeObject方法。
http接口接收stream，并反序列化为Object bean即可：
//read object from stream 反序列化将stream生成Object对象
ObjectInputStream ois = new ObjectInputStream(req.getInputStream());//HttpServletRequest req对象
Object value = ois.readObject();后面就是replace map了，不再赘述。
总之，通过这个需求算是进一步了解了java序列化的作用 ：）
