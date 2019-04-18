# HttpClient4.x使用中出现的几个问题及解决方法 - z69183787的专栏 - CSDN博客
2016年10月25日 17:01:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3080
最近才开始接触这个工具包，官网上目前已经更新到[HttpClient4.5.2](http://hc.apache.org/downloads.cgi)了。google的时候看到HttpClient从4.0版本改了底层，因此使用时会出现一些方法不再适用的问题，这里记录出现的一些问题以及相应替代的方法。
1、 创建HttpClient时使用下面的语句，会出现`org.apache.http.impl.client.DefaultHttpClient' is deprecated`
```
HttpClient httpClient = new DefaultHttpClient();
```
可以使用如下语句代替：[来源](http://stackoverflow.com/questions/15336477/deprecated-java-httpclient-how-hard-can-it-be)
```
HttpClient httpClient = HttpClientBuilder.create().build();
```
或者
```
// 创建HttpClientBuilder
HttpClientBuilder httpClientBuilder = HttpClientBuilder.create();
// HttpClient
CloseableHttpClient httpClient = httpClientBuilder.build();
```
2、 设置代理，HttpClient4.5.2版本可以使用如下语句：
```
//创建代理
HttpHost proxy = new HttpHost("your proxy IP", port);
RequestConfig config = RequestConfig.custom().setProxy(proxy).build();
//设置HttpPost/HttpGet使用代理
httpost.setConfig(config);
```
3、get或post时，如果访问的网站是https协议的，可以用如下方式访问：信任所有证书，如果可以具体到每个证书，就更好了。
```
try {
            SSLContext sslContext = new SSLContextBuilder().loadTrustMaterial(null, new TrustStrategy() {
                //信任所有
                public boolean isTrusted(X509Certificate[] chain,
                                         String authType) throws CertificateException {
                    return true;
                }
            }).build();
            SSLConnectionSocketFactory sslsf = new SSLConnectionSocketFactory(sslContext);
            return HttpClients.custom().setSSLSocketFactory(sslsf).build();
        } catch (KeyManagementException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (KeyStoreException e) {
            e.printStackTrace();
        }
        return HttpClients.createDefault();
    }
```
