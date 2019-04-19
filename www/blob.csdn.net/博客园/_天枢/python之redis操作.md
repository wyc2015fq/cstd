# python之redis操作 - _天枢 - 博客园
## [python之redis操作](https://www.cnblogs.com/yhleng/p/10342328.html)
2019-01-31 15:57 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10342328)
在了解redis之前先了解一下python两种序列化方式:
python用于序列化的两个模块
 　　json：用于字符串和Python数据类型间进行转换
 　　pickle: 用于python特有的类型和python的数据类型间进行转换
 　　json提供四个功能：dumps,dump,loads,load
 　　pickle提供四个功能：dumps,dump,loads,load
pickle可以存储什么类型的数据呢？
 所有python支持的原生类型：布尔值，整数，浮点数，复数，字符串，字节，None。
由任何原生类型组成的列表，元组，字典和集合。
函数，类，类的实例
这里要用到pickle序列化包,这是python内置包.
```
import redis
import pickle
class Redis(object):
    @staticmethod
    def connect(host='127.0.0.1', port=6379, db=0):
        ret = redis.StrictRedis(host, port, db)
        return ret
    @staticmethod
    def set_key(r_conn, key, data, ex=None, px=None, bool=False):
        r_conn.set(pickle.dumps(key), pickle.dumps(data), ex, px, bool)
    @staticmethod
    def get_key(r_conn, key):
        data = r_conn.get(pickle.dumps(key))
        if data is None:
            return None
        return pickle.loads(data)
#创建redis链接对象
r = Redis.connect()
#设置redis键值.这里支持各种类型(因为pickle序列化)
# Redis.set_key(r, 'auto', '123')
#读取redis键值
print(Redis.get_key(r, 'auto'))
```
