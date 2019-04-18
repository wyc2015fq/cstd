# Redis - 事务（multi，exec，watch，unwatch） - z69183787的专栏 - CSDN博客
2018年02月05日 17:07:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：162
个人分类：[Redis-基础](https://blog.csdn.net/z69183787/article/category/2923783)
# 1.事务
## 1.1 概述
Redis中的事务（transaction）是一组命令的集合。事务同命令一样都是Redis的最小执行单位， 一个事务中的命令要么都执行，要么都不执行 。
事务的应用非常普遍，如银行转账过程中A给B汇款，首先系统从A的账户中将钱划走，然后向B的账户增加相应的金额。这两个步骤必须属于同一个事务，要么全执行，要么全不执行。否则只执行第一步，钱就凭空消失了，这显然让人无法接受。
事务的原理是先将属于一个事务的命令发送给Redis，然后再让Redis依次执行这些命令。
例如：
```
redis＞MULTI
OK
redis＞SADD "user:1:following" 2
QUEUED
redis＞SADD "user:2:followers" 1
QUEUED
redis＞EXEC
1) (integer) 1
2) (integer) 1
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
上面的代码演示了事务的使用方式。
（1）首先使用MULTI命令告诉Redis：“下面我发给你的命令属于同一个事务，你先不要执行，而是把它们暂时存起来。”Redis回答：“OK。”
（2）而后我们发送了两个SADD命令来实现关注和被关注操作，可以看到Redis遵守了承诺，没有执行这些命令，而是 返回QUEUED表示这两条命令已经进入等待执行的事务队列 中了。
（3）当把所有要在同一个事务中执行的命令都发给Redis后，我们 使用EXEC命令告诉Redis将等待执行的事务队列中的所有命令（即刚才所有返回QUEUED的命令）按照发送顺序依次执行。EXEC命令的返回值就是这些命令的返回值组成的列表，返回值顺序和命令的顺序相同。
Redis保证一个事务中的所有命令要么都执行，要么都不执行。如果在发送EXEC命令前客户端断线了，则Redis会清空事务队列，事务中的所有命令都不会执行。而 一旦客户端发送了EXEC命令，所有的命令就都会被执行，即使此后客户端断线也没关系，因为Redis中已经记录了所有要执行的命令。
除此之外，Redis的事务还能保证一个事务内的命令依次执行而不被其他命令插入。试想客户端A需要执行几条命令，同时客户端B发送了一条命令，如果不使用事务，则客户端B的命令可能会插入到客户端A的几条命令中执行。如果不希望发生这种情况，也可以使用事务。
## 1.2 错误处理
如果一个事务中的某个命令执行出错，Redis会怎样处理呢？要回答这个问题，首先需要知道什么原因会导致命令执行出错。
（1）语法错误。 语法错误指命令不存在或者命令参数的个数不对。比如：
```
redis＞MULTI
OK
redis＞SET key value
QUEUED
redis＞SET key
(error)ERR wrong number of arguments for 'set' command
redis＞ ERRORCOMMAND key
(error) ERR unknown command 'ERRORCOMMAND'
redis＞ EXEC
(error) EXECABORT Transaction discarded because of previous errors.
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
跟在MULTI命令后执行了3个命令：一个是正确的命令，成功地加入事务队列；其余两个命令都有语法错误。而只要有 一个命令有语法错误，执行EXEC命令后Redis就会直接返回错误，连语法正确的命令也不会执行 ① 。
注释： 
①Redis 2.6.5之前的版本会忽略有语法错误的命令，然后执行事务中其他语法正确的命令。就此例而言，SET key value会被执行，EXEC命令会返回一个结果：1) OK。
（2）运行错误。 运行错误指在命令执行时出现的错误，比如使用散列类型的命令操作集合类型的键，这种错误在实际执行之前Redis是无法发现的，所以在事务里这样的命令是会被Redis接受并执行的。如果事务里的 一条命令出现了运行错误，事务里其他的命令依然会继续执行（包括出错命令之后的命令） ，示例如下：
```
redis＞MULTI
OK
redis＞SET key 1
QUEUED
redis＞SADD key 2
QUEUED
redis＞SET key 3
QUEUED
redis＞EXEC
1) OK
2) (error) ERR Operation against a key holding the wrong kind of value
3) OK
redis＞GET key
"3"
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
可见虽然SADD key 2出现了错误，但是SET key 3依然执行了。
Redis的事务没有关系数据库事务提供的回滚（rollback）① 功能 。为此开发者必须在事务执行出错后自己收拾剩下的摊子（将数据库复原回事务执行前的状态等）。
注释：①事务回滚是指将一个事务已经完成的对数据库的修改操作撤销。
1.3 watch命令介绍
我们已经知道在一个事务中只有当所有命令都依次执行完后才能得到每个结果的返回值，可是有些情况下需要先获得一条命令的返回值，然后再根据这个值执行下一条命令。例如，INCR命令可以使用GET和SET命令自己实现，但会出现竞态条件，伪代码如下：
```
def incr( key)
value=GET key
if not value
value=0
value= value+1
SET key, value
return value
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
就目前了解的内容我们肯定会想到可以用 事务来实现incr函数以防止竞态条件，可是因为事务中的每个命令的执行结果都是最后一起返回的，所以无法将前一条命令的结果作为下一条命令的参数，即在执行SET命令时无法获得GET命令的返回值，也就无法做到增1的功能了。
为了解决这个问题，我们需要换一种思路。即在GET获得键值后保证该键值不被其他客户端修改，直到函数执行完成后才允许其他客户端修改该键键值，这样也可以防止竞态条件。要实现这一思路需要请出事务家族的另一位成员： WATCH 。 WATCH命令可以监控一个或多个键，一旦其中有一个键被修改（或删除），之后的事务就不会执行。 监控一直持续到EXEC命令（事务中的命令是在EXEC之后才执行的，所以在MULTI命令后可以修改WATCH监控的键值），如：
```
redis＞SET key 1
OK
redis＞WATCH key
OK
redis＞SET key 2
OK
redis＞MULTI
OK
redis＞SET key 3
QUEUED
redis＞EXEC
(nil)
redis＞GET key
"2"
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
上例中在执行WATCH命令后、事务执行前修改了key的值（即SET key 2），所以最后事务中的命令SET key 3没有执行，EXEC命令返回空结果。
学会了WATCH命令就可以通过事务自己实现incr函数了，伪代码如下：
```
def incr( key)
WATCH key
value=GET key
if not value
value=0
value= value+1
MULTI
SET key, value
result=EXEC
return result[0
]
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
因为EXEC命令返回值是多行字符串类型，所以代码中使用result[0]来获得其中第一个结果。
提示:
由于WATCH命令的作用只是当被监控的键值被修改后 阻止 之后一个事务的执行，而不能保证其他客户端不修改这一键值，所以我们需要在EXEC执行失败后重新执行整个函数。
执行EXEC命令后会取消对所有键的监控 ，如果不想执行事务中的命令也可以使用 UNWATCH 命令来取消监控。比如，我们要实现hsetxx函数，作用与HSETNX命令类似，只不过是仅当字段存在时才赋值。为了避免竞态条件我们使用事务来完成这一功能：
```
def hsetxx( key, field, value)
WATCH key
isFieldExists = HEXISTS key, field
if isFieldExists is 1
MULTI
HSET key, field, value
EXEC
else
UNWATCH
return isFieldExists
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
在代码中会判断要赋值的字段是否存在，如果字段不存在的话就不执行事务中的命令，但需要使用UNWATCH命令来保证下一个事务的执行不会受到影响。
