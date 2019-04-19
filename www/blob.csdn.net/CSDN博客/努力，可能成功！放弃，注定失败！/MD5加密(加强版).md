# MD5加密(加强版) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月24日 08:30:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1259
> 
## Message Digest Algorithm MD5（中文名为[消息摘要算法](http://baike.baidu.com/view/2313810.htm)第五版）为计算机安全领域广泛使用的一种散列函数，用以提供消息的完整性保护.
在开发中,如果我们用HTTP发送请求的话,或多或少会涉及到用户的账户信息,这个时候我们可以用MD5对用户的信息进行加密，下面本菜菜推荐几种常用的加密方法^_^
就是普通的用md5加密
先在项目中导入MD5的类,github上应该有,不过我已经整合到项目里面,最后会放入下载地址.
```java
```java
//测试用的密码
    NSString*passWord = @"12345678";
    //进行加密
    NSString*keyString = [passWord md5String];
    NSLog(@"%@",keyString);
```
```
但是发现这种太简单,还是很容易被百度的md5"暴力破解"......
所以又多了三种方法.
### 加盐
要加密的密码 + 一个复杂的字符串，防止用户输入的密码过于简单
### HMAC
要加密的密码 + 一个字符串进行混合
再 md5 计算
把计算的结果 + 要加密的密码再次混合
再进行 md5 计算
### 时间戳密码 这种方式是最安全的
把key(传给服务器的秘钥) 用md5加密
把要加密的密码和之前用md5加密后的key再进行hmac加密
从服务器获取时间
第二步产生的hmac+时间混合 再和第一次产生的key值进行hmac加密
但是楼主发现这几个写法实现太繁琐,没什么技术含量,所以就把它们封装到一个类中,只要创建这个类,就能一句话加密.
当然,楼主这样也是很菜滴~
demo下载地址就放在后边了,里边详细写了加密原理和步骤，下面就简单的介绍一下封装后的效果
### 代码示范：
## 使用前先导入头文件：
![](http://upload-images.jianshu.io/upload_images/3324576-f43ff32a37b1a20d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.png
1.加盐方式
```java
```java
//创建对象
    MM_MD5*md5 = [MM_MD5 new];
    //传入加密的密码和要加入的"盐"
    NSString*passWordWithSald = [md5 getPassWord:@"123" andFixString:@"%$@&bb"];
    NSLog(@"%@",passWordWithSald);
```
```
2.HMAC
```java
```java
//创建对象
    MM_MD5*md5 = [MM_MD5 new];
    //传入加密的密码和要混合的字符串
    NSString*passWordWithSald = [md5 getPassWordHmac:@"123456" andFixString:@"348fjfh&G"];
    NSLog(@"%@",passWordWithSald);
```
```
3.时间戳密码 
```java
```java
//创建对象
    MM_MD5*md5 = [MM_MD5 new];
    //传入加密的密码和秘钥 以及服务器地址,要从服务器上获取时间
    NSString*passWordWithSald = [md5 getPassWord:@"123456" andkey:@"key" andTimeWithUrlString:@"xxxxxxx"];
    NSLog(@"%@",passWordWithSald);
```
```
好啦，就这么简单,是不是比以前要简单许多？ 
## 楼主github地址：
[https://github.com/YMS1230/YMS_MD5_demo.git](https://github.com/YMS1230/YMS_MD5_demo.git)
大家多多关注呀~
文／The_boy（简书作者）
原文链接：http://www.jianshu.com/p/83cd474132cc
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
