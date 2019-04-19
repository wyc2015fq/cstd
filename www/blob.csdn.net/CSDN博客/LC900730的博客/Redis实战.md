# Redis实战 - LC900730的博客 - CSDN博客
2018年01月06日 17:17:01[lc900730](https://me.csdn.net/LC900730)阅读数：164
## Redis5种数据结构
- String 字符串、整数、浮点数
- List 一个链表，链表上每个节点包含一个字符串
- Set 包含字符串的无序收集器，并且被包含的每个字符串都是各不相同的
- Hash 键值对的无序散列表
- Zset 字符串成员与浮点数分值之间的有序映射，元素的排列顺序由分值大小决定
### Redis字符串
字符串可以存储3种类型的值：字符串、整数、浮点数。
- 
当用户将一个值存储到Redis字符串里面的时候，如果这个值可以被解释为十进制整数或浮点数，那么redis会察觉到这一点，并允许对这个字符串执行各种INCR*和DECR*操作。
- 
如果键不存在或保存了一个空串，那么值会被当做0处理。
### Redis列表
|RPUSH|RPUSH key-name value [value1 …]|
|----|----|
|LPUSH|LPUSH key-name value [value1 …]|
|RPOP|RPOP key-name 移除并返回列表最右端元素|
|LPOP|LPOP key-name 移除并返回列表最左元素|
|LINDEX|LINDEX key-name offset—返回列表中偏移量为offset的元素|
|LRANGE|LRANGE key-name start end—返回列表从start到end的元素，包含start和end|
|LTRIM|LTRIM key-name start end–对列表进行修剪，只保留start到end范围内的元素，start和end的元素也会被保留|
### REDIS中集合
Redis的集合以无序方式来存储多个各不相同的元素，用户可以快速对集合执行添加元素操作、移除元素操作以及检查一个元素是否存在于集合里。
|SADD|SADD key-name item [item…]：将一个或者多个元素添加到集合里面，并返回被添加元素当中原来不存在于集合里面的元素数量|
|----|----|
|SREM|SREM key-name item [item…]—从集合中移除一个或多个元素，并返回移除元素的数量|
|SISMEMBER|SISMEMBER key-name item–检查元素item是否存在于集合key-name里|
|SCARD|SCARD key-name –返回集合中包含的元素数量|
|SMEMBERS|SMEMBERS key-name –返回集合中包含的所有元素|
|SRANDMEMBER|SRANDMEMBER key-name [count]–从集合中随机返回一个或多个元素。当count为正数，返回的随机元素不会重复。当count为负数，命令返回的随机元素可能会出现重复。|
|SPOP|SPOP key-name：随机移除集合中一个元素，并返回被移除的元素|
|SMOVE|SMOVE source-key dest-key item ：如果集合source-key包含元素item，那么从source-key里面移除元素item，并将元素item添加到集合dest-key中；如果item被成功移除，那么命令返回1，否则返回0。|
### Redis中散列
Redis中散列可以存储多个键值对之间的映射。和字符串一样，散列存储的值既可以是字符串又可以是数字值，并且用户同样可以对散列存储的数字值，并且用户同样可以对散列存储的数字值执行自增操作或者自减操作。
### 脚注
生成一个脚注[^footnote].
- 行内公式，数学公式为：$\Gamma(n) = (n-1)!\quad\forall n\in\mathbb N$。
- 块级公式：
$	x = \dfrac{-b \pm \sqrt{b^2 - 4ac}}{2a} $
更多LaTex语法请参考 [这儿](http://meta.math.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference).
### UML 图:
可以渲染序列图：
Created with Raphaël 2.1.2张三张三李四李四嘿，小四儿, 写博客了没?李四愣了一下，说：忙得吐血，哪有时间写。
或者流程图：
Created with Raphaël 2.1.2开始我的操作确认？结束yesno
- 关于 **序列图** 语法，参考 [这儿](http://bramp.github.io/js-sequence-diagrams/),
- 关于 **流程图** 语法，参考 [这儿](http://adrai.github.io/flowchart.js/).
## 离线写博客
即使用户在没有网络的情况下，也可以通过本编辑器离线写博客（直接在曾经使用过的浏览器中输入[write.blog.csdn.net/mdeditor](http://write.blog.csdn.net/mdeditor)即可。**Markdown编辑器**使用浏览器离线存储将内容保存在本地。
用户写博客的过程中，内容实时保存在浏览器缓存中，在用户关闭浏览器或者其它异常情况下，内容不会丢失。用户再次打开浏览器时，会显示上次用户正在编辑的没有发表的内容。
博客发表后，本地缓存将被删除。　
用户可以选择  把正在写的博客保存到服务器草稿箱，即使换浏览器或者清除缓存，内容也不会丢失。
> 
**注意：**虽然浏览器存储大部分时候都比较可靠，但为了您的数据安全，在联网后，**请务必及时发表或者保存到服务器草稿箱**。
## 浏览器兼容
- 目前，本编辑器对Chrome浏览器支持最为完整。建议大家使用较新版本的Chrome。
- IE９以下不支持
- IE９，１０，１１存在以下问题 
- 不支持离线功能
- IE9不支持文件导入导出
- IE10不支持拖拽文件导入
