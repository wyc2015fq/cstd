# okhttp3简单封装GET和POST请求工具类（https） - z69183787的专栏 - CSDN博客
2019年02月12日 11:28:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：417
个人分类：[OkHttp-使用](https://blog.csdn.net/z69183787/article/category/8663601)
[https://blog.csdn.net/fxjzzyo/article/details/78761373](https://blog.csdn.net/fxjzzyo/article/details/78761373)
简单封装了okhttp3的工具类以便于以后直接拿来使用。
使用的okhttp版本为：
compile 'com.squareup.okhttp3:okhttp:3.8.1'
1
该工具类的功能如下：
Get请求，同步方式获取网络数据
Post请求，同步方式获取数据
Get请求，异步方式获取网络数据
Post请求，异步方式获取数据
支持HTTPS请求，自动跳过证书验证
判断当前网络是否可用
其中Post请求提交的是键值对<String,String>1. 完整代码
```java
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;
import java.io.IOException;
import java.security.SecureRandom;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.TimeUnit;
import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;
/**
 * Created by fxjzzyo on 2017/7/12.
 */
public class NetUtils {
    private static final byte[] LOCKER = new byte[0];
    private static NetUtils mInstance;
    private OkHttpClient mOkHttpClient;
    private NetUtils() {
        okhttp3.OkHttpClient.Builder ClientBuilder=new okhttp3.OkHttpClient.Builder();
        ClientBuilder.readTimeout(20, TimeUnit.SECONDS);//读取超时
        ClientBuilder.connectTimeout(6, TimeUnit.SECONDS);//连接超时
        ClientBuilder.writeTimeout(60, TimeUnit.SECONDS);//写入超时
        //支持HTTPS请求，跳过证书验证
        ClientBuilder.sslSocketFactory(createSSLSocketFactory());
        ClientBuilder.hostnameVerifier(new HostnameVerifier() {
            @Override
            public boolean verify(String hostname, SSLSession session) {
                return true;
            }
        });
        mOkHttpClient=ClientBuilder.build();
    }
    /**
     * 单例模式获取NetUtils
     * @return
     */
    public static NetUtils getInstance() {
        if (mInstance == null) {
            synchronized (LOCKER) {
                if (mInstance == null) {
                    mInstance = new NetUtils();
                }
            }
        }
        return mInstance;
    }
    /**
     * get请求，同步方式，获取网络数据，是在主线程中执行的，需要新起线程，将其放到子线程中执行
     * @param url
     * @return
     */
    public  Response getDataSynFromNet(String url) {
        //1 构造Request
        Request.Builder builder = new Request.Builder();
        Request request=builder.get().url(url).build();
        //2 将Request封装为Call
        Call call = mOkHttpClient.newCall(request);
        //3 执行Call，得到response
        Response response = null;
        try {
            response = call.execute();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return response;
    }
    /**
     * post请求，同步方式，提交数据，是在主线程中执行的，需要新起线程，将其放到子线程中执行
     * @param url
     * @param bodyParams
     * @return
     */
    public  Response postDataSynToNet(String url,Map<String,String> bodyParams) {
        //1构造RequestBody
        RequestBody body=setRequestBody(bodyParams);
        //2 构造Request
        Request.Builder requestBuilder = new Request.Builder();
        Request request = requestBuilder.post(body).url(url).build();
        //3 将Request封装为Call
        Call call = mOkHttpClient.newCall(request);
        //4 执行Call，得到response
        Response response = null;
        try {
            response = call.execute();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return response;
    }
    /**
     * 自定义网络回调接口
     */
    public interface MyNetCall{
        void success(Call call, Response response) throws IOException;
        void failed(Call call, IOException e);
    }
    /**
     * get请求，异步方式，获取网络数据，是在子线程中执行的，需要切换到主线程才能更新UI
     * @param url
     * @param myNetCall
     * @return
     */
    public  void getDataAsynFromNet(String url, final MyNetCall myNetCall) {
        //1 构造Request
        Request.Builder builder = new Request.Builder();
        Request request=builder.get().url(url).build();
        //2 将Request封装为Call
        Call call = mOkHttpClient.newCall(request);
        //3 执行Call
        call.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                myNetCall.failed(call,e);
            }
            @Override
            public void onResponse(Call call, Response response) throws IOException {
                myNetCall.success(call,response);
            }
        });
    }
    /**
     * post请求，异步方式，提交数据，是在子线程中执行的，需要切换到主线程才能更新UI
     * @param url
     * @param bodyParams
     * @param myNetCall
     */
    public  void postDataAsynToNet(String url, Map<String,String> bodyParams, final MyNetCall myNetCall) {
        //1构造RequestBody
        RequestBody body=setRequestBody(bodyParams);
        //2 构造Request
        Request.Builder requestBuilder = new Request.Builder();
        Request request = requestBuilder.post(body).url(url).build();
        //3 将Request封装为Call
        Call call = mOkHttpClient.newCall(request);
        //4 执行Call
        call.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                myNetCall.failed(call,e);
            }
            @Override
            public void onResponse(Call call, Response response) throws IOException {
                myNetCall.success(call,response);
            }
        });
    }
    /**
     * post的请求参数，构造RequestBody
     * @param BodyParams
     * @return
     */
    private RequestBody setRequestBody(Map<String, String> BodyParams){
        RequestBody body=null;
        okhttp3.FormBody.Builder formEncodingBuilder=new okhttp3.FormBody.Builder();
        if(BodyParams != null){
            Iterator<String> iterator = BodyParams.keySet().iterator();
            String key = "";
            while (iterator.hasNext()) {
                key = iterator.next().toString();
                formEncodingBuilder.add(key, BodyParams.get(key));
                Log.d("post http", "post_Params==="+key+"===="+BodyParams.get(key));
            }
        }
        body=formEncodingBuilder.build();
        return body;
    }
    /**
     * 判断网络是否可用
     * @param context
     * @return
     */
    public static boolean isNetworkAvailable(Context context) {
        ConnectivityManager cm = (ConnectivityManager) context
                .getSystemService(Context.CONNECTIVITY_SERVICE);
        if (cm == null) {
        } else {
      //如果仅仅是用来判断网络连接
      //则可以使用cm.getActiveNetworkInfo().isAvailable();
            NetworkInfo[] info = cm.getAllNetworkInfo();
            if (info != null) {
                for (int i = 0; i < info.length; i++) {
                    if (info[i].getState() == NetworkInfo.State.CONNECTED) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    /**
     * 生成安全套接字工厂，用于https请求的证书跳过
     * @return
     */
    public SSLSocketFactory createSSLSocketFactory() {
        SSLSocketFactory ssfFactory = null;
        try {
            SSLContext sc = SSLContext.getInstance("TLS");
            sc.init(null, new TrustManager[]{new TrustAllCerts()}, new SecureRandom());
            ssfFactory = sc.getSocketFactory();
        } catch (Exception e) {
        }
        return ssfFactory;
    }
    /**
     * 用于信任所有证书
     */
    class TrustAllCerts implements X509TrustManager {
        @Override
        public void checkClientTrusted(X509Certificate[] x509Certificates, String s) throws CertificateException {
        }
        @Override
        public void checkServerTrusted(X509Certificate[] x509Certificates, String s) throws CertificateException {
        }
        @Override
        public X509Certificate[] getAcceptedIssuers() {
            return new X509Certificate[0];
        }
    }
}
```
2. 用法举例
推荐使用异步请求，因为已经把网络请求自动放到子线程了，用起来稍简单。而同步请求还需要自己new Thread+handler来做，几乎和原始的网络请求没区别了。因此这里举例就只举异步请求了。
异步GET请求
点击按钮登录 
```java
public void login(View view) {
  final String account = etAccount.getText().toString();
  final String pass = etPassword.getText().toString();
  if (account.isEmpty() || pass.isEmpty()) {
    Toast.makeText(this, "用户名或密码不能为空！", Toast.LENGTH_SHORT).show();
           return;
       }
       if(!Global.isNetAvailable)
       {
           Toast.makeText(this, "网络不可用！", Toast.LENGTH_SHORT).show();
           return;
       }
       //进度条
       loginProgress.setVisibility(View.VISIBLE);
       //获取网络工具类实例
       NetUtils netUtils = NetUtils.getInstance();
       //请求网络，一句代码搞定
       netUtils.getDataAsynFromNet(Global.LOGIN + "?username=" + account + "&password=" + pass,
               new NetUtils.MyNetCall() {
                   @Override
                   public void success(Call call, Response response) throws IOException {
                       Log.i("tag", "success");
                       String result = response.body().string();
                       final ResponseBean responseBean = JSON.parseObject(result, ResponseBean.class);
                       if (responseBean != null) {
                           runOnUiThread(new Runnable() {
                               @Override
                               public void run() {
                                   loginProgress.setVisibility(View.GONE);
                                   String errcode = responseBean.getErrcode();
                                   if (errcode.equals("0")) {//登录成功
                                       //记录学号
                                       Global.account = account;
                                       //存储用户名密码
                                       saveUserName(account, pass);
                                       Intent intent = new Intent(LoginActivity.this, MainActivity.class);
                                       startActivity(intent);
                                       LoginActivity.this.finish();
                                   } else {
                                       Toast.makeText(LoginActivity.this, "请求失败！错误代码： " + errcode, Toast.LENGTH_SHORT).show();
                                   }
                               }
                           });
                       }
                   }
                   @Override
                   public void failed(Call call, IOException e) {
                       Log.i("tag", "failed");
                       runOnUiThread(new Runnable() {
                           @Override
                           public void run() {
                               loginProgress.setVisibility(View.GONE);
                               Toast.makeText(LoginActivity.this, "请求失败！", Toast.LENGTH_SHORT).show();
                           }
                       });
                   }
               }
       );
   }
```
异步POST请求
点击按钮，提交数据
```java
public void postSelect() {
        //确保选择了楼号
        if (tvTargetBuilding.getText().toString().isEmpty()) {
            Toast.makeText(getActivity(),"请选择楼号！",Toast.LENGTH_SHORT).show();
            return;
        }
        //构造请求参数
        Map<String, String> reqBody = new ConcurrentSkipListMap<>();
        reqBody.put("num", "1");
        reqBody.put("stuid", Global.account);
        reqBody.put("buildingNo", selectBuilding + "");
        //获取网络请求工具类实例
        NetUtils netUtils = NetUtils.getInstance();
        //提交数据
        netUtils.postDataAsynToNet(Global.SELECT_ROOM, reqBody, new NetUtils.MyNetCall() {
            @Override
            public void success(Call call, Response response) throws IOException {
                Log.i("tag", "success");
                String result = response.body().string();
                Log.i("tag", "result: " + result);
                //解析数据
                JSONObject jsonObject1 = JSON.parseObject(result);
                if (jsonObject1 != null) {
                    final int error_code = jsonObject1.getIntValue("error_code");
                    getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Log.i("tag", "errcode: " + error_code);
                            if (error_code == 0) {//提交成功
                                Toast.makeText(getActivity(), "选择成功！", Toast.LENGTH_SHORT).show();
                                //跳转到selectSuccessfragment
                                MainActivity.mainActivityInstance.switchFragment(getParentFragment(),SelectSuccessFragment.newInstance("", ""));
                            } else {
                                Toast.makeText(getActivity(), "选择失败！错误代码： " + error_code, Toast.LENGTH_SHORT).show();
                            }
                        }
                    });
                }
            }
            @Override
            public void failed(Call call, IOException e) {
                Log.i("tag", "failed");
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(getActivity(), "请求失败！", Toast.LENGTH_SHORT).show();
                    }
                });
            }
        });
    }
```
什么？还嫌代码太长？
其实很短的，除去注释和打印的日志以及一些不相干的部分（我这里的是一个实际的项目，所以涉及到一些其他的东西），你会发现代码很少，核心的代码就是一句。（这时候你可能要说，哼，是一句，但一句有十多行。。。拜托~不要那么懒啦好不好 -_-|||）
要注意的是获取数据后，更新UI要在主线程中。我这里使用的是runOnUiThread也可以用Handler。
用Handler后你会发现这一部分的代码很少了，因为只需要把获取的数据用Message发送到Handler处理就好了，这样就把代码分散开了
用runOnUiThread的好处是很直接，不过看起来代码都堆到一起，其实很方便的。
完结，撒花~
