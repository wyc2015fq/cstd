
# Redis 脚本 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:25:50[seven-soft](https://me.csdn.net/softn)阅读数：148个人分类：[Redis																](https://blog.csdn.net/softn/article/category/6332853)



Redis 脚本使用 Lua 解释器来执行脚本。 Reids 2.6 版本通过内嵌支持 Lua 环境。执行脚本的常用命令为**EVAL**。
### 语法
Eval 命令的基本语法如下：
redis127.0.0.1:6379>EVAL script numkeys key[key...]arg[arg...]
### 实例
以下实例演示了 redis 脚本工作过程：
redis127.0.0.1:6379>EVAL"return {KEYS[1],KEYS[2],ARGV[1],ARGV[2]}"2key1 key2 first second1)"key1"2)"key2"3)"first"4)"second"
---

## Redis 脚本命令
下表列出了 redis 脚本常用命令：
|序号|命令及描述|
|1|[EVAL script numkeys key [key ...] arg [arg ...]](http://www.runoob.com/redis/scripting-eval.html)
|执行 Lua 脚本。|
|2|[EVALSHA sha1 numkeys key [key ...] arg [arg ...]](http://www.runoob.com/redis/scripting-evalsha.html)
|执行 Lua 脚本。|
|3|[SCRIPT EXISTS script [script ...]](http://www.runoob.com/redis/scripting-script-exists.html)
|查看指定的脚本是否已经被保存在缓存当中。|
|4|[SCRIPT FLUSH](http://www.runoob.com/redis/scripting-script-flush.html)
|从脚本缓存中移除所有脚本。|
|5|[SCRIPT KILL](http://www.runoob.com/redis/scripting-script-kill.html)
|杀死当前正在运行的 Lua 脚本。|
|6|[SCRIPT LOAD script](http://www.runoob.com/redis/scripting-script-load.html)
|将脚本 script 添加到脚本缓存中，但并不立即执行这个脚本。|

