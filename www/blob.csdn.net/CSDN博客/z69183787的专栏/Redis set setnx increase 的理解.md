# Redis set setnx increase 的理解 - z69183787的专栏 - CSDN博客
2016年09月02日 17:04:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2608
increase decrease：原子性
setnx：key不存在时set 可以用来充当锁的功能
set：一般操作 
- 
EX seconds - 设置指定的到期时间，单位为秒。
- 
PX milliseconds - 设置指定到期时间，单位为毫秒。
- 
NX - 只有设置键，如果它不存在。
- 
XX - 只有设置键，如果它已经存在。
### 例子
redis 127.0.0.1:6379> SET yiibai redis EX 60 NX OK
上面的例子将设置键yiibai，60秒到期时，如果该键不存在。
