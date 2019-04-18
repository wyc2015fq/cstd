# springboot中redis的使用(三) - weixin_33985507的博客 - CSDN博客
2018年09月29日 22:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
接上一次[springboot中redis的使用(二)](https://www.jianshu.com/p/22528fd3b62c)
# Redis脚本的使用步骤
- 写Lua脚本, 放在Resource目录下的Script子目录下
```java
-- checkandget.lua
local current = redis.call('EXISTS', KEYS[1])
if tonumber(current) == 1 then 
  local currentValue = redis.call('GET', KEYS[1])
  if tonumber(currentValue) > 10
    return currentValue
  end
end
return -1
```
- 定义获取Lua脚本的实例Bean
```java
@Bean
public RedisScript<Long> script() {
  ScriptSource scriptSource = new ResourceScriptSource(new ClassPathResource("scripts/checkandget.lua");
  return RedisScript.of(scriptSource, Long.class);
}
```
- 注入Redis脚本Bean，使用execute方法执行脚本
```java
@Autowired
  RedisScript<Long> script;
  public Long checkAndGet(String key) {
    return redisTemplate.execute(script, singletonList(key));
  }
```
# Lua脚本注意事项
Lua中的不等于的符号是 ~=
Lua中的与或非的符号是 and/or/not
Lua中连接字符串的符号是 ..
Lua中单行注释的符号是 --
Lua中的NULL的符号是 nil
# Redis 事务
事务可以通过上面Lua脚本的形式，直接使用Redis MULTI和EXEC命令。
MULTI命令标记一个事务块的开始。事务块内的多条命令会按照先后顺序被放进一个队列当中，最后由EXEC命令原子性(atomic)地执行。
也可以通过RedisTemplate提供的方式，如下通过SessionCallback可以保证多个操作在同一个连接内完成。
```java
// 执行事务
List<Object> txResults = redisTemplate.execute(new SessionCallback<List<Object>>() {
  public List<Object> execute(RedisOperations operations) throws DataAccessException {
    operations.multi();
    operations.opsForSet().add("key1", "value1");
    operations.opsForSet().add("key2", "value2");
    return operations.exec();
  }
});
```
