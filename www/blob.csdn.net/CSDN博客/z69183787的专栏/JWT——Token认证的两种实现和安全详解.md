# JWT——Token认证的两种实现和安全详解 - z69183787的专栏 - CSDN博客
2018年04月08日 15:29:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2862
### 前言：
最近因为项目中需要解决跨域取值的问题，所有考虑到用Token认证做技术支撑点，自己看了许多与之相关的文章，从中总结出了以下两个要点(签名和token时间)。在说这两个要点之前先大概简单说一下与之有关的一些问题。
首先，如果你对token认证的知识一点都不了解，那么我觉得这篇文章还不太适合你，因为我在这里不会在把相关的基础知识再说明一遍，因为网上有很多相关的文章，讲的都比较好，我会在文章下边参考文献中附上链接。但是还是说一下重点的几个点：
1.header(头部)，头部信息主要包括（参数的类型--JWT,签名的算法--HS256）
2.poyload(负荷)，负荷基本就是自己想要存放的信息(因为信息会暴露，不应该在载荷里面加入任何敏感的数据)，有两个形式，下边会讲到
3.sign(签名)，签名的作用就是为了防止恶意篡改数据，下边会详细说明
我的理解，在Java的实现中可以有两种方式，一种是不借助第三方jar，自己生成token，另一种的借助第三方jar，传入自己需要的负荷信息，生成token。接下来就根据这两个逐个说明。Token的组成就是header.poyload.sign。
## 自己生成token：
看过参考资料的朋友应该都知道，header和poyload的组成都是json字符串，所以先创建头部的json，然后用base64编码(org.apache.axis.encoding.Base64)，这里选择的base64要对应着编码和解码（Base64是一种编码，也就是说，它是可以被翻译回原来的样子来的。它并不是一种加密过程）。然后再创建负荷的json，然后也同样用base64编码，这样就生成了两个字符串，然后用.拼接到一起就形成了现在的形式：eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJmcm9tX3VzZXIiOiJCIiwidGFyZ2V0X3VzZXIiOiJBIn0。在这里只是给大家一个演示，
实际中根据每个人的负荷参数的不同，编码后所生成的字符串也不同。因为没有借助第三方的jar，
所有接下来要对上边拼接成的字符串进行hs256的算法加密生成sign签名，这里需要自己手动去写一个类，然后提供一
个静态方法供外界的调用。
类的实现代码如下：
[java][view plain](https://blog.csdn.net/buyaoshuohua1/article/details/73739419#)[copy](https://blog.csdn.net/buyaoshuohua1/article/details/73739419#)
- import javax.crypto.Mac;  
- import javax.crypto.spec.SecretKeySpec;  
- import org.apache.commons.codec.binary.Base64;  
- publicclass HS256 {  
- publicstatic String returnSign(String message) {  
-     String hash = "";  
- //别人篡改数据，但是签名的密匙是在服务器存储，密匙不同，生成的sign也不同。
- //所以根据sign的不同就可以知道是否篡改了数据。
-     String secret = "mystar";//密匙
- try {  
-         Mac sha256_HMAC = Mac.getInstance("HmacSHA256");  
-         SecretKeySpec secret_key = new SecretKeySpec(secret.getBytes(),"HmacSHA256");  
-         sha256_HMAC.init(secret_key);  
-         hash = Base64.encodeBase64String(sha256_HMAC.doFinal(message.getBytes()));  
-         System.out.println(message+"#####"+hash);  
-     } catch (Exception e) {  
-         System.out.println("Error");  
-     }  
- return hash;  
-     }  
-     }  
这样token的三部分就生成了，然后当做参数传到前台，用cookie存储就可以在同一客户端调用了。
当从客户端带过来token参数的时候，直接对头部和负荷再次调用加密算法，看生成的新的签名和之前的签名是否一致，
判断数据是否被篡改。
## 借用第三方的jar(jjwt-0.7.0.jar)生成token：
## 在这里自己已经通过代码测试，直接先看代码：
![](https://img-blog.csdn.net/20170802233339334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnV5YW9zaHVvaHVhMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
调用这个方法会自动对header和poyload进行base64的编码，你看过源码就知道它用的是哪一种，用的是自己jar包自带的(io.jsonwebtoken.impl.Base64Codec)，跟自己生成token时，用的base64的jar不一样，自己在此列出来：
[java][view
 plain](https://blog.csdn.net/buyaoshuohua1/article/details/73739419#)[copy](https://blog.csdn.net/buyaoshuohua1/article/details/73739419#)
- publicstaticvoid main(String[] args) {  
- // String token = createJWT("11","22","222",11);
- // System.out.println(token);
-         JSONObject json_header = new JSONObject();  
-         json_header.put("typ", "JWT");//类型
-         json_header.put("alg", "HS256");//加密算法
- // String header =
- // org.apache.axis.encoding.Base64.encode(json_header.toString().getBytes());
-         String header = Base64Codec.BASE64URL.encode(json_header.toString()  
-                 .getBytes());  
-         String aa = Base64Codec.BASE64URL.decodeToString(header);  
-         System.out.println(header + "--" + aa);  
-     }  
接着上边createJWT()方法说，只要把自己定义的负荷json串当做参数传入就行，并且签名也会对应的生成，返回一个完整的token。在测试的过程中，发现即使自己不定义token的头部，也会自动生成header，只是里边没有typ这样的参数定义，只有HS256，这里源码里边，默认了alg的value。在这里我想说明的是，假如外界会篡改参数，他肯定也知道构成，会把负荷里边的参数取出来，也许会修改，然后编码放回去，但是头部的信息对他来说用处不大，所以自己在这个方法里边，默认把头部加上，负荷的值还是自己在调用的时候传入进来。这样执行完，把生成的token就当做参数传到前台，存储在cookie里边。
然后再说一下，前台带过来token参数时候，怎么处理，看代码：
![](https://img-blog.csdn.net/20170802235357495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnV5YW9zaHVvaHVhMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个过程的原理跟自己生成token判断原理一样，都是从新生成sign，只是一个是调用自己的方法，另外一个是调用第三方的方法，自己看了源码（public abstract JwtBuilder signWith(SignatureAlgorithm paramSignatureAlgorithm,
 String paramString);），没能单独把第三方生成sign的方法提出来，只是一个接口，但是跟上边的加密算法实现原理应该是基本一致的。
至此，token签名这一块的问题大致就先说到这了，然后再来说一下token过期时间问题。这个相对来说不是太复杂，可以在负荷里边多带一个参数，把过期时间放进去，其实里边有一个exp标签名就是存储过期时间字段的，但是自己在测试过程中，发现每次读出来的都是最原始的时间，自己当时也再花时间去看，因为我觉得自己带参数其实一个道理。存储的是时间戳。
存储：
long nowMillis = System.currentTimeMillis();
long expMillis = nowMillis + 1000*2;//设置token二秒过期
获取：
Date aa = new Date(Long.parseLong(claims.get("aa").toString()));
方式就是这样了，我大概列了出来。到时可以存储一个生成token时间和token过期时间，然后服务器接收到的时候，可以根据当前的时间去判断。当前时间大于token生成时间并且小于token过期时间的情况下，继续走你接下来的业务操作。
附上参考文献：
http://blog.csdn.net/u011537073/article/details/52177204
http://www.cnblogs.com/anny0404/p/5318692.html
http://blog.leapoahead.com/2015/09/06/understanding-jwt/
