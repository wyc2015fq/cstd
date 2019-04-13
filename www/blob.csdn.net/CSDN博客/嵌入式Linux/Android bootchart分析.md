
# Android bootchart分析 - 嵌入式Linux - CSDN博客

2015年10月09日 10:02:16[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：3419



## 1.首先确保编译的init被烧录到板子里面去了，源码的位置在system/core/init/
## 2.第一次修改后，编译了system/core/init/然后又编译了./mkkernel 生成boot.img 但是烧录进去还是不成功
## 3.然后　发现有一个宏没有设置　在bootchart.h里面，BOOTCHART　修改后重新编译，烧了所有的文件才可以，单烧boot.img也不行。
一般都是这几个位置影响开机时间
•Bootloader init
•Kernel init
•Zygote class preloading
•Package scanning
•Service initialization
改善开机速度几个比较好的网站：
http://elinux.org/Improving_Android_Boot_Time_Outline
http://elinux.org/Improving_Android_Boot_Time

在机器的操作如下
\#echo 120 > /data/bootchart-start \#120 秒是超时时间,表示 bootchart 记
录多长时间后停止记录,随时可以用命令'echo 1 > /data/bootchart-stop' 停
止
\#mkdir /data/bootchart
\#在 data 创建该目录,用来存放生成的
记录文件
\#reboot
\#重启开发板后 bootchart 会开始记录

代码修改如下，加了一下日志

```python
<span style="font-family:Microsoft YaHei;">weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ git diff mediatek/config/mid713l_lp_lvds/ProjectConfig.mk
diff --git a/mediatek/config/mid713l_lp_lvds/ProjectConfig.mk b/mediatek/config/mid713l_lp_lvds/ProjectConfig.mk
index 42dc2b2..2b4e06b 100755
--- a/mediatek/config/mid713l_lp_lvds/ProjectConfig.mk
+++ b/mediatek/config/mid713l_lp_lvds/ProjectConfig.mk
@@ -1528,3 +1528,5 @@ ADUPS_FOTA_WITHOUT_MENU=no
 
 #HCN200 CUSTOM MACRO
 MID713L_PANEL_GAMMA=yes
+#weiqifa modify add
+INIT_BOOTCHART=true
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$

weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ git diff system/core/init/bootchart.h
diff --git a/system/core/init/bootchart.h b/system/core/init/bootchart.h
index 39d2d4f..d322114 100644
--- a/system/core/init/bootchart.h
+++ b/system/core/init/bootchart.h
@@ -18,7 +18,7 @@
 #define _BOOTCHART_H
 
 #ifndef BOOTCHART
-# define  BOOTCHART  0
+# define  BOOTCHART  1 //weiqifa modify
 #endif
 
 #if BOOTCHART
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ git diff system/core/init/init.c
diff --git a/system/core/init/init.c b/system/core/init/init.c
index 045e416..8c4f8af 100755
--- a/system/core/init/init.c
+++ b/system/core/init/init.c
@@ -968,11 +968,11 @@ static int bootchart_init_action(int nargs, char **args)
 {
     bootchart_count = bootchart_init();
     if (bootchart_count < 0) {
-        ERROR("bootcharting init failure\n");
+        printf("bootcharting init failure\n");
     } else if (bootchart_count > 0) {
-        NOTICE("bootcharting started (period=%d ms)\n", bootchart_count*BOOTCHART_POLLING_MS);
+        printf("bootcharting started (period=%d ms)\n", bootchart_count*BOOTCHART_POLLING_MS);
     } else {
-        NOTICE("bootcharting ignored\n");
+        printf("bootcharting ignored\n");
     }
 
     return 0;
@@ -1384,7 +1384,7 @@ int main(int argc, char **argv)
 #ifdef MTK_INIT
         init_parse_config_file("/init.project.rc");
 #ifdef MTK_MLC_NAND_SUPPORT</span>
```


## ４．通过busybox工具把data/bootchart／下面的文件打包后再pull出来
网址 http://www.busybox.net/downloads/binaries
下载　busybox-armv7l　这个　这个是arm架构的busybox
adb push busybox-armv7l sdcard/
adb shell
\#cd sdcard/
\#cp busybox-armv7l /system/xbin/
\#chmod 755 busybox-armv7l
\#./busybox-armv7l --install ./  要加后面的./说明是安装在当前的目录下
然后用　busybox-armv7l tar -czf bootchart.tgz *
adb pull data/bootchart/bootchart.tgz

$cd ../bootchart-0.9/
\#该目录是从 http://www.bootchart.org/download.html 下载的源码解压后的
目录,直接用 apt-get install bootchart 安装的 bootchart 不能使用.
$ant
$java -jar bootchart.jar ../bootchart/bootchart.tgz
\# 在 上 述 源 码 目 录 执 行 ant 命 令 编 译 后 后 生 成 bootchart.jar 文 件 ,
执行该命令后会在当前目录生成所需 bootchart.png 图片文件。
参考资料:
http://elinux.org/Using_Bootchart_on_Android

## ５．通过搜索关键字查找不同阶段开机的时间
ADP1 kernel boot time
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/log$ grep -wrn "Freeing init memory" ./
./mobilelog/APLog_2013_0101_000118/kernel_log.boot:2608:<6>[    8.803294].(0)Freeing init memory: 380K
apps 扫描时间
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/log$ grep -wrn "Time to scan packages" ./
./mobilelog/APLog_2013_0101_000118/sys_log.boot:778:01-01 23:14:32.442762   705   705 I PackageManager: Time to scan packages: 32.978 seconds
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/log$
## ６、测试　把system app下面的apk去掉一些
如图：
![](https://img-blog.csdn.net/20151104112302615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 得到去掉apk和去掉apk后两个开机的时间比较
![](https://img-blog.csdn.net/20151104112307909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## ７、在Zygote 孵化器　这里打印扫描哪些东西的时间
日志如下：

```python
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ adb shell logcat -s Zygote
--------- beginning of /dev/log/system
--------- beginning of /dev/log/main
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x501.DirectoryString...took3806ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x501.DirectoryString$1...took3806ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x501.Name...took3807ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x501.Name$1...took3807ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.AlgorithmIdentifier...took3807ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.AlgorithmIdentifier$1...took3807ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.BasicConstraints...took3807ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.BasicConstraints$1...took3808ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Certificate...took3812ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Certificate$1...took3812ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.EDIPartyName...took3812ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.EDIPartyName$1...took3812ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.ExtendedKeyUsage...took3812ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.ExtendedKeyUsage$1...took3830ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Extension...took3831ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Extension$1...took3831ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Extension$2...took3831ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.ExtensionValue...took3831ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Extensions...took3831ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Extensions$1...took3831ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.GeneralName...took3833ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.GeneralName$1...took3833ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.GeneralName$2...took3833ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.GeneralNames...took3833ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.GeneralNames$1...took3833ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.KeyUsage...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.ORAddress...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.ORAddress$1...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.ORAddress$2...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.OtherName...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.OtherName$1...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.SubjectPublicKeyInfo...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.SubjectPublicKeyInfo$1...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.TBSCertificate...took3834ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.TBSCertificate$1...took3835ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Time...took3835ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Time$1...took3835ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Validity...took3835ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.security.x509.Validity$1...took3835ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.xml.ExpatAttributes...took3835ms.
I/Zygote  (  188): Preloadingorg.apache.harmony.xml.ExpatParser...took3835ms.
I/Zygote  (  188): Preloadingorg.apache.http.ConnectionReuseStrategy...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.FormattedHeader...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.Header...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.HeaderElement...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.HeaderElementIterator...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.HeaderIterator...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpClientConnection...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpConnection...took3836ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpConnectionMetrics...took3839ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpEntity...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpEntityEnclosingRequest...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpHost...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpInetConnection...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpMessage...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpRequest...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpRequestInterceptor...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpResponse...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpResponseFactory...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpResponseInterceptor...took3840ms.
I/Zygote  (  188): Preloadingorg.apache.http.HttpVersion...took3841ms.
I/Zygote  (  188): Preloadingorg.apache.http.NameValuePair...took3842ms.
I/Zygote  (  188): Preloadingorg.apache.http.ProtocolVersion...took3842ms.
I/Zygote  (  188): Preloadingorg.apache.http.ReasonPhraseCatalog...took3842ms.
I/Zygote  (  188): Preloadingorg.apache.http.RequestLine...took3842ms.
I/Zygote  (  188): Preloadingorg.apache.http.StatusLine...took3842ms.
I/Zygote  (  188): Preloadingorg.apache.http.auth.AuthSchemeFactory...took3842ms.
I/Zygote  (  188): Preloadingorg.apache.http.auth.AuthSchemeRegistry...took3843ms.
I/Zygote  (  188): Preloadingorg.apache.http.auth.AuthState...took3843ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.AuthenticationHandler...took3843ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.CookieStore...took3846ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.CredentialsProvider...took3846ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.HttpClient...took3846ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.HttpRequestRetryHandler...took3846ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.RedirectHandler...took3846ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.RequestDirector...took3866ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.ResponseHandler...took3866ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.UserTokenHandler...took3866ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.methods.AbortableHttpRequest...took3866ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.methods.HttpEntityEnclosingRequestBase...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.methods.HttpGet...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.methods.HttpPost...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.methods.HttpRequestBase...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.methods.HttpUriRequest...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.params.HttpClientParams...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.protocol.RequestAddCookies...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.protocol.RequestDefaultHeaders...took3867ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.protocol.RequestProxyAuthentication...took3868ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.protocol.RequestTargetAuthentication...took3868ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.protocol.ResponseProcessCookies...took3868ms.
I/Zygote  (  188): Preloadingorg.apache.http.client.utils.URIUtils...took3868ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.BasicManagedEntity...took3868ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ClientConnectionManager...took3868ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ClientConnectionOperator...took3869ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ClientConnectionRequest...took3869ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ConnectionKeepAliveStrategy...took3869ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ConnectionReleaseTrigger...took3869ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.EofSensorInputStream...took3869ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.EofSensorWatcher...took3869ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ManagedClientConnection...took3870ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.OperatedClientConnection...took3870ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.params.ConnManagerPNames...took3870ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.params.ConnManagerParams...took3870ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.params.ConnManagerParams$1...took3870ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.params.ConnPerRoute...took3870ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.params.ConnPerRouteBean...took3870ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.params.ConnRoutePNames...took3871ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.params.ConnRouteParams...took3871ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.BasicRouteDirector...took3871ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.HttpRoute...took3871ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.HttpRouteDirector...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.HttpRoutePlanner...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.RouteInfo...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.RouteInfo$LayerType...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.RouteInfo$TunnelType...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.routing.RouteTracker...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.scheme.LayeredSocketFactory...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.scheme.PlainSocketFactory...took3872ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.scheme.Scheme...took3873ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.scheme.SchemeRegistry...took3873ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.scheme.SocketFactory...took3873ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ssl.AbstractVerifier...took3873ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ssl.AllowAllHostnameVerifier...took3873ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ssl.BrowserCompatHostnameVerifier...took3873ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ssl.SSLSocketFactory...took3874ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ssl.StrictHostnameVerifier...took3874ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.ssl.X509HostnameVerifier...took3874ms.
I/Zygote  (  188): Preloadingorg.apache.http.conn.util.InetAddressUtils...took3876ms.
I/Zygote  (  188): Preloadingorg.apache.http.cookie.CookieAttributeHandler...took3876ms.
I/Zygote  (  188): Preloadingorg.apache.http.cookie.CookieIdentityComparator...took3876ms.
I/Zygote  (  188): Preloadingorg.apache.http.cookie.CookieOrigin...took3895ms.
I/Zygote  (  188): Preloadingorg.apache.http.cookie.CookiePathComparator...took3895ms.
I/Zygote  (  188): Preloadingorg.apache.http.cookie.CookieSpec...took3895ms.
I/Zygote  (  188): Preloadingorg.apache.http.cookie.CookieSpecFactory...took3895ms.
I/Zygote  (  188): Preloadingorg.apache.http.cookie.CookieSpecRegistry...took3896ms.
I/Zygote  (  188): Preloadingorg.apache.http.entity.AbstractHttpEntity...took3896ms.
I/Zygote  (  188): Preloadingorg.apache.http.entity.BasicHttpEntity...took3896ms.
I/Zygote  (  188): Preloadingorg.apache.http.entity.ByteArrayEntity...took3896ms.
I/Zygote  (  188): Preloadingorg.apache.http.entity.ContentLengthStrategy...took3897ms.
I/Zygote  (  188): Preloadingorg.apache.http.entity.HttpEntityWrapper...took3897ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.AbstractHttpClientConnection...took3898ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.DefaultConnectionReuseStrategy...took3898ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.DefaultHttpResponseFactory...took3898ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.EnglishReasonPhraseCatalog...took3899ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.HttpConnectionMetricsImpl...took3899ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.SocketHttpClientConnection...took3900ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.auth.BasicSchemeFactory...took3900ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.auth.DigestSchemeFactory...took3901ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.AbstractAuthenticationHandler...took3901ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.AbstractHttpClient...took3901ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.BasicCookieStore...took3901ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.BasicCredentialsProvider...took3902ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.ClientParamsStack...took3902ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultConnectionKeepAliveStrategy...took3902ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultHttpClient...took3903ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultHttpRequestRetryHandler...took3903ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultProxyAuthenticationHandler...took3903ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultRedirectHandler...took3903ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultRequestDirector...took3905ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultTargetAuthenticationHandler...took3905ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.DefaultUserTokenHandler...took3906ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.EntityEnclosingRequestWrapper...took3906ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.RequestWrapper...took3906ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.client.RoutedRequest...took3906ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.AbstractClientConnAdapter...took3907ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.AbstractPoolEntry...took3907ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.AbstractPooledConnAdapter...took3908ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.DefaultClientConnection...took3908ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.DefaultClientConnectionOperator...took3908ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.DefaultResponseParser...took3909ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.IdleConnectionHandler...took3909ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.IdleConnectionHandler$TimeValues...took3909ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.ProxySelectorRoutePlanner...took3910ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.ProxySelectorRoutePlanner$1...took3910ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.AbstractConnPool...took3911ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.BasicPoolEntry...took3911ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.BasicPoolEntryRef...took3911ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.BasicPooledConnAdapter...took3911ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.ConnPoolByRoute...took3912ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.ConnPoolByRoute$1...took3912ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.PoolEntryRequest...took3912ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.RefQueueHandler...took3912ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.RefQueueWorker...took3933ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.RouteSpecificPool...took3933ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager...took3933ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.ThreadSafeClientConnManager$1...took3933ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.conn.tsccm.WaitingThreadAborter...took3933ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.AbstractCookieAttributeHandler...took3934ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.AbstractCookieSpec...took3934ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BasicCommentHandler...took3934ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BasicExpiresHandler...took3934ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BasicMaxAgeHandler...took3934ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BasicPathHandler...took3934ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BasicSecureHandler...took3936ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BestMatchSpec...took3936ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BestMatchSpecFactory...took3936ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.BrowserCompatSpecFactory...took3936ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.CookieSpecBase...took3936ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.NetscapeDraftSpecFactory...took3937ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2109DomainHandler...took3937ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2109Spec...took3937ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2109SpecFactory...took3937ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2109VersionHandler...took3937ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2965CommentUrlAttributeHandler...took3937ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2965DiscardAttributeHandler...took3938ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2965DomainAttributeHandler...took3938ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2965PortAttributeHandler...took3938ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2965Spec...took3938ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2965SpecFactory...took3938ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.cookie.RFC2965VersionAttributeHandler...took3938ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.entity.EntityDeserializer...took3938ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.entity.EntitySerializer...took3939ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.entity.LaxContentLengthStrategy...took3939ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.entity.StrictContentLengthStrategy...took3939ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.AbstractMessageParser...took3939ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.AbstractMessageWriter...took3939ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.AbstractSessionInputBuffer...took3939ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.AbstractSessionOutputBuffer...took3940ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.ChunkedInputStream...took3940ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.ContentLengthInputStream...took3940ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.ContentLengthOutputStream...took3940ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.HttpRequestWriter...took3940ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.HttpTransportMetricsImpl...took3940ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.SocketInputBuffer...took3940ms.
I/Zygote  (  188): Preloadingorg.apache.http.impl.io.SocketOutputBuffer...took3941ms.
I/Zygote  (  188): Preloadingorg.apache.http.io.HttpMessageParser...took3941ms.
I/Zygote  (  188): Preloadingorg.apache.http.io.HttpMessageWriter...took3941ms.
I/Zygote  (  188): Preloadingorg.apache.http.io.HttpTransportMetrics...took3941ms.
I/Zygote  (  188): Preloadingorg.apache.http.io.SessionInputBuffer...took3941ms.
I/Zygote  (  188): Preloadingorg.apache.http.io.SessionOutputBuffer...took3941ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.AbstractHttpMessage...took3941ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicHeader...took3942ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicHeaderElement...took3942ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicHeaderElementIterator...took3942ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicHeaderValueParser...took3942ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicHttpResponse...took3943ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicLineFormatter...took3943ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicLineParser...took3943ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicListHeaderIterator...took3963ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicNameValuePair...took3964ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicRequestLine...took3964ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BasicStatusLine...took3964ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.BufferedHeader...took3964ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.HeaderGroup...took3964ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.HeaderValueParser...took3964ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.LineFormatter...took3964ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.LineParser...took3965ms.
I/Zygote  (  188): Preloadingorg.apache.http.message.ParserCursor...took3965ms.
I/Zygote  (  188): Preloadingorg.apache.http.params.AbstractHttpParams...took3965ms.
I/Zygote  (  188): Preloadingorg.apache.http.params.BasicHttpParams...took3965ms.
I/Zygote  (  188): Preloadingorg.apache.http.params.CoreConnectionPNames...took3965ms.
I/Zygote  (  188): Preloadingorg.apache.http.params.CoreProtocolPNames...took3965ms.
I/Zygote  (  188): Preloadingorg.apache.http.params.HttpConnectionParams...took3965ms.
I/Zygote  (  188): Preloadingorg.apache.http.params.HttpParams...took3966ms.
I/Zygote  (  188): Preloadingorg.apache.http.params.HttpProtocolParams...took3966ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.BasicHttpContext...took3966ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.BasicHttpProcessor...took3966ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.HTTP...took3966ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.HttpContext...took3966ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.HttpProcessor...took3967ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.HttpRequestExecutor...took3967ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.HttpRequestInterceptorList...took3967ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.HttpResponseInterceptorList...took3967ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.RequestConnControl...took3967ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.RequestContent...took3967ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.RequestExpectContinue...took3968ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.RequestTargetHost...took3968ms.
I/Zygote  (  188): Preloadingorg.apache.http.protocol.RequestUserAgent...took3968ms.
I/Zygote  (  188): Preloadingorg.apache.http.util.ByteArrayBuffer...took3968ms.
I/Zygote  (  188): Preloadingorg.apache.http.util.CharArrayBuffer...took3968ms.
I/Zygote  (  188): Preloadingorg.apache.http.util.LangUtils...took3968ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.AttributesImpl...took3968ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.AutoDetector...took3969ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.Element...took3969ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.ElementType...took3969ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.HTMLModels...took3969ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.HTMLScanner...took3969ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.HTMLSchema...took3970ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.Parser...took3970ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.Parser$1...took3970ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.ScanHandler...took3970ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.Scanner...took3970ms.
I/Zygote  (  188): Preloadingorg.ccil.cowan.tagsoup.Schema...took3970ms.
I/Zygote  (  188): Preloadingorg.json.JSON...took3970ms.
I/Zygote  (  188): Preloadingorg.json.JSONArray...took3971ms.
I/Zygote  (  188): Preloadingorg.json.JSONObject...took3989ms.
I/Zygote  (  188): Preloadingorg.json.JSONObject$1...took3989ms.
I/Zygote  (  188): Preloadingorg.json.JSONStringer...took3989ms.
I/Zygote  (  188): Preloadingorg.json.JSONStringer$Scope...took3990ms.
I/Zygote  (  188): Preloadingorg.json.JSONTokener...took3990ms.
I/Zygote  (  188): Preloadingorg.kxml2.io.KXmlParser...took3991ms.
I/Zygote  (  188): Preloadingorg.kxml2.io.KXmlParser$ValueContext...took3991ms.
I/Zygote  (  188): Preloadingorg.xml.sax.Attributes...took3991ms.
I/Zygote  (  188): Preloadingorg.xml.sax.ContentHandler...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.DTDHandler...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.EntityResolver...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.ErrorHandler...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.InputSource...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.Locator...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.XMLReader...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.ext.LexicalHandler...took3992ms.
I/Zygote  (  188): Preloadingorg.xml.sax.helpers.DefaultHandler...took3992ms.
I/Zygote  (  188): Preloadingorg.xmlpull.v1.XmlPullParser...took3993ms.
I/Zygote  (  188): Preloadingorg.xmlpull.v1.XmlSerializer...took3993ms.
I/Zygote  (  188): Preloadingsun.misc.Unsafe...took3993ms.
I/Zygote  (  188): ...preloaded 2774 classes in 3993ms.
I/Zygote  (  188): Preloading resources...
I/Zygote  (  188): ...preloaded 274 resources in 952ms.
I/Zygote  (  188): ...preloaded 31 resources in 33ms.
I/Zygote  (  188): Accepting command socket connections
I/Zygote  (  694): Process: zygote socket opened
```
代码添加如下：
```python
diff --git a/frameworks/base/core/java/com/android/internal/os/ZygoteInit.java b/frameworks/base/core/java/com/android/internal/os/ZygoteInit.java
index 4dbd15e..3a58451 100644
--- a/frameworks/base/core/java/com/android/internal/os/ZygoteInit.java
+++ b/frameworks/base/core/java/com/android/internal/os/ZygoteInit.java
@@ -286,7 +286,8 @@ public class ZygoteInit {
             /// M: Added for BOOTPROF
             int count = 0;
             long startTime = SystemClock.uptimeMillis();
-
+           long lastTime = SystemClock.uptimeMillis();//weiqifa modify add
+           long nextTime = SystemClock.uptimeMillis();//weiqifa modify add 
             // Drop root perms while running static initializers.
             setEffectiveGroup(UNPRIVILEGED_GID);
             setEffectiveUser(UNPRIVILEGED_UID);
@@ -318,6 +319,10 @@ public class ZygoteInit {
                             Log.v(TAG, "Preloading " + line + "...");
                         }
                         Class.forName(line);
+                       nextTime=SystemClock.uptimeMillis();
+                       if((nextTime-lastTime)>50){
+                               Log.i(TAG,"Preloading"+line+"...took"+(nextTime-lastTime)+"ms.");                       
+                       }
                         if (Debug.getGlobalAllocSize() > PRELOAD_GC_THRESHOLD) {
```
结论：这些东西太多了，添加　扫描话了很多时间　造成开机时间变慢


