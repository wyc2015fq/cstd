# Visa-Api对接引发的 外部接口设计及SSL方面的考虑 - z69183787的专栏 - CSDN博客
2019年02月13日 23:55:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：66
最近一个项目有幸与Visa公司进行API对接，让自己在 外部 Http API对接方面又有了新的认识，之前通常的对接方式无外乎有如下几种：
- 对请求参数 a + b + c + d + 时间戳 + 密钥 进行md5 加密，进行消息摘要的校验；
- 请求参数A（API固有参数+时间戳+密钥+请求体B） md5 消息摘要校验，请求体 B 使用对称、非对称加密 进行加解密获取；
- （Visa的情况）请求Header 中 加入 Http Basic认证，同时Https请求，并且使用 Two way SSL认证；
关于3中提到的 SSL 双向认证这次我也是第一次接触，单项认证和双向认证可以看：
通常需要一个SSLContext， SSLContext 需要配置一个 TrustManager，如果是双向通信，还需要一个 KeyManager。
- 单行https TrustManager、
- 双向https TrustManager KeyManager
- KeyManager 负责提供证书和私钥，证书发给对方服务器端peer，决定将哪一个认证证书发送给对端服务器
- TrustManager 负责验证服务器端 peer 发来的证书，决定对端的认证证书是否被信任
开发调试查看完整SSL调试信息，可增加启动参数 -Djavax.net.debug=all，亲测很有帮助，在服务端返回 非法HttpStatus Code时可以通过调试信息查看具体原因
具体相关内容介绍可参考：
单行认证与双行认证的区别：[https://blog.csdn.net/z69183787/article/details/87253530](https://blog.csdn.net/z69183787/article/details/87253530)
Https 认证开发实践（核心）：[https://blog.csdn.net/z69183787/article/details/87262585](https://blog.csdn.net/z69183787/article/details/87262585)
Visa-Api Reference 讲解 two way ssl（包含证书及密钥仓库jks生成） ：[https://blog.csdn.net/z69183787/article/details/87254283](https://blog.csdn.net/z69183787/article/details/87254283)
附上双向认证核心代码：
keystore是Java保存密钥对的秘钥仓库，文件后缀为keystore，后缀类似.jks .truststore都是保存秘钥和证书的仓库
.cer:存储公钥证书的文件格式。
.crt:客户端认证的证书、私钥。
双向认证需要服务端和客户端都提供cer格式的证书，然后导入到彼此的keystore中
```java
KeyStore keystore = KeyStore.getInstance(KeyStore.getDefaultType());
            FileInputStream keystoreInputStream = new FileInputStream(keystorePath);
            keystore.load(keystoreInputStream, keystorePassword.toCharArray());
            keystoreInputStream.close();
            SSLContext sslcontext = SSLContexts.custom().useProtocol("TLS")
                    .loadKeyMaterial(keystore, keystorePassword.toCharArray())
                    .loadTrustMaterial(new File(keystorePath), keystorePassword.toCharArray())
                    .build();
            HostnameVerifier hostnameverifier = null;
            SSLConnectionSocketFactory sslSocketFactory = new SSLConnectionSocketFactory(sslcontext, null, null,
                    hostnameverifier);
            CloseableHttpClient httpClient = proxyHostName != null && proxyHostName != "" && proxyPortNumber != 0
                        ? HttpClients.custom().setSSLSocketFactory(sslSocketFactory).setProxy(new HttpHost(proxyHostName, proxyPortNumber)).build()
                        : HttpClients.custom().setSSLSocketFactory(sslSocketFactory).build();
```
