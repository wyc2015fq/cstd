# 网络安全与OpenSSL - Arthur的随笔 - CSDN博客
2013年06月07日 14:07:31[largetalk](https://me.csdn.net/largetalk)阅读数：2695
# 网络安全与OpenSSL[](#openssl)
这两天看了个介绍网络安全的视频，有点豁然开朗的感觉，结合前段时间看OpenSSL的各种不懂，就有了这篇笔记类似的文章
## 1. 名词解释[](#id1)
加密，解密，明文，密文：不解释
- 
对称加密:
加密解密密码一样，DES, AES, Rijndael, 3DES, blowfish等加密算法- 
非对称加密:
加密解密密码不一样，又称公开密钥加密，RSA, ECC- 
散列函数/摘要:
即hash, 不可逆，抗碰撞， MD5, SHA1- 
DES:
曾是美国联邦标准，已经不太安全的加密算法。密钥长度56位，加密块64位, 16轮置换。![](https://img-blog.csdn.net/20130607140631984)
- 
RSA:
非对称加密，被广泛使用，SSL, TLS的理论基础
> 
随机选两个大素数， P和Q, P不等于Q, 计算N=P*Q
根据欧拉函数，求得r=(P-1)(Q-1)
选择一个数e, e满足1 < e < r, 并且e与r互质，即gcd(e, r)＝1
计算出d, d满足e*d = 1 mod r, 0 <= d <= r
(e, r)是公钥， （d, r)是私钥）
对于消息M, C = (M**e) mod r 就是加密过程， M = (C**d) mod r 就是解密过程
- 
数字签名:
用私钥加密消息摘要，起到消息认证和消息不可抵赖作用- 
证书:
由第三方机构颁发的能证明证书拥有者身份的文件，包含证书机构私钥加密的信息和证书人信息和公钥
加密是用来解决消息泄密功能， 摘要是为了防止消息被篡改，签名为了消息不可抵赖，证书用来确认身份。
- 
Kerberos:
基于对称加密的一种网络认证协议，首先，client向AS请求身份认证，AS返回身份token, 接着client向TGS传送身份token和应用server id, TGS检查配置返回票据TGT, client使用TGT登陆应用server- 
X509:
X500目录服务一部分，基于公钥加密和数字签名，被广泛使用的数字证书标准。
在签发者处，首先求出传递消息Info的HASH值，然后用私钥对Hash值做加密，然后将传递消息原文Info和经过加密的HASH值一起发给接受者，接受者首先用签发者的公钥密码解开Hash，然后对收到的Info原文计算HASH值，然后比对是否相对，如果相同，则认证成功。
## 2. OpenSSL 使用简介[](#id2)
- 
散列:
echo ‘abcd’ | openssl md5
echo ‘abcd’ | openssl sha1
- 
RSA:
生成密钥 openssl genrsa -out private.key 1024
提取公钥 openssl rsa -in private.key -pubout -out pub.key
用公钥加密消息 echo ‘abcd’ | openssl rsautl -encrypt -inkey pub.key -pubin > encrypt.result
用私钥解密消息 cat encrypt.result | openssl rsautl -decrypt -inkey private.key
生成ssl证书 openssl req -new -x509 -key private.key -out cacert.pem -days 1000
- 
DSA 用于签名:
生成密钥 openssl dsaparam -out dsaparam.pem 1024
> 
openssl gendsa -out privkey.pem dsaparam.pem
生成公钥 openssl dsa -in privkey.pem -out pubkey.pem -pubout
> 
rm dsaparam.pem
用私钥签名 echo ‘123456’ | openssl dgst -dss1 -sign privkey.pem > sign.result
用公钥验证 echo ‘123456’ | openssl dgst -dss1 -verify pubkey.pem -signature sign.result
- 
Base64:
echo ‘abc’ | openssl enc -base64 -A > base64.info
cat base64.info | openssl enc -d -base64 -A
## 3. OpenSSL python 包装[](#openssl-python)
openssl有3个模块
crypto 加密模块
rand 伪随机数生成器
ssl ssl相关
## 3.1 rand[](#rand)
rand.seed 添加种子
rand.bytes 生成指定长度的随机数
rand.write_files 将随机数写入文件
## 3.2 crypto[](#crypto)
continue...
## 3.3 ssl[](#ssl)
continue...
