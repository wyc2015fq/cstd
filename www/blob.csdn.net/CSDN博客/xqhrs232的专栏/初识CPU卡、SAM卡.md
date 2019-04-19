# 初识CPU卡、SAM卡 - xqhrs232的专栏 - CSDN博客
2017年02月08日 23:33:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：408
原文地址::[http://blog.csdn.net/songkexin/article/details/5862613](http://blog.csdn.net/songkexin/article/details/5862613)
相关文章
1、
## 什么是PSAM卡、SAM卡、SIM卡？
 ----[http://blog.sina.com.cn/s/blog_658272b90100qo3w.html](http://blog.sina.com.cn/s/blog_658272b90100qo3w.html)
IC卡按照接口方式可分为接触式卡、非接触式卡、复合卡；按器件技术可分为非加密存储卡、加密存储卡和CPU卡。
加密存储卡是对持卡人的认证，只有输入正确的密码才能访问或者修改卡中的数据，最典型的是手机SIM卡的PIN码；当设置PIN码后，开机必须输入PIN码，如果连续几次输入错误，就必须更高权限的PUK码来修改PIN码，如果PUK码也连续输错，那就只有换卡了。
加密存储卡保证了对持卡人的认证，但是，在保证系统安全性上还不够。
1. 密码输入是采用透明传输，在伪造的ATM机或者网络上，这个密码很容易被截取；
2. 逻辑加密卡无法认证应用是否合法；
3. 对于系统集成商来说，密码和加密[算法](http://lib.csdn.net/base/datastructure)是透明的；
由此，引入了CPU卡；
CPU卡在三个方面保证了安全：
1. 对人：持卡者合法性认证：持卡者需要输入口令。
2. 对卡：卡的合法性认证；内部认证。
3. 对系统：系统的合法性认证；外部认证。
卡的合法性认证：
CPU卡发送随机数给卡（如地铁卡），卡收到随机数后用加密算法加密，将加密后的值传给CPU卡，CPU卡解密并与发送的随机数比较，如果相等，则认为卡合法。
系统的合法性认证（例如手持POS是否是合法的经过认证的厂商生产的）：
CPU卡发送随机数给POS自带的卡或者模块，POS自带的卡或者模块将随机数加密后，传回CPU卡，CPU卡解密并与发送的随机数比较，如果相等，则认为系统合法。这个过程在开机时做。
在加密和解密过程中，涉及到两个因素，一个是加解密算法、一个是密钥。加解密算法是公开的，在CPU卡中，有[操作系统](http://lib.csdn.net/base/operatingsystem)叫COS：Chip
 OS；由卡的生产商提供，并提供加解密算法。卡的生产商必须经过专门的机构认证。密钥则有发卡机构掌握，层层发卡，权限不同。
SAM卡：全称是Security Access module；是一种特殊的CPU卡；存储了密钥和加解密算法。
目前SAM卡分了很多种：
PSAM卡：终端安全控制模块，一般用于小额支付扣款中；
ESAM：厂商（系统）的SAM卡，用于设备的认证；
ISAM：用于充值；
在具体的实现中，会比较多样化。例如，有的设备认证并不是用ESAM卡，而是采用专用的模块。这样，就存在一个问题，即密钥用软件实现，可能会存在密钥泄露的问题；一种解决的方法是存储多组密钥，在随机数中指定采用一组密钥。
普通卡的发行一般采用密钥对唯一的物理卡号加密的方式。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/yestotofu/archive/2009/12/29/5092110.aspx](http://blog.csdn.net/yestotofu/archive/2009/12/29/5092110.aspx)
[](http://blog.csdn.net/songkexin/article/details/5862613#)[](http://blog.csdn.net/songkexin/article/details/5862613#)[](http://blog.csdn.net/songkexin/article/details/5862613#)[](http://blog.csdn.net/songkexin/article/details/5862613#)[](http://blog.csdn.net/songkexin/article/details/5862613#)[](http://blog.csdn.net/songkexin/article/details/5862613#)
- 
顶
0- 
踩
