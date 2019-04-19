# Android OkHttp文件上传与下载的进度监听扩展 - =朝晖= - 博客园
# [Android OkHttp文件上传与下载的进度监听扩展](https://www.cnblogs.com/dhcn/p/7130470.html)
http://www.loongwind.com/archives/290.html
　　上一篇文章介绍了用Retrofit实现文件的上传与下载,但是我们发现没办法监听上传下载的进度,毕竟我们在做开发的时候经常是要显示上传或者下载的进度了.虽然Retrofit没有给我们提供现成的api来监听进度,但是Retrofit很灵活,它底层网络访问是用的okhttp实现的,当然我们也可以设置其他第三方网络请求库,因为Retrofit可以设置client,我们可以由此来扩展下载上传的进度监听.
　　本文使用okhttp作为client来做,其实说白了跟用okhttp做下载上传进度监听几乎一样,参考了这篇文章:[Android OkHttp文件上传与下载的进度监听扩展](http://blog.csdn.net/sbsujjbcy/article/details/48194701)
##### 1. 首先我们写两个接口用来下载和上传的进度监听回调:
```
/**
 * 请求体进度回调接口，用于文件上传进度回调
 */
public interface ProgressRequestListener {
    void onRequestProgress(long bytesWritten, long contentLength, boolean done);
}
/**
 * 响应体进度回调接口，用于文件下载进度回调
 */
public interface ProgressResponseListener {
    void onResponseProgress(long bytesRead, long contentLength, boolean done);
}
```
##### 2. 实现 ProgressRequestBody、ProgressResponseBody
ProgressRequestBody类继承RequestBody用于请求进度监听,用于文件上传进度监听:
```
import java.io.IOException;
import okhttp3.MediaType;
import okhttp3.RequestBody;
import okio.Buffer;
import okio.BufferedSink;
import okio.ForwardingSink;
import okio.Okio;
import okio.Sink;
import rx.Observable;
/**
 * 包装的请求体，处理进度
 */
public  class ProgressRequestBody extends RequestBody {
    //实际的待包装请求体
    private  RequestBody requestBody;
    //进度回调接口
    private  ProgressRequestListener progressListener;
    //包装完成的BufferedSink
    private BufferedSink bufferedSink;
    /**
     * 构造函数，赋值
     * @param requestBody 待包装的请求体
     * @param progressListener 回调接口
     */
    public ProgressRequestBody(RequestBody requestBody, ProgressRequestListener progressListener) {
        this.requestBody = requestBody;
        this.progressListener = progressListener;
    }
    /**
     * 重写调用实际的响应体的contentType
     * @return MediaType
     */
    @Override
    public MediaType contentType() {
        return requestBody.contentType();
    }
    /**
     * 重写调用实际的响应体的contentLength
     * @return contentLength
     * @throws IOException 异常
     */
    @Override
    public long contentLength() throws IOException {
        return requestBody.contentLength();
    }
    /**
     * 重写进行写入
     * @param sink BufferedSink
     * @throws IOException 异常
     */
    @Override
    public void writeTo(BufferedSink sink) throws IOException {
        if (bufferedSink == null) {
//            //包装
            bufferedSink = Okio.buffer(sink(sink));
        }
        //写入
        requestBody.writeTo(bufferedSink);
        //必须调用flush，否则最后一部分数据可能不会被写入
        bufferedSink.flush();
    }
    /**
     * 写入，回调进度接口
     * @param sink Sink
     * @return Sink
     */
    private Sink sink(Sink sink) {
        return new ForwardingSink(sink) {
            //当前写入字节数
            long bytesWritten = 0L;
            //总字节长度，避免多次调用contentLength()方法
            long contentLength = 0L;
            @Override
            public void write(Buffer source, long byteCount) throws IOException {
                super.write(source, byteCount);
                if (contentLength == 0) {
                    //获得contentLength的值，后续不再调用
                    contentLength = contentLength();
                }
                //增加当前写入的字节数
                bytesWritten += byteCount;
                //回调
                if(progressListener != null){
                    progressListener.onRequestProgress(bytesWritten, contentLength, bytesWritten == contentLength);
                }
            }
        };
    }
}
```
ProgressResponseBody继承ResponseBody用于下载进度监听:
```
package com.cm.retrofit.http;
/**
 * Created by Cmad on 2016/4/28.
 */
import java.io.IOException;
import okhttp3.MediaType;
import okhttp3.ResponseBody;
import okio.Buffer;
import okio.BufferedSource;
import okio.ForwardingSource;
import okio.Okio;
import okio.Source;
/**
 * 包装的响体，处理进度
 */
public class ProgressResponseBody extends ResponseBody {
    //实际的待包装响应体
    private final ResponseBody responseBody;
    //进度回调接口
    private final ProgressResponseListener progressListener;
    //包装完成的BufferedSource
    private BufferedSource bufferedSource;
    /**
     * 构造函数，赋值
     *
     * @param responseBody     待包装的响应体
     * @param progressListener 回调接口
     */
    public ProgressResponseBody(ResponseBody responseBody, ProgressResponseListener progressListener) {
        this.responseBody = responseBody;
        this.progressListener = progressListener;
    }
    /**
     * 重写调用实际的响应体的contentType
     *
     * @return MediaType
     */
    @Override
    public MediaType contentType() {
        return responseBody.contentType();
    }
    /**
     * 重写调用实际的响应体的contentLength
     *
     * @return contentLength
     * @throws IOException 异常
     */
    @Override
    public long contentLength() {
        return responseBody.contentLength();
    }
    /**
     * 重写进行包装source
     *
     * @return BufferedSource
     */
    @Override
    public BufferedSource source() {
        if (bufferedSource == null) {
            //包装
            bufferedSource = Okio.buffer(source(responseBody.source()));
        }
        return bufferedSource;
    }
    /**
     * 读取，回调进度接口
     *
     * @param source Source
     * @return Source
     */
    private Source source(Source source) {
        return new ForwardingSource(source) {
            //当前读取字节数
            long totalBytesRead = 0L;
            @Override
            public long read(Buffer sink, long byteCount) throws IOException {
                long bytesRead = super.read(sink, byteCount);
                //增加当前读取的字节数，如果读取完成了bytesRead会返回-1
                totalBytesRead += bytesRead != -1 ? bytesRead : 0;
                //回调，如果contentLength()不知道长度，会返回-1
                if(progressListener != null){
                    progressListener.onResponseProgress(totalBytesRead, responseBody.contentLength(), bytesRead == -1);
                }
                return bytesRead;
            }
        };
    }
}
```
上面两个类里的`writeTo`跟`source`方法用到了`sink`跟`Source`,这两个类属于Okio,也是Square开源的[Java](http://lib.csdn.net/base/java) io补充库,有兴趣的可以去了解一下,这里就不做详细介绍了.
##### 3. 实现HttpClientHelper类
HttpClientHelper用于创建okhttp client，并对okhttpclient添加拦截事件，将requestBody和responseBody替换成我们自己实现的ProgressRequestBody和ProgressResponseBody
```
package com.cm.retrofit.http;
import java.io.IOException;
import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
/**
 * Created by Cmad on 2016/4/28.
 */
public class HttpClientHelper {
    /**
     * 包装OkHttpClient，用于下载文件的回调
     * @param progressListener 进度回调接口
     * @return 包装后的OkHttpClient
     */
    public static OkHttpClient addProgressResponseListener(final ProgressResponseListener progressListener){
        OkHttpClient.Builder client = new OkHttpClient.Builder();
        //增加拦截器
        client.addInterceptor(new Interceptor() {
            @Override
            public Response intercept(Chain chain) throws IOException {
                //拦截
                Response originalResponse = chain.proceed(chain.request());
                //包装响应体并返回
                return originalResponse.newBuilder()
                        .body(new ProgressResponseBody(originalResponse.body(), progressListener))
                        .build();
            }
        });
        return client.build();
    }
    /**
     * 包装OkHttpClient，用于上传文件的回调
     * @param progressListener 进度回调接口
     * @return 包装后的OkHttpClient
     */
    public static OkHttpClient addProgressRequestListener(final ProgressRequestListener progressListener){
        OkHttpClient.Builder client = new OkHttpClient.Builder();
        //增加拦截器
        client.addInterceptor(new Interceptor() {
            @Override
            public Response intercept(Chain chain) throws IOException {
                Request original = chain.request();
                Request request = original.newBuilder()
                        .method(original.method(), new ProgressRequestBody(original.body(),progressListener))
                        .build();
                return chain.proceed(request);
            }
        });
        return client.build();
    }
}
```
##### 4. 使用
ServiceGenerator类:
```
package com.cm.retrofit.service;
import com.cm.retrofit.http.HttpClientHelper;
import com.cm.retrofit.http.ProgressRequestListener;
import com.cm.retrofit.http.ProgressResponseListener;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.converter.protobuf.ProtoConverterFactory;
import retrofit2.converter.scalars.ScalarsConverterFactory;
/**
 * Created by Cmad on 2016/4/28.
 */
public class ServiceGenerator {
    private static final String HOST = "http://www.xxx.com/ ";
    private static Retrofit.Builder builder = new Retrofit.Builder()
            .baseUrl(HOST)
            .addConverterFactory(ProtoConverterFactory.create())
            .addConverterFactory(ScalarsConverterFactory.create())
            .addConverterFactory(GsonConverterFactory.create());
    public static <T> T createService(Class<T> tClass){
        return builder.build().create(tClass);
    }
    /**
     * 创建带响应进度(下载进度)回调的service
     */
    public static <T> T createResponseService(Class<T> tClass, ProgressResponseListener listener){
        return builder
                .client(HttpClientHelper.addProgressResponseListener(listener))
                .build()
                .create(tClass);
    }
    /**
     * 创建带请求体进度(上传进度)回调的service
     */
    public static <T> T createReqeustService(Class<T> tClass, ProgressRequestListener listener){
        return builder
                .client(HttpClientHelper.addProgressRequestListener(listener))
                .build()
                .create(tClass);
    }
}
```
使用:
```
//用于上传
UpLoadService service = ServiceGenerator.createReqeustService(UpLoadService.class,this);
//下载
DownloadService downloadService = ServiceGenerator.createResponseService(DownloadService.class, this);
```
　　使用方法跟之前的一样,只是生成对应api service的时候调用带进度接口的方法就可以了,然后传入实现了回调接口的对象就可以.然后在回调里面进行界面的展示.
　　这样我们就实现了对上传下载的进度监听
##### 补充:
　　上传进度监听我们也可以不用在okhttp的拦截里设置,可以在请求封装的requestbody的时候将requestbody封装成ProgressRequestBody,如下:
```
//之前的请求方法
UpLoadService service = ServiceGenerator.createService(UpLoadService.class);
File file = new File(fileUri);
RequestBody requestFile =
         RequestBody.create(MediaType.parse("multipart/form-data"), file);
 MultipartBody.Part body =
         MultipartBody.Part.createFormData("file", file.getName(), requestFile);
Call<ResponseBody> call = service.upload(body); 
//添加进度回调
UpLoadService service = ServiceGenerator.createService(UpLoadService.class);
File file = new File(fileUri);
RequestBody requestFile =
         RequestBody.create(MediaType.parse("multipart/form-data"), file);
//将requestFile封装成ProgressRequestBody传入       
MultipartBody.Part body =
         MultipartBody.Part.createFormData("file", file.getName(), new ProgressRequestBody(requestFile,this));//this是在当前类实现了ProgressRequestListener接口
Call<ResponseBody> call = service.upload(body);
```
这样一样也可以实现对上传的进度监听.

