# Android数据加密方案 - weixin_33985507的博客 - CSDN博客
2016年11月24日 15:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
很多人都知道Android数据需要加密，毕竟反编译、抓包，你懂的，可是有时候不知道从何入手，比如菜鸟的我，经过这几天的折腾之后，我整理了一些思路，有不对的地方，还请多多指点。
1、混淆文件：这个比较简单，不知道如何混淆的，推荐   [http://www.jianshu.com/p/f3455ecaa56e](https://www.jianshu.com/p/f3455ecaa56e)
2、加固：APP上线之前最好做一下加固，常用的有：360加固宝，爱加密等
3、HTTPS：现在很多APP都用HTTPS作为网络传输的保证，防止中间人攻击，提高数据传输的安全性（用Retrofit的网络请求框架的，要加上HTTPS也不是什么难事，推荐 [http://www.jianshu.com/p/16994e49e2f6](https://www.jianshu.com/p/16994e49e2f6) ，这里说的HTTPS是指自签的）
4、如果你没用HTTPS的话，为了确保传输安全，还需对传输的数据进行加密，这里我推荐用AES+RSA进行加密，（不知道这两种加密算法的自行百度一下，了解一下原理），我画了一张流程图：
![3775979-f748111e7142f697.png](https://upload-images.jianshu.io/upload_images/3775979-f748111e7142f697.png)
数据加密流程.png
 具体过程：是先由服务器创建RSA密钥对，RSA公钥保存在安卓的so文件里面，服务器保存RSA私钥。而安卓创建AES密钥（这个密钥也是在so文件里面），并用该AES密钥加密待传送的明文数据，同时用接受的RSA公钥加密AES密钥，最后把用RSA公钥加密后的AES密钥同密文一起通过Internet传输发送到服务器。当服务器收到这个被加密的AES密钥和密文后，首先调用服务器保存的RSA私钥，并用该私钥解密加密的AES密钥，得到AES密钥。最后用该AES密钥解密密文得到明文。（用Retrofit的网络请求框架的，要加密参数和解密服务器传输过来的数据需自定义Converter，推荐[http://blog.csdn.net/zr940326/article/details/51549310](https://link.jianshu.com?t=http://blog.csdn.net/zr940326/article/details/51549310)）
5、so文件：ndk开发的so，可以存放一些重要的数据，如：密钥、私钥、API_KEy等，不过这里我建议大家使用分段存放，C层（so文件）+String文件（string.xml）+gradle文件，用的时候再拼接合并，还有如上图所示，AES的加密算法是放在C层进行实现的，这样也是最大程度保护我们数据的安全
Q:公钥传输是否有安全问题？（RSA key）R:固定key，也就是保存在so文件中，理论上不会不安全，当然也可以动态从服务器获取，但传输不安全（前提不是https）
Q:AES key存储在哪里比较好？R:分段存放，C层（so文件）+String文件（string.xml）+gradle文件；也可以从服务获取
我觉得思路、方案是最重要的，代码实现相对来说还是比较容易的！
