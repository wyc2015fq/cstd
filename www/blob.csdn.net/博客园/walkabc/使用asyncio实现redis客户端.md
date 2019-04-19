# 使用asyncio实现redis客户端 - walkabc - 博客园
# [使用asyncio实现redis客户端](https://www.cnblogs.com/hitfire/p/8562560.html)
redis协议格式请参考，http://doc.redisfans.com/topic/protocol.html
这里简单介绍下：
```
*<参数数量> \r\n
$<参数 1 的字节数量> \r\n
<参数 1 的数据> \r\n
$<参数 N 的字节数量> \r\n
<参数 N 的数据> \r\n
```
发送给redis服务器时的数据要按照redis要求的协议格式发送，只有这样redis服务器才能成功解析。
首先根据协议格式写一个封包方法，代码如下：
```
def format_command(self, commands):
        length = len(commands)
        command = "*{}\r\n".format(length)
        for v in commands:
            bytes = v.encode("utf-8")
            bytes_length = len(bytes)
            sub_command = "${}\r\n".format(bytes_length) + "{}\r\n".format(v)
            command += sub_command
        return command
```
看到format_command函数中的“*”和“$”符号了么。其实就是根据commands列表中的数据然后按照redis协议格式封装起来的。
弄懂了如何安装redis协议封装数据之后，就可以把数据发送到redis服务器了。
asyncio的官方demo可参考：
https://docs.python.org/3/library/asyncio-stream.html#tcp-echo-client-using-streams
下面就是完整的代码，无其他依赖，顺利执行之后，可以通过redis-cli命令行查看是否设置成功。
```
class AsyncRedis:
    def __init__(self, host, port, loop):
        self.host = host
        self.port = port
        self.loop = loop
        self.separator = "\r\n".encode()
    async def connect(self):
        reader, writer = await asyncio.open_connection(self.host, self.port, loop=self.loop)
        self.reader = reader
        self.writer = writer
    def format_command(self, commands):
        length = len(commands)
        command = "*{}\r\n".format(length)
        for v in commands:
            bytes = v.encode("utf-8")
            bytes_length = len(bytes)
            sub_command = "${}\r\n".format(bytes_length) + "{}\r\n".format(v)
            command += sub_command
        print(command)
        return command
    def execute_command(self, command):
        self.writer.write(command.encode("utf-8"))
    async def set(self, key, value):
        command = self.format_command(["SET", key, value])
        self.execute_command(command)
        ret, error = await self.wait_ret()
        print(ret)
        return ret
    async def hset(self, hash_key, key, value):
        command = self.format_command(["HSET", hash_key, key, value])
        self.execute_command(command)
    async def get(self, key):
        command = self.format_command(['GET', key])
        self.execute_command(command)
        ret = await self.wait_ret()
        return ret
    async def wait_ret(self):
        ret = await self.reader.readuntil(self.separator)
        ret = ret.decode()
        mark = ret[0:1]
        if mark == "$":
            pos = ret.index("\r\n")
            ret = ret[1:pos]
            ret = await self.reader.read(int(ret))
            ret = ret.decode()
            return ret, True
        elif mark == "+":
            pos = ret.index("\r\n")
            ret = ret[1:pos]
            return ret, True
        elif mark == "-":
            pos = ret.index("\r\n")
            ret = ret[1:pos]
            return ret, False
    async def close(self):
        self.writer.close()
import asyncio
async def NewRedis(loop):
    redis = AsyncRedis("127.0.0.1", 6379, loop)
    await redis.connect()
    # await redis.get("name")
    await redis.set("name", "云想衣裳花想容，春风拂槛露华浓。\r\n 若非群玉山头见，会向瑶台月下逢。")
loop = asyncio.get_event_loop()
loop.run_until_complete(NewRedis(loop))
loop.close()
```

