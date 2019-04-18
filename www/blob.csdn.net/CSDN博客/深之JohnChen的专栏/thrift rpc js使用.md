# thrift rpc js使用 - 深之JohnChen的专栏 - CSDN博客

2018年12月07日 11:04:37[byxdaz](https://me.csdn.net/byxdaz)阅读数：268


一、thrift js使用方式

使用thrift js时，PRotocol需要使用TJSONPRotocol/TBinaryProtocol协议，Thrift.Protocol对应的是TJSONPRotocol。Transport需要使用TXHRTransport/TWebSocketTransport通道，Thrift.Transport对应的是TXHRTransport。

thrift-0.10.0源代码中的js代码在thrift-0.10.0\lib\js目录中。test目录下的src目录是java程序，test.html是测试页面。

thrift js使用场景：

Node.js JavaScript 服务端（Node.js本身支持http协议），thrift js客户端

java或vertx等服务端（需要支持http协议），thrift js客户端

二、实例

1、书写test.thrift接口，使用thrift-0.10.0.exe生成java、js的数据结构和代理类。

2、java程序

2.1、java程序中，实现Iface接口方法。

2.2、java服务器程序

Httpd.java

```
package www.junchen.com;

import java.io.File;
import java.io.IOException;
import java.io.InterruptedIOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URLDecoder;
import java.util.Collection;
import java.util.Iterator;
import java.util.Locale;

import org.apache.http.ConnectionClosedException;
import org.apache.http.HttpEntity;
import org.apache.http.HttpEntityEnclosingRequest;
import org.apache.http.HttpException;
import org.apache.http.HttpRequest;
import org.apache.http.HttpResponse;
import org.apache.http.HttpServerConnection;
import org.apache.http.HttpStatus;
import org.apache.http.MethodNotSupportedException;
import org.apache.http.entity.ContentProducer;
import org.apache.http.entity.EntityTemplate;
import org.apache.http.entity.FileEntity;
import org.apache.http.impl.DefaultHttpResponseFactory;
import org.apache.http.impl.DefaultHttpServerConnection;
import org.apache.http.impl.NoConnectionReuseStrategy;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.CoreConnectionPNames;
import org.apache.http.params.CoreProtocolPNames;
import org.apache.http.params.HttpParams;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.BasicHttpProcessor;
import org.apache.http.protocol.HttpContext;
import org.apache.http.protocol.HttpProcessor;
import org.apache.http.protocol.HttpRequestHandler;
import org.apache.http.protocol.HttpRequestHandlerRegistry;
import org.apache.http.protocol.HttpService;
import org.apache.http.util.EntityUtils;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TJSONProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TMemoryBuffer;

import eu.medsea.mimeutil.MimeType;
import eu.medsea.mimeutil.MimeUtil2;
import eu.medsea.mimeutil.detector.ExtensionMimeDetector;

public class Httpd {

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
        if (args.length < 1) {
            System.err.println("Please specify document root directory");
            System.exit(1);
        }
        Thread t = new RequestListenerThread(9090, args[0]);
        t.setDaemon(false);
        t.start();
	}

    static class HttpFileHandler implements HttpRequestHandler {

        private final String docRoot;

        public HttpFileHandler(final String docRoot) {
            super();
            this.docRoot = docRoot;
        }

        public void handle(final HttpRequest request, final HttpResponse response, final HttpContext context) throws HttpException, IOException {

            String method = request.getRequestLine().getMethod().toUpperCase(Locale.ENGLISH);
            if (!method.equals("GET") && !method.equals("HEAD") && !method.equals("POST")) {
                throw new MethodNotSupportedException(method + " method not supported");
            }
            String target = request.getRequestLine().getUri();

            if (request instanceof HttpEntityEnclosingRequest && target.equals("/service")) {
                HttpEntity entity = ((HttpEntityEnclosingRequest) request).getEntity();
                byte[] entityContent = EntityUtils.toByteArray(entity);
                System.out.println("Incoming content: " + new String(entityContent));
                
                final String output = this.thriftRequest(entityContent);
                
                System.out.println("Outgoing content: "+output);
                ContentProducer producer = new ContentProducer() {

					@Override
					public void writeTo(OutputStream outstream) throws IOException {
						// TODO Auto-generated method stub
                        OutputStreamWriter writer = new OutputStreamWriter(outstream, "UTF-8");
                        writer.write(output);
                        writer.flush();
					}
                };
                EntityTemplate body = new EntityTemplate(producer);
                body.setContentType("text/html; charset=UTF-8");
                response.setEntity(body);
            } else {
                if(target.indexOf("?") != -1) {
                 target = target.substring(1, target.indexOf("?"));
                }

                final File file = new File(this.docRoot, URLDecoder.decode(target, "UTF-8"));

                if (!file.exists()) {

                    response.setStatusCode(HttpStatus.SC_NOT_FOUND);
                    EntityTemplate body = new EntityTemplate(new ContentProducer() {

                        public void writeTo(final OutputStream outstream) throws IOException {
                            OutputStreamWriter writer = new OutputStreamWriter(outstream, "UTF-8");
                            writer.write("<html><body><h1>");
                            writer.write("File ");
                            writer.write(file.getPath());
                            writer.write(" not found");
                            writer.write("</h1></body></html>");
                            writer.flush();
                        }

                    });
                    body.setContentType("text/html; charset=UTF-8");
                    response.setEntity(body);
                    System.out.println("File " + file.getPath() + " not found");

                } else if (!file.canRead() || file.isDirectory()) {

                    response.setStatusCode(HttpStatus.SC_FORBIDDEN);
                    EntityTemplate body = new EntityTemplate(new ContentProducer() {

                        public void writeTo(final OutputStream outstream) throws IOException {
                            OutputStreamWriter writer = new OutputStreamWriter(outstream, "UTF-8");
                            writer.write("<html><body><h1>");
                            writer.write("Access denied");
                            writer.write("</h1></body></html>");
                            writer.flush();
                        }

                    });
                    body.setContentType("text/html; charset=UTF-8");
                    response.setEntity(body);
                    System.out.println("Cannot read file " + file.getPath());

                } else {

                    String mimeType = "application/octet-stream";
                    MimeUtil2 mimeUtil = new MimeUtil2();
                    synchronized (this) {
                        mimeUtil.registerMimeDetector(ExtensionMimeDetector.class.getName());
                    }
                    Collection<MimeType> collection = mimeUtil.getMimeTypes(file);
                    Iterator<MimeType> iterator = collection.iterator();
                    while(iterator.hasNext()) {
                        MimeType mt = iterator.next();
                        mimeType =  mt.getMediaType() + "/" + mt.getSubType();
                        break;
                    }

                    response.setStatusCode(HttpStatus.SC_OK);
                    FileEntity body = new FileEntity(file, mimeType);
                    response.addHeader("Content-Type", mimeType);
                    response.setEntity(body);
                    System.out.println("Serving file " + file.getPath());

                }
            }
        }
        
        private String thriftRequest(byte[] input){
            try{
            
                //Input
                TMemoryBuffer inbuffer = new TMemoryBuffer(input.length);           
                inbuffer.write(input);              
                TProtocol  inprotocol   = new TJSONProtocol(inbuffer);                   
                
                //Output
                TMemoryBuffer outbuffer = new TMemoryBuffer(100);           
                TProtocol outprotocol   = new TJSONProtocol(outbuffer);
                
                TProcessor processor = new Test.Processor(new TestImpl());      
                processor.process(inprotocol, outprotocol);
                
                byte[] output = new byte[outbuffer.length()];
                outbuffer.readAll(output, 0, output.length);
            
                return new String(output,"UTF-8");
            }catch(Throwable t){
                return "Error:"+t.getMessage();
            }
             
                     
        }
        
    }

    static class RequestListenerThread extends Thread {

        private final ServerSocket serversocket;
        private final HttpParams params;
        private final HttpService httpService;
        private final String docroot;

        public RequestListenerThread(int port, final String docroot) throws IOException {
        	this.docroot = docroot;
            this.serversocket = new ServerSocket(port);
            this.params = new BasicHttpParams();
            this.params.setIntParameter(CoreConnectionPNames.SO_TIMEOUT, 1000).setIntParameter(CoreConnectionPNames.SOCKET_BUFFER_SIZE, 8 * 1024)
                    .setBooleanParameter(CoreConnectionPNames.STALE_CONNECTION_CHECK, false).setBooleanParameter(CoreConnectionPNames.TCP_NODELAY, true)
                    .setParameter(CoreProtocolPNames.ORIGIN_SERVER, "HttpComponents/1.1");

            // Set up the HTTP protocol processor
            HttpProcessor httpproc = new BasicHttpProcessor();

            // Set up request handlers
            HttpRequestHandlerRegistry reqistry = new HttpRequestHandlerRegistry();
            reqistry.register("*", new HttpFileHandler(docroot));

            // Set up the HTTP service
            this.httpService = new HttpService(httpproc, new NoConnectionReuseStrategy(), new DefaultHttpResponseFactory());
            this.httpService.setParams(this.params);
            this.httpService.setHandlerResolver(reqistry);
        }

        public void run() {
            System.out.println("Listening on port " + this.serversocket.getLocalPort());
            System.out.println("Point your browser to http://localhost:9090/thrift-web-test/test.html");
            
            while (!Thread.interrupted()) {
                try {
                    // Set up HTTP connection
                    Socket socket = this.serversocket.accept();
                    DefaultHttpServerConnection conn = new DefaultHttpServerConnection();
                    System.out.println("Incoming connection from " + socket.getInetAddress());
                    conn.bind(socket, this.params);

                    // Start worker thread
                    Thread t = new WorkerThread(this.httpService, conn);
                    t.setDaemon(true);
                    t.start();
                } catch (InterruptedIOException ex) {
                    break;
                } catch (IOException e) {
                    System.err.println("I/O error initialising connection thread: " + e.getMessage());
                    break;
                }
            }
        }
    }

    static class WorkerThread extends Thread {

        private final HttpService httpservice;
        private final HttpServerConnection conn;

        public WorkerThread(final HttpService httpservice, final HttpServerConnection conn) {
            super();
            this.httpservice = httpservice;
            this.conn = conn;
        }

        public void run() {
            System.out.println("New connection thread");
            HttpContext context = new BasicHttpContext(null);
            try {
                while (!Thread.interrupted() && this.conn.isOpen()) {
                    this.httpservice.handleRequest(this.conn, context);
                }
            } catch (ConnectionClosedException ex) {
                System.err.println("Client closed connection");
            } catch (IOException ex) {
                System.err.println("I/O error: " + ex.getMessage());
            } catch (HttpException ex) {
                System.err.println("Unrecoverable HTTP protocol violation: " + ex.getMessage());
            } finally {
                try {
                    this.conn.shutdown();
                } catch (IOException ignore) {
                }
            }
        }

    }
}
```

上面程序依赖于thrift-0.10.0代码和一些jar包。如下图：

![](https://img-blog.csdnimg.cn/20181207111412598.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2J5eGRheg==,size_16,color_FFFFFF,t_70)

3、html页面

test.html

```
<!DOCTYPE html>
    <html lang="en">
      <head>
        <meta charset="utf-8">
        <title>Hello Thrift</title>
      </head>
      <body>
        number1: <input type="text" id="number1">
        number2: <input type="text" id="number2">
        result: <input type="text" id="result">
        <input type="button" id="add" value="add" >
        <div id="output"></div>
  
        <script src="thrift.js"></script>
        <script src="gen-js/test_types.js"></script>
        <script src="gen-js/Test.js"></script>
        <script>
          (function() {
            //var transport = new Thrift.Transport("http://localhost:9090/service");
            var transport = new Thrift.Transport("/service");
            var protocol  = new Thrift.TJSONProtocol(transport);
            var client    = new TestClient(protocol);
            var number1 = document.getElementById("number1");
            var number2 = document.getElementById("number2");
            var resultElement = document.getElementById("result");
            var outputElement = document.getElementById("output");
            document.getElementById("add").addEventListener("click", function(){
                client.add(number1.value,number2.value, function(result) {
                  outputElement.innerHTML = result;
                  resultElement.value = result;
                });
              });
          })();
        </script>
      </body>
    </html>
```

注意：

1）、将web页面，包含thrift.js等资源文件拷贝到一个目录下。

2）、执行java程序时，需要指定对应的web页面的根目录，比如d:

3）、跨域问题，将web页面放到java程序配置的程序目录中。
[完整代码下载](https://download.csdn.net/download/byxdaz/10834386)

