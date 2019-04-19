# 利用WordPress用户密码算法规则修改用户密码 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月06日 11:33:45[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：742
WordPress用户密码保存在wp_users数据表的user_pass字段，密码是通过Portable PHP password hashing framework类产生的，
密码的形式是随机且不可逆，同一个明文的密码在不同时间，产生的密文也不一样，相对来说较为安全。
WordPress用户密码产生的过程是：当需要生成用户密码的时候，随机产生了一个salt，然后将salt和password相加，又进行了count次md5，最后和encode64的hash数值累加，就得到了一个以$P$开头的密码，这个密码每次产生的结果都不一样，下面就是产生WordPress密码的代码，将其放在WordPress根目录下，就可以生成一个加密的password，用这个密码替换掉wp_users数据表的user_pass字段即可修改密码。
**<?php**
require_once( './wp-includes/class-phpass.php');
$password = 'aaa';    
$wp_hasher = new PasswordHash(8, TRUE);
echo $wp_hasher->HashPassword($password);
可以把这个库用到自己的程序中了，检查密码是否成功用另一个函数CheckPassword($password, $stored_hash)，第一个为明文，第2个参数为之前加密过的密码
## 参考
[http://www.williamlong.info/archives/1978.html](http://www.williamlong.info/archives/1978.html)
