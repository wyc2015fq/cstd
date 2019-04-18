# Redis使用lua脚本实现increase + expire 的原子操作 - z69183787的专栏 - CSDN博客
2018年01月11日 14:20:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2029
lua脚本：
```
local current = redis.call('incrBy',KEYS[1],ARGV[1]);" +
            " if current == tonumber(ARGV[1]) then" +
            " local t = redis.call('ttl',KEYS[1]);" +
            " if t == -1 then " +
            " redis.call('expire',KEYS[1],ARGV[2])" +
            " end;" +
            " end;" +
            " return current";
```
JedisCluter
```java
public Long incrBy(final String key, final long step, final int defaultExpire) {
        return new JedisClusterCommand<Long>(connectionHandler, maxRedirections, Protocol.Command.INCR) {
            @Override
            public Long execute(Jedis connection) {
                if (defaultExpire > 0) {
                    Object val = connection.eval(ScriptConstant.INCRBY_1_EXPIRE, 1, new String[]{key, String.valueOf(step), String.valueOf(defaultExpire)});
                    return Long.valueOf(val.toString());
                } else {
                    return connection.incrBy(key, step);
                }
            }
        }.run(key);
    }
```
