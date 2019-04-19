# php hash_hmac  与python hmac 区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月29日 19:01:21[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：235
使用 HMAC 方法生成带有密钥的哈希值
hash_hmac ( string $algo , string $data , string $key [, bool $raw_output = false ] )
参数 ¶
**algo**
要使用的哈希算法名称，例如："md5"，"sha256"，"sha1" 等。 如何获取受支持的算法清单，请参见 [hash_algos()](https://php.net/manual/zh/function.hash-algos.php)。
**data**
要进行哈希运算的消息。
**key**
使用 HMAC 生成信息摘要时所使用的密钥。
**raw_output**
设置为 TRUE 输出原始二进制数据，
 设置为 FALSE 输出小写 16 进制字符串。

python的
import hashlib
import hmac
hmac.**new**(*key*, *msg=None*, *digestmod=None*)
第一个参数：秘钥
2：数据
3：算法
hashlib.sha256，
 hashlib.sha1
[http://wiki.jikexueyuan.com/project/explore-python/Standard-Modules/hmac.html](http://wiki.jikexueyuan.com/project/explore-python/Standard-Modules/hmac.html)
digest对应php的raw_output=TRUE

import base64
base64.b64encode(s)
base64.b64decode(aa)
