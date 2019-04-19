# Retrofit 2.0 超能实践，完美支持Https传输 - =朝晖= - 博客园
# [Retrofit 2.0 超能实践，完美支持Https传输](https://www.cnblogs.com/dhcn/p/7125081.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/sk719887916/article/details/51597816
前阵子看到圈子里Retrofit 2.0，RxJava([Android](http://lib.csdn.net/base/android)), OkHttp3.3 ，加之支持[Android](http://lib.csdn.net/base/android)和 [iOS](http://lib.csdn.net/base/ios) 的[React Native](http://lib.csdn.net/base/reactnative) , 火的不要不要的, 2015年新技术一大波来袭 ，看着自己项目还在用HttpClient, AsyncTask的原生开发 感觉自己已成火星人，实在顶不住内心的自卑压力，加之对新技术的追求，入手[移动开发](http://lib.csdn.net/base/wechat)新三剑客，虽然目前关于他们的目前介绍的资料一大把，但是自己亲自实践后，发现坑不少，为了能方便其他人安全顺利入坑，今天就先从Retrofit说起，前方高能，准备躲避。
## Retrofit 2.0
Retrofit是SQUARE美国一家移动支付公司最近新发布的在[android](http://lib.csdn.net/base/android)平台上http访问的开源项目
# 一 什么Retrofit
官方标语；A type-safe HTTP client for Android and [Java](http://lib.csdn.net/base/javaee)
语意很明显一款android安全类型的http客户端， 那么怎么样才算安全？支持https？支持本地线程安全？ 
发现Rertofit其内部都是支持lambda语法（国内称只链式语法），内部支持okhttp, 并且支持响应式RxJAava，当然jdk1.8 和android studio工具也支持lambda。带着这些疑问 我开始探究一下。
在此之前准备入手资料：
> 
国外博客 
[https://inthecheesefactory.com/blog/retrofit-2.0/en](https://inthecheesefactory.com/blog/retrofit-2.0/en)
官方github 
[http://square.github.io/retrofit/](http://square.github.io/retrofit/)
## 二 Retrofit怎么使用
下文之前先给大家看下传统的httpclient（url） + AsyncTask实现的登录功能，这样我们才能发现Retrofit的优雅之处.
传统方式：
```
/**
 * Represents an asynchronous login/registration task used to authenticate
 * the user.
 */
public class UserLoginTask extends AsyncTask<Void, Void, Boolean> {
    private final String mEmail;
    private final String mPassword;
    UserLoginTask(String email, String password) {
        mEmail = email;
        mPassword = password;
    }
    @Override
    protected Boolean doInBackground(Void... params) {
        // TODO: attempt authentication against a network service.
        try {
            // Simulate network access.
            String result = "";
            BufferedReader in = null;
            String path ="http://localhost:8080/login/?" +"email =" + mEmail + "& password =" + mPassword;
            URL url =new URL(path);
            HttpURLConnection conn = (HttpURLConnection)url.openConnection();
            conn.setConnectTimeout(5 * 1000);
            conn.setRequestMethod("GET");
            InputStream inStream = conn.getInputStream();
            in = new BufferedReader(
                    new InputStreamReader(conn.getInputStream()));
            String line;
            while ((line = in.readLine()) != null)
            {
                result += "\n" + line;
            }
        }catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (ProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        for (String credential : DUMMY_CREDENTIALS) {
            String[] pieces = credential.split(":");
            if (pieces[0].equals(mEmail)) {
                // Account exists, return true if the password matches.
                return pieces[1].equals(mPassword);
            }
        }
        // TODO: register the new account here.
        return true;
    }
    @Override
    protected void onPostExecute(final Boolean success) {
        mAuthTask = null;
        if (success) {
            // do SomeThing
        } else {
            mPasswordView.setError(getString(R.string.error_incorrect_password));
            mPasswordView.requestFocus();
        }
    }
    @Override
    protected void onCancelled() {
        mAuthTask = null;
        showProgress(false);
    }
}
private void enterhome() {
    Intent intent = new Intent(LoginActivity.this, MainListActivity.class);
    startActivity(intent);
}
```
发现原理也很简单，点击loginbtn开启一个异步线程 在AsyncTask在 doInBackground中访问登录API，在onPostExecute中进行UI更新；也能很简单流畅的解决UI线程请求网络 非UI线程更新UI的问题, 接下来介绍用Retrofit来实现以上相同功能。
2 Rxtrofit
```
/**
 * 登录！
 */
private  void getLogin() {
    Retrofit retrofit = new Retrofit.Builder()
            .baseUrl("http://localhost:8080/")
            .addConverterFactory(GsonConverterFactory.create())
            .build();
    ApiManager apiService = retrofit.create(ApiManager.class);
    Call<LoginResult> call = apiService.getData("lyk", "1234");
   call.enqueue(new Callback<LoginResult>() {
       @Override
       public void onResponse(Call<LoginResult> call, Response<LoginResult> response) {
           if (response.isSuccess()) {
               // do SomeThing
           } else {
              //直接操作UI
           }
       }
       @Override
       public void onFailure(Call<LoginResult> call, Throwable t) {
           // do onFailure代码
       }
   });
}
```
ApiManager接口
```
/**
 * Created by LIUYONGKUI on 2016-05-03.
*/
public interface ApiManager {
 @GET("login/")
 Call<LoginResult> getData(@Query("name") String name, @Query("password") String pw);
```
好了 看了以上代码 或许你已经看到了他的链式优雅高大上的地方了，也许看不懂，也许会懵逼 但没关系我们继续入门。
## 1 配置gradle
```
compile 'com.squareup.retrofit2:retrofit:2.0.0-beta4'
compile 'com.squareup.retrofit2:converter-gson:2.0.0-beta4'
```
com.squareup.retrofit2:converter-gson:2.0.0-beta4 此依赖非必须，只是方便我对http返回的数据进行解析。
## 2 定义实例化
1》初始化Retrofit
```
Retrofit retrofit = new Retrofit.Builder()
          .baseUrl("http://localhost:8080/")
          .addConverterFactory(GsonConverterFactory.create())
           .build();
```
通过 Retrofit.Builder 来创建一个retrofit客户端，接着添加host url, 然后制定数据解析器，上面依赖的gson就是用在这里做默认数据返回的， 之后通过build()创建出来 
Retrofit也支持且内部自带如下格式：
- Gson: com.squareup.retrofit2:converter-gson
- Jackson: com.squareup.retrofit2:converter-jackson
- Moshi: com.squareup.retrofit2:converter-moshi
- Protobuf: com.squareup.retrofit2:converter-protobuf
- Wire: com.squareup.retrofit2:converter-wire
- Simple XML: com.squareup.retrofit2:converter-simplexml
- Scalars (primitives, boxed, and String): com.squareup.retrofit2:converter-scalars
2》编写API
```
@GET("login/")
  Call<LoginResult> getData(@Query("name") String name, @Query("password") String pw);
```
Call是支持Cloneable序列化的 并支持泛型，且此类Retrofit统一返回对象，支持Callback回调，我们可以传入制定的解析Modle，就会在主线程里返回对应的model数据，这里主要用注解@get @post s设置请求方式，后面“login/”是方法Url, @Query(“name”)来设定body的parameters.
3》 调用API 
Retrofit支持异步和同步，这里我们用call.enqueue(new Callback来采用异步请求，如果 call.execute() 则采用同步方式
```
Call<LoginResult> call = apiService.getData("lyk", "1234");
   call.enqueue(new Callback<LoginResult>() {
       @Override
       public void onResponse(Call<LoginResult> call, Response<LoginResult> response) {
       }
       @Override
       public void onFailure(Call<LoginResult> call, Throwable t) {
       }
   });
}
```
如果还未理解请阅读参考入门资料:[Retrofit 2.0：有史以来最大的改进](http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2015/0915/3460.html)
# 三 进阶拓展
通过以上的介绍和案列，我们了解了怎样运用Retrofit请求网络数据，展现数据更新UI，但实际开发中会存在很多问题，很多同学会遇到：Retrofit的内部Log都无法输出 , header怎么加入，请求怎么支持https，包括怎么结合RxJava.? 不用担心，这些Retrofit 2.0 都给我提供了自定义的Interceptor（拦截器），通过不同的Interceptor可以实现不同的自定义请求形式，比如统一加head，参数，加入证书（ssl）等，前提必须结合okhttp来实现 , 通过给OkHttpClient添加Interceptor，然后给Retrofit设置http客户端即可.Retrofit提供了 
.client(）方法供我们传入自定义的网络客户端，当然默然就是okhttps. 如果无法自动导包 需要我们自己添加对okhttp的依赖
> 
compile ‘com.squareup.okhttp3:okhttp:3.3.1’
OkHttp入门请移步：
~https://github.com/square/okhttp 
~ [OKHttp源码解析](http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2015/0326/2643.html)
## 1 开启Log
```
Retrofit retrofit = new Retrofit.Builder().client(new OkHttpClient.Builder()
                     .addNetworkInterceptor(
                                new   HttpLoggingInterceptor().setLevel(HttpLoggingInterceptor.Level.HEADERS))        .build())
```
## 2 增加头部信息
统一通用header
```
new Retrofit.Builder()
           .addConverterFactory(GsonConverterFactory.create())
           .client(new OkHttpClient.Builder()
                   .addInterceptor(new Interceptor() {
                       @Override
                       public Response intercept(Chain chain) throws IOException {
                           Request request = chain.request()
                                   .newBuilder()
                                   .addHeader("mac", "f8:00:ea:10:45")
                                   .addHeader("uuid", "gdeflatfgfg5454545e")
                                   .addHeader("userId", "Fea2405144")
                                   .addHeader("netWork", "wifi")
                                   .build();
                           return chain.proceed(request);
                       }
                   })
                   .build()
```
当然可以对单一的某个API加入header
```
@Headers({ 
"Accept: application/vnd.github.v3.full+json", 
"User-Agent: Retrofit-your-App"})‘
@get("users/{username}")
Call<User> getUser(@Path("username") String username);
```
## 3 添加证书Pinning
证书可以在自定义的OkHttpClient加入certificatePinner 实现
```
OkHttpClient client = new OkHttpClient.Builder()
    .certificatePinner(new CertificatePinner.Builder()
            .add("YOU API.com", "sha1/DmxUShsZuNiqPQsX2Oi9uv2sCnw=")
            .add("YOU API..com", "sha1/SXxoaOSEzPC6BgGmxAt/EAcsajw=")
            .add("YOU API..com", "sha1/blhOM3W9V/bVQhsWAcLYwPU6n24=")
            .add("YOU API..com", "sha1/T5x9IXmcrQ7YuQxXnxoCmeeQ84c=")
            .build())
```
## 4 支持https
加密和普通http客户端请求支持https一样，步骤如下：
> 
1 CertificateFactory 得到Context.getSocketFactory 
2 添加证书源文件 
3 绑定到okhttpClient 
4设置okhttpClient到retrofit中
证书同样可以设置到okhttpclient中，我们可以把证书放到raw路径下
```
SLSocketFactory sslSocketFactory =getSSLSocketFactory_Certificate(context,"BKS", R.raw.XXX);
```
准备证书源文件：
加入证书源文件，我的证书是放在Raw下面的： 
![这里写图片描述](http://img.blog.csdn.net/20160729200026890)
绑定证书
```
protected static SSLSocketFactory getSSLSocketFactory(Context context, int[] certificates) {
    if (context == null) {
        throw new NullPointerException("context == null");
    }
    CertificateFactory certificateFactory;
    try {
        certificateFactory = CertificateFactory.getInstance("X.509");
        KeyStore keyStore = KeyStore.getInstance(KeyStore.getDefaultType());
        keyStore.load(null, null);
        for (int i = 0; i < certificates.length; i++) {
            InputStream certificate = context.getResources().openRawResource(certificates[i]);
            keyStore.setCertificateEntry(String.valueOf(i), certificateFactory.generateCertificate(certificate));
            if (certificate != null) {
                certificate.close();
            }
        }
        SSLContext sslContext = SSLContext.getInstance("TLS");
        TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        trustManagerFactory.init(keyStore);
        sslContext.init(null, trustManagerFactory.getTrustManagers(), new SecureRandom());
       return sslContext.getSocketFactory();
```
指定支持的host
/** 
* set HostnameVerifier 
* {@link HostnameVerifier} 
*/ 
protected static HostnameVerifier getHostnameVerifier(final String[] hostUrls) {
```
HostnameVerifier TRUSTED_VERIFIER = new HostnameVerifier() {
            public boolean verify(String hostname, SSLSession session) {
                boolean ret = false;
                for (String host : hostUrls) {
                    if (host.equalsIgnoreCase(hostname)) {
                        ret = true;
                    }
                }
                return ret;
            }
        };
    return TRUSTED_VERIFIER;
}
```
设置setSocketFactory
```
okhttpBuilder.socketFactory(HttpsFactroy.getSSLSocketFactory(context,   certificates));
```
certificates 是你raw下证书源ID, `int[] certificates = {R.raw.myssl}`
设置setNameVerifie
```
okhttpBuilder.hostnameVerifier(HttpsFactroy.getHostnameVerifier(hosts));
```
hosts是你的host数据 列如 String hosts[]`= {“https//:aaaa,com”, “https//:bbb.com”}
实现自定义 添加到Retrofit
```
okHttpClient = okhttpBuilder.build();
        retrofit = new Retrofit.Builder()
            .client(okHttpClient)
            .build();
```
# 总结
看了以上的知识点你发现Retrofit同等支持RxJava,通过以下Call适配模式.就可以关联RxJava
```
retrofit .addCallAdapterFactory(RxJavaCallAdapterFactory.create())
```
关于 Retrofit+ RxJava的案列,以及实际遇到的坑下篇再介绍。RxJava也是一款强大的多线程通讯利器，让你的应用开发中无时无刻，随心所欲进行多线程响应式编程开发。
参考文章：
- [OkHttp使用教程](http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2015/0106/2275.html)
- [https://inthecheesefactory.com/blog/retrofit-2.0/en](https://inthecheesefactory.com/blog/retrofit-2.0/en)
- 支持原文：
- [http://blog.csdn.net/sk719887916/article/details/51597816](http://blog.csdn.net/sk719887916/article/details/51597816)
Retrofit系列:
- 
[Retrofit 2.0(一) 超能实践，完美支持Https传输](http://blog.csdn.net/sk719887916/article/details/51597816)
- 
[Retrofit2.0（二） 完美同步Cookie实现免登录](http://blog.csdn.net/sk719887916/article/details/51700659)
- 
[Retrofit 2.0 超能实践（三），轻松实现文件/图片上传](http://blog.csdn.net/sk719887916/article/details/51755427)
- 
[Retrofit 2.0 超能实践（四），完成大文件断点下载](http://blog.csdn.net/sk719887916/article/details/51988507)
- 
[基于Retrofit2.0 封装的超好用的RetrofitClient工具类](http://blog.csdn.net/sk719887916/article/details/51958010)
- 
[玩转IOC，教你徒手实现自定义的Retrofit框架](http://blog.csdn.net/sk719887916/article/details/51957819)
